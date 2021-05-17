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

#include "../include/communication.h"
#include "../include/linkedList.h"
#include "../include/utils.h"
#include "../include/consumer.h"
#include "../include/queue.h"

pthread_mutex_t clientMutex;
extern int clientTimeOut;
extern int serverClosed;
Queue queue;
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
        }
        
        free(numberStr);
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
    if(argc == 4){
        strncpy(number, args[1], strlen(args[1]) + 1);
        strncpy(number, number + 2, strlen(number) - 1);
    } else if(argc == 6){
        strncpy(number, args[2], strlen(args[2]) + 1);
    }
    sscanf(number, "%d", sizeBuffer);
    strncpy(pathFIFO, args[argc - 1], strlen(args[argc - 1]) + 1);
    
    
    free(number);
    return 0;
}

int main(int argc, char *args[]){
    clientTimeOut = 0;
    checkArgs(argc, args);
    char *pathFIFO = (char * ) malloc(100);
    int nsecs, sizeBuffer;

    parseArgs(argc, args, &nsecs, &sizeBuffer, pathFIFO);
    uint64_t initialTime = time(NULL);
    serverClosed = 0;
    
    LinkedListElement *first, *last, *aux;
    first = NULL;

    createFIFO(pathFIFO);

    pthread_t threadConsumer;
    if (pthread_create(&threadConsumer, NULL, thread_consumer, NULL)) {
        fprintf(stderr, "Failed to create thread\n");
    }
    first = initLinkedList(threadConsumer);
    last = first;

    //queue_init(sizeBuffer);
    queue.first = NULL;
    do{
        Message *response = (Message *)malloc(sizeof(Message));
        if(!clientTimeOut){
            
            int fd = open(pathFIFO, O_RDONLY);
            read(fd, response, sizeof(Message));
            pthread_t thread;
            writeLog(response, RECVD);
            if (pthread_create(&thread, NULL, thread_func, response)) {
                fprintf(stderr, "Failed to create thread\n");
            }
            last = addElement(last,thread);
        }
    }while((time(NULL) < initialTime + nsecs));

    pthread_mutex_lock(&clientMutex);
    serverClosed = 1;
    pthread_mutex_unlock(&clientMutex);
    
    aux = first;

    while(aux != NULL){
        pthread_join(aux->thread,NULL);
        aux = aux->next;
    }
  
    free(pathFIFO);
    freeLinkedList(first);
    deleteFIFO(pathFIFO);
    return 0;
}