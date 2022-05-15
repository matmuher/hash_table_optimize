#include <time.h>
#include "hash_table.h"

float stress_test (__m256i* words, uint32_t words_num, hash_table_t* ma_hash_table)
{
    // STRESS TEST
    clock_t search_clocks = clock();

    for (uint32_t word_id = 0; word_id < words_num; word_id++)
    {
        const uint32_t RUNS_NUM = 1000;

        for (uint32_t run_id = 0; run_id < RUNS_NUM; run_id++)
            search_in_hash_table(ma_hash_table, words[word_id]);
    }
    search_clocks = clock() - search_clocks;
    float search_time = (float) search_clocks / CLOCKS_PER_SEC; 
    
    return search_time;  
}

float stress_test_average (hash_table_t* ma_hash_table, __m256i* words, uint32_t words_num)
{
    const uint32_t TESTS_NUM = 5;

    float sum_time = 0;
    for (uint32_t test_id = 0; test_id < TESTS_NUM; test_id++)
    {
        float cur_test_time = stress_test(words, words_num, ma_hash_table);
        printf("Test %u: %.2f\n", test_id, cur_test_time);
        sum_time += cur_test_time;
    }
    float aver_time = sum_time / TESTS_NUM;  

    return aver_time;
}