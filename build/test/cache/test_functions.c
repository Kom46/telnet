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



    unsigned long mem[15] = {0};

    char answer[255] = {0};

    sprintf(answer, "%p:", (void *)mem);

    for (int i = 0; i < 15; i++)

    {

        memcpy(&mem[i], &i, sizeof(typeof(mem[0])));

        char str[snprintf(

                         ((void *)0)

                             , 0, " 0x%x", i)];

        sprintf(str, " 0x%x", i);

        strcat(answer, str);

    }

    char request[snprintf(

                         ((void *)0)

                             , 0, "%p %d", (void *)mem, 15)];

    sprintf(request, "%p %d", (void *)mem, 15);

    char *result = memory_dump(request);

    UnityAssertEqualString((const char*)((answer)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(37));

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

   ), (UNITY_UINT)(48), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%p 0x%x", &byte, write_val)];

    sprintf(request2, "%p 0x%x", &byte, write_val);

    result = write_byte(request);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((byte)), (

   ((void *)0)

   ), (UNITY_UINT)(53), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

}



void test_write_halfword(void)

{

    unsigned short halfword = 0;

    unsigned long write_val = 10;

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

   ), (UNITY_UINT)(65), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%p 0x%x", &halfword, write_val)];

    sprintf(request2, "%p 0x%x", &halfword, write_val);

    result = write_halfword(request2);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((halfword)), (

   ((void *)0)

   ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_HEX32);

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

   ), (UNITY_UINT)(82), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

    char request2[snprintf(

                          ((void *)0)

                              , 0, "%p 0x%x", &word, write_val)];

    sprintf(request2, "%p 0x%x", &word, write_val);

    result = write_word(request2);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((write_val)), (UNITY_INT)(UNITY_INT32)((word)), (

   ((void *)0)

   ), (UNITY_UINT)(87), UNITY_DISPLAY_STYLE_HEX32);

    free(result);

}



void test_write_func(void)

{

    unsigned char data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

    unsigned char ptr[(sizeof(data)/sizeof(data[0]))];

    char request[255] = {0};

    memset(ptr, 0, (sizeof(data)/sizeof(data[0])));

    sprintf(request, "%p", ptr);

    for (int i = 0; i < (sizeof(data)/sizeof(data[0])); i++) {

        size_t size = snprintf(

                              ((void *)0)

                                  , 0, " 0x%x", data[i]);

        char tmp[size];

        sprintf(tmp, " 0x%x", data[i]);

        strcat(request, tmp);

    }

    char *result = write_func(request);

    UnityAssertEqualIntArray(( const void*)((data)), ( const void*)((ptr)), (UNITY_UINT32)(((sizeof(data)/sizeof(data[0])))), (

   ((void *)0)

   ), (UNITY_UINT)(105), UNITY_DISPLAY_STYLE_UINT8, UNITY_ARRAY_TO_ARRAY);

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

                              , 0, "%p: 0x%x", &byte, byte)];

    sprintf(response, "%p: 0x%x", &byte, byte);

    char *result = read_byte(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(117));

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

                              , 0, "%p: 0x%x", &halfword, halfword)];

    sprintf(response, "%p: 0x%x", &halfword, halfword);

    char *result = read_halfword(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(129));

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

                              , 0, "%p: 0x%x", &word, word)];

    sprintf(response, "%p: 0x%x", &word, word);

    char *result = read_word(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(140));

    free(result);

}











__attribute__((visibility("default"))) int foo(void)

{

    return 0;

};



void test_resolve_symbol(void)

{

    char *symbol_name = "malloc";

    int (*fn)(void);

    fn = &malloc;

    char request[snprintf(

                         ((void *)0)

                             , 0, "%s", symbol_name)];

    sprintf(request, "%s", symbol_name);

    char response[snprintf(

                          ((void *)0)

                              , 0, "%s: %p", symbol_name, fn)];

    sprintf(response, "%s: %p", symbol_name, fn);

    char *result = resolve_symbol(request);

    UnityAssertEqualString((const char*)((response)), (const char*)((result)), (

   ((void *)0)

   ), (UNITY_UINT)(163));

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

   ), (UNITY_UINT)(179), UNITY_DISPLAY_STYLE_INT);

    free(result);

}



void test_get_func_by_name(void) {

    UnityAssertEqualNumber((UNITY_INT64)((memory_dump)), (UNITY_INT64)((get_func_by_name("mem_dump"))), (

   ((void *)0)

   ), (UNITY_UINT)(184), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_byte)), (UNITY_INT64)((get_func_by_name("s_u8"))), (

   ((void *)0)

   ), (UNITY_UINT)(185), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_halfword)), (UNITY_INT64)((get_func_by_name("s_u16"))), (

   ((void *)0)

   ), (UNITY_UINT)(186), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_word)), (UNITY_INT64)((get_func_by_name("s_u32"))), (

   ((void *)0)

   ), (UNITY_UINT)(187), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((write_func)), (UNITY_INT64)((get_func_by_name("mem_write"))), (

   ((void *)0)

   ), (UNITY_UINT)(188), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((read_byte)), (UNITY_INT64)((get_func_by_name("r_u8"))), (

   ((void *)0)

   ), (UNITY_UINT)(189), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((read_halfword)), (UNITY_INT64)((get_func_by_name("r_u16"))), (

   ((void *)0)

   ), (UNITY_UINT)(190), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((read_word)), (UNITY_INT64)((get_func_by_name("r_u32"))), (

   ((void *)0)

   ), (UNITY_UINT)(191), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((resolve_symbol)), (UNITY_INT64)((get_func_by_name("r"))), (

   ((void *)0)

   ), (UNITY_UINT)(192), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)((run_func)), (UNITY_INT64)((get_func_by_name("c"))), (

   ((void *)0)

   ), (UNITY_UINT)(193), UNITY_DISPLAY_STYLE_HEX64);

}
