#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "../include/linkedList.h"
#include "../include/log.h"
#include "../include/utils.h"
#include "../include/communication.h"
#include "../include/queue.h"

pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fifoMutex;
int clientTimeOut = 0;
int serverClosed;
int producersFinished;
Queue *queue;
char *pathFIFO;
int fd;

void sig_handler(int signum) {
    pthread_mutex_lock(&clientMutex);
    close(fd);
    serverClosed = 1;
    pthread_mutex_unlock(&clientMutex);
}

int checkArgs(int argc, char *args[]) {
    bool hasError = false;
    bool separatedFlags = !strcmp(args[1], "-t");

    char *numberStr = (char *)malloc(strlen(args[2]));

    if (separatedFlags && (argc == 4 || argc == 6)) {
        numberStr = (char *)realloc(numberStr, strlen(args[2]) + 1);
        if (numberStr != NULL) {
            strncpy(numberStr, args[2], strlen(args[2]) + 1);
            if (!isNumber(numberStr)) {
                hasError = true;
            } 
        } else {
            hasError = true;
        }

        if (argc == 6){
            numberStr = (char *)realloc(numberStr, strlen(args[4]) + 1);

            if (numberStr != NULL) {
                strncpy(numberStr, args[4], strlen(args[4]) + 1);
                if (!isNumber(numberStr)) {
                    hasError = true;
                }
            } else {
                hasError = true;
            }
        }

    } else if (!separatedFlags && (argc == 3 || argc == 4)) {
        numberStr = (char *)realloc(numberStr, strlen(args[1]) - 1);
        if (numberStr != NULL) {
            strncpy(numberStr, args[1] + 2, strlen(args[1]) - 1);
            if (!isNumber(numberStr)) {
                hasError = true;
            } 
        } else {
            hasError = true;
        }

        if (argc == 4){
            numberStr = (char *)realloc(numberStr, strlen(args[2])- 1);
            if (numberStr != NULL) {
                strncpy(numberStr, args[2] + 2, strlen(args[2]) - 1);
                if (!isNumber(numberStr)) {
                    hasError = true;
                } 
            } else {
                hasError = true;
            }
        }
    } else {
        hasError = true;
    }
    free(numberStr);
    if (hasError) {
        printf("Usage: %s <-t nsecs> [-l bufsz] fifoname\n", args[0]);
        exit(1);
    }
    return 0;
}

int parseArgs(int argc, char *args[], int *nsecs, int *sizeBuffer,
                                                     char *pathFIFO) {
    char *number = (char *)malloc(strlen(args[1]) + 1);
    bool separatedFlags = !strcmp(args[1], "-t");

    if(separatedFlags && (argc == 4 || argc == 6)){
        number = (char *)realloc(number, strlen(args[2])+ 1);
        if (number == NULL)
            return -1;
        strncpy(number, args[2], strlen(args[2]) + 1);
        sscanf(number, "%d", nsecs);
        if (argc == 6) {
            number = (char *)realloc(number, strlen(args[4])+ 1);
            if (number == NULL)
                return -1;
            strncpy(number, args[4], strlen(args[4]) + 1);
            sscanf(number, "%d", sizeBuffer);
        }
    } else if (!separatedFlags && (argc == 3 || argc == 4)) {
        number = (char *)realloc(number, 2 *strlen(args[1]) + 1);
        if (number == NULL)
            return -1;
        strncpy(number, args[1], strlen(args[1]) + 1);
        strncpy(number, number + 2, strlen(number) - 1);
        sscanf(number, "%d", nsecs);

        number = (char *)realloc(number, strlen(args[2]) + 1);

        if (number == NULL)
            return -1;

        if(argc == 4){
            strncpy(number, args[2], strlen(args[2]) + 1);
            strncpy(number, number + 2, strlen(number) - 1);
            sscanf(number, "%d", sizeBuffer);
        }
    }
    strncpy(pathFIFO, args[argc - 1], strlen(args[argc - 1]) + 1);
    free(number);

    return 0;
}

int main(int argc, char *args[]) {
    clientTimeOut = 0;
    serverClosed = 0;
    producersFinished = 0;
    checkArgs(argc, args);
    char *pathFIFO = (char *)malloc(100);
    int nsecs, sizeBuffer = 100;

    parseArgs(argc, args, &nsecs, &sizeBuffer, pathFIFO);
    signal(SIGALRM, sig_handler);
    
    alarm(nsecs);
    LinkedListElement *first, *last, *aux;
    first = NULL;

    createFIFO(pathFIFO);

    pthread_t threadConsumer;
    if (pthread_create(&threadConsumer, NULL, thread_consumer, NULL)) {
        fprintf(stderr, "Failed to create thread\n");
    }
    queue = initQueue(sizeBuffer);

    fd = open(pathFIFO, O_RDONLY);

    do {
        Message *request = (Message *)malloc(sizeof(Message));
        if (request == NULL)
            continue;
        if (!serverClosed) {
            if (read(fd, request, sizeof(Message)) == sizeof(Message)) {
                pthread_t thread;
                writeLog(request, RECVD);
                if (pthread_create(&thread, NULL, thread_func, request)) {
                    fprintf(stderr, "Failed to create thread\n");
                }
                if (first == NULL) {
                    first = initLinkedList(thread);
                    last = first;
                } else {
                    last = addElement(last, thread);
                }
            }
        } else {
            free(request);
        }
    } while (!serverClosed);

    unlink(pathFIFO);

    aux = first;

    while (aux != NULL) {
        pthread_join(aux->thread, NULL);
        aux = aux->next;
    }

    producersFinished = 1;

    pthread_join(threadConsumer, NULL);

    free(pathFIFO);
    destroyQueue(queue);
    freeLinkedList(first);

    return 0;
}
