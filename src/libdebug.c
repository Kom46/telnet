#include <pthread.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/in.h>

#include "ring_buffer/ringbuffer.h"
#include "internals/functions.h"

#define MAX_REQUEST_LEN 1024
#define MAX_FAIL_COUNT 10
#define PORT 5555

typedef enum
{
    REQUEST = 0,
    RESPONSE
} STATES;

function_callback_t process_function_request(char **args)
{
    char *func_name = strtok(*args, " ");
    function_callback_t result = get_func_by_name(func_name);
    return result;
}

char *run_state_machine(STATES *state, char *args)
{
    char *result = NULL;
    switch (*state)
    {
    case REQUEST:
        if (args != NULL)
        {
            result = process_function_request(args)(args);
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
        struct timeval tv = {.tv_sec = 10, .tv_usec = 0};
        if (select(1, &read_fds, &write_fds, &except_fds, &tv) > -1)
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
                    ring_buffer_queue_arr(&recv_ring_buffer, recv_buff, count);
                }
            }
            if (FD_ISSET(sock, &write_fds) && state == RESPONSE)
            {
                // we can write
                size_t count = ring_buffer_num_items(&send_ring_buffer);
                char send_arr[count];
                memset(send_arr, 0, sizeof(char));
                ring_buffer_dequeue_arr(&send_ring_buffer, send_arr, count);
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
        }

        size_t size = 0;
        bool terminator_found = false;
        if (!ring_buffer_is_empty(&recv_ring_buffer))
        {
            char data = 0xff;
            while (ring_buffer_num_items > size)
            {
                ring_buffer_peek(&recv_ring_buffer, &data, size++);
                if (data == '\0')
                {
                    terminator_found = true;
                    break;
                }
            }
        }
        char state_args[size];
        if (terminator_found)
        {
            memset(state_args, 0, sizeof(char));
            ring_buffer_dequeue_arr(&recv_ring_buffer, state_args, size);
        }
        char *response = run_state_machine(&state, terminator_found ? state_args : NULL);
        if (response != NULL)
        {
            ring_buffer_queue_arr(&send_ring_buffer, response, strlen(response));
            free(response);
        }
        // timeout
        if ((tv.tv_sec == 0) && (tv.tv_usec == 0))
        {
            if (--fail_count <= 0)
                break;
            continue;
        }
    }
    pthread_exit((void *)0);
}

int init_server(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {.sin_addr.s_addr = INADDR_ANY,
                               .sin_family = AF_INET,
                               .sin_port = htons(PORT)};
    if (sock < 0)
        perror("opening socket");
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(sock, (void *)&addr, sizeof(addr)))
        perror("binding tcp socket");
    if (listen(sock, 1) == -1)
        perro("listen");
    int client_sock = -1;
    struct sockaddr client_sock_addr = {0};
    while (client_sock = accept(sock, &client_sock_addr, sizeof(client_sock_addr))) {
        pthread_t client = 0;
        pthread_create(&client, NULL, client_function, (void *)client_sock);
        pthread_detach(client);
    }
    return 0;
}