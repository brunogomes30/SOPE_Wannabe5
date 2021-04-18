#ifndef INCLUDE_XMOD_H_
#define INCLUDE_XMOD_H_

#include<unistd.h>
#include<sys/time.h>



    /**
 * @brief 
 * Changes permissions of a single file
 * @param path 
 * @param modeStr 
 * @param flags 
 * @param previousMode 
 * @return int 
 */
    int xmod(char *path, char *modeStr,int8_t flags, mode_t previousMode);

/**
 * @brief 
 * Changes permissions of the directory, creating a new process to handle it
 * @param path 
 * @param nargs 
 * @param args 
 * @param flags 
 */
void goThroughDirectory(char *path, int nargs, char *args[], int8_t flags);


/**
 * @brief 
 * If it's first process, sends a signal to stop all processes and prompts the user to continue.
 * Else waits for the CONT signal.
 * @param signal 
 */
void sigintHandler(int signal);

/**
 * @brief 
 * Setups signal handlers
 */
void receiveSignal();

/**
 * @brief 
 * Writes the signal log
 * @param sig 
 */
void genericSignalHandler(int sig);

/**
 * @brief 
 * Stops all the processes until SIG_CONT is sent by the parent
 */
void stopProcesses();

/**
 * @brief 
 * Converts a symbolic mode into an octal mode
 * @param modeStr 
 * @param newMode 
 * @return int 
 */
int symbolicXmod(char *modeStr, mode_t *newMode);


#endif //INCLUDE_XMOD_H_