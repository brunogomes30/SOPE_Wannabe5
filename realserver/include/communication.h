#ifndef REALSERVER_INCLUDE_COMMUNICATION_H_
#define REALSERVER_INCLUDE_COMMUNICATION_H_

#include "../include/common.h"
#include "../include/log.h"

typedef struct
{
    int rid;
    char *fifo;
} ClientThreadArgs;

int serverClosed;
int publicFIFOfd;
int clientTimeOut;



/**
 * @brief 
 * Thread function 
 * @param arg  
 */
void *thread_func(void *arg);

#endif //REALSERVER_INCLUDE_COMMUNICATION_H_
