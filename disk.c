#include "oslabs.h"
#include <limits.h>

const struct RCB NULLRCB = {0, 0, 0, 0, 0};

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.request_id == 0)
    {
        return new_request;
    }
    else
    {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt)
{
    if (*queue_cnt == 0)
    {
        return NULLRCB;
    }
    int earliest = 0;
    for (int i = 1; i < *queue_cnt; i++)
    {
        if (request_queue[i].arrival_timestamp < request_queue[earliest].arrival_timestamp)
        {
            earliest = i;
        }
    }

    struct RCB earliest_request = request_queue[earliest];

    for (int i = earliest; i < *queue_cnt - 1; i++)
    {
        request_queue[i] = request_queue[i + 1];
    }
    *queue_cnt -= 1;
    return earliest_request;
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.request_id == 0)
    {
        return new_request;
    }
    else
    {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder)
{
    if (*queue_cnt == 0)
    {
        return NULLRCB;
    }
    int shortest = 0;
    int shortest_distance = INT_MAX;
    for (int i = 0; i < *queue_cnt; i++)
    {
        int distance = abs(request_queue[i].cylinder - current_cylinder);
        if (distance < shortest_distance)
        {
            shortest = i;
            shortest_distance = distance;
        }
        else if (distance == shortest_distance)
        {
            if (request_queue[i].arrival_timestamp < request_queue[shortest].arrival_timestamp)
            {
                shortest = i;
            }
        }
    }
    struct RCB shortest_request = request_queue[shortest];
    for (int i = shortest; i < *queue_cnt - 1; i++)
    {
        request_queue[i] = request_queue[i + 1];
    }
    *queue_cnt -= 1;
    return shortest_request;
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (current_request.request_id == 0)
    {
        return new_request;
    }
    else
    {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

// struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction)
// {
//     if (*queue_cnt == 0)
//     {
//         return NULLRCB;
//     }

//     int closest = 0;
//     int closest_distance = -1;

//     for (int i = 0; i < *queue_cnt; i++)
//     {
//         int distance = request_queue[i].cylinder - current_cylinder;
//         if (distance == 0)
//         {
//             int earliest = 0;
//             for (int j = 1; j < *queue_cnt; j++)
//             {
//                 if (request_queue[j].arrival_timestamp < request_queue[earliest].arrival_timestamp)
//                 {
//                     earliest = j;
//                 }
//             }
//             struct RCB earliest_request = request_queue[earliest];
//             for (int j = earliest; j < *queue_cnt - 1; j++)
//             {
//                 request_queue[j] = request_queue[j + 1];
//             }
//             *queue_cnt -= 1;
//             return earliest_request;
//         }
//         if (scan_direction == 1)
//         {
//             if (distance >= 0 && (closest_distance == -1 || distance < closest_distance))
//             {
//                 closest = i;
//                 closest_distance = distance;
//             }
//         }
//         else
//         {
//             if (distance <= 0 && (closest_distance == -1 || -distance < closest_distance))
//             {
//                 closest = i;
//                 closest_distance = -distance;
//             }
//         }
//     }

//     if (closest == -1)
//     {
//         closest_distance = -1;
//         for (int i = 0; i < *queue_cnt; i++)
//         {
//             int distance = request_queue[i].cylinder - current_cylinder;
//             if (closest_distance == -1 || abs(distance) < closest_distance)
//             {
//                 closest = i;
//                 closest_distance = abs(distance);
//             }
//         }
//     }

//     struct RCB closest_request = request_queue[closest];
//     for (int i = closest; i < *queue_cnt - 1; i++)
//     {
//         request_queue[i] = request_queue[i + 1];
//     }
//     *queue_cnt -= 1;
//     return closest_request;
// }
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int *scan_direction)
{
    if (*queue_cnt == 0)
    {
        return NULLRCB;
    }

    int closest = -1;
    int closest_distance = INT_MAX;

    for (int i = 0; i < *queue_cnt; i++)
    {
        int distance = request_queue[i].cylinder - current_cylinder;

        if ((*scan_direction == 1 && distance >= 0) || (*scan_direction == -1 && distance <= 0))
        {
            if (abs(distance) < closest_distance)
            {
                closest = i;
                closest_distance = abs(distance);
            }
            else if (abs(distance) == closest_distance && request_queue[i].arrival_timestamp < request_queue[closest].arrival_timestamp)
            {
                closest = i;
            }
        }
    }

    if (closest == -1)
    {
        *scan_direction = -*scan_direction;
        closest_distance = INT_MAX;

        for (int i = 0; i < *queue_cnt; i++)
        {
            int distance = request_queue[i].cylinder - current_cylinder;

            if (abs(distance) < closest_distance)
            {
                closest = i;
                closest_distance = abs(distance);
            }
            else if (abs(distance) == closest_distance && request_queue[i].arrival_timestamp < request_queue[closest].arrival_timestamp)
            {
                closest = i;
            }
        }
    }

    struct RCB closest_request = request_queue[closest];

    for (int i = closest; i < *queue_cnt - 1; i++)
    {
        request_queue[i] = request_queue[i + 1];
    }

    *queue_cnt -= 1;

    return closest_request;
}