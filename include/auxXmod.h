#ifndef INCLUDE_AUXXMOD_H_
#define INCLUDE_AUXXMOD_H_

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/logFile.h"
#include "../include/macros.h"

u_int8_t getFlags(int nargs, char *args[]);

void getSymbolic(mode_t mode, char *output);

u_int64_t timeDifferenceMS(XmodData *processData);

void getArgStr(int nargs, char* args[], char* logMsg);

#endif //INCLUDE_AUXXMOD_H_