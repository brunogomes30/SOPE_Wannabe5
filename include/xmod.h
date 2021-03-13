#ifndef XMOD_H
#define XMOD_H

#include<unistd.h>
#include<sys/time.h>


int xmod(char *path, char *modeStr, u_int8_t flags, mode_t previousMode);

void goThroughDirectory(char *path, int nargs, char *args[], u_int8_t flags);

void getSymbolic(mode_t mode, char *output);

void sigintHandler(int signal);

int symbolicXmod(char *modeStr, mode_t *newMode);

long timedifference_msec();

#endif 