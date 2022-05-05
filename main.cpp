#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* hash_cell;

struct hash_cell
    {
    char* data;
    hash_cell* next_collision_element;
    }

const size_t HASH_TABLE_SIZE = 10;

// First letter ASCII
size_t ascii_hash (char* key)
{
    return key[0];
}

void add_element_hash_table (hash_cell* hash_table, char* str)
{
    // Todo: pass a function pointer
    // Make HASH_TABLE_SIZE element of hash_table structure
    size_t hash_index = ascii_hash(str) % HASH_TABLE_SIZE;

    hash_cell found_cell = hash_table[hash_index]; 

    if (found_cell.data == NULL)
    {
        hash_table[hash_index] = strdup(str);
    }
    else
    {
        while (found_cell.data != NULL)
            found_cell = found_cell.next_collision_element;
    }

}

void print_hash_table (hash_cell* hash_table)
{
    for (size_t index = 0; index < HASH_TABLE_SIZE; index++) 
        printf("[%d] %s\n", index, hash_table[index]);
}

int main()
{
    hash_cell hash_table[HASH_TABLE_SIZE] = {}; 

    size_t TEST_STRS_NUM = 5;
    char* test_strs[TEST_STRS_NUM] = {"meow", "crya", "fuck", "lol", "mom"};

    for (size_t str_id = 0; str_id < TEST_STRS_NUM; str_id++) 
        add_element_hash_table(hash_table, test_strs[str_id]);

    print_hash_table(hash_table);

    return 0;
}