#ifndef QUEUE_H
#define QUEUE_H
#include "../include/common.h"
#include <stdbool.h>
#include <stdlib.h> 
#include <semaphore.h>

typedef struct Node Node;


struct Node {
    Message k;
    Node *Next;
};

typedef struct {
    Node *first, *last;
    int size, maxSize;
    sem_t empty;
    sem_t full;
} Queue;

Message copyMessage(Message *message);

Queue* initQueue(int maxSize);

bool emptyBuffer(Queue *queue);

bool push(Queue *queue, Message *Message);

Message pop(Queue *queue);

void destroyQueue(Queue * queue);

#endif //QUEUE_H