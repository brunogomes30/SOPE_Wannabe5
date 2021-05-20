#ifndef REALSERVER_INCLUDE_COMMUNICATION_H_
#define REALSERVER_INCLUDE_COMMUNICATION_H_

#include "common.h"

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

/**
 * @brief 
 * Thread function 
 * @param arg  
 */
void *thread_func(void *arg);

#endif //REALSERVER_INCLUDE_COMMUNICATION_H_