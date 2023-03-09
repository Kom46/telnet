#include "inc/internals/functions.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"
















void setUp(void)

{

}



void tearDown(void)

{

}



void test_memory_dump(void)

{

    unsigned char mem[15] = {0};

    for (int i = 0; i < 15; i++)

    {

        memcpy(&mem[i], &i, sizeof(typeof(mem[0])));

    }

    size_t size = 0;

    size += snprintf(

                    ((void *)0)

                        , 0, "0x%02x, 0x%02x, 0x%02x, 0x%02x 0x%02x, 0x%02x, 0x%02x, 0x%02x, // %c%c%c%c%c%c%c%c\r\n", 0, 0, 0, 0, 0, 0, 0, 0, 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a') * (15 / 8);

    size += (snprintf(

                     ((void *)0)

                         , 0, " 0x%02x,", 00) * (15 % 8)) + snprintf(

                                                                              ((void *)0)

                                                                                  , 0, " // %c%c%c%c%c%c%c", 'a', 'a', 'a', 'a');



    size += 2;

    int counter = 15;

    char answer[size];

    bzero(answer, size);

    for (int i = 0; i < 15; i += 8)

    {



        if (counter >= 8)

        {

            char tmp[snprintf(

                             ((void *)0)

                                 , 0, "0x%02x, 0x%02x, 0x%02x, 0x%02x 0x%02x, 0x%02x, 0x%02x, 0x%02x, // %c%c%c%c%c%c%c%c\r\n")];

            sprintf(tmp, "0x%02x, 0x%02x, 0x%02x, 0x%02x 0x%02x, 0x%02x, 0x%02x, 0x%02x, // %c%c%c%c%c%c%c%c\r\n", mem[(i)], mem[(i) + 1], mem[(i) + 2], mem[(i) + 3], mem[(i) + 4], mem[(i) + 5], mem[(i) + 6], mem[(i) + 7], ((mem[(i)]) >= 32 ? mem[(i)] : ' '), ((mem[(i) + 1]) >= 32 ? mem[(i) + 1] : ' '), ((mem[(i + 2)]) >= 32 ? mem[(i + 2)] : ' '), ((mem[(i) + 3]) >= 32 ? mem[(i) + 3] : ' '), ((mem[(i) + 4]) >= 32 ? mem[(i) + 4] : ' '), ((mem[(i) + 5]) >= 32 ? mem[(i) + 5] : ' '), ((mem[(i) + 6]) >= 32 ? mem[(i) + 6] : ' '), ((mem[(i) + 7]) >= 32 ? mem[(i) + 7] : ' '));

            strcat(answer, tmp);

        }

        else

        {

            for (size_t j = 0; j < (15 % 8); j++)

            {

                char tmp[snprintf(

                                 ((void *)0)

                                     , 0, ", 0x%02x // %c%c%c%c%c%c%c\r\n,")];

                static char tmp2[7] = {0};

                tmp2[j] = ((mem[i + j]) >= 32 ? mem[i + j] : ' ');

                if (j == 0)

                {

                    sprintf(tmp, "0x%02x", mem[i + j]);

                }

                else

                {

                    sprintf(tmp, ", 0x%02x", mem[i + j]);

                }

                if (j == ((15 % 8) - 1))

                {

                    sprintf(tmp, "%s // %s", tmp, tmp2);

                    strcat(tmp, "\r\n");

                }

                strcat(answer, tmp);

            }

        }

        counter -= 8;

    }

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p %d", (void *)mem, 15)];

    sprintf(request, "%p %d", (void *)mem, 15);

    char *result = 

                  ((void *)0)

                      ;

    result = memory_dump(request);

    UnityAssertEqualString((const char*)((answer)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(85));

    free(result);

}



void test_write_byte(void)

{

    unsigned char byte = 0;

    unsigned char write_val = 5;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p %hhu", &byte, write_val)];

    sprintf(request, "%p %hhu", &byte, write_val);

    char *result = write_byte(request);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((byte)), (

   ((void *)0)

   ), (UNITY_UINT)(96), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%p 0x%x", &byte, write_val)];

    sprintf(request2, "%p 0x%x", &byte, write_val);

    result = write_byte(request);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((byte)), (

   ((void *)0)

   ), (UNITY_UINT)(101), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

}



void test_write_halfword(void)

{

    unsigned short halfword = 0;

    unsigned short write_val = 10;

    char *result = 

                  ((void *)0)

                      ;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p %hu", &halfword, write_val)];

    sprintf(request, "%p %hu", &halfword, write_val);

    result = write_word(request);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((halfword)), (

   ((void *)0)

   ), (UNITY_UINT)(113), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%p 0x%04x", &halfword, write_val)];

    sprintf(request2, "%p 0x%04x", &halfword, write_val);

    result = write_halfword(request2);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((halfword)), (

   ((void *)0)

   ), (UNITY_UINT)(118), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

}



void test_write_word(void)

{

    unsigned long word = 0;

    unsigned long write_val = 10;

    char *result = 

                  ((void *)0)

                      ;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p %u", &word, write_val)];

    sprintf(request, "%p %u", &word, write_val);

    result = write_word(request);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((word)), (

   ((void *)0)

   ), (UNITY_UINT)(130), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%p 0x%x", &word, write_val)];

    sprintf(request2, "%p 0x%x", &word, write_val);

    result = write_word(request2);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((word)), (

   ((void *)0)

   ), (UNITY_UINT)(135), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

}



void test_write_func(void)

{

    unsigned char data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

    unsigned char ptr[(sizeof(data) / sizeof(data[0]))];

    char request[255] = {0};

    memset(ptr, 0, (sizeof(data) / sizeof(data[0])));

    sprintf(request, "%p", ptr);

    for (int i = 0; i < (sizeof(data) / sizeof(data[0])); i++)

    {

        size_t size = snprintf(

                              ((void *)0)

                                  , 0, " 0x%x", data[i]);

        char tmp[size];

        sprintf(tmp, " 0x%x", data[i]);

        strcat(request, tmp);

    }

    char *result = write_func(request);

    UnityAssertEqualIntArray(( const void*)((data)), ( const void*)((ptr)), (UNITY_UINT32)(((sizeof(data) / sizeof(data[0])))), (

   ((void *)0)

   ), (UNITY_UINT)(154), UNITY_DISPLAY_STYLE_UINT8, UNITY_ARRAY_TO_ARRAY);

    free(result);

}



void test_read_byte(void)

{

    unsigned char byte = 10;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p", &byte)];

    sprintf(request, "%p", &byte);

    char response[snprintf(

                          ((void *)0)

                              , 0, "%p: 0x%02x", &byte, byte)];

    sprintf(response, "%p = 0x%02x", &byte, byte);

    char *result = read_byte(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(166));

    free(result);

}



void test_read_halfword(void)

{

    unsigned short halfword = 10;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p", &halfword)];

    sprintf(request, "%p", &halfword);

    char response[snprintf(

                          ((void *)0)

                              , 0, "%p = 0x%04x", &halfword, halfword)];

    sprintf(response, "%p = 0x%04x", &halfword, halfword);

    char *result = read_halfword(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(178));

    free(result);

}

void test_read_word(void)

{

    unsigned long word = 10;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p", &word)];

    sprintf(request, "%p", &word);

    char response[snprintf(

                          ((void *)0)

                              , 0, "%p = 0x%08x", &word, word)];

    sprintf(response, "%p = 0x%08x", &word, word);

    char *result = read_word(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(189));

    free(result);

}











__attribute__((visibility("default"))) int foo(void)

{

    return 0;

};



void test_resolve_symbol(void)

{

    char *symbol_name = "malloc";

    void *fn = &malloc;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%s", symbol_name)];

    sprintf(request, "%s", symbol_name);

    char response[snprintf(

                          ((void *)0)

                              , 0, "symbol '%s' at %p", symbol_name, fn)];

    sprintf(response, "symbol '%s' at %p", symbol_name, fn);

    char *result = resolve_symbol(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(211));

    printf("%s\n", result);

    free(result);

}



void test_resolve_function(void)

{

    char *result = 

                  ((void *)0)

                      ;

    char *symbol_name = "malloc";

    void *ptr = dlsym(

                     ((void *)0)

                         , symbol_name);

    Dl_info info = {0};

    dladdr(ptr, &info);

    char response[snprintf(

                          ((void *)0)

                              , 0,

                            "Address '%p' located at %s within the program %s",

                                ptr, info.dli_fname, info.dli_sname ?: "NULL")];

    sprintf(response, "Address '%p' located at %s within the program %s",

                        ptr, info.dli_fname, info.dli_sname ?: "NULL");

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p", malloc)];

    sprintf(request, "%p", malloc);

    result = resolve_function(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(231));

    printf("%s", result);

    free(result);

}



void test_run_func(void)

{

    int a = 0;

    int new_val = 10;

    int bar(void)

    {

        a = new_val;

        return 0;

    }

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p", &bar)];

    sprintf(request, "%p", &bar);

    char *result = run_func(request);

    UnityAssertEqualNumber((UNITY_INT)((new_val)), (UNITY_INT)((a)), (

   ((void *)0)

   ), (UNITY_UINT)(248), UNITY_DISPLAY_STYLE_INT);

    printf("%s\n", result);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%s(%d,%d)", "calloc", "10", "1")];

    sprintf(request2, "%s(%d,%d)", "calloc", 10, 1);

    result = run_func(request2);

    printf("%s\n", result);

    free(result);

}



void test_get_func_by_name(void)

{

    UnityAssertEqualNumber((UNITY_INT64)((memory_dump)), (UNITY_INT64)((get_func_by_name("mem_dump"))), (

   ((void *)0)

   ), (UNITY_UINT)(260), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_byte)), (UNITY_INT64)((get_func_by_name("w_u8"))), (

   ((void *)0)

   ), (UNITY_UINT)(261), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_halfword)), (UNITY_INT64)((get_func_by_name("w_u16"))), (

   ((void *)0)

   ), (UNITY_UINT)(262), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_word)), (UNITY_INT64)((get_func_by_name("w_u32"))), (

   ((void *)0)

   ), (UNITY_UINT)(263), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_func)), (UNITY_INT64)((get_func_by_name("mem_write"))), (

   ((void *)0)

   ), (UNITY_UINT)(264), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((read_byte)), (UNITY_INT64)((get_func_by_name("r_u8"))), (

   ((void *)0)

   ), (UNITY_UINT)(265), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((read_halfword)), (UNITY_INT64)((get_func_by_name("r_u16"))), (

   ((void *)0)

   ), (UNITY_UINT)(266), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((read_word)), (UNITY_INT64)((get_func_by_name("r_u32"))), (

   ((void *)0)

   ), (UNITY_UINT)(267), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((resolve_symbol)), (UNITY_INT64)((get_func_by_name("s"))), (

   ((void *)0)

   ), (UNITY_UINT)(268), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((resolve_function)), (UNITY_INT64)((get_func_by_name("r"))), (

   ((void *)0)

   ), (UNITY_UINT)(269), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((run_func)), (UNITY_INT64)((get_func_by_name("c"))), (

   ((void *)0)

   ), (UNITY_UINT)(270), UNITY_DISPLAY_STYLE_HEX64);

}
