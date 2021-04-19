#include "communication.h"
#include "common.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t clientMutex;

void initFIFO(char *myfifo){
    if(mkfifo(myfifo,0777) == -1){
        printf("ERRO\n");
    }
}

void *thread_func(void *arg){
    ClientThreadArgs *threadArgs = (ClientThreadArgs *) arg;
    Message *message = (Message *)malloc(sizeof(Message));
    message->tid = pthread_self();
    message->pid = getpid();
    message->tskload = getTaskRandom();
    message->rid = threadArgs->rid;
    char * myfifo = threadArgs->fifo;
    
    printf("before mutext\n");
    pthread_mutex_lock(&clientMutex);
    printf("After lock\n");
    writeToFIFO(message, myfifo);
    printf("after write\n");
    pthread_mutex_unlock(&clientMutex);
    printf("end thread func\n");
    pthread_exit(NULL);
    return NULL;
}

void writeToFIFO(Message *message, char *fifo){
    int fd  = open(fifo, O_WRONLY);
    if(fd == -1){
        fprintf(stderr, "Error opening FIFO \n");
        return;
    }
    write(fd, message, sizeof(Message));

    close(fd);
}

void readFromFIFO(char *myfifo){
    char response[100];

    int fd = open(myfifo, O_RDONLY);
    read(fd, response, sizeof(response));
    printf("Message = %s\n", response);
    close(fd);
} 
