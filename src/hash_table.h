#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "input_proc.h"

struct hash_cell
    {
    const char* data;
    hash_cell* next_collision_element;
    };

struct hash_table_t
    {
    hash_cell* hash_table;
    uint32_t HASH_TABLE_SIZE;
    uint32_t power_of_two;
    uint32_t (*hash_func)(const char*);
    const char* hash_func_title;
    };

// Hash table interact functions
hash_table_t* hash_table_init (uint32_t power_of_two, uint32_t (*hash_func)(const char*), const char* hash_func_title);
void hash_table_destruct (hash_table_t* ma_hash_table);
void fill_hash_table (line_buf* words, uint32_t words_num, hash_table_t* ma_hash_table);
hash_cell* search_in_hash_table (hash_table_t* ma_hash_table, const char* key);
void print_hash_table (hash_table_t* ma_hash_table);
void add_element_hash_table (hash_table_t* ma_hash_table, const char* str);
void reset_hash_table (hash_table_t* ma_hash_table);

// Hash functions
uint32_t ascii_hash (const char* key);
uint32_t ones_hash (const char* key);
uint32_t len_hash (const char* key);
uint32_t check_sum_hash (const char* key);
uint32_t ror_hash (const char* key);
uint32_t pjw_hash (const char* key);

#endif