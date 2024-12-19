#ifndef EDITOR_H
#define EDITOR_H

#include <stddef.h>
#include "utils.h"

#define CTRL(k) ((k) & 0x1F)

// Editor state structure
typedef struct {
    int cursor_x, cursor_y;    // Cursor position
    int screen_rows, screen_cols; // Screen dimensions
    char *filename;            // File name (not used yet)
    char *buffer;              // Text buffer
    int dirty;                 // Unsaved changes flag
    MODE mode;
} EditorState;

// Function declarations
void init_editor(EditorState *editor, char * filename);
int process_input(EditorState *editor, int ch);
void render_screen(const EditorState *editor);

#endif
