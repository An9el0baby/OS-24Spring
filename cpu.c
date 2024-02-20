#include "oslabs.h"

const struct PCB NULLPCB = {0, 0, 0, 0, 0, 0, 0};

int is_nullpcb(struct PCB pcb){
    return pcb.process_id == 0 && pcb.arrival_timestamp == 0 && pcb.total_bursttime == 0 && pcb.execution_starttime == 0 && pcb.execution_endtime == 0 && pcb.remaining_bursttime == 0 && pcb.process_priority == 0;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    if (is_nullpcb(current_process)){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    // if the new process has lower priority than the current process (i.e. higher number), add it to the queue
    if (new_process.process_priority >=  current_process.process_priority){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt) += 1;
        return current_process;
    }
    // if the new process has higher priority than the current process, add the current process to the queue and return the new process
    else{
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt) += 1;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    if (*queue_cnt == 0){
        return NULLPCB;
    }
    // find the process with the highest priority
    struct PCB next_process = ready_queue[0];
    for (int i = 1; i < *queue_cnt; i++){
        if (ready_queue[i].process_priority < next_process.process_priority){
            next_process = ready_queue[i];
        }
    }
    // remove the process from the queue
    for (int i = 0; i < *queue_cnt; i++){
        if (ready_queue[i].process_id == next_process.process_id){
            for (int j = i; j < *queue_cnt - 1; j++){
                ready_queue[j] = ready_queue[j + 1];
            }
            (*queue_cnt) -= 1;
            break;
        }
    }
    // set the execution start time and end time
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}


struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
    if (is_nullpcb(current_process)){
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    // if the new process does not have a shorter remaining burst time than the current process, add it to the queue
    if (new_process.total_bursttime >= current_process.remaining_bursttime){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt) += 1;
        return current_process;
    // if the new process has a shorter remaining burst time than the current process, add the current process to the queue and return the new process
    } else {
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= (time_stamp - current_process.execution_starttime);
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt) += 1;
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    if (*queue_cnt == 0){
        return NULLPCB;
    }
    // find the process with the shortest remaining burst time
    struct PCB next_process = ready_queue[0];
    for (int i = 1; i < *queue_cnt; i++){
        if (ready_queue[i].remaining_bursttime < next_process.remaining_bursttime){
            next_process = ready_queue[i];
        }
    }
    // remove the process from the queue
    for (int i = 0; i < *queue_cnt; i++){
        if (ready_queue[i].process_id == next_process.process_id){
            for (int j = i; j < *queue_cnt - 1; j++){
                ready_queue[j] = ready_queue[j + 1];
            }
            (*queue_cnt) -= 1;
            break;
        }
    }
    // set the execution start time and end time
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    if (is_nullpcb(current_process)){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt) += 1;
    return current_process;
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum){
    if (*queue_cnt == 0){
        return NULLPCB;
    }
    // find the process with earliest arrival time
    struct PCB next_process = ready_queue[0];
    for (int i = 1; i < *queue_cnt; i++){
        if (ready_queue[i].arrival_timestamp < next_process.arrival_timestamp){
            next_process = ready_queue[i];
        }
    }
    // remove the process from the queue
    for (int i = 0; i < *queue_cnt; i++){
        if (ready_queue[i].process_id == next_process.process_id){
            for (int j = i; j < *queue_cnt - 1; j++){
                ready_queue[j] = ready_queue[j + 1];
            }
            (*queue_cnt) -= 1;
            break;
        }
    }
    // set the execution start time and end time
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + MIN(time_quantum, next_process.remaining_bursttime);
    return next_process;
}