#ifndef CONSUMER_H
#define CONSUMER_H
#include "../include/common.h"

/**
 * @brief Thread consumer's function
 * 
 * @param arg 
 * @return void* 
 */
void *thread_consumer(void *arg);

/**
 * @brief 
 * 
 * @param fifo 
 * @param message 
 * @return int 
 */
int writeToFIFO(char *fifo, Message *message);

#endif //CONSUMER_H