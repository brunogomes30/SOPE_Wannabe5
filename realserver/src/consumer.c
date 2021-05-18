#include "../include/consumer.h"
#include "../include/queue.h"
#include "../include/utils.h"
#include "../include/log.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t serverMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifoMutex = PTHREAD_MUTEX_INITIALIZER;

extern int serverClosed;
extern Queue *queue;
extern bool emptyBuffer;
extern int clientTimeOut;

int writeToFIFO(char *fifo, Message *message){
    int filedesc;
    for (int i = 0; i < 30; i++){
        if ((filedesc = open(fifo, O_WRONLY)) > 0){
            write(filedesc, message, sizeof(Message));
            close(filedesc);
            return 0;
        }
    }
    return -1;
}

void *thread_consumer(void *arg){
    Message* message = (Message*) malloc(sizeof(Message));

    while(!serverClosed){
     
        if(!emptyBuffer){
            pthread_mutex_lock(&serverMutex);
            *message = front(queue)->k;
            pop(queue);
            pthread_mutex_unlock(&serverMutex);

            if (message != NULL){
                char privateFIFO[100];
                snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid);    
                if (writeToFIFO(privateFIFO,message) == -1){
                    writeLog(message, FAILD);
                    clientTimeOut = 1;
                }
                else
                    writeLog(message, TSKDN);
            }
        } else {
            usleep(5000);
        }
    }

    while(!emptyBuffer){
        pthread_mutex_lock(&serverMutex);
        *message = front(queue)->k;
        pop(queue);
        pthread_mutex_unlock(&serverMutex);
        if (message != NULL){
            char privateFIFO[100];
            snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid);  
            if (writeToFIFO(privateFIFO,message) == -1){
                writeLog(message, FAILD);
            }
            else
                writeLog(message, LATE);
        }
    }
    
    return NULL;
}