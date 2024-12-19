#include "io.h"
#include <stdio.h>
#include <stdlib.h>

int open_file(EditorState *editor, const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) return -1;

    int itemsread = fread(editor->buffer, sizeof(char), editor->screen_cols * editor->screen_rows, file);
    fclose(file);

    return itemsread;
}

int save_file(EditorState *editor) 
{
    FILE *file = fopen(editor->filename, "w");
    if (file == NULL) return -1;

    char * str = editor->buffer;
    int itemswritten = fwrite(str, sizeof(char), editor->cursor_x * (editor->cursor_y+1), file);
    fclose(file);

    if (itemswritten < editor->cursor_x * (editor->cursor_y+1)) {
        return -2;
    }

    return itemswritten;
}