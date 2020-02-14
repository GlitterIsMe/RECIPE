//
// Created by 張藝文 on 2020/2/12.
//

#ifndef RECIPE_INSTRUCTION_COUNTER_H
#define RECIPE_INSTRUCTION_COUNTER_H
#include <pthread.h>
/*unsigned int& count_clflush();
unsigned int& count_mfence();*/
#include <atomic>

void add_clflush();
void add_mfence();

void reset_clflush();
void reset_mfence();

unsigned long count_clflush();
unsigned long count_mfence();

#endif //RECIPE_INSTRUCTION_COUNTER_H
