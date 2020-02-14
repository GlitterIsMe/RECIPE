//
// Created by 張藝文 on 2020/2/12.
//

#ifndef RECIPE_INSTRUCTION_COUNTER_H
#define RECIPE_INSTRUCTION_COUNTER_H
#include <pthread.h>
/*unsigned int& count_clflush();
unsigned int& count_mfence();*/

extern unsigned int count_clflush;
extern unsigned int count_mfence;

extern void add_clflush();
extern void add_mfence();

#endif //RECIPE_INSTRUCTION_COUNTER_H
