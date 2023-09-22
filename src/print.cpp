//
// Created by os on 8/4/23.
//
#include "../h/print.hpp"
#include "../lib/console.h"

void printstring(char const *string)
{
    while (*string != '\0'){
        __putc(*string);
        string++;
    }
}

void printinteger(uint64 integer)
{
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0){
        neg = 1;
        x = -integer;
    } else{ x = integer;}

    i = 0;
    do{
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { __putc(buf[i]); }
}

void printhexadecimal(uint64 integer) {
    static char digits[] = "0123456789abcdef";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0){
        neg = 1;
        x = -integer;
    } else{ x = integer;}

    i = 0;
    do{
        buf[i++] = digits[x % 16];
    } while ((x /= 16) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { __putc(buf[i]); }
}
