#ifndef INCLUDE_LOGFILE_H_ 
#define INCLUDE_LOGFILE_H_ 

#include <sys/types.h>

#include "../include/macros.h"

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

void initLog(XmodData *processData);

void writeLog(int pid, enum logEvent event, char *msg, XmodData *processData);

#endif //INCLUDE_LOGFILE_H_ 