//
// Created by 張藝文 on 2020/2/12.
//
#include <pthread.h>

#include "instruction_counter.h"

unsigned int count_clflush;
unsigned int count_mfence;

pthread_mutex_t mutex;

void add_clflush(){
    pthread_mutex_lock(&mutex);
    count_clflush++;
    pthread_mutex_unlock(&mutex);
}

void add_mfence(){
    pthread_mutex_lock(&mutex);
    count_mfence++;
    pthread_mutex_unlock(&mutex);
}
