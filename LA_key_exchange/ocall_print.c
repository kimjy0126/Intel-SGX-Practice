#include <stdio.h>

void ocall_prints(char* str)
{
    printf("%s", str);
}

void ocall_printx(char ch)
{
    printf("%02x", (unsigned char)ch);
}

void ocall_printe(char* str, int n)
{
    printf("%s (ret: 0x%x)\n", str, (unsigned int)n);
}
