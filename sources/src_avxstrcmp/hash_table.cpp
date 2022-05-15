#include "hash_table.h"
#include <math.h>

extern "C" int A_strcmp (const char * string1, const char * string2);

const int SIMD_EQUAL_MASK = -1;
extern const uint8_t YMM_SIZE;

// SERVICE FUNCTIONS
void free_hash_table_content (hash_table_t* ma_hash_table);
void count_collisions (hash_table_t* ma_hash_table, uint32_t* collisions_num);
void u_array_to_file (uint32_t* array, uint32_t elements_num, const char* title);

hash_table_t* hash_table_init (uint32_t power_of_two, uint32_t (*hash_func)(const char*), const char* hash_func_title)
{
    hash_table_t* ma_hash_table = (hash_table_t*) calloc (1, sizeof(hash_table_t));

    ma_hash_table->power_of_two = power_of_two;
    ma_hash_table->HASH_TABLE_SIZE = lroundf(pow(2,power_of_two));
    ma_hash_table->hash_table = (hash_cell*) aligned_alloc(YMM_SIZE, ma_hash_table->HASH_TABLE_SIZE * sizeof(hash_cell));
    memset(ma_hash_table->hash_table, 0, ma_hash_table->HASH_TABLE_SIZE * sizeof(hash_cell));

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

void fill_hash_table (__m256i* words, uint32_t words_num, hash_table_t* ma_hash_table)
{
    printf("Start processing of %s!\n", ma_hash_table->hash_func_title);

    for (uint32_t word_id = 0; word_id < words_num / 1; word_id++) 
    {
        printf("Processing word %u/%u\r", word_id, words_num);
        add_element_hash_table(ma_hash_table, words + word_id);
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
        printf("[%u] %s ", index, (char*) &ma_hash_table->hash_table[index].data);

        hash_cell* collide_cell = ma_hash_table->hash_table + index;
        collide_cell = collide_cell->next_collision_element;

        while (collide_cell)
        {
            printf("-> %s ", (char*) &collide_cell->data);
            collide_cell = collide_cell->next_collision_element;
        }

        putchar('\n');
    }
}

void add_element_hash_table (hash_table_t* ma_hash_table, __m256i* str)
{
    uint32_t hash_index = ma_hash_table->hash_func((char*) str) % ma_hash_table->HASH_TABLE_SIZE;

    hash_cell* found_cell = ma_hash_table->hash_table + hash_index; 
    
    // Check whether data is zero or not
    __m256i zeros = _mm256_set1_epi8('\0');
    __m256i zero_cmp_mask = _mm256_cmpeq_epi8(found_cell->data, zeros);
    int is_zero_data = _mm256_movemask_epi8(zero_cmp_mask);
    if (is_zero_data == SIMD_EQUAL_MASK) found_cell->data = *str;
    else
    {
        while (found_cell->next_collision_element != NULL) 
            found_cell = found_cell->next_collision_element;

        hash_cell* new_collision_element = (hash_cell*) aligned_alloc(YMM_SIZE, sizeof(hash_cell));
        memset(new_collision_element, 0, sizeof(hash_cell));
        new_collision_element->data = *str;
        found_cell->next_collision_element = new_collision_element; 
    }
}

void free_hash_table_content (hash_table_t* ma_hash_table)
{
    for (uint32_t index = 0; index < ma_hash_table->HASH_TABLE_SIZE; index++) 
    {
        hash_cell* collide_cell = ma_hash_table->hash_table + index;

        collide_cell = collide_cell->next_collision_element;

        while (collide_cell)
        {
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
        hash_cell* collide_cell = &ma_hash_table->hash_table[index];

        collide_cell = collide_cell->next_collision_element;

        while (collide_cell) 
        {
            collide_cell = collide_cell->next_collision_element;
            collisions_num[index]++;

        }
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
    uint32_t hash = *key++;

    while(*key)
    {
        // ror(hash) ^ key[letter_id]
        hash = ( (hash << 1) | (hash >> 31) ) ^ (*key++);
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

hash_cell* search_in_hash_table (hash_table_t* ma_hash_table, __m256i key)
{
    uint32_t hash_index = ma_hash_table->hash_func((char*) &key) % ma_hash_table->HASH_TABLE_SIZE;
    hash_cell* collide_cell = ma_hash_table->hash_table + hash_index;

    while (collide_cell)
    {
        __m256i cmp_mask = _mm256_cmpeq_epi8(collide_cell->data, key);
        int cmp_status = _mm256_movemask_epi8(cmp_mask);

        if (cmp_status == SIMD_EQUAL_MASK) return collide_cell;

        collide_cell = collide_cell->next_collision_element;
    }

    return NULL;
}
