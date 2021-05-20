#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>


#include "../include/communication.h"
#include "../include/utils.h"
#include "../include/log.h"
#include "../include/lib.h"
#include "../include/queue.h"

pthread_mutex_t serverMutex;
Queue *queue;
int clientTimeOut;
int producersFinished;
int serverClosed;
extern bool fullBuffer;



int writeToFIFO(char *fifo, Message *message){
    int filedesc;
    for (int i = 0; i < 3; i++)
{
        if ((filedesc = open(fifo, O_WRONLY | O_NONBLOCK)) > 0){
            if (write(filedesc, message, sizeof(Message)) == sizeof(Message)){
                close(filedesc);
                return 0;
            }
            close(filedesc);
            return -1;
        }
        else{
            usleep(100000);
        }
    }
    return -1;
}

void *thread_consumer(void *arg)
{

    while (!emptyBuffer(queue) || !producersFinished){
        Message * message = pop(queue);
        if (message != NULL){
            char privateFIFO[100];
            snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid);
            if (writeToFIFO(privateFIFO, message) == -1){
                writeLog(message, FAILD);
                clientTimeOut = 1;
            }
            else{
                if (message->tskres == -1){
                    writeLog(message, LATE);
                }
                else{
                    writeLog(message, TSKDN);
                }
            }
            free(message);
        }
    }
    pthread_exit(NULL);
}

void *thread_func(void *arg){    
    //sleep(1);
    Message* message = (Message *) arg;

    if(!serverClosed){
        message->tskres = task(message->tskload);
        writeLog(message, TSKEX);
    }
    push(queue,message);
    //pthread_mutex_unlock(&serverMutex);
    free(message);
    pthread_exit(NULL);
}
