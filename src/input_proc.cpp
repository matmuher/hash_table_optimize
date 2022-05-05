#include "input_proc.h"

int get_size (FILE* file_pointer)
{
    assert (file_pointer != NULL);

    size_t cur_pos = ftell (file_pointer);

    // Get size
    fseek (file_pointer, 0, SEEK_END);
    int file_size = ftell (file_pointer);

    // Return internal pointer
    fseek (file_pointer, cur_pos, SEEK_SET);

    return file_size;
}

int is_eof (FILE* file_pointer)
{
    assert (file_pointer != NULL);

    int cur_pos = ftell (file_pointer);
    int file_size = get_size (file_pointer);

    return cur_pos == file_size;
}

char* format_line (char* line, line_buf* line_ptrs)
{
    assert (line != NULL);
    assert (line_ptrs != NULL);

    // Skip non-al/num chars
    int sym_id = 0;
    while (!isalnum (line[sym_id])) {sym_id++;}

    // Start with lowercase
    //line[sym_id] = tolower (line[sym_id]);

    // beg_ptr points to the first char in line
    line_ptrs->beg_ptr = &line[sym_id];

    // Stops when find zero or end of line
    char* last_alnum = &line[sym_id];
    while (line[++sym_id])
        {
        if (isalnum (line[sym_id]))
            {
            last_alnum = &line[sym_id];
            }
        }

    // End of line — straightly after last al/num symbol
    *++last_alnum = '\0';

    line_ptrs->end_ptr = last_alnum;

    return line_ptrs->beg_ptr;
}

int is_empty_line (const char* str)
{
    assert (str != NULL);

    while (!isalnum (*str))
        {
        if (*str == '\0')
            {
            break;
            }

        str++;
        }

    // Loop reached end of line without finding
    // al/num char
    return !*str;
}

char* read_to_buffer (const char* file_name, size_t* file_size)
{
    assert (file_name != NULL);
    assert (*file_size > 0);

    FILE* file_pointer = fopen (file_name, "r");

    // Allocate empty buffer
    char* buffer = NULL;
    // +1 byte for '\0' in the end
    *file_size = get_size (file_pointer) + 1;

    buffer = (char*) calloc (*file_size, sizeof (*buffer));

    // Fill the buffer
    size_t fread_return = fread (buffer, sizeof (*buffer), *file_size, file_pointer);

    fclose (file_pointer);

    // In case of 'r' mode of fopen() on Windows
    if (fread_return != *file_size)
        {
        *file_size = fread_return;

        buffer = (char*) realloc (buffer, *file_size);
        }

    return buffer;
}

void write_line_buf_to_file (const char* file_name, int lines_num, const line_buf* line_ptrs)
{
    assert (file_name != NULL);
    assert (lines_num > 0);
    assert (line_ptrs != NULL);

    FILE* file_writer = fopen (file_name, "w");

    for (int cur_line = 0; cur_line < lines_num; cur_line++)
        {
        fputs ((line_ptrs + cur_line)->beg_ptr, file_writer);
        putc ('\n', file_writer);
        }

    fclose (file_writer);
    }

void put_zeros (char* const buffer, size_t file_size)
    {
    assert (buffer != NULL);
    assert (file_size > 0);

    char* cur_pos = buffer;
    while (cur_pos < buffer + file_size)
        {
        if (*cur_pos == '\n' || *cur_pos == ' ')
            {
            *cur_pos = '\0';
            }
        cur_pos++;
        }

    *(buffer + file_size) = '\0';
}

int count_correct_lines (char* const buffer, size_t file_size)
{
    assert (buffer != NULL);
    assert (file_size > 0);

    char* cur_pos = buffer;
    size_t correct_lines = 0;

    while (cur_pos < buffer + file_size)
        {
        char* str_end = strchr (cur_pos, '\0');

        if (!is_empty_line (cur_pos)) correct_lines++;

        cur_pos = str_end + 1;
        }

    return correct_lines;
}

line_buf* get_strings (const char* file_name, size_t* lines_num)
{
    assert (file_name != NULL);
    assert (lines_num != NULL);

    size_t file_size = -1;

    char* buffer = read_to_buffer (file_name, &file_size);

    put_zeros (buffer, file_size);

    *lines_num = count_correct_lines (buffer, file_size);

    line_buf* line_ptrs = NULL;

    line_ptrs = (line_buf*) calloc (*lines_num, sizeof (line_buf));

    char* cur_pos = buffer;
    size_t cur_line = 0;

    while (cur_line < *lines_num)
        {
        char* str_end = strchr (cur_pos, '\0');

        if (!is_empty_line (cur_pos))
            {
            line_ptrs[cur_line].beg_ptr = cur_pos;
            line_ptrs[cur_line].end_ptr = str_end;

            cur_line++;
            }

        cur_pos = str_end + 1;
        }

    return line_ptrs;
}

void print_line_buf (line_buf* text, size_t lines_num)
{
    for (size_t line_id = 0; line_id < lines_num; line_id++)
        printf ("%s\n", text[line_id].beg_ptr);
}