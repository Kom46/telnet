#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
// function prototype for telnet communication
    typedef char *(*function_callback_t)(const char *args);
#ifdef TEST
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
    char * resolve_function(char *args);
    char * run_func(char *args);
#endif

    function_callback_t get_func_by_name(char *name);
#endif