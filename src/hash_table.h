#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_proc.h"

extern const size_t HASH_TABLE_SIZE;

struct hash_cell
    {
    const char* data;
    hash_cell* next_collision_element;
    };

// Hash table interact functions
void fill_hash_table    (line_buf* words, size_t words_num, hash_cell* hash_table, 
                        size_t (*hash_func)(const char*), const char* title);    
void print_hash_table (hash_cell* hash_table);
void add_element_hash_table (hash_cell* hash_table, size_t (*hash_func)(const char*), const char* str);
void count_collisions (hash_cell* hash_table, size_t* collisions_num);
void lu_array_to_file (size_t* array, size_t elements_num, const char* title);
void reset_hash_table (hash_cell* hash_table);
void free_hash_table (hash_cell* hash_table);

// Hash functions
size_t ascii_hash (const char* key);
size_t ones_hash (const char* key);
size_t len_hash (const char* key);
size_t check_sum_hash (const char* key);


#endif