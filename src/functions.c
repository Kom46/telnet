#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "internals/functions.h"

#ifndef TEST
// functions for telnet communication
    char * memory_dump(char *args);
    char * write_byte(char *args);
    char * write_halfword(char *args);
    char * write_word(char *args);
    char * write_func(char *args);
    char * read_byte(char *args);
    char * read_halfword(char *args);
    char * read_word(char *args);
    char * resolve_symbol(char *args);
    char * run_func(char *args);
#endif

static enum emode {
    ADDR = 1,
    SYM = 2
};

static struct
{
    function_callback_t func_ptr;
    char *func_name;
} func_table[] = {
    {(function_callback_t)memory_dump, "mem_dump"},
    {(function_callback_t)write_func, "mem_write"},
    {(function_callback_t)write_byte, "s_u8"},
    {(function_callback_t)write_halfword, "s_u16"},
    {(function_callback_t)write_word, "s_u32"},
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
    if (strstr(ptr_string, "0x"))
    {
        sscanf(ptr_string, "%p", &result);
    }
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
    Dl_info info = {0};
    void *handle = dlopen(NULL, RTLD_GLOBAL | RTLD_NOW);
    void *ptr = NULL;
    enum emode mode = 0;
    if (strstr(args, "0x"))
    {
        ptr = (void *)strtoul(args, NULL, 0);
        mode = ADDR;
    }
    else
    {
        ptr = dlsym(NULL, args);
        mode = SYM;
    }
    if (ptr != NULL)
    {
        dladdr(ptr, &info);
    }

#ifdef TEST
    if (ptr == NULL)
    {
        fprintf(stderr, "dlerror is %s\n", dlerror());
    }
#endif
    dlclose(handle);
    char tmp[8] = {0};
    switch (mode)
    {
    case ADDR:
        if (ptr != NULL)
        {
            result = (char *)calloc(snprintf(NULL, 0, "Address '%p' located at %s within the \
                                program %s",
                                             ptr, info.dli_fname, info.dli_sname ?: "NULL"),
                                    sizeof(char));
            sprintf(result, "Address '%p' located at %s within the program %s",
                    ptr, info.dli_fname, info.dli_sname ?: "NULL");
        }
        else
            sprintf(tmp, "address");
        break;
    case SYM:
        if (ptr != NULL)
        {
            result = (char *)calloc(snprintf(NULL, 0, "symbol '%s' at %p", args, ptr), sizeof(char));
            sprintf(result, "symbol '%s' at %p", args, ptr);
        }
        else
            sprintf(tmp, "symbol");
        break;
    default:
        break;
    }
    if (result == NULL)
    {
        result = (char *)calloc(snprintf(NULL, 0, "%s %s could not be resolved", tmp, args), sizeof(char));
        sprintf(result, "%s %s could not be resolved", tmp, args);
    }

    return result;
}

typedef union
{
    unsigned long (*fn)();
    unsigned long (*fn_l)(unsigned long);
    unsigned long (*fn_s)(char *);
    unsigned long (*fn_ll)(unsigned long, unsigned long);
    unsigned long (*fn_ls)(unsigned long, char *);
    unsigned long (*fn_sl)(char *, unsigned long);
    unsigned long (*fn_ss)(char *, char *);
    unsigned long (*fn_lll)(unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_lls)(unsigned long, unsigned long, char *);
    unsigned long (*fn_lsl)(unsigned long, char *, unsigned long);
    unsigned long (*fn_lss)(unsigned long, char *, char *);
    unsigned long (*fn_sll)(char *, unsigned long, unsigned long);
    unsigned long (*fn_sls)(char *, unsigned long, char *);
    unsigned long (*fn_ssl)(char *, char *, unsigned long);
    unsigned long (*fn_sss)(char *, char *, char *);
    unsigned long (*fn_llll)(unsigned long, unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_llls)(unsigned long, unsigned long, unsigned long, char *);
    unsigned long (*fn_llsl)(unsigned long, unsigned long, char *, unsigned long);
    unsigned long (*fn_llss)(unsigned long, unsigned long, char *, char *);
    unsigned long (*fn_lsll)(unsigned long, char *, unsigned long, unsigned long);
    unsigned long (*fn_lsls)(unsigned long, char *, unsigned long, char *);
    unsigned long (*fn_lssl)(unsigned long, char *, char *, unsigned long);
    unsigned long (*fn_lsss)(unsigned long, char *, char *, char *);
    unsigned long (*fn_slll)(char *, unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_slls)(char *, unsigned long, unsigned long, char *);
    unsigned long (*fn_slsl)(char *, unsigned long, char *, unsigned long);
    unsigned long (*fn_slss)(char *, unsigned long, char *, char *);
    unsigned long (*fn_ssll)(char *, char *, unsigned long, unsigned long);
    unsigned long (*fn_ssls)(char *, char *, unsigned long, char *);
    unsigned long (*fn_sssl)(char *, char *, char *, unsigned long);
    unsigned long (*fn_ssss)(char *, char *, char *, char *);
    unsigned long (*fn_lllll)(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_sllll)(char *, unsigned long, unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_lslll)(unsigned long, char *, unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_llsll)(unsigned long, unsigned long, char *, unsigned long, unsigned long);
    unsigned long (*fn_lllsl)(unsigned long, unsigned long, unsigned long, char *, unsigned long);
    unsigned long (*fn_lllls)(unsigned long, unsigned long, unsigned long, unsigned long, char *);
    unsigned long (*fn_sllls)(char *, unsigned long, unsigned long, unsigned long, char *);
    unsigned long (*fn_lslls)(unsigned long, char *, unsigned long, unsigned long, char *);
    unsigned long (*fn_llsls)(unsigned long, unsigned long, char *, unsigned long, char *);
    unsigned long (*fn_lllss)(unsigned long, unsigned long, unsigned long, char *, char *);
    unsigned long (*fn_sllss)(char *, unsigned long, unsigned long, char *, char *);
    unsigned long (*fn_lslss)(unsigned long, char *, unsigned long, char *, char *);
    unsigned long (*fn_llsss)(unsigned long, unsigned long, char *, char *, char *);
    unsigned long (*fn_slsss)(char *, unsigned long, char *, char *, char *);
    unsigned long (*fn_lssss)(unsigned long, char *, char *, char *, char *);
    unsigned long (*fn_lsssl)(unsigned long, char *, char *, char *, unsigned long);
    unsigned long (*fn_lslsl)(unsigned long, char *, unsigned long, char *, unsigned long);
    unsigned long (*fn_lssll)(unsigned long, char *, char *, unsigned long, unsigned long);
    unsigned long (*fn_lssls)(unsigned long, char *, char *, unsigned long, char *);
    unsigned long (*fn_sllsl)(char *, unsigned long, unsigned long, char *, unsigned long);
    unsigned long (*fn_slsll)(char *, unsigned long, char *, unsigned long, unsigned long);
    unsigned long (*fn_slsls)(char *, unsigned long, char *, unsigned long, char *);
    unsigned long (*fn_slssl)(char *, unsigned long, char *, char *, unsigned long);
    unsigned long (*fn_sslll)(char *, char *, unsigned long, unsigned long, unsigned long);
    unsigned long (*fn_sslls)(char *, char *, unsigned long, unsigned long, char *);
    unsigned long (*fn_sslsl)(char *, char *, unsigned long, char *, unsigned long);
    unsigned long (*fn_sssll)(char *, char *, char *, unsigned long, unsigned long);
    unsigned long (*fn_sslss)(char *, char *, unsigned long, char *, char *);
    unsigned long (*fn_sssls)(char *, char *, char *, unsigned long, char *);
    unsigned long (*fn_ssssl)(char *, char *, char *, char *, unsigned long);
    unsigned long (*fn_llssl)(unsigned long, unsigned long, char *, char *, unsigned long);
    unsigned long (*fn_sssss)(char *, char *, char *, char *, char *);
} func_ptr_u;

typedef union
{
    enum eone_arg_fn
    {
        FUNC_L = 0b1,
        FUNC_S = 0b0
    } one_arg_fn;

    enum etwo_args_fn
    {
        FUNC_LL = 0b11,
        FUNC_LS = 0b10,
        FUNC_SL = 0b01,
        FUNC_SS = 0b00
    } two_arg_fn;

    enum ethree_args_fn
    {
        FUNC_LLL = 0b111,
        FUNC_LLS = 0b110,
        FUNC_LSL = 0b101,
        FUNC_LSS = 0b100,
        FUNC_SLL = 0b011,
        FUNC_SLS = 0b010,
        FUNC_SSL = 0b001,
        FUNC_SSS = 0b000
    } three_arg_fn;

    enum efour_args_fn
    {
        FUNC_LLLL = 0b1111,
        FUNC_LLLS = 0b1110,
        FUNC_LLSL = 0b1101,
        FUNC_LLSS = 0b1100,
        FUNC_LSLL = 0b1011,
        FUNC_LSLS = 0b1010,
        FUNC_LSSL = 0b1001,
        FUNC_LSSS = 0b1000,
        FUNC_SLLL = 0b0111,
        FUNC_SLLS = 0b0110,
        FUNC_SLSL = 0b0101,
        FUNC_SSLL = 0b0011,
        FUNC_SSLS = 0b0010,
        FUNC_SSSL = 0b0001,
        FUNC_SSSS = 0b0000
    } four_arg_fn;

    enum efive_args_fn
    {
        FUNC_LLLLL = 0b11111,
        FUNC_LLLLS = 0b11110,
        FUNC_LLLSL = 0b11101,
        FUNC_LLLSS = 0b11100,
        FUNC_LLSLL = 0b11011,
        FUNC_LLSLS = 0b11010,
        FUNC_LLSSL = 0b11001,
        FUNC_LLSSS = 0b11000,
        FUNC_LSLLL = 0b10111,
        FUNC_LSLLS = 0b10110,
        FUNC_LSLSL = 0b10101,
        FUNC_LSLSS = 0b10100,
        FUNC_LSSLL = 0b10011,
        FUNC_LSSLS = 0b10010,
        FUNC_LSSSL = 0b10001,
        FUNC_LSSSS = 0b10000,
        FUNC_SLLLL = 0b01111,
        FUNC_SLLLS = 0b01110,
        FUNC_SLLSL = 0b01101,
        FUNC_SLLSS = 0b01100,
        FUNC_SLSLL = 0b01011,
        FUNC_SLSLS = 0b01010,
        FUNC_SLSSL = 0b01001,
        FUNC_SLSSS = 0b01000,
        FUNC_SSLLL = 0b00111,
        FUNC_SSLLS = 0b00110,
        FUNC_SSLSL = 0b00101,
        FUNC_SSLSS = 0b00100,
        FUNC_SSSLL = 0b00011,
        FUNC_SSSLS = 0b00010,
        FUNC_SSSSL = 0b00001,
        FUNC_SSSSS = 0b00000
    } five_arg_fn;
} fn_type;

char *run_func(char *args)
{
#define MAX_ARGS 5
    char *result = NULL;
    func_ptr_u ptr;
    ptr.fn = NULL;
    char *arg = strtok(args, " (");
    char *name = NULL;
    enum emode mode = 0;
    if (arg != NULL)
    {
        mode = (strstr(arg, "0x")) ? ADDR : SYM;
        if (mode == SYM)
        {
            name = strdup(arg);
        }
        ptr.fn = get_ptr_from_string(arg);
    }
    if (ptr.fn != NULL)
    {
        char *sep_args[MAX_ARGS] = {0};
        int args_count = 0;
        arg = strchr(arg, '(');
        for (size_t i = 0; i < MAX_ARGS; i++)
        {
            if ((arg = strtok(NULL, ",")) != NULL)
            {
                sep_args[i] = strdup(arg);
                char *tmp = strchr(sep_args[i], ')');
                if (tmp != NULL)
                {
                    *tmp = '\0';
                }
                args_count++;
            }
            else
                break;
        }
        int retval = 0;
        fn_type func_type;
        func_type.five_arg_fn = 0;
        switch (args_count)
        {
        case 0:
            retval = ptr.fn();
            break;
        case 1:
            if (!strstr(sep_args[0], "\""))
            {
                func_type.one_arg_fn |= (1 << 0UL);
            }
            switch (func_type.one_arg_fn)
            {
            case FUNC_L:
                retval = ptr.fn_l(strtoul(sep_args[0], NULL, 0));
                break;
            case FUNC_S:
                retval = ptr.fn_s(sep_args[0]);
                break;
            default:
                break;
            }
            break;
        case 2:
            for (int i = 0; i < args_count; i++)
            {
                if (!strstr(sep_args[i], "\""))
                {
                    func_type.two_arg_fn |= (1 << i);
                }
            }
            switch (func_type.two_arg_fn)
            {
            case FUNC_LL:
                retval = ptr.fn_ll(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0));
                break;
            case FUNC_LS:
                retval = ptr.fn_ls(strtoul(sep_args[0], NULL, 0), sep_args[1]);
                break;
            case FUNC_SL:
                retval = ptr.fn_sl(sep_args[0], strtoul(sep_args[1], NULL, 0));
                break;
            case FUNC_SS:
                retval = ptr.fn_ss(sep_args[0], sep_args[1]);
                break;
            default:
                break;
            }
            break;
        case 3:
            for (size_t i = 0; i < args_count; i++)
            {
                if (!strstr(sep_args[i], "\""))
                {
                    func_type.three_arg_fn |= (1 << i);
                }
            }
            switch (func_type.three_arg_fn)
            {
            case FUNC_LLL:
                retval = ptr.fn_lll(strtoul(sep_args[0], NULL, 0),
                                    strtoul(sep_args[1], NULL, 0), strtoul(sep_args[2], NULL, 0));
                break;
            case FUNC_LLS:
                retval = ptr.fn_lls(strtoul(sep_args[0], NULL, 0),
                                    strtoul(sep_args[1], NULL, 0), sep_args[2]);
                break;
            case FUNC_LSL:
                retval = ptr.fn_lsl(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                    strtoul(sep_args[2], NULL, 0));
                break;
            case FUNC_LSS:
                retval = ptr.fn_lss(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                    sep_args[2]);
                break;
            case FUNC_SLL:
                retval = ptr.fn_sll(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                    strtoul(sep_args[2], NULL, 0));
                break;
            case FUNC_SLS:
                retval = ptr.fn_sls(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                    sep_args[2]);
                break;
            case FUNC_SSL:
                retval = ptr.fn_ssl(sep_args[0], sep_args[1],
                                    strtoul(sep_args[2], NULL, 0));
                break;
            case FUNC_SSS:
                retval = ptr.fn_sss(sep_args[0], sep_args[1], sep_args[2]);
                break;
            default:
                break;
            }
            break;
        case 4:
            for (size_t i = 0; i < args_count; i++)
            {
                if (!strstr(sep_args[i], "\""))
                {
                    func_type.four_arg_fn |= (1 << i);
                }
            }
            switch (func_type.four_arg_fn)
            {
            case FUNC_LLLL:
                retval = ptr.fn_llll(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                     strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_LLLS:
                retval = ptr.fn_llls(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                     strtoul(sep_args[2], NULL, 0), sep_args[3]);
                break;
            case FUNC_LLSL:
                retval = ptr.fn_llsl(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                     sep_args[2], strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_LLSS:
                retval = ptr.fn_llss(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                     sep_args[2], sep_args[3]);
                break;
            case FUNC_LSLL:
                retval = ptr.fn_lsll(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                     strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_LSLS:
                retval = ptr.fn_lsls(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                     strtoul(sep_args[2], NULL, 0), sep_args[3]);
                break;
            case FUNC_LSSL:
                retval = ptr.fn_lssl(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                     sep_args[2], strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_LSSS:
                retval = ptr.fn_lsss(strtoul(sep_args[0], NULL, 0),
                                     sep_args[1], sep_args[2], sep_args[3]);
                break;
            case FUNC_SLLL:
                retval = ptr.fn_slll(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                     strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_SLLS:
                retval = ptr.fn_slls(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                     strtoul(sep_args[2], NULL, 0), sep_args[3]);
                break;
            case FUNC_SLSL:
                retval = ptr.fn_slsl(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                     sep_args[2], strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_SSLL:
                retval = ptr.fn_ssll(sep_args[0], sep_args[1],
                                     strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_SSLS:
                retval = ptr.fn_ssls(sep_args[0], sep_args[1],
                                     strtoul(sep_args[2], NULL, 0), sep_args[3]);
                break;
            case FUNC_SSSL:
                retval = ptr.fn_sssl(sep_args[0], sep_args[1], sep_args[2],
                                     strtoul(sep_args[3], NULL, 0));
                break;
            case FUNC_SSSS:
                retval = ptr.fn_ssss(sep_args[0], sep_args[1], sep_args[2], sep_args[3]);
                break;
            default:
                break;
            }
            break;
        case 5:
            for (size_t i = 0; i < args_count; i++)
            {
                if (!strstr(sep_args[i], "\""))
                {
                    func_type.five_arg_fn |= (1 << i);
                }
            }
            switch (func_type.five_arg_fn)
            {
            case FUNC_LLLLL:
                retval = ptr.fn_lllll(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0),
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LLLLS:
                retval = ptr.fn_lllls(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0),
                                      sep_args[4]);
                break;
            case FUNC_LLLSL:
                retval = ptr.fn_lllsl(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), sep_args[3], strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LLLSS:
                retval = ptr.fn_lllss(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), sep_args[3], sep_args[4]);
                break;
            case FUNC_LLSLL:
                retval = ptr.fn_llsll(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], strtoul(sep_args[3], NULL, 0), strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LLSLS:
                retval = ptr.fn_llsls(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], strtoul(sep_args[3], NULL, 0), sep_args[4]);
                break;
            case FUNC_LLSSL:
                retval = ptr.fn_llssl(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0), sep_args[2],
                                      sep_args[3], strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LLSSS:
                retval = ptr.fn_llsss(strtoul(sep_args[0], NULL, 0), strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], sep_args[3], sep_args[4]);
                break;
            case FUNC_LSLLL:
                retval = ptr.fn_lslll(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0),
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LSLLS:
                retval = ptr.fn_lslls(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0),
                                      sep_args[4]);
                break;
            case FUNC_LSLSL:
                retval = ptr.fn_lslsl(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      strtoul(sep_args[2], NULL, 0), sep_args[3],
                                      strtoul(sep_args[0], NULL, 0));
                break;
            case FUNC_LSLSS:
                retval = ptr.fn_lslss(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      strtoul(sep_args[2], NULL, 0), sep_args[3], sep_args[4]);
                break;
            case FUNC_LSSLL:
                retval = ptr.fn_lssll(strtoul(sep_args[0], NULL, 0), sep_args[1], sep_args[2],
                                      strtoul(sep_args[3], NULL, 0), strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LSSLS:
                retval = ptr.fn_lssls(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      sep_args[2], strtoul(sep_args[3], NULL, 0), sep_args[4]);
                break;
            case FUNC_LSSSL:
                retval = ptr.fn_lsssl(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      sep_args[2], sep_args[3], strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_LSSSS:
                retval = ptr.fn_lssss(strtoul(sep_args[0], NULL, 0), sep_args[1],
                                      sep_args[2], sep_args[3], sep_args[4]);
                break;
            case FUNC_SLLLL:
                retval = ptr.fn_sllll(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), strtoul(sep_args[3], NULL, 0),
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SLLLS:
                retval = ptr.fn_sllls(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0),
                                      strtoul(sep_args[3], NULL, 0), sep_args[4]);
                break;
            case FUNC_SLLSL:
                retval = ptr.fn_sllsl(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), sep_args[3],
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SLLSS:
                retval = ptr.fn_sllss(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      strtoul(sep_args[2], NULL, 0), sep_args[3], sep_args[4]);
                break;
            case FUNC_SLSLL:
                retval = ptr.fn_slsll(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], strtoul(sep_args[3], NULL, 0),
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SLSLS:
                retval = ptr.fn_slsls(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], strtoul(sep_args[3], NULL, 0), sep_args[4]);
                break;
            case FUNC_SLSSL:
                retval = ptr.fn_slssl(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], sep_args[3], strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SLSSS:
                retval = ptr.fn_slsss(sep_args[0], strtoul(sep_args[1], NULL, 0),
                                      sep_args[2], sep_args[3], sep_args[4]);
                break;
            case FUNC_SSLLL:
                retval = ptr.fn_sslll(sep_args[0], sep_args[1],
                                      strtoul(sep_args[2], NULL, 0),
                                      strtoul(sep_args[3], NULL, 0),
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SSLLS:
                retval = ptr.fn_sslls(sep_args[0], sep_args[1],
                                      strtoul(sep_args[2], NULL, 0),
                                      strtoul(sep_args[3], NULL, 0),
                                      sep_args[4]);
                break;
            case FUNC_SSLSL:
                retval = ptr.fn_sslsl(sep_args[0], sep_args[1],
                                      strtoul(sep_args[2], NULL, 0), sep_args[3],
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SSLSS:
                retval = ptr.fn_sslss(sep_args[0], sep_args[1],
                                      strtoul(sep_args[2], NULL, 0),
                                      sep_args[3], sep_args[4]);
                break;
            case FUNC_SSSLL:
                retval = ptr.fn_sssll(sep_args[0], sep_args[1], sep_args[2],
                                      strtoul(sep_args[3], NULL, 0),
                                      strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SSSLS:
                retval = ptr.fn_sssls(sep_args[0], sep_args[1], sep_args[2],
                                      strtoul(sep_args[3], NULL, 0),
                                      sep_args[4]);
                break;
            case FUNC_SSSSL:
                retval = ptr.fn_ssssl(sep_args[0], sep_args[1], sep_args[2],
                                      sep_args[3], strtoul(sep_args[4], NULL, 0));
                break;
            case FUNC_SSSSS:
                retval = ptr.fn_sssss(sep_args[0], sep_args[1], sep_args[2],
                                      sep_args[3], sep_args[4]);
                break;
            }
            break;
        default:
            break;
        }
        switch (mode)
        {
        case ADDR:
            result = (char *)calloc(snprintf(NULL, 0,
                                             "func at %p returned 0x%x", (void *)ptr.fn, retval),
                                    sizeof(char));
            sprintf(result, "func at %p returned 0x%x", (void *)ptr.fn, retval);
            break;
        case SYM:
            result = (char *)calloc(snprintf(NULL, 0,
                                             "func '%s' at %p returned 0x%x",name, (void *)ptr.fn, retval),
                                    sizeof(char));
            sprintf(result, "func '%s' at %p returned 0x%x", name, (void *)ptr.fn, retval);
            free(name);
            break;
        default:
            break;
        }
    }
    else
    {
        result = strdup("Function execution failed!");
    }
    return result;
#undef MAX_ARGS
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