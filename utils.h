#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <ncurses.h>

typedef enum
{
    STANDARD,
    MOVE,
} MODE;


void printll (WINDOW * win, const char *text, int height, int width);
char * createll (char * filename, uint8_t isDirty, MODE mode, uint8_t col, uint8_t lne);
char * mstringify (MODE mode);

#endif
