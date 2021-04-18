#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../include/logFile.h"
#include "../include/auxXmod.h"



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
    FILE *file = fopen(getenv("LOG_FILENAME"), "w+");
    if (file == NULL) {
        fprintf(stderr, "Fopen error\n");
        return;
    }
    fclose(file);
}