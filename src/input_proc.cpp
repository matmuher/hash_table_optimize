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
            while (!isalnum(*cur_pos)) {cur_pos++;};

            if (*cur_pos == '\0') continue;

            line_ptrs[cur_line].beg_ptr = cur_pos;
            line_ptrs[cur_line].end_ptr = str_end;

            cur_line++;
            }

        cur_pos = str_end + 1;
        }

    return line_ptrs;
}
