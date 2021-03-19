#ifndef INCLUDE_AUXXMOD_H_
#define INCLUDE_AUXXMOD_H_

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/logFile.h"
#include "../include/macros.h"

/**
 * @brief Get the Flags object
 * Gets the flags from the args
 * @param nargs 
 * @param args 
 * @return u_int8_t 
 */
int8_t getFlags(int nargs, char *args[]);

/**
 * @brief 
 * Returns the symbolic mode of a mode
 * @param mode 
 * @param output 
 */
void getSymbolic(mode_t mode, char *output);

/**
 * @brief 
 * Calculates the difference between crrrent time and processData's time
 * @param processData 
 * @return u_int64_t 
 */
u_int64_t timeDifferenceMS(XmodData *processData);

/**
 * @brief Get the Arg Str object
 * Concatenates arguments into a single string
 * @param nargs 
 * @param args 
 * @param logMsg 
 */
void getArgStr(int nargs, char* args[], char* logMsg);

#endif //INCLUDE_AUXXMOD_H_