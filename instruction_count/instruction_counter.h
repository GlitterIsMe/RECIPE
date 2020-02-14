//
// Created by 張藝文 on 2020/2/12.
//

#ifndef RECIPE_INSTRUCTION_COUNTER_H
#define RECIPE_INSTRUCTION_COUNTER_H

/*unsigned int& count_clflush();
unsigned int& count_mfence();*/

extern unsigned int count_clflush;
extern unsigned int count_mfence;
pthread_mutex_t mutex;

extern void add_clflush()
    pthread_mutex_lock(&mutex);
    count_clflush++;
    pthread_mutex_unlock(&mutex);
}
extern void add_mfence(){
    pthread_mutex_lock(&mutex);
    count_mfence++;
    pthread_mutex_unlock(&mutex);
}

#endif //RECIPE_INSTRUCTION_COUNTER_H
