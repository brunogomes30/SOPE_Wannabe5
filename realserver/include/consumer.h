#ifndef REALSERVER_INCLUDE_CONSUMER_H_
#define REALSERVER_INCLUDE_CONSUMER_H_
#include "../include/common.h"
#include "../include/queue.h"

int serverClosed;
Queue *queue;
int clientTimeOut;
int producersFinished;

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

#endif //REALSERVER_INCLUDE_CONSUMER_H_