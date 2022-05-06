#include <stdio.h>
#include "hash_table.h"
#include "input_proc.h"



int main()
{
    #if 1
    // INPUT
    size_t words_num = 0;
    const char* file_name = "Text/WhiteFang.txt";
    line_buf* words = get_strings(file_name, &words_num);

    hash_cell hash_table[HASH_TABLE_SIZE] = {}; 

    fill_hash_table(words, words_num, hash_table, ascii_hash, "First letter ASCII");
    // fill_hash_table(words, words_num, hash_table, ones_hash, "Always ones");
    fill_hash_table(words, words_num, hash_table, len_hash, "By length");
    fill_hash_table(words, words_num, hash_table, check_sum_hash, "Check sum");
    #endif

    return 0;
}

