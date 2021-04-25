#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>


int getRandomNumber(int lower, int upper);

int getTaskRandom();

int getInst();

bool isNumber(char *str);

void createFIFO(char* fifo);

void deleteFIFO();


#endif
