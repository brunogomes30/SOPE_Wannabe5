#include "utils.h"
#include<time.h>
#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int getRandomNumber(int lower, int upper) {
    if(upper > lower)
        return (rand() % (upper - lower)) + lower;
    else 
        return -1;
}

int getTaskRandom(){
    return getRandomNumber(1, 10);
}

int getInst(){
    return time(0);
}

bool isNumber(char *str){
    for(unsigned i=0; str[i]!= 0; i++){
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

void createFIFO(char *fifo){
    if(mkfifo(fifo,0777) == -1){
        printf("ERRO\n");
    }
}

void deleteFIFO(){
    char* fifo = (char*)malloc(100);
    snprintf(fifo, 100, "/tmp/%d.%ld", getpid(), pthread_self()); 
    unlink(fifo);
    free(fifo);
}
