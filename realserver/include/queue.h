#ifndef QUEUE_H
#define QUEUE_H
#include "../include/common.h"
#include <stdbool.h>
#include <stdlib.h> 

typedef struct Node Node;


struct Node {
    Message *k;
    Node *Next;
};

typedef struct {
    Node *first, *last;
} Queue;

bool empty(Queue *queue);

void push(Queue *queue, Node *MyNode);

Node* front(Queue *queue);

void pop(Queue *queue);

#endif //QUEUE_H