// #ifdef TEST

// #include "unity.h"

#include "libdebug.h"

unsigned char export_char = 5;
unsigned short export_halfword = 10;
unsigned long export_word = 15;
unsigned char export_mem[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

int main(int argc, char *argv[])
{
    init_telnet_server();
    for (;;)
    {
    }
}

// #endif // TEST
