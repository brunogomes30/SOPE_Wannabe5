#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

typedef struct {
    int rid;
    char *fifo;
} ClientThreadArgs;

void initFIFO(char * fifo);
void writeToFIFO();
int readFromFIFO();
void *thread_func(void *arg);
#endif