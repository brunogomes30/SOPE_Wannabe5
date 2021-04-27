#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "common.h"
#include "log.h"


typedef struct {
    int rid;
    char *fifo;
} ClientThreadArgs;

int serverClosed;
int publicFIFOfd;

void copyMessage(Message* copy, Message* toCopy);
Message* getServerResponse(char* privateFIFO, char* publicFIFO,Message* message);
Message* initializeMessage(ClientThreadArgs* threadArgs);
int writeToFIFO(char *fifo, Message *message);
int readFromFIFO(char *fifo, Message *message);
void *thread_func(void *arg);
#endif