#include <pthread.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>

#include "ring_buffer/ringbuffer.h"
#include "internals/functions.h"

#define MAX_REQUEST_LEN 1024
#define MAX_FAIL_COUNT 10
#define MAX_CONNECTIONS 10
#define PORT 5555

typedef enum
{
    REQUEST = 0,
    RESPONSE
} STATES;

static char *process_function_request(char *args)
{
    char *result = NULL;
    char *first_arg = strtok(args, " ");
    char *second_arg = NULL;
    char *suffix = NULL;

    if (first_arg == NULL)
    {
        return result;
    }

    if (strstr(first_arg, "mem"))
    {
        suffix = strtok(NULL, " ");
    }
    if (strstr(first_arg, "r") || strstr(first_arg, "w"))
    {
        second_arg = strtok(NULL, " ");

        if (second_arg != NULL && (strstr(second_arg, "u8") || strstr(second_arg, "u16") || strstr(second_arg, "u32")))
        {
            suffix = second_arg;
            second_arg = NULL;
        }
    }
    char func_name[snprintf(NULL, 0, "%s_%s", first_arg, suffix == NULL ? "" : suffix)];
    sprintf(func_name, "%s", first_arg);
    if (suffix != NULL)
    {
        sprintf(func_name, "%s_%s", first_arg, suffix);
    }

    function_callback_t func = get_func_by_name(func_name);
    if (func != NULL)
    {
        if (second_arg == NULL)
        {
            size_t len = strlen(first_arg) + 1;
            if (suffix != NULL)
            {
                len += strlen(suffix) + 1;
            }
            second_arg = &args[len];
        }
        for (char *ptr = strchr(second_arg, '\r'); ptr != NULL; ptr = strchr(second_arg, '\r'))
            *ptr = '\0';
        for (char *ptr = strchr(second_arg, '\n'); ptr != NULL; ptr = strchr(second_arg, '\n'))
            *ptr = '\0';
        result = func(second_arg);
    }
    return result;
}

static char *run_state_machine(STATES *state, char *args)
{
    char *result = NULL;
    switch (*state)
    {
    case REQUEST:
        if (args != NULL)
        {
            result = process_function_request(args);
            if (result != NULL)
                *state = RESPONSE;
        }
        break;
    case RESPONSE:
        *state = REQUEST;
        break;
    default:
        break;
    }
    return result;
}

static void client_function(void *arg)
{
    int sock = *((int *)arg);

    char request[MAX_REQUEST_LEN] = {0}, response[MAX_REQUEST_LEN] = {0};
    ring_buffer_t recv_ring_buffer = {0};
    ring_buffer_t send_ring_buffer = {0};
    ring_buffer_init(&recv_ring_buffer, request, MAX_REQUEST_LEN);
    ring_buffer_init(&send_ring_buffer, response, MAX_REQUEST_LEN);

    STATES state = REQUEST;

    int fail_count = MAX_FAIL_COUNT;
    for (;;)
    {
        int result = 0;
        fd_set read_fds, write_fds, except_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&except_fds);

        FD_SET(sock, &read_fds);
        FD_SET(sock, &write_fds);
        FD_SET(sock, &except_fds);

        struct timeval tv = {.tv_sec = 10, .tv_usec = 0};
        if (select(sock + 1, &read_fds, &write_fds, &except_fds, &tv) > -1)
        {
            if (FD_ISSET(sock, &read_fds) && (state == REQUEST))
            {
                // we can read
                int count = -1;
                ioctl(sock, FIONREAD, &count);
                if (count > -1)
                {
                    char recv_buff[count];
                    memset(recv_buff, 0, count);
                    if (recv(sock, recv_buff, count, 0) == -1)
                    {
                        fprintf(stderr, "recv return -1, error is %s\n",
                                strerror(errno));
                        break;
                    };
#ifdef DEBUG
                    fprintf(stderr, "recv: %s\n", recv_buff);
#endif
                    ring_buffer_queue_arr(&recv_ring_buffer, recv_buff, count);
                }
            }
            if (FD_ISSET(sock, &write_fds) && (state == RESPONSE))
            {
                // we can write
                size_t count = ring_buffer_num_items(&send_ring_buffer);
                count += 2;
                char send_arr[count];
                memset(send_arr, 0, sizeof(char) * count);
                ring_buffer_dequeue_arr(&send_ring_buffer, send_arr, count);
                sprintf(send_arr, "%s\r\n", send_arr);
                if (send(sock, send_arr, count, 0) == -1)
                {
                    fprintf(stderr, "send return -1, error is %s\n",
                            strerror(errno));
                    break;
                }
            }
            if (FD_ISSET(sock, &except_fds))
            {
                // exception
                if (--fail_count <= 0)
                    break;
            }
            // timeout
            if ((tv.tv_sec == 0) && (tv.tv_usec == 0))
            {
                if (--fail_count <= 0)
                    break;
                continue;
            }
        }

        size_t size = 0;
        bool terminator_found = false;
        if (!ring_buffer_is_empty(&recv_ring_buffer))
        {
            char data = 0xff;
            while (ring_buffer_num_items(&recv_ring_buffer) > size)
            {
                ring_buffer_peek(&recv_ring_buffer, &data, size++);
                if (data == '\n')
                {
                    terminator_found = true;
                    break;
                }
            }
        }
        char state_args[size + 1];
        if (terminator_found)
        {
            memset(state_args, 0, size * sizeof(char) + 1);
            ring_buffer_dequeue_arr(&recv_ring_buffer, state_args, size);
        }
        char *response = run_state_machine(&state, terminator_found ? state_args : NULL);
        if (response != NULL)
        {
            ring_buffer_queue_arr(&send_ring_buffer, response, strlen(response));
            free(response);
        }
    }
    close(sock);
    pthread_exit((void *)0);
}

int init_telnet_server(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {.sin_addr.s_addr = INADDR_ANY,
                               .sin_family = AF_INET,
                               .sin_port = (port > -1) ? htons(port) : htons(PORT)};
    if (sock < 0)
        perror("opening socket");
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(sock, (void *)&addr, sizeof(addr)))
        perror("binding tcp socket");
    if (listen(sock, MAX_CONNECTIONS) == -1)
        perror("listen");
    int client_sock = -1;
    struct sockaddr client_sock_addr = {0};
    socklen_t client_sock_addr_len = (socklen_t)sizeof(client_sock_addr);
    while (client_sock = accept(sock, &client_sock_addr, &client_sock_addr_len))
    {
        if (client_sock > -1)
        {
            pthread_t client = 0;
            pthread_create(&client, NULL, client_function, (void *)&client_sock);
            pthread_detach(client);
        }
#ifdef DEBUG
        else
            perror("accept return -1");
#endif
    }
    return 0;
}