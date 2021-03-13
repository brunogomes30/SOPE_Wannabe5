#ifndef LOGFILE_H
#define LOGFILE_H

#include <sys/types.h>
#include"macros.h"
typedef struct{
    int nModif, nTotal;
    char *currentDirectory;
    struct timeval startTime;
} XmodData;

enum logEvent{
    PROC_CREAT,
    PROC_EXIT,
    SIGNAL_RECV,
    SIGNAL_SENT,
    FILE_MODF
};

static char *eventsStr[] = {"PROC_CREAT", "PROC_EXIT", "SIGNAL_RECV", "SIGNAL_SENT", "FILE_MODF"};

void initLog(XmodData *processData);

void writeLog(int pid, enum logEvent event, char *msg, XmodData *processData);

#endif