#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "io.h"

void init();

int main(int argc, char ** argv) 
{
    if (argc != 2) return EXIT_FAILURE;
    if (strlen(argv[1]) >= 256) return EXIT_FAILURE;

    init();
    EditorState editor;
    init_editor(&editor, argv[1]);

    while (1) {
        render_screen(&editor);
        int ch = getch();
        int code = process_input(&editor, ch);
        if (code == 1) break;
        refresh();
    }
    
    free(editor.filename);
    free(editor.buffer);
    endwin();  
    return 0;
}

void init()
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    noecho();
    cbreak();
}