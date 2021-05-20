#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <stdint.h>
#include <stdbool.h>
#include<string.h>
#include <unistd.h>
#include<pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

#include "../include/communication.h"
#include "../include/linkedList.h"
#include "../include/utils.h"
#include "../include/consumer.h"
#include "../include/queue.h"

pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifoMutex;
int clientTimeOut = 0;
int serverClosed;
int producersFinished;
Queue *queue;
char *pathFIFO;
int fd;    

void sig_handler(int signum){
    pthread_mutex_lock(&clientMutex);
    close(fd);
    serverClosed = 1;
    pthread_mutex_unlock(&clientMutex);
}

int checkArgs(int argc, char *args[]){
    bool hasError = false;
    if(argc == 4 || argc == 6){
        char *numberStr;
        if(!strcmp(args[1], "-t") && argc == 6){
            //args[1] == "-t"
            numberStr = (char *) malloc(strlen(args[2])+1);
            if (numberStr != NULL)
                strncpy(numberStr, args[2], strlen(args[2]) + 1);
        } else {
            numberStr = (char *)malloc(strlen(args[1] - 2));
            if (numberStr != NULL)
                strncpy(numberStr, args[1] + 2, strlen(args[1]) - 1);
        }
        if (numberStr != NULL){
            if(!isNumber(numberStr)){
                hasError = true;
            }
            free(numberStr);
        }

        if(!strcmp(args[3], "-l") && argc == 6){
            //args[1] == "-t"
            numberStr = (char *) malloc(strlen(args[4])+1);
            if (numberStr != NULL)
                strncpy(numberStr, args[4], strlen(args[4]) + 1);
        } else {
            numberStr = (char *)malloc(strlen(args[2] - 2));
            if (numberStr != NULL)
                strncpy(numberStr, args[2] + 2, strlen(args[2]) - 1);
        }
        if (numberStr != NULL){
            if(!isNumber(numberStr)){
                hasError = true;
            }
            free(numberStr);
        }
    } else{
        hasError = true;
    }

    if(hasError){
        printf("Usage: %s <-t nsecs> [-l bufsz] fifoname\n", args[0]);
        exit(1);
    }
    return 0;
}

int parseArgs(int argc, char *args[], int *nsecs, int *sizeBuffer, char *pathFIFO){
    char * number = (char *) malloc(sizeof(strlen(args[1]) + 1));
    if (number == NULL)
        return -1;
    if(argc == 4){
        strncpy(number, args[1], strlen(args[1]) + 1);
        strncpy(number, number + 2, strlen(number) - 1);
    } else if(argc == 6){
        strncpy(number, args[2], strlen(args[2]) + 1);
    }
    sscanf(number, "%d", nsecs);
    free(number);
    number = (char *) malloc(sizeof(strlen(args[1]) + 1));
    if(argc == 4){
        strncpy(number, args[2], strlen(args[2]) + 1);
        strncpy(number, number + 2, strlen(number) - 1);
    } else if(argc == 6){
        strncpy(number, args[4], strlen(args[4]) + 1);
    }
    sscanf(number, "%d", sizeBuffer);
    strncpy(pathFIFO, args[argc - 1], strlen(args[argc - 1]) + 1);
    
    
    free(number);
    return 0;
}

int main(int argc, char *args[]){
    clientTimeOut = 0;
    serverClosed = 0;
    producersFinished = 0;
    checkArgs(argc, args);
    char *pathFIFO = (char * ) malloc(100);
    int nsecs, sizeBuffer;

    parseArgs(argc, args, &nsecs, &sizeBuffer, pathFIFO);
    signal(SIGALRM,sig_handler); // Register signal handler
 
    alarm(nsecs); 
    LinkedListElement *first, *last, *aux;
    first = NULL;

    createFIFO(pathFIFO);

    pthread_t threadConsumer;
    if (pthread_create(&threadConsumer, NULL, thread_consumer, NULL)) {
        fprintf(stderr, "Failed to create thread\n");
    }
    queue = initQueue(sizeBuffer);
    //Message *response = (Message *)malloc(sizeof(Message));
        
    fd = open(pathFIFO, O_RDONLY);

    do{
        Message *response = (Message *)malloc(sizeof(Message));
        //pthread_mutex_lock(&fifoMutex);
        if(!serverClosed){
            if(read(fd, response, sizeof(Message)) == sizeof(Message)){
                pthread_t thread;
                writeLog(response, RECVD);
                if (pthread_create(&thread, NULL, thread_func, response)) {
                    fprintf(stderr, "Failed to create thread\n");
                }
                if(first == NULL) {
                    first = initLinkedList(thread);
                    last = first;
                }else{
                    last = addElement(last,thread);
                }
            }
        } else {
            free(response);
        }
        //pthread_mutex_unlock(&fifoMutex);
    }while(!serverClosed);

    unlink(pathFIFO);

    aux = first;

    while(aux != NULL){
        pthread_join(aux->thread,NULL);
        aux = aux->next;
    }

    producersFinished = 1;

    pthread_join(threadConsumer, NULL);

    //free(response);
    free(pathFIFO);
    destroyQueue(queue);
    freeLinkedList(first);

    return 0;
}