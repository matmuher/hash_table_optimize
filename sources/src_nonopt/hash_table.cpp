#include "hash_table.h"
#include <math.h>

extern "C" int A_strcmp (const char * string1, const char * string2);

// SERVICE FUNCTIONS
void free_hash_table_content (hash_table_t* ma_hash_table);
void count_collisions (hash_table_t* ma_hash_table, uint32_t* collisions_num);
void u_array_to_file (uint32_t* array, uint32_t elements_num, const char* title);

hash_table_t* hash_table_init (uint32_t power_of_two, uint32_t (*hash_func)(const char*), const char* hash_func_title)
{
    hash_table_t* ma_hash_table = (hash_table_t*) calloc (1, sizeof(hash_table_t));

    ma_hash_table->power_of_two = power_of_two;
    ma_hash_table->HASH_TABLE_SIZE = lroundf(pow(2,power_of_two));
    ma_hash_table->hash_table = (hash_cell*) calloc(ma_hash_table->HASH_TABLE_SIZE, sizeof(hash_cell));

    ma_hash_table->hash_func = hash_func;
    ma_hash_table->hash_func_title = hash_func_title;

    return ma_hash_table;
}

void hash_table_destruct (hash_table_t* ma_hash_table)
{
    free_hash_table_content(ma_hash_table);
    free(ma_hash_table->hash_table);
    free(ma_hash_table);
}

void fill_hash_table (line_buf* words, uint32_t words_num, hash_table_t* ma_hash_table)
{
    printf("Start processing of %s!\n", ma_hash_table->hash_func_title);

    for (uint32_t word_id = 0; word_id < words_num / 1; word_id++) 
    {
        printf("Processing word %u/%u\r", word_id, words_num);
        add_element_hash_table(ma_hash_table, words[word_id].beg_ptr);
    }
    putchar('\n');

    // Analize collisions
    uint32_t collisions_num[ma_hash_table->HASH_TABLE_SIZE] = {};
    count_collisions(ma_hash_table, collisions_num);
    u_array_to_file(collisions_num, ma_hash_table->HASH_TABLE_SIZE, ma_hash_table->hash_func_title);
    // print_hash_table(hash_table);
    system("python3 plotter.py");

    printf("Filling with %s completed\n", ma_hash_table->hash_func_title);
}

void print_hash_table (hash_table_t* ma_hash_table)
{
    for (uint32_t index = 0; index < ma_hash_table->HASH_TABLE_SIZE; index++) 
    {
        printf("[%u] %s ", index, ma_hash_table->hash_table[index].data);

        hash_cell* collide_cell = ma_hash_table->hash_table + index;
        collide_cell = collide_cell->next_collision_element;

        while (collide_cell)
        {
            printf("-> %s ", collide_cell->data);
            collide_cell = collide_cell->next_collision_element;
        }

        putchar('\n');
    }
}

void add_element_hash_table (hash_table_t* ma_hash_table, const char* str)
{
    uint32_t hash_index = ma_hash_table->hash_func(str) % ma_hash_table->HASH_TABLE_SIZE;

    hash_cell* found_cell = ma_hash_table->hash_table + hash_index; 

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

void free_hash_table_content (hash_table_t* ma_hash_table)
{
    for (uint32_t index = 0; index < ma_hash_table->HASH_TABLE_SIZE; index++) 
    {
        hash_cell* collide_cell = ma_hash_table->hash_table + index;
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

void count_collisions (hash_table_t* ma_hash_table, uint32_t* collisions_num)
{
    uint32_t sum = 0;

    for (uint32_t index = 0; index < ma_hash_table->HASH_TABLE_SIZE; index++) 
    {
        hash_cell* collide_cell = ma_hash_table->hash_table + index;

        collide_cell = collide_cell->next_collision_element;

        while (collide_cell) 
        {
            collide_cell = collide_cell->next_collision_element;
            collisions_num[index]++;
        }
        // printf("%d\n", collisions_num[index]);
        sum += collisions_num[index];
    }

    float average = (float) sum / ma_hash_table->HASH_TABLE_SIZE;

    printf("average list size: %.1f\n", average); 

    float dispersion2 = 0;
    for (uint32_t index = 0; index < ma_hash_table->HASH_TABLE_SIZE; index++)
    {
        dispersion2 += pow((collisions_num[index] - average), 2);
    }

    printf("Dispersion is %.1f\n", pow(dispersion2 / ma_hash_table->HASH_TABLE_SIZE, 0.5));
}

// long unsigned integer
void u_array_to_file (uint32_t* array, uint32_t elements_num, const char* title)
{
    FILE* collisions_num_file = fopen("collisions_num.txt", "w");

    fprintf(collisions_num_file, "%s\n", title);
    fprintf(collisions_num_file, "%u\n", elements_num);

    for (uint32_t index = 0; index < elements_num; index++)
        for (uint32_t collision_id = 0; collision_id < array[index]; collision_id++)
            fprintf(collisions_num_file, "%u\n", index);

    fclose(collisions_num_file);
}

uint32_t ascii_hash (const char* key)
{
    return key[0];
}

uint32_t ones_hash (const char* key)
{
    return 1;
}

uint32_t len_hash (const char* key)
{
    return strlen(key);
}

uint32_t check_sum_hash (const char* key)
{
    uint32_t check_sum = 0;

    while (*key) check_sum += *key++;

    return check_sum;
}

uint32_t ror_hash (const char* key)
{
    uint32_t len = strlen(key), hash = key[0];

    unsigned char* key_ = (unsigned char*) key;

    for (uint32_t letter_id = 1; letter_id < len; letter_id++)
    {
        // ror(hash) ^ key[letter_id]
        hash = ( (hash << 1) | (hash >> 31) ) ^ key_[letter_id];
    }

    return hash;
}

uint32_t pjw_hash (const char* key)
{
    uint32_t len = strlen(key);
    uint32_t hash = 0;

    for (uint32_t letter_id = 0; letter_id < len; letter_id++)
    {
        hash = (hash << 4) + key[letter_id];
        uint32_t g = hash & 0xf0000000;
        if (g != 0)
        {
            hash = hash ^ (g >> 24);
            hash = hash ^ g; 
        }                   
    }

    return hash;
}

void reset_hash_table (hash_table_t* ma_hash_table)
{
    free_hash_table_content(ma_hash_table);
    memset(ma_hash_table->hash_table, 0, ma_hash_table->HASH_TABLE_SIZE * sizeof(hash_cell));
}

hash_cell* search_in_hash_table (hash_table_t* ma_hash_table, const char* key)
{

    uint32_t hash_index = ma_hash_table->hash_func(key) % ma_hash_table->HASH_TABLE_SIZE;
    hash_cell* collide_cell = ma_hash_table->hash_table + hash_index;

    while (collide_cell)
    {
        if (collide_cell->data && !strcmp(collide_cell->data, key)) return collide_cell;

        collide_cell = collide_cell->next_collision_element;
    }

    return NULL;
}
