#ifndef D_HAMLET_FUNCTIONS_H_INCLUDED
#define D_HAMLET_FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

/*!
@brief struct that should contain pointers to begin and end of line
*/
struct line_buf
        {
        char* buffer;
        char* beg_ptr;
        char* end_ptr;
        };

/*!
@brief Read file_name with "r" mode and write it to allocated buffer
@details if smart_mode is switched on:\n\n
         read_to_buffer calls elephant_calloc for easier memory allocation,\n
         elephant_calloc_extern.h must be included
         and elephant_calloc.cpp must be included to the project.\n
         As memory was allocated:\n
         [YOU SHOULD USE MEMORY_FREE FUNCTION WHEN FINISH USING LINES]\n\n
         if smart_mode is switched off - free after using you must free\n
         line_buf object[0].beg_ptr, it points to begin of allocated memory.\n
@return Pointer to allocated buffer\n
        FREE IT AFTER USING
*/
char* read_to_buffer (const char* file_pointer, size_t* file_size);

/*!
@brief Returns size of file in bytes
@details Uses fseek function to find end of file\n
         and ftell to get bytes size of file\n
         !! FILE MUST BE OPENED !!
*/
int get_size (FILE* file_pointer);

/*!
@brief Determines whether it is end of file or not
@details Compares ftell (file_pointer) and fseek (file_pointer, 0, SEEK_END);
@params[in] file_pointer pointer to opened file stream
@return true if internal file pointer is in the end,\n
        false if it is not
*/
int is_eof (FILE* file_pointer);

/*!
@brief Prints line-delimiter
*/
void say_delimiter();

/*!
@brief Checks if there are letter- or digit-chars in str
@params[in] str C-string
@return false if there is letter- or digit-char\n
        true if there are no chars\n
*/
int is_empty_line (const char* str);

/*!
@brief Prepares line for correct sorting
*/
char* format_line (char* line, line_buf* line_ptrs);

/*!
@brief Formats lines from line_buf structure object
       to ignore punctuation in the begin and in the end
*/
line_buf* prepare_data (line_buf* line_ptrs, uint32_t lines_num);

/*!
@brief Get lines_num lines from file, package it in line_buf structure object
@details if smart_mode is switched on:\n\n
         get_strings calls elephant_calloc for better memory allocation,\n
         elephant_calloc_extern.h must be included
         and elephant_calloc.cpp must be included to the project.\n
         As memory was allocated:\n
         [YOU SHOULD USE MEMORY_FREE FUNCTION WHEN FINISH USING LINES]\n\n
         line_buf object[0].beg_ptr, it points to begin of allocated memory.\n
@params[in] *lines_num pointer to variable where number of lines will be written
@params[out] lines_num number_of_lines
@return pointer to line_buf structure object with written lines
*/
line_buf* get_strings (const char* file_name, uint32_t* lines_num);

/*!
@brief Replace '\n' with '\0' in buffer
       Adds '\0' in the end
*/
void put_zeros (char* const buffer, uint32_t file_size);

/*!
@brief Count not empty lines
*/
int count_correct_lines (char* const buffer, uint32_t file_size);

/*!
@brief Prints lines from line_buf object
*/
void print_line_buf (line_buf* text, uint32_t lines_num);


#endif // D_HAMLET_FUNCTIONS_H_INCLUDED