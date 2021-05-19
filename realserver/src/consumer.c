#include "../include/consumer.h"
#include "../include/queue.h"
#include "../include/utils.h"
#include "../include/log.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

pthread_mutex_t serverMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifoMutex = PTHREAD_MUTEX_INITIALIZER;

extern int serverClosed;
extern Queue *queue;
extern int clientTimeOut;
extern int producersFinished;

int writeToFIFO(char *fifo, Message *message)
{
    int filedesc;
    for (int i = 0; i < 30; i++)
    {
        if ((filedesc = open(fifo, O_WRONLY | O_NONBLOCK)) > 0)
        {
            if (write(filedesc, message, sizeof(Message)) < sizeof(Message)){
                close(filedesc);
                return -1;
            }
            close(filedesc);
            return 0;
        }
        else
        {
            usleep(5 * 1000);
        }
    }
    return -1;
}

void *thread_consumer(void *arg)
{
    Message *message = (Message *)malloc(sizeof(Message));

    while (!serverClosed)
    {
        //pthread_mutex_lock(&serverMutex);
        message = pop(queue);
        //pthread_mutex_unlock(&serverMutex);
        if (message != NULL)
        {
            char privateFIFO[100];
            snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid);
            if (writeToFIFO(privateFIFO, message) == -1)
            {
                writeLog(message, FAILD);
                clientTimeOut = 1;
            }
            else
            {
                if (message->tskres == -1)
                {
                    writeLog(message, LATE);
                }
                else
                {
                    writeLog(message, TSKDN);
                }
            }
        }
    }

    while (!emptyBuffer(queue) || !producersFinished)
    {
        message = pop(queue);
        if (message != NULL)
        {
            char privateFIFO[100];
            snprintf(privateFIFO, sizeof(privateFIFO), "/tmp/%d.%ld", message->pid, message->tid);
            if (writeToFIFO(privateFIFO, message) == -1)
            {
                writeLog(message, FAILD);
            }
            else
                writeLog(message, LATE);
        }
    }

    return NULL;
}