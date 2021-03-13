#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/logFile.h"

void writeLog(int pid, enum logEvent event, char *msg, XmodData *processData){

    if (getenv("LOG_FILENAME") != NULL){

        FILE *file = fopen(getenv("LOG_FILENAME"), "a+");
        if (file == NULL){

            fprintf(stderr, "Fopen error\n");
            return;
        }

        fprintf(file, "%lu ; %d ; %s ; %s\n", timedifference_msec(processData), pid, eventsStr[event], msg);
        fclose(file);
    }
}

void initLog(XmodData* processData) {

    gettimeofday(&processData->startTime, NULL);

    char *buffer = (char *)malloc(sizeof(char) * 50);
    sprintf(buffer, "xmodStartTime= %lu %lu", processData->startTime.tv_sec, processData->startTime.tv_usec);
    putenv(buffer);
    
    FILE *file = fopen(getenv("LOG_FILENAME"), "w+");
    if (file == NULL) {
        fprintf(stderr, "Fopen error\n");
        return;
    }
    fclose(file);
}