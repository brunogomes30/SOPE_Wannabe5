#include "../include/utils.h"
#include<time.h>
#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int getInst(){
    return time(0);
}

bool isNumber(char *str){
    for(unsigned i=0; str[i]!= 0; i++){
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

int FIFOexists(char* fifo){
    return !access(fifo, F_OK);
}

void createFIFO(char *fifo){
    if(mkfifo(fifo,0777) == -1){
        printf("ERRO\n");
    }
}

void deleteFIFO(){
    char fifo[100];
    snprintf(fifo, sizeof(fifo), "/tmp/%d.%ld", getpid(), pthread_self()); 
    unlink(fifo);
}
