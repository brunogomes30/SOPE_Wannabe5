#include "communication.h"
#include "common.h"
#include "utils.h"
#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
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
    message->tskres = -1;
    message->rid = threadArgs->rid;
    char * myfifo = threadArgs->fifo;
    
    char privateFIFO[100];
    snprintf(privateFIFO, 100, "/tmp/%d.%ld", message->pid, message->tid); 
    printf("Private fifo = %s\n", privateFIFO);
    initFIFO(privateFIFO);
    
    pthread_mutex_lock(&clientMutex);
    writeToFIFO(message, myfifo);
    pthread_mutex_unlock(&clientMutex);
    writeLog(message, IWANT);

    

    usleep(getRandomNumber(10, 100) * 1000);
    Message *response = (Message *)malloc(sizeof(Message));
    if(readFromFIFO(privateFIFO, response) == -1){
        //timeout
        writeLog(message, GAVUP);
    }
    if(response->tskres == -1){
        //Server closed
        writeLog(response, CLOSD);
    } else {
        writeLog(response, GOTRS);
    }

    if(unlink(privateFIFO) == -1){
        fprintf(stderr, "Failed to delete private FIFO\n");
    }
    free(message);
    free(response);
    pthread_exit(NULL);
    return NULL;
}

void writeToFIFO(Message *message, char *fifo){
    int fd  = open(fifo, O_WRONLY );
    if(fd == -1){
        fprintf(stderr, "Error opening FIFO \n");
        return;
    }
    write(fd, message, sizeof(Message));
    close(fd);
}

int readFromFIFO(char *myfifo, Message *response){
    
    printf("Before read \n");
    int fd = open(myfifo, O_RDONLY);
    /*
    struct timeval timeout;
    int filedesc = open( myfifo, O_RDWR );
    fd_set set;
    FD_ZERO(&set); 
    FD_SET(filedesc, &set); 

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    int rv = select(filedesc + 1, &set, NULL, NULL, &timeout);
    if(rv == -1){
        //error
    } else if (rv == 0){
        //timeout
        return -1;
    } else {
        read(fd, response, sizeof(Message));    
    }    
    */
    read(fd, response, sizeof(Message));    
    
    printf("After read 1\n");
    close(fd);
    printf("After read 2\n");
    return 0;
} 
