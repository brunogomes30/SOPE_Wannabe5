#ifndef XMOD_H
#define XMOD_H

#include<unistd.h>


int xmod(char *path, char *modeStr, short flags);
void getSymbolic(mode_t mode, char *output);
void sigintHandler(int signal);
int symbolicChmod(char *modeStr, mode_t *newMode);

#endif 