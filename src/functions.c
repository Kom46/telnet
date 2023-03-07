#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "internals/functions.h"

static struct
{
    function_callback_t func_ptr;
    char *func_name;
} func_table[] = {
    {(function_callback_t)memory_dump, "mem_dump"},
    {(function_callback_t)write_byte, "s_u8"},
    {(function_callback_t)write_halfword, "s_u16"},
    {(function_callback_t)write_word, "s_u32"},
    {(function_callback_t)write_func, "mem_write"},
    {(function_callback_t)read_byte, "r_u8"},
    {(function_callback_t)read_halfword, "r_u16"},
    {(function_callback_t)read_word, "r_u32"},
    {(function_callback_t)resolve_symbol, "r"},
    {(function_callback_t)run_func, "c"}};

char *memory_dump(char *args)
{
    char *result = NULL;
    char *arg = NULL;
    if ((arg = strtok(args, " ")) != NULL)
    {
        char *char_ptr = NULL;
        int base = 0;
        void *ptr = NULL;
        sscanf(args, "%p", &ptr);
        if ((arg = strtok(NULL, " ")) != NULL)
        {
            unsigned int count = 0;
            sscanf(arg, "%d", &count);
            int size = snprintf(NULL, 0, "%p:", ptr);
            result = (char *)calloc(size, sizeof(char));
            sprintf(result, "%p:", ptr);
            for (void *curr_ptr = ptr; curr_ptr < (ptr + count * (sizeof(void *))); curr_ptr += sizeof(void *))
            {
                char curr_size = snprintf(NULL, 0, " 0x%x", *((unsigned int *)curr_ptr));
                char curr_buff[curr_size];
                memset(curr_buff, 0, curr_size);
                sprintf(curr_buff, " 0x%x", *((unsigned int *)curr_ptr));
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
    int base = 0;
    sscanf(ptr_string, "%p", &result);
    if (result == NULL)
    {
        sscanf(ptr_string, "%d", &result);
    }
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
        char *char_ptr = NULL;
        sscanf(arg2, "%hhu", &val);
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

char *write_halfword(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg1 = strtok(args, " ");
    if (arg1 != NULL)
    {
        ptr = get_ptr_from_string(arg1);
    }
    unsigned short val = 0;
    char *arg2 = strtok(NULL, " ");
    if (arg2 != NULL)
    {
        char *char_ptr = NULL;
        int base = 0;
        val = (unsigned short)strtoul(arg2, &char_ptr, base);
        if (ptr != NULL)
        {
            *((unsigned short *)ptr) = val;
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
    unsigned long val = 0;
    char *arg2 = strtok(NULL, " ");
    if (arg2 != NULL)
    {
        char *char_ptr = NULL;
        int base = 0;
        val = (unsigned long)strtoul(arg2, &char_ptr, base);
        if (ptr != NULL)
        {
            *((unsigned long *)ptr) = val;
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
    unsigned char *mem = NULL;
    int size = 0;
    while ((arg = strtok(NULL, " ")) != NULL)
    {
        mem = (unsigned long *)realloc(mem, ++size * sizeof(unsigned char));
        if (mem == NULL)
            break;
        char *char_ptr = NULL;
        int base = 0;
        unsigned long val = (unsigned long)strtoul(arg, &char_ptr, base);
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
        result = (char *)calloc(snprintf(NULL, 0, "Writed %d bytes at address %p", size, ptr), sizeof(char));
        sprintf(result, "Writed %d bytes at address %p", size, ptr);
    }
    return result;
}

char *read_byte(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg = strtok(args, " ");
    if (arg != NULL)
    {
        ptr = get_ptr_from_string(arg);
    }
    unsigned char val = *((unsigned char *)ptr);
    size_t str_len = snprintf(NULL, 0, "%p: %hhu", ptr, val);
    result = (char *)calloc(str_len, sizeof(char));
    if (result != NULL)
        sprintf(result, "%p: 0x%x", ptr, val);
    else
        result = strdup("Failed to read data");
    return result;
}

char *read_halfword(char *args)
{
    char *result = NULL;
    void *ptr = NULL;
    char *arg = strtok(args, " ");
    if (arg != NULL)
    {
        ptr = get_ptr_from_string(arg);
    }
    unsigned short val = *((unsigned short *)ptr);
    size_t str_len = snprintf(NULL, 0, "%p: 0x%x", ptr, val);
    result = (char *)calloc(str_len, sizeof(char));
    if (result != NULL)
        sprintf(result, "%p: 0x%x", ptr, val);
    else
        result = strdup("Failed to read data");
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
    unsigned long val = *((unsigned long *)ptr);
    size_t str_len = snprintf(NULL, 0, "%p: 0x%x", ptr, val);
    result = (char *)calloc(str_len, sizeof(char));
    if (result != NULL)
        sprintf(result, "%p: 0x%x", ptr, val);
    else
        result = strdup("Failed to read data");
    return result;
}

char *resolve_symbol(char *args)
{
    char *result = NULL;
    void *handle = dlopen(NULL, RTLD_GLOBAL | RTLD_NOW);
    void *ptr = dlsym(handle, args);
#ifdef TEST
    if (ptr == NULL)
    {
        fprintf(stderr, "dlerror is %s\n", dlerror());
    }
#endif
    dlclose(handle);
    result = (char *)calloc(snprintf(NULL, 0, "%s: %p", args, ptr), sizeof(char));
    sprintf(result, "%s: %p", args, ptr);
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

function_callback_t get_func_by_name(char *name)
{
    function_callback_t result = NULL;
    for (int i = 0; i < ARRAY_SIZE(func_table); i++)
    {
        if (!strcmp(name, func_table[i].func_name))
        {
            result = func_table[i].func_ptr;
            break;
        }
    }
    return result;
}