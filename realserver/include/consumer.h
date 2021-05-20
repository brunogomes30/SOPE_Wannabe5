#ifndef REALSERVER_INCLUDE_CONSUMER_H_
#define REALSERVER_INCLUDE_CONSUMER_H_
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

#endif //REALSERVER_INCLUDE_CONSUMER_H_