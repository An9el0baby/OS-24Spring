#include "oslabs.h"
#include <limits.h>

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == 1)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }
    if (*frame_cnt > 0)
    {
        *frame_cnt -= 1;
        int frame_number = frame_pool[*frame_cnt];
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return frame_number;
    }
    int min_timestamp = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < *table_cnt; i++)
    {
        if (page_table[i].is_valid && page_table[i].arrival_timestamp < min_timestamp)
        {
            min_timestamp = page_table[i].arrival_timestamp;
            min_index = i;
        }
    }

    if (min_index != -1)
    {
        int frame_number = page_table[min_index].frame_number;
        page_table[min_index].is_valid = 0;
        page_table[min_index].frame_number = -1;
        page_table[min_index].arrival_timestamp = -1;
        page_table[min_index].last_access_timestamp = -1;
        page_table[min_index].reference_count = -1;

        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return frame_number;
    }
    return -1;
}
int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int page_faults = 0;
    int current_timestamp = 1;
    for (int i = 0; i < reference_cnt; i++)
    {
        int page_number = refrence_string[i];
        if (page_table[page_number].is_valid == 0)
        {
            page_faults++;
        }
        current_timestamp++;

        if (!frame_cnt)
        {
            current_timestamp++;
        }
        process_page_access_fifo(page_table, &table_cnt, page_number, frame_pool, &frame_cnt, current_timestamp);
    }
    return page_faults;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp)
{
    if (page_table[page_number].is_valid == 1)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }
    if (*frame_cnt > 0)
    {
        *frame_cnt -= 1;
        int frame_number = frame_pool[*frame_cnt];
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return frame_number;
    }
    int min_lru_timestamp = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < *table_cnt; i++)
    {
        if (page_table[i].is_valid && page_table[i].last_access_timestamp < min_lru_timestamp)
        {
            min_lru_timestamp = page_table[i].last_access_timestamp;
            min_index = i;
        }
    }
    if (min_index != -1)
    {
        int frame_number = page_table[min_index].frame_number;
        page_table[min_index].is_valid = 0;
        page_table[min_index].frame_number = -1;
        page_table[min_index].arrival_timestamp = -1;
        page_table[min_index].last_access_timestamp = -1;
        page_table[min_index].reference_count = -1;

        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return frame_number;
    }
    return -1;
}
int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int page_faults = 0;
    int current_timestamp = 1;
    for (int i = 0; i < reference_cnt; i++)
    {
        int page_number = refrence_string[i];
        if (page_table[page_number].is_valid == 0)
        {
            page_faults++;
        }
        current_timestamp++;

        if (!frame_cnt)
        {
            current_timestamp++;
        }
        process_page_access_lru(page_table, &table_cnt, page_number, frame_pool, &frame_cnt, current_timestamp);
    }
    return page_faults;
}
int process_page_access_lfu(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp)
{
}
int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
}