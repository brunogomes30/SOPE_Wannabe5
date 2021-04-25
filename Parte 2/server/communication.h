#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "alarm.h"
#include "common.h"
#include "log.h"


typedef struct {
    int rid;
    char *fifo;
} ClientThreadArgs;

int serverClosed;

int FIFOexists(char* fifo);
void initFIFO(char * fifo);
void copyMessage(Message* copy, Message* toCopy);
Message* getServerResponse(char* privateFIFO, char* publicFIFO,Message* message);
int sendServerRequest(char* publicFIFO, Message* message);
Message* initializeMessage(ClientThreadArgs* threadArgs);
//isto devia ter os parâmetros, não??
int writeToFIFO();
int readFromFIFO();
void *thread_func(void *arg);
#endif