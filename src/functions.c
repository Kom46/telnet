#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *memory_dump(char *args)
{
    char *result = NULL;
    char *arg = NULL;
    if ((arg = strtok(args, " ")) != NULL)
    {
        char *char_ptr = strstr(arg, "0x");
        int base = 10;
        if (char_ptr != NULL)
        {
            arg = char_ptr + 2;
            base = 16;
        }
        void *ptr = (void *)strtoul(arg, &char_ptr, base);
        if ((arg = strtok(NULL, " ")) != NULL)
        {
            char_ptr = strstr(arg, "0x");
            int base = 10;
            if (char_ptr != NULL)
            {
                arg = char_ptr + 2;
                base = 16;
            }
            unsigned int count = (void *)strtoul(arg, &char_ptr, base);
            int size = snprintf(NULL, 0, "%p:", ptr);
            result = (char *)calloc(size, sizeof(char));
            sprintf(result, "%p:", ptr);
            for (; ptr <= (ptr + count); ptr++)
            {
                char curr_size = snprintf(NULL, 0, "0x%x", *((unsigned int *)ptr));
                char curr_buff[curr_size];
                memset(curr_buff, 0, curr_size);
                sprintf(curr_buff, " 0x%x", *((unsigned int *)ptr));
                size += curr_size;
                result = (char *)realloc(result, size);
                strcat(result, curr_buff);
            }
        }
        else
        {
            result = strdup("Wrong args to function!");
        }
    }
    else
    {
        result = strdup("Wrong args to function!");
    }

    return result;
}

static inline void *get_ptr_from_string(char *ptr_string)
{
    void *result = NULL;
    char *char_ptr = strstr(ptr_string, "0x");
    int base = 10;
    if (char_ptr != NULL)
    {
        ptr_string = char_ptr + 2;
        base = 16;
    }
    result = (void *)strtoul(ptr_string, &char_ptr, base);
    if (result == NULL)
    {
        // lets try by symbol
        result = dlsym(NULL, ptr_string);
    }
    return result;
}

char *write_byte(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg1 = strtok(args, " ");
    if (arg1 != NULL)
    {
        ptr = get_ptr_from_string(arg1);
    }
    unsigned char val = 0;
    char *arg2 = strtok(NULL, " ");
    if (arg2 != NULL)
    {
        char *char_ptr = strstr(arg2, "0x");
        int base = 10;
        if (char_ptr != NULL)
        {
            arg1 = char_ptr + 2;
            base = 16;
        }
        val = (unsigned char)strtoul(arg2, &char_ptr, base);
        if (ptr != NULL)
        {
            *((unsigned char *)ptr) = val;
            result = strdup("Write success");
        }
        else
        {
            result = strdup("Pointer is invalid");
        }
    }
    else
    {
        result = strdup("Value is absent");
    }
    return result;
}

char *write_word(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg1 = strtok(args, " ");
    if (arg1 != NULL)
    {
        ptr = get_ptr_from_string(arg1);
    }
    unsigned int val = 0;
    char *arg2 = strtok(NULL, " ");
    if (arg2 != NULL)
    {
        char *char_ptr = strstr(arg2, "0x");
        int base = 10;
        if (char_ptr != NULL)
        {
            arg2 = char_ptr + 2;
            base = 16;
        }
        val = (unsigned int)strtoul(arg2, &char_ptr, base);
        if (ptr != NULL)
        {
            *((unsigned int *)ptr) = val;
            result = strdup("Write success");
        }
        else
        {
            result = strdup("Pointer is invalid");
        }
    }
    else
    {
        result = strdup("Value is absent");
    }
    return result;
}

char *write_func(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg = strtok(args, " ");
    if (arg != NULL)
    {
        ptr = get_ptr_from_string(arg);
    }
    unsigned int *mem = NULL;
    int size = 0;
    while ((arg = strtok(NULL, " ")) != NULL)
    {
        mem = (unsigned int *)realloc(mem, ++size * sizeof(unsigned int));
        if (mem == NULL)
            break;
        char *char_ptr = strstr(arg, "0x");
        int base = 10;
        if (char_ptr != NULL)
        {
            arg = char_ptr + 2;
            base = 16;
        }
        unsigned long val = (unsigned int)strtoul(arg, &char_ptr, base);
        if (mem != NULL)
        {
            mem[size - 1] = val;
        }
        else
        {
            result = strdup("Write fail");
            break;
        }
    }
    if (mem)
    {
        memmove(ptr, mem, size);
        free(mem);
        result = strdup("Write success");
    }
    return result;
}

char *read_word(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg = strtok(args, " ");
    if (arg != NULL)
    {
        ptr = get_ptr_from_string(arg);
    }
    unsigned int val = *((unsigned int *)ptr);
    size_t str_len = snprintf(NULL, 0, "%p %x", ptr, val);
    result = (char *)calloc(str_len, sizeof(char));
    if (result != NULL)
        sprintf(result, "%p %x", ptr, val);
    else
        result = strdup("Failed to read data");
    return result;
}

char *resolve_symbol(char *args)
{
    char *result = NULL;
    void *ptr = dlsym(NULL, args);
    result = (char *)calloc(snprintf(NULL, 0, "%p", ptr), sizeof(char));
    sprintf(result, "%p", ptr);
    return result;
}

char *run_func(char *args)
{
    char *result = NULL;
    typedef void (*function_type_t)();
    function_type_t ptr = NULL;
    char *arg = strtok(args, " ");
    if (arg != NULL)
    {
        ptr = (function_type_t)get_ptr_from_string(arg);
    }
    if (ptr != NULL)
    {
        ptr();
        result = (char *)calloc(snprintf(NULL, 0,
                                         "function execution at address %p is successfull", (void *)ptr),
                                sizeof(char));
        sprintf(result, "function execution at address %p is successfull", (void *)ptr);
    }
    else
    {
        result = strdup("Function execution failed!");
    }
    return result;
}