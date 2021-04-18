#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include <stdbool.h>
#include<string.h>
#include <unistd.h>
#include<pthread.h>
#include "alarm.h"
#include "utils.h"

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
    char * number;
    if(argc == 3){
        number = args[1];
        pathFIFO = args[2];
        strncpy(number, number + 2, sizeof(number) - 2);
    } else if(argc == 4){
        number = args[2];
        pathFIFO = args[3];
    }
    sscanf(number, "%d", nsecs);
    return 0;
}

int main(int argc, char *args[]){
    checkArgs(argc, args);
    char *pathFIFO = "";
    int nsecs;
    parseArgs(argc, args, &nsecs, pathFIFO);

    setupAlarm();
    alarm(nsecs);

    while(1){
        usleep(50 * 1000);
    }



    return 0;
}