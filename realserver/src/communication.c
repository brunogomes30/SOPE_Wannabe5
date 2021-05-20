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
#include "../include/lib.h"
#include "../include/queue.h"

pthread_mutex_t serverMutex;
extern Queue *queue;
extern int serverClosed;
extern bool fullBuffer;


void *thread_func(void *arg){    
    //sleep(1);
    Message* message = (Message *) arg;

    if(!serverClosed){
        message->tskres = task(message->tskload);
        writeLog(message, TSKEX);
    }
    push(queue,message);
    free(message);
    pthread_exit(NULL);
}