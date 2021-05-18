#ifndef QUEUE_H
#define QUEUE_H
#include "../include/common.h"
#include <stdbool.h>
#include <stdlib.h> 

typedef struct Node Node;


struct Node {
    Message k;
    Node *Next;
};

typedef struct {
    Node *first, *last;
    int size, maxSize;
} Queue;

Message copyMessage(Message *message);

Queue* initQueue(int maxSize);

bool empty(Queue *queue);

bool push(Queue *queue, Message *Message);

Node* front(Queue *queue);

void pop(Queue *queue);

#endif //QUEUE_H