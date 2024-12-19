#ifndef IO_H
#define IO_H

#include "editor.h"

// File I/O
int open_file(EditorState *editor, const char *filename);
int save_file(EditorState *editor);

// Terminal handling
void enable_raw_mode();
void disable_raw_mode();

#endif