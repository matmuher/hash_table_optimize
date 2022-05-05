#include "hash_table.h"

const size_t HASH_TABLE_SIZE = 100000;

void fill_hash_table    (line_buf* words, size_t words_num, hash_cell* hash_table, 
                        size_t (*hash_func)(const char*), const char* title)
{
    printf("Start processing of %s!\n", title);
    for (size_t word_id = 0; word_id < words_num; word_id++) 
    {
        printf("Processing word %lu/%lu\r", word_id, words_num);
        add_element_hash_table(hash_table, hash_func, words[word_id].beg_ptr);
    }
    putchar('\n');

    // Analize collisions
    size_t collisions_num[HASH_TABLE_SIZE] = {};
    count_collisions(hash_table, collisions_num);
    lu_array_to_file(collisions_num, HASH_TABLE_SIZE, title);
    system("python3 plotter.py");

    free_hash_table(hash_table);
    reset_hash_table(hash_table);

    printf("Filling with %s completed\n", title);
}

void print_hash_table (hash_cell* hash_table)
{
    for (size_t index = 0; index < HASH_TABLE_SIZE; index++) 
    {
        printf("[%lu] %s ", index, hash_table[index].data);

        hash_cell* collide_cell = hash_table + index;
        collide_cell = collide_cell->next_collision_element;

        while (collide_cell)
        {
            printf("-> %s ", collide_cell->data);
            collide_cell = collide_cell->next_collision_element;
        }

        putchar('\n');
    }
}

void add_element_hash_table (hash_cell* hash_table, size_t (*hash_func)(const char*), const char* str)
{
    // Todo: pass a function pointer
    // Make HASH_TABLE_SIZE element of hash_table structure
    size_t hash_index = hash_func(str) % HASH_TABLE_SIZE;

    hash_cell* found_cell = hash_table + hash_index; 

    if (found_cell->data == NULL) found_cell->data = strdup(str);
    else
    {
        while (found_cell->next_collision_element != NULL) 
            found_cell = found_cell->next_collision_element;

        hash_cell* new_collision_element = (hash_cell*) calloc(1, sizeof(hash_cell));
        new_collision_element->data = strdup(str);
        found_cell->next_collision_element = new_collision_element; 
    }

}

void free_hash_table (hash_cell* hash_table)
{
    for (size_t index = 0; index < HASH_TABLE_SIZE; index++) 
    {
        hash_cell* collide_cell = hash_table + index;
        free((char*) collide_cell->data);

        collide_cell = collide_cell->next_collision_element;

        while (collide_cell)
        {
            free((char*) collide_cell->data);
            hash_cell* doomed_cell = collide_cell;
            collide_cell = collide_cell->next_collision_element;
            free(doomed_cell);
        }
    }
}

void count_collisions (hash_cell* hash_table, size_t* collisions_num)
{
   for (size_t index = 0; index < HASH_TABLE_SIZE; index++) 
    {
        hash_cell* collide_cell = hash_table + index;

        collide_cell = collide_cell->next_collision_element;

        while (collide_cell) 
        {
            collide_cell = collide_cell->next_collision_element;
            collisions_num[index]++;
        }
    } 
}

// long unsigned integer
void lu_array_to_file (size_t* array, size_t elements_num, const char* title)
{
    FILE* collisions_num_file = fopen("collisions_num.txt", "w");

    fprintf(collisions_num_file, "%s\n", title);

    for (size_t index = 0; index < elements_num; index++)
        for (size_t collision_id = 0; collision_id < array[index]; collision_id++)
            fprintf(collisions_num_file, "%lu\n", index);

    fclose(collisions_num_file);
}

size_t ascii_hash (const char* key)
{
    return key[0];
}

size_t ones_hash (const char* key)
{
    return 1;
}

size_t len_hash (const char* key)
{
    return strlen(key);
}

size_t check_sum_hash (const char* key)
{
    size_t check_sum = 0;

    while (*key) check_sum += *key++;

    return check_sum;
}

void reset_hash_table (hash_cell* hash_table)
{
    memset(hash_table, 0, HASH_TABLE_SIZE * sizeof(hash_cell));
}
