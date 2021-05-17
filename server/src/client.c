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

pthread_mutex_t clientMutex;
extern int clientTimeOut;

int checkArgs(int argc, char *args[]){
    bool hasError = false;
    if(argc >= 3 && argc <=4){
        char *numberStr;
        if(!strcmp(args[1], "-t") ){
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
        free(numberStr);
    } else{
        hasError = true;
    }

    if(hasError){
        printf("Usage: %s <-t nsecs> fifoname\n", args[0]);
        exit(1);
    }
    return 0;
}

int parseArgs(int argc, char *args[], int *nsecs, char *pathFIFO){
    char * number = (char *) malloc(sizeof(strlen(args[1]) + 1));
    if (number == NULL)
        return -1;
    if(argc == 3){
        strncpy(number, args[1], strlen(args[1]) + 1);
        strncpy(number, number + 2, strlen(number) - 1);
    } else if(argc == 4){
        strncpy(number, args[2], strlen(args[2]) + 1);
    }

    strncpy(pathFIFO, args[argc - 1], strlen(args[argc - 1]) + 1);
    sscanf(number, "%d", nsecs);
    
    free(number);
    return 0;
}

int main(int argc, char *args[]){
    clientTimeOut = 0;
    checkArgs(argc, args);
    char *pathFIFO = (char * ) malloc(100);
    int nsecs;

    parseArgs(argc, args, &nsecs, pathFIFO);
    uint64_t initialTime = time(NULL);

    int id = 0;
    serverClosed = 0;
    LinkedListElement *first, *last, *aux;
    first = NULL;

    while((publicFIFOfd = open(pathFIFO, O_WRONLY)) == -1 && (time(NULL) < initialTime + nsecs)){
        usleep(500);
    }

    do{
        usleep(getRandomNumber(10, 50) * 1000);
        if(!serverClosed){
            pthread_t thread;

            ClientThreadArgs *threadArgs = (ClientThreadArgs *)malloc(sizeof(ClientThreadArgs));
            threadArgs->rid = id++;
            threadArgs->fifo = pathFIFO;
            if (pthread_create(&thread, NULL, thread_func, threadArgs)) {
                fprintf(stderr, "Failed to create thread\n");
                continue;
            }

            if (id == 1){
                first = initLinkedList(thread);
                last = first;
            }else{
                last = addElement(last,thread);
            }
        }
    }while((time(NULL) < initialTime + nsecs));
    fprintf(stderr,"NUM THREADS: %d\n", id);

    pthread_mutex_lock(&clientMutex);
    clientTimeOut = 1;
    pthread_mutex_unlock(&clientMutex);
    
    aux = first;
    
    while(aux != NULL){
        pthread_join(aux->thread,NULL);
        aux = aux->next;
    }
  
    free(pathFIFO);
    freeLinkedList(first);
    return 0;
}