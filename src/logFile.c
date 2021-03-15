#include "../include/logFile.h"
#include "../include/auxXmod.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


static char *eventsStr[] = {"PROC_CREAT", "PROC_EXIT", "SIGNAL_RECV", "SIGNAL_SENT", "FILE_MODF"};

void writeLog(int pid, enum logEvent event, char *msg, XmodData *processData){

    if (getenv("LOG_FILENAME") != NULL){

        FILE *file = fopen(getenv("LOG_FILENAME"), "a+");
        if (file == NULL){

            fprintf(stderr, "Fopen error\n");
            return;
        }
        fprintf(file, "%ld ; %d ; %s ; %s\n", timeDifferenceMS(processData), pid, eventsStr[event], msg);
        fclose(file);
    }
}

void initLog(XmodData* processData) {

    gettimeofday(&processData->startTime, NULL);

    char *buffer = (char *) malloc(sizeof(char) * 50);
    snprintf(buffer, sizeof(char) * 50, "xmodStartTime= %lu %lu", processData->startTime.tv_sec, processData->startTime.tv_usec);
    putenv(buffer);
    
    FILE *file = fopen(getenv("LOG_FILENAME"), "w+");
    if (file == NULL) {
        fprintf(stderr, "Fopen error\n");
        return;
    }
    fclose(file);
}