//
// Created by 張藝文 on 2020/2/12.
//

#include "instruction_counter.h"

unsigned int& count_clflush(){
    static unsigned int num_clflush;
    return num_clflush;
}

unsigned int& count_mfence(){
    static unsigned int num_mfence;
    return num_mfence;
}