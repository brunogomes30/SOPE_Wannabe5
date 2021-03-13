#ifndef AUXXMOD_H
#define AUXXMOD_H

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "logFile.h"
#include "macros.h"
u_int8_t getFlags(int nargs, char *args[]);
void getSymbolic(mode_t mode, char *output);
long timedifference_msec(XmodData *processData);

#endif