#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
// function prototype for telnet communication
    typedef char *(*function_callback_t)(const char *args);
// functions for telnet communication
    char * memory_dump(char *args);
    char * write_byte(char *args);
    char * write_word(char *args);
    char * write_func(char *args);
    char * read_word(char *args);
    char * resolve_symbol(char *args);
    char * run_func(char *args);

    static struct {
        function_callback_t func_ptr;
        char *func_name;
    } func_table[] = {
        {(function_callback_t) memory_dump, "memory_dump"},
        {(function_callback_t) write_byte, "wryte_byte"},
        {(function_callback_t) write_word, "write_word"},
        {(function_callback_t) write_func, "write_func"},
        {(function_callback_t) read_word, "read_word"},
        {(function_callback_t) resolve_symbol, "resolve_symbol"},
        {(function_callback_t) run_func, "run_func"}
    };
#endif