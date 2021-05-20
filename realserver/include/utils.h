#ifndef REALSERVER_INCLUDE_UTILS_H_
#define REALSERVER_INCLUDE_UTILS_H_
#include <stdbool.h>

int getInst();

bool isNumber(char *str);

int FIFOexists(char* fifo);

void createFIFO(char* fifo);

void deleteFIFO();

#endif //REALSERVER_INCLUDE_UTILS_H_
