#include "input_proc.h"

const uint8_t YMM_SIZE = 32; // bytes

int get_size (FILE* file_pointer)
{
    assert (file_pointer != NULL);

    uint32_t cur_pos = ftell (file_pointer);

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

char* read_to_buffer (const char* file_name, uint32_t* file_size)
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
    uint32_t fread_return = fread (buffer, sizeof (*buffer), *file_size, file_pointer);

    fclose (file_pointer);

    // In case of 'r' mode of fopen() on Windows
    if (fread_return != *file_size)
        {
        *file_size = fread_return;

        buffer = (char*) realloc (buffer, *file_size);
        }

    return buffer;
}

void put_zeros (char* const buffer, uint32_t file_size)
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

int count_words (char* const buffer, uint32_t file_size)
{
    assert (buffer != NULL);
    assert (file_size > 0);

    char* cur_pos = buffer;
    uint32_t correct_lines = 0;

    while (cur_pos < buffer + file_size)
        {
        char* str_end = strchr (cur_pos, '\0');

        if (!is_empty_line (cur_pos)) correct_lines++;

        cur_pos = str_end + 1;
        }

    return correct_lines;
}

__m256i* get_words (const char* file_name, uint32_t* words_num)
{
    assert (file_name != NULL);
    assert (words_num != NULL);

    uint32_t file_size = -1;

    char* buffer = read_to_buffer (file_name, &file_size);

    put_zeros (buffer, file_size);

    *words_num = count_words (buffer, file_size);


    __m256i* words_buf_avx = (__m256i*) aligned_alloc(YMM_SIZE, YMM_SIZE * (*words_num));
    uint64_t cur_word = 0;

    char* cur_pos = NULL;
    cur_pos = buffer;
    uint32_t long_words = 0;
    while (cur_word < *words_num)
        {
        char* str_end = strchr (cur_pos, '\0');

        if (!is_empty_line (cur_pos))
            {
            while (!isalnum(*cur_pos)) {cur_pos++;};

            if (*cur_pos == '\0') continue;

            // Except this macros it's possible to use
            // alignement via: union {char word[]; __m256i var}
            // is it more portable?
            alignas(32) char ymm_word_format[YMM_SIZE] = {};
            if(strlen(cur_pos) > 32) long_words++;
            strncpy(ymm_word_format, cur_pos, YMM_SIZE);
            words_buf_avx[cur_word++] = _mm256_load_si256((__m256i const *)  ymm_word_format);
            }

        cur_pos = str_end + 1;
        }
    printf("We have %u long words\n", long_words);
    free(buffer);

    return words_buf_avx;
}
