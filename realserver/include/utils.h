#ifndef SERVER_INCLUDE_UTILS_H_
#define SERVER_INCLUDE_UTILS_H_
#include <stdbool.h>


int getRandomNumber(int lower, int upper);

int getTaskRandom();

int getInst();

bool isNumber(char *str);

int FIFOexists(char* fifo);

void createFIFO(char* fifo);

void deleteFIFO();


#endif //SERVER_INCLUDE_UTILS_H_
