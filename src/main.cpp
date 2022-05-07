#include <stdio.h>
#include "hash_table.h"
#include "input_proc.h"

int main()
{
    const uint32_t hash_table_size_power_of_two = 12;
    hash_table_t* ma_hash_table = hash_table_init(hash_table_size_power_of_two, pjw_hash, "PJW hash");

    // INPUT
    uint32_t words_num = 0;
    const char* file_name = "Text/english2.txt";
    line_buf* words = get_strings(file_name, &words_num);
    fill_hash_table(words, words_num, ma_hash_table);

    float aver_time = stress_test_average(ma_hash_table, words, words_num);
    printf ("[Average stress test passing time is %.2f]\n", aver_time);  

    hash_table_destruct(ma_hash_table);

    return 0;
}

