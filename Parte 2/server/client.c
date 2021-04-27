#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include <stdbool.h>
#include<string.h>
#include <unistd.h>
#include<pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "communication.h"
#include "linkedList.h"
#include "utils.h"

pthread_mutex_t clientMutex;

int checkArgs(int argc, char *args[]){
    bool hasError = false;
    if(argc >= 3 && argc <=4){
        char *numberStr;
        if(!strcmp(args[1], "-t") ){
            //args[1] == "-t"
            numberStr = malloc(strlen(args[2]));
            strncpy(numberStr, args[2], strlen(args[2]));
            
        } else {
            numberStr = malloc(strlen(args[1] - 2));
            strncpy(numberStr, args[1] + 2, strlen(args[1]) - 2);
        }
        if(!isNumber(numberStr)){
            hasError = true;
        }
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
    checkArgs(argc, args);
    char *pathFIFO = (char * ) malloc(100);
    int nsecs;

    parseArgs(argc, args, &nsecs, pathFIFO);
    long int initialTime = time(NULL);
    /*setupAlarm();
    alarm(nsecs);*/

    int id = 0;
    serverClosed = 0;
    LinkedListElement *first, *last, *aux;

    while((publicFIFOfd = open(pathFIFO, O_WRONLY)) == -1){
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
            }
            
            if (id == 1){
                first = initLinkedList(thread);
                last = first;
            }else{
                last = addElement(last,thread);
            }
        }
    }while(/*!serverClosed && !clientTimeOut*/time(NULL) < initialTime + nsecs);
    fprintf(stderr,"NUM THREADS: %d\n", id);

    pthread_mutex_lock(&clientMutex);
    clientTimeOut = 1;
    pthread_mutex_unlock(&clientMutex);
    
    aux = first;
    fprintf(stderr,"Before joins\n");
    for(int i = 0; i <= id; i++){
        close(i+3);
    }
    
    while(aux != NULL){
        pthread_join(aux->thread,NULL);
        aux = aux->next;
    }
    fprintf(stderr,"After joins\n");


    //unlink(pathFIFO);
    free(pathFIFO);
    freeLinkedList(first);
    return 0;
}