#include "utils.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

void printll(WINDOW *win, const char *text, int height, int width) 
{
    wattron(win, COLOR_PAIR(1));
    mvwhline(win, height - 1, 0, ' ', width);
    mvwprintw(win, height - 1, 0, "%s", text);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}

char * createll (char * filename, uint8_t isDirty, MODE mode, uint8_t col, uint8_t lne)
{
    char * dirty = "(*)";
    char * modeString = mstringify(mode);
    char cursorStr[17] = {0};
    if (snprintf(cursorStr, sizeof(cursorStr), "col=%d, lne=%d", col, lne) < 0) return "";
    
    char * ll;
    if (isDirty == 1) ll = malloc(
        sizeof(char)
         * (strlen(filename)
          + strlen(modeString)
           + strlen(cursorStr)
            + strlen(dirty)));
    else ll = malloc(
        sizeof(char)
         * (strlen(filename)
          + strlen(modeString)
           + strlen(cursorStr)));
    ll[0] = '\0';

    strcat(ll, filename);
    if (isDirty == 1) strcat(ll, dirty);
    strcat(ll, modeString);
    strcat(ll, cursorStr);

    return ll;
}

char * mstringify(MODE mode)
{
    if (mode == STANDARD) return " | -- STANDARD -- | ";
    if (mode == MOVE) return " | -- MOVE -- | ";
    return " | -- UNKNOWN MODE -- | ";
}