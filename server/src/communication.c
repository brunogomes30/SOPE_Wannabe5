#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/communication.h"
#include "../include/utils.h"

pthread_mutex_t clientMutex;

int readFromFIFO(char *fifo,char * publicFifo, Message *response){
    int filedesc;

    if (!FIFOexists(publicFifo)) {
        pthread_mutex_lock(&clientMutex);
        serverClosed = 1;
        pthread_mutex_unlock(&clientMutex);
    }
    
    while(!clientTimeOut){
        if (( filedesc = open(fifo, O_RDONLY | O_NONBLOCK )) != 0) {
            close(filedesc);

            int fd = open(fifo, O_RDONLY);
            if(read(fd, response, sizeof(Message)) == sizeof(Message)){
                close(fd);
                return 0;
            }
        }
    }
    return -1;
} 

void copyMessage(Message* copy, Message* toCopy){
    copy->pid = toCopy->pid;
    copy->rid = toCopy->rid;
    copy->tid = toCopy->tid;
    copy->tskload = toCopy->tskload;
    copy->tskres = toCopy->tskres;
}

Message* getServerResponse(char* privateFIFO, char* publicFIFO,Message* message){ 
    Message *response = (Message *)malloc(sizeof(Message));

    if (response == NULL)
        return NULL;

    if(readFromFIFO(privateFIFO,publicFIFO, response) == 0){
        if(response->tskres == -1){
            pthread_mutex_lock(&clientMutex);
            serverClosed = 1;
            pthread_mutex_unlock(&clientMutex);
            writeLog(response,CLOSD);
        }else{
            writeLog(response,GOTRS);
        }
    }else{
        writeLog(message, GAVUP);
        copyMessage(response,message); 
    }
   
    deleteFIFO();
    return response;
}

Message* initializeMessage(ClientThreadArgs* threadArgs){
    Message *message = (Message *)malloc(sizeof(Message));
    if (message == NULL)
        return NULL;

    message->tid = pthread_self();
    message->pid = getpid();
    message->tskload = getTaskRandom();
    message->tskres = -1;
    message->rid = threadArgs->rid;

    return message;
}

void *thread_func(void *arg){
    ClientThreadArgs *threadArgs = (ClientThreadArgs *) arg;
    char * publicFIFO = threadArgs->fifo;
    Message *message = initializeMessage(threadArgs);
    if (message == NULL)
        return NULL;
   
    if(write(publicFIFOfd ,message, sizeof(Message)) == sizeof(Message)){
        writeLog(message, IWANT);
        char privateFIFO[100];
        snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid); 
        createFIFO(privateFIFO);

        Message *response = getServerResponse(privateFIFO, publicFIFO, message);
        free(response);
    }

    free(message);
    free(arg);
    pthread_exit(NULL);
}