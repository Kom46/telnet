// #define TEST
#ifdef TEST

#define _GNU_SOURCE
#include "unity.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <dlfcn.h>

#include "internals/functions.h"

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

void setUp(void)
{
}

void tearDown(void)
{
}

void test_memory_dump(void)
{
#define ELEMS_COUNT 15
#define EIGHT_CELLS_PATTERN "0x%02x, 0x%02x, 0x%02x, 0x%02x 0x%02x, 0x%02x, 0x%02x, 0x%02x, // %c%c%c%c%c%c%c%c\r\n"
#define ONE_SELL_PATTERN ", 0x%02x // %c%c%c%c%c%c%c\r\n,"
#define EIGHT_SELL_DATA(data, i) data[(i)], data[(i) + 1], data[(i) + 2], data[(i) + 3], data[(i) + 4], data[(i) + 5], \
                                 data[(i) + 6], data[(i) + 7]
#define CHAR_PRINT(a) ((a) >= 32 ? a : ' ')
#define EIGHT_CHAR_PRINT(data, i) CHAR_PRINT(data[(i)]), CHAR_PRINT(data[(i) + 1]), CHAR_PRINT(data[(i + 2)]),     \
                                  CHAR_PRINT(data[(i) + 3]), CHAR_PRINT(data[(i) + 4]), CHAR_PRINT(data[(i) + 5]), \
                                  CHAR_PRINT(data[(i) + 6]), CHAR_PRINT(data[(i) + 7])
    unsigned char mem[ELEMS_COUNT] = {0};
    for (int i = 0; i < ELEMS_COUNT; i++)
    {
        memcpy(&mem[i], &i, sizeof(typeof(mem[0])));
    }
    size_t size = 0;
    size += snprintf(NULL, 0, EIGHT_CELLS_PATTERN, 0, 0, 0, 0, 0, 0, 0, 0, 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a') * (ELEMS_COUNT / 8);
    size += (snprintf(NULL, 0, " 0x%02x,", 00) * (ELEMS_COUNT % 8)) + snprintf(NULL, 0, " // %c%c%c%c%c%c%c", 'a', 'a', 'a', 'a');
    // for final \r\n
    size += 2;
    int counter = ELEMS_COUNT;
    char answer[size];
    bzero(answer, size);
    for (int i = 0; i < ELEMS_COUNT; i += 8)
    {

        if (counter >= 8)
        {
            char tmp[snprintf(NULL, 0, EIGHT_CELLS_PATTERN)];
            sprintf(tmp, EIGHT_CELLS_PATTERN, EIGHT_SELL_DATA(mem, i), EIGHT_CHAR_PRINT(mem, i));
            strcat(answer, tmp);
        }
        else
        {
            for (size_t j = 0; j < (ELEMS_COUNT % 8); j++)
            {
                char tmp[snprintf(NULL, 0, ONE_SELL_PATTERN)];
                static char tmp2[7] = {0};
                tmp2[j] = CHAR_PRINT(mem[i + j]);
                if (j == 0)
                {
                    sprintf(tmp, "0x%02x", mem[i + j]);
                }
                else
                {
                    sprintf(tmp, ", 0x%02x", mem[i + j]);
                }
                if (j == ((ELEMS_COUNT % 8) - 1))
                {
                    sprintf(tmp, "%s // %s", tmp, tmp2);
                    strcat(tmp, "\r\n");
                }
                strcat(answer, tmp);
            }
        }
        counter -= 8;
    }
    char request[snprintf(NULL, 0, "%p %d", (void *)mem, ELEMS_COUNT)];
    sprintf(request, "%p %d", (void *)mem, ELEMS_COUNT);
    char *result = NULL;
    result = memory_dump(request);
    TEST_ASSERT_EQUAL_STRING(answer, result);
    free(result);
}

void test_write_byte(void)
{
    unsigned char byte = 0;
    unsigned char write_val = 5;
    char request[snprintf(NULL, 0, "%p %hhu", &byte, write_val)];
    sprintf(request, "%p %hhu", &byte, write_val);
    char *result = write_byte(request);
    TEST_ASSERT_EQUAL_HEX(write_val, byte);
    free(result);
    char request2[snprintf(NULL, 0, "%p 0x%x", &byte, write_val)];
    sprintf(request2, "%p 0x%x", &byte, write_val);
    result = write_byte(request);
    TEST_ASSERT_EQUAL_HEX(write_val, byte);
    free(result);
}

void test_write_halfword(void)
{
    unsigned short halfword = 0;
    unsigned short write_val = 10;
    char *result = NULL;
    char request[snprintf(NULL, 0, "%p %hu", &halfword, write_val)];
    sprintf(request, "%p %hu", &halfword, write_val);
    result = write_word(request);
    TEST_ASSERT_EQUAL_HEX(write_val, halfword);
    free(result);
    char request2[snprintf(NULL, 0, "%p 0x%04x", &halfword, write_val)];
    sprintf(request2, "%p 0x%04x", &halfword, write_val);
    result = write_halfword(request2);
    TEST_ASSERT_EQUAL_HEX(write_val, halfword);
    free(result);
}

void test_write_word(void)
{
    unsigned long word = 0;
    unsigned long write_val = 10;
    char *result = NULL;
    char request[snprintf(NULL, 0, "%p %u", &word, write_val)];
    sprintf(request, "%p %u", &word, write_val);
    result = write_word(request);
    TEST_ASSERT_EQUAL_HEX(write_val, word);
    free(result);
    char request2[snprintf(NULL, 0, "%p 0x%x", &word, write_val)];
    sprintf(request2, "%p 0x%x", &word, write_val);
    result = write_word(request2);
    TEST_ASSERT_EQUAL_HEX(write_val, word);
    free(result);
}

void test_write_func(void)
{
    unsigned char data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    unsigned char ptr[ARR_SIZE(data)];
    char request[255] = {0};
    memset(ptr, 0, ARR_SIZE(data));
    sprintf(request, "%p", ptr);
    for (int i = 0; i < ARR_SIZE(data); i++)
    {
        size_t size = snprintf(NULL, 0, " 0x%x", data[i]);
        char tmp[size];
        sprintf(tmp, " 0x%x", data[i]);
        strcat(request, tmp);
    }
    char *result = write_func(request);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, ptr, ARR_SIZE(data));
    free(result);
}

void test_read_byte(void)
{
    unsigned char byte = 10;
    char request[snprintf(NULL, 0, "%p", &byte)];
    sprintf(request, "%p", &byte);
    char response[snprintf(NULL, 0, "%p: 0x%02x", &byte, byte)];
    sprintf(response, "%p = 0x%02x", &byte, byte);
    char *result = read_byte(request);
    TEST_ASSERT_EQUAL_STRING(response, result);
    free(result);
}

void test_read_halfword(void)
{
    unsigned short halfword = 10;
    char request[snprintf(NULL, 0, "%p", &halfword)];
    sprintf(request, "%p", &halfword);
    char response[snprintf(NULL, 0, "%p = 0x%04x", &halfword, halfword)];
    sprintf(response, "%p = 0x%04x", &halfword, halfword);
    char *result = read_halfword(request);
    TEST_ASSERT_EQUAL_STRING(response, result);
    free(result);
}
void test_read_word(void)
{
    unsigned long word = 10;
    char request[snprintf(NULL, 0, "%p", &word)];
    sprintf(request, "%p", &word);
    char response[snprintf(NULL, 0, "%p = 0x%08x", &word, word)];
    sprintf(response, "%p = 0x%08x", &word, word);
    char *result = read_word(request);
    TEST_ASSERT_EQUAL_STRING(response, result);
    free(result);
}

#ifndef EXPORT_API
#define EXPORT_API __attribute__((visibility("default")))
#endif

EXPORT_API int foo(void)
{
    return 0;
};

void test_resolve_symbol(void)
{
    char *symbol_name = "malloc";
    void *fn = &malloc;
    char request[snprintf(NULL, 0, "%s", symbol_name)];
    sprintf(request, "%s", symbol_name);
    char response[snprintf(NULL, 0, "symbol '%s' at %p", symbol_name, fn)];
    sprintf(response, "symbol '%s' at %p", symbol_name, fn);
    char *result = resolve_symbol(request);
    TEST_ASSERT_EQUAL_STRING(response, result);
    printf("%s\n", result);
    free(result);
}

void test_resolve_function(void)
{
    char *result = NULL;
    char *symbol_name = "malloc";
    void *ptr = dlsym(NULL, symbol_name);
    Dl_info info = {0};
    dladdr(ptr, &info);
    char response[snprintf(NULL, 0,
                            "Address '%p' located at %s within the program %s",
                                ptr, info.dli_fname, info.dli_sname ?: "NULL")];
    sprintf(response, "Address '%p' located at %s within the program %s",
                        ptr, info.dli_fname, info.dli_sname ?: "NULL");
    char request[snprintf(NULL, 0, "%p", malloc)];
    sprintf(request, "%p", malloc);
    result = resolve_function(request);
    TEST_ASSERT_EQUAL_STRING(response, result);
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
    char request[snprintf(NULL, 0, "%p", &bar)];
    sprintf(request, "%p", &bar);
    char *result = run_func(request);
    TEST_ASSERT_EQUAL(new_val, a);
    printf("%s\n", result);
    free(result);
    char request2[snprintf(NULL, 0, "%s(%d,%d)", "calloc", "10", "1")];
    sprintf(request2, "%s(%d,%d)", "calloc", 10, 1);
    result = run_func(request2);
    printf("%s\n", result);
    free(result);
}

void test_get_func_by_name(void)
{
    TEST_ASSERT_EQUAL_PTR(memory_dump, get_func_by_name("mem_dump"));
    TEST_ASSERT_EQUAL_PTR(write_byte, get_func_by_name("w_u8"));
    TEST_ASSERT_EQUAL_PTR(write_halfword, get_func_by_name("w_u16"));
    TEST_ASSERT_EQUAL_PTR(write_word, get_func_by_name("w_u32"));
    TEST_ASSERT_EQUAL_PTR(write_func, get_func_by_name("mem_write"));
    TEST_ASSERT_EQUAL_PTR(read_byte, get_func_by_name("r_u8"));
    TEST_ASSERT_EQUAL_PTR(read_halfword, get_func_by_name("r_u16"));
    TEST_ASSERT_EQUAL_PTR(read_word, get_func_by_name("r_u32"));
    TEST_ASSERT_EQUAL_PTR(resolve_symbol, get_func_by_name("s"));
    TEST_ASSERT_EQUAL_PTR(resolve_function, get_func_by_name("r"));
    TEST_ASSERT_EQUAL_PTR(run_func, get_func_by_name("c"));
}
#endif // TEST
