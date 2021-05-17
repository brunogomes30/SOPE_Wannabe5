#include "../include/consumer.h"
#include "../include/queue.h"
#include "../include/utils.h"
#include "../include/log.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t serverMutex;
extern int serverClosed;
extern Queue queue;
int writeToFIFO(char *fifo, Message *message){
    int filedesc;
    printf("1111\n");
    filedesc = open(fifo, O_WRONLY ); 
    printf("22222\n");
    write(filedesc, message, sizeof(Message));
    close(filedesc);
    return 0;
    
    //close(filedesc);
    //return -1;
}



void *thread_consumer(void *arg){
    while(!serverClosed){
        
        if(!queue_isEmpty()){
            printf("CONSUMIDOR \n");
            pthread_mutex_lock(&serverMutex);
            Message *message = queue.front();
            queue.pop();
            pthread_mutex_unlock(&serverMutex);
            char privateFIFO[100];
            snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid); 
            printf("privateFIFO = %s\n", privateFIFO);
            writeToFIFO(privateFIFO, message);
            writeLog(message, TSKDN);
        } else {
            usleep(5000);
        }
    }
    return NULL;

}