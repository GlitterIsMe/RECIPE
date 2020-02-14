//
// Created by 張藝文 on 2020/2/14.
//
#include <atomic>
#include "instruction_counter.h"

std::atomic_ulong clflush;
std::atomic_ulong mfence;

void add_clflush() {
    clflush++;
}

void add_mfence() {
    mfence++;
}

void reset_clflush(){
    clflush.store(0, std::memory_order_relaxed);
}

void reset_mfence(){
    mfence.store(0, std::memory_order_relaxed);
}


unsigned long count_clflush(){
    return clflush.load(std::memory_order_relaxed);
}

unsigned long count_mfence(){
    return mfence.load(std::memory_order_relaxed);
}