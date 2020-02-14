//
// Created by 張藝文 on 2020/2/12.
//
#include <threads.h>

#include "instruction_counter.h"

unsigned int count_clflush;
unsigned int count_mfence;

mtx_t mtx;

void add_clflush(){
    mtx_lock(&mtx);
    count_clflush++;
    mtx_unlock(&mtx);
}

void add_mfence(){
    mtx_lock(&mtx);
    count_mfence++;
    mtx_unlock(&mtx);
}
