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

void *thread_func(void *arg){    
        
    Message *message = (Message *) malloc(sizeof(Message));
    message = (Message *) arg;

    writeLog(message, TSKEX);
    message->tskres = task(message->tskload);
    
    Node *newNode =  (Node* ) malloc(sizeof(Node));
    newNode->k = message;

    pthread_mutex_lock(&serverMutex);
    push(queue,newNode);
    pthread_mutex_unlock(&serverMutex);
    
    free(message);
    pthread_exit(NULL);
}