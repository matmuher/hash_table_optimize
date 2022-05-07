#include <stdio.h>
#include <time.h>
#include "hash_table.h"
#include "input_proc.h"

int main()
{
    #if 1
    
    const uint32_t hash_table_size_power_of_two = 12;
    hash_table_t* ma_hash_table = hash_table_init(hash_table_size_power_of_two, pjw_hash, "PJW hash");

    // INPUT
    size_t words_num = 0;
    const char* file_name = "Text/english2.txt";
    line_buf* words = get_strings(file_name, &words_num);
    fill_hash_table(words, words_num, ma_hash_table);

    #endif
    #if 0
    //fill_hash_table(words, words_num, hash_table, ror_hash, "Ror hash");
    fill_hash_table(words, words_num, hash_table, check_sum_hash, "Check sum");
    fill_hash_table(words, words_num, hash_table, ascii_hash, "First letter ASCII");
    fill_hash_table(words, words_num, hash_table, len_hash, "By length");
    fill_hash_table(words, words_num, hash_table, ones_hash, "Always ones");
    #endif

    hash_table_destruct(ma_hash_table);

    return 0;
}

