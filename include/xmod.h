#ifndef XMOD_H
#define XMOD_H

#include<unistd.h>


enum logEvent {PROC_CREAT, PROC_EXIT, SIGNAL_RECV, SIGNAL_SENT, FILE_MODE};
static char *eventsStr[] = {"PROC_CREAT", "PROC_EXIT", "SIGNAL_RECV", "SIGNAL_SENT", "FILE_MODF"};
int xmod(char *path, char *modeStr, short flags, mode_t previousMode);
void getSymbolic(mode_t mode, char *output);
void sigintHandler(int signal);
int symbolicChmod(char *modeStr, mode_t *newMode);
long timedifference_msec();
#endif 