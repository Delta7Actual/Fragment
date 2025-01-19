#include "editor.h"
#include "utils.h"
#include "io.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

int handle_standard(EditorState * editor, int ch);
int handle_move(EditorState * editor, char ch);

void init_editor(EditorState * editor, char * filename)
{
    editor->cursor_x = 0;
    editor->cursor_y = 0;
    editor->dirty = 0;
    editor->mode = STANDARD;

    editor->filename = malloc(sizeof(char) * strlen(filename) + 1);
    if (editor->filename == NULL)
    {
        endwin();
        save_file(editor);
        exit(EXIT_FAILURE);
    }
    strcpy(editor->filename, filename);

    getmaxyx(stdscr, editor->screen_rows, editor->screen_cols);

    editor->buffer = malloc(editor->screen_rows * editor->screen_cols);
    memset(editor->buffer, '\0', editor->screen_rows * editor->screen_cols);

    int itemsread = open_file(editor, filename);
    if (itemsread == -1) {
        memset(editor->buffer, ' ', editor->screen_rows * editor->screen_cols);
        itemsread = 0;
    }

    editor->cursor_y = itemsread / editor->screen_cols;
    editor->cursor_x = itemsread % editor->screen_cols;
}


int process_input(EditorState * editor, int ch)
{
    if (ch == CTRL('X'))
    {
        return 1; // Exit
    }
    if (ch == CTRL('D'))
    {
        save_file(editor);
        editor->dirty = 0;
        return 0;
    }
    if (ch == CTRL('W'))
    {
        editor->mode = (editor->mode == STANDARD) ? MOVE : STANDARD;
        return 0;
    }

    if (editor->mode == STANDARD) return handle_standard(editor, ch);
    if (editor->mode == MOVE) return handle_move(editor, ch);
    
    return -1;
}

int handle_standard(EditorState * editor, int ch)
{
    if (ch >= 32 && ch <= 126) // Printable characters
    {
        int pos = editor->cursor_y * editor->screen_cols + editor->cursor_x;
        editor->buffer[pos] = ch;

        // Move the cursor
        editor->cursor_x++;
        if (editor->cursor_x >= editor->screen_cols) {
            editor->cursor_x = 0;
            editor->cursor_y++;
            if (editor->cursor_y >= editor->screen_rows) {
                editor->cursor_y = editor->screen_rows - 1;
            }
        }
    }
    else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b')
    {
        // Move the cursor
        if (editor->cursor_y > 0 || editor->cursor_x > 0) 
        {
            if (editor->cursor_x == 0) {
                editor->cursor_y--;
                editor->cursor_x = editor->screen_cols - 1;
            } else {
                editor->cursor_x--;
            }
        }    

        int pos = editor->cursor_y * editor->screen_cols + editor->cursor_x;
        editor->buffer[pos] = ' ';
    }
    else if(ch == '\n' || ch == '\r')
    {
        int pos = editor->cursor_y * editor->screen_cols + editor->cursor_x;
        int lastPos = pos + (editor->screen_cols - editor->cursor_x);

        // Pad the rest of the line with spaces
        for (int i = pos; i < lastPos; i++)
        {
            editor->buffer[i] = ' ';
        }

        // Add newline character
        editor->buffer[lastPos - 1] = '\n';

        // Move the cursor to the next line
        if (editor->cursor_y < editor->screen_rows - 1)
        {
            editor->cursor_y++;
            editor->cursor_x = 0;
        }
        else
        {
            editor->cursor_y = editor->screen_rows - 1; // Stay at the bottom
            // Optional: implement scrolling here if needed
        }
    }

    editor->dirty = 1;
    return 0;
}

int handle_move(EditorState * editor, char ch)
{
    if (ch == 'w' || ch == 'W')
    {
        if (editor->cursor_y > 0)
        {
            editor->cursor_y--;
        }
    }
    else if (ch == 'd' || ch == 'D')
    {
        if (editor->cursor_x < editor->screen_cols - 1)
        {
            editor->cursor_x++;
        }
        else
        {
            if (editor->cursor_y < editor->screen_rows - 1)
            {
                editor->cursor_y++;
                editor->cursor_x = 0;
            }
        }
    }
    else if (ch == 's' || ch == 'S')
    {
        if (editor->cursor_y < editor->screen_rows - 1)
        {
            editor->cursor_y++;
        }
    }
    else if (ch == 'a' || ch == 'A')
    {
        if (editor->cursor_x > 0)
        {
            editor->cursor_x--;
        }
        else
        {
            if (editor->cursor_y > 0)
            {
                editor->cursor_y--;
                editor->cursor_x = editor->screen_cols - 1;
            }
        }
    }
    return 0;  
}

void render_screen(const EditorState * editor) 
{
    clear();

    for (int row = 0; row < editor->screen_rows; row++)
    {
        mvprintw(row, 0, "%.*s", editor->screen_cols,
                 editor->buffer + row * editor->screen_cols);
    }
    printw("%d,%d", editor->cursor_x, editor->cursor_y);
    char *status_line = createll(editor->filename, editor->dirty, editor->mode, editor->cursor_x, editor->cursor_y);
    printll(stdscr, status_line, editor->screen_rows, editor->screen_cols);
    free(status_line);

    // Move cursor to the current position
    move(editor->cursor_y, editor->cursor_x);
    refresh();
}