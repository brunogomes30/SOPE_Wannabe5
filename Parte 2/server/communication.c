#include "communication.h"
#include "utils.h"
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
extern int clientTimeOut;

int FIFOexists(char* fifo){

    int fd = open(fifo, O_WRONLY | O_NONBLOCK);
    if (fd == -1){
        serverClosed = 1;
        return 0;
    }
    close(fd);

    return 1;
}

int writeToFIFO(char *fifo, Message *message){
    if (!FIFOexists(fifo))
        return -1;

    int fd  = open(fifo, O_WRONLY);
    if(fd == -1){
        return -1;
    }

    write(fd, message, sizeof(Message));
    close(fd);
    return 0;
}

int readFromFIFO(char *fifo, Message *response){
    
        struct timeval timeout;
        int filedesc = open(fifo, O_RDWR);
        fd_set set;
        FD_ZERO(&set); 
        FD_SET(filedesc, &set); 
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        int rv = select(filedesc + 1, &set, NULL, NULL, &timeout);
        if(rv !=-1 && rv != 0){
            read(filedesc, response, sizeof(Message));
            close(filedesc);

            return 0; 
        }
        
        close(filedesc);
    return -1;
} 

void copyMessage(Message* copy, Message* toCopy){
    copy->pid = toCopy->pid;
    copy->rid = toCopy->rid;
    copy->tid = toCopy->tid;
    copy->tskload = toCopy->tskload;
    copy->tskres = toCopy->tskres;
}

Message* getServerResponse(char* privateFIFO, char* publicFIFO,Message* message){ 
    Message *response = (Message *)malloc(sizeof(Message));
    
    //Server exists so block program to read
    if (FIFOexists(publicFIFO)){
        if(readFromFIFO(privateFIFO,response) == 0){
            //Server stop warning
            if(response->tskres == -1){
                writeLog(response,CLOSD);
            }
            else{
                writeLog(response,GOTRS);
            }
        }else{
            writeLog(message, GAVUP);
            copyMessage(response,message); 
        }
    }
    else{
         //Server closed
        writeLog(message,CLOSD);
        copyMessage(response,message);
    }
   
    deleteFIFO();
    return response;
}

int sendServerRequest(char* publicFIFO, Message* message){
    int waitingWindow = 0;

    //Busy waiting for the public FIFO to be created
    while( waitingWindow++ < 10000 ){
        if (writeToFIFO(publicFIFO,message) != -1){
            return 0;
        }
    }

    FIFOexists(publicFIFO);
    return -1;
}

Message* initializeMessage(ClientThreadArgs* threadArgs){
    Message *message = (Message *)malloc(sizeof(Message));

    message->tid = pthread_self();
    message->pid = getpid();
    message->tskload = getTaskRandom();
    message->tskres = -1;
    message->rid = threadArgs->rid;

    return message;
}

void *thread_func(void *arg){
    ClientThreadArgs *threadArgs = (ClientThreadArgs *) arg;
    char * publicFIFO = threadArgs->fifo;
    Message *message = initializeMessage(threadArgs);
   
    pthread_mutex_lock(&clientMutex);
    char privateFIFO[100];
    snprintf(privateFIFO, 100, "/tmp/%d.%ld", message->pid, message->tid); 
    createFIFO(privateFIFO);
    sendServerRequest(publicFIFO,message);
    writeLog(message, IWANT);
    pthread_mutex_unlock(&clientMutex);

    Message *response = getServerResponse(privateFIFO, publicFIFO, message);

    free(message);
    free(response);
    deleteFIFO();
    unlink(privateFIFO);
    pthread_exit(NULL);
}
