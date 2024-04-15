#include "oslabs.h"

const struct MEMORY_BLOCK NULLBLOCK = {0, 0, 0, 0};

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    int best_index = -1;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
        {
            if (best_index == -1 || memory_map[i].segment_size < memory_map[best_index].segment_size)
            {
                best_index = i;
            }
        }
    }

    if (best_index == -1)
    {
        return NULLBLOCK;
    }

    struct MEMORY_BLOCK old_block = memory_map[best_index];
    struct MEMORY_BLOCK new_block =
        {
            old_block.start_address,
            old_block.start_address + request_size - 1,
            request_size,
            process_id};

    if (old_block.segment_size == request_size)
    {
        memory_map[best_index] = new_block;
    }
    else
    {
        for (int i = *map_cnt; i > best_index; i--)
        {
            memory_map[i] = memory_map[i - 1];
        }
        memory_map[best_index] = new_block;
        struct MEMORY_BLOCK temp = {
            new_block.end_address + 1,
            old_block.end_address,
            old_block.segment_size - request_size,
            0};
        memory_map[best_index + 1] = temp;
        (*map_cnt)++;
    }
    return new_block;
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
        {
            struct MEMORY_BLOCK old_block = memory_map[i];
            struct MEMORY_BLOCK new_block = {
                old_block.start_address,
                old_block.start_address + request_size - 1,
                request_size,
                process_id};
            if (old_block.segment_size == request_size)
            {
                memory_map[i] = new_block;
            }
            else
            {
                for (int j = *map_cnt; j > i; j--)
                {
                    memory_map[j] = memory_map[j = 1];
                }
                memory_map[i] = new_block;
                struct MEMORY_BLOCK temp = {
                    new_block.end_address + 1,
                    old_block.end_address,
                    old_block.segment_size - request_size,
                    0};
                memory_map[i + 1] = temp;
                (*map_cnt)++;
            }
            return new_block;
        }
    }
    return NULLBLOCK;
}
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    int worst_index = -1;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
        {
            if (worst_index == -1 || memory_map[i].segment_size > memory_map[worst_index].segment_size)
            {
                worst_index = i;
            }
        }
    }

    if (worst_index == -1)
    {
        return NULLBLOCK;
    }
    struct MEMORY_BLOCK old_block = memory_map[worst_index];
    struct MEMORY_BLOCK new_block =
        {
            old_block.start_address,
            old_block.start_address + request_size - 1,
            request_size,
            process_id};

    if (old_block.segment_size == request_size)
    {
        memory_map[worst_index] = new_block;
    }
    else
    {
        for (int i = *map_cnt; i > worst_index; i--)
        {
            memory_map[i] = memory_map[i - 1];
        }
        memory_map[worst_index] = new_block;
        struct MEMORY_BLOCK temp = {
            new_block.end_address + 1,
            old_block.end_address,
            old_block.segment_size - request_size,
            0};
        memory_map[worst_index + 1] = temp;
        (*map_cnt)++;
    }
    return new_block;
}
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id, int last_address)
{
}
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt)
{
}