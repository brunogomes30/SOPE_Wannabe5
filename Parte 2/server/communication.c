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


int FIFOexists(char* fifo){

    int fd = open(fifo, O_WRONLY);
    if (fd == -1){
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
    int filedesc = open(fifo, O_RDONLY);
    if(read(filedesc, response, sizeof(Message)) == sizeof(Message)){
            close(filedesc);
            return 0;
    }
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

        if(readFromFIFO(privateFIFO,response) == 0){
            //Server stop warning
            if(response->tskres == -1){
                pthread_mutex_lock(&clientMutex);
                serverClosed = 1;
                pthread_mutex_unlock(&clientMutex);
                writeLog(response,CLOSD);
            }
            else{
                writeLog(response,GOTRS);
            }
        }else{
            writeLog(message, GAVUP);
            copyMessage(response,message); 
        }
   
    deleteFIFO();
    return response;
}

int sendServerRequest(char* publicFIFO, Message* message){
    int waitingWindow = 0;

    //Busy waiting for the public FIFO to be created

    while( !clientTimeOut ){
        if (writeToFIFO(publicFIFO,message) != -1){
            //pthread_mutex_unlock(&clientMutex);
            return 0;
        }
        sleep(1);
    }
    pthread_mutex_lock(&clientMutex);
    serverClosed = 1;
    pthread_mutex_unlock(&clientMutex);

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
    //pthread_mutex_lock(&clientMutex);
    ClientThreadArgs *threadArgs = (ClientThreadArgs *) arg;
    char * publicFIFO = threadArgs->fifo;
    Message *message = initializeMessage(threadArgs);
   
    if(write(publicFIFOfd ,message, sizeof(Message)) == sizeof(Message)){
        writeLog(message, IWANT);
        char privateFIFO[100];
        snprintf(privateFIFO, 100, "/tmp/%d.%ld", message->pid, message->tid); 
        createFIFO(privateFIFO);

        //pthread_mutex_lock(&clientMutex);
        Message *response = getServerResponse(privateFIFO, publicFIFO, message);
        //pthread_mutex_unlock(&clientMutex);
        free(response);
    }
    free(message);
    free(arg);
    //deleteFIFO();
    //unlink(privateFIFO);
    //pthread_mutex_unlock(&clientMutex);
    pthread_exit(NULL);
}