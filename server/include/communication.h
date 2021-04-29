#ifndef SERVER_INCLUDE_COMMUNICATION_H_
#define SERVER_INCLUDE_COMMUNICATION_H_

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
 * Copies message "toCopy" parameters to message "copy"
 * @param copy 
 * @param toCopy 
 */
void copyMessage(Message *copy, Message *toCopy);

/**
 * @brief 
 * Attempt to read server response from private FIFO
 * @param privateFIFO 
 * @param publicFIFO 
 * @param message
 */
Message *getServerResponse(char *privateFIFO, char *publicFIFO, Message *message);

/**
 * @brief 
 * Creates message to send to server
 * @param threadArgs 
 */
Message *initializeMessage(ClientThreadArgs *threadArgs);

/**
 * @brief 
 * Reads message response from private FIFO 
 * @param fifo 
 * @param message 
 */
int readFromFIFO(char *fifo, char * publicFifo,Message *message);

/**
 * @brief 
 * Thread function 
 * @param arg  
 */
void *thread_func(void *arg);

#endif //SERVER_INCLUDE_COMMUNICATION_H_
