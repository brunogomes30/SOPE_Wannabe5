#include "../include/consumer.h"
#include "../include/queue.h"
#include "../include/utils.h"
#include "../include/log.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t serverMutex = PTHREAD_MUTEX_INITIALIZER;
extern int serverClosed;
extern Queue *queue;

int writeToFIFO(char *fifo, Message *message){
    int filedesc;
    filedesc = open(fifo, O_WRONLY ); 
    write(filedesc, message, sizeof(Message));
    close(filedesc);
    return 0;
    
    //close(filedesc);
    //return -1;
}



void *thread_consumer(void *arg){
    Message *message = (Message *) malloc(sizeof(Message));

    while(!serverClosed){
        pthread_mutex_lock(&serverMutex);
        bool isEmpty = empty(queue);
        pthread_mutex_unlock(&serverMutex);



        if(!isEmpty){
            pthread_mutex_lock(&serverMutex);
            
            message = front(queue)->k;
            pop(queue);
            if (message != NULL){
                pthread_mutex_unlock(&serverMutex);

                char privateFIFO[100];
                snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid); 

                pthread_mutex_lock(&serverMutex);
                writeToFIFO(privateFIFO, message);
                pthread_mutex_unlock(&serverMutex);

                writeLog(message, TSKDN);
            }
        } else {
            usleep(5000);
        }
    }

    free(message);
    return NULL;
}