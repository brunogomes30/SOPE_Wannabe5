#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/queue.h"

bool fullBuffer = false;
bool emptyBuffer = true;


Message copyMessage(Message *toCopy){
    Message message;

    message.pid = toCopy->pid;
    message.rid = toCopy->rid;
    message.tid = toCopy->tid;
    message.tskload = toCopy->tskload;
    message.tskres = toCopy->tskres;

    return message;
}

Queue* initQueue(int maxSize){
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->maxSize = maxSize;
    queue->size = 0;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

bool empty(Queue *queue) {
    return (queue->first == NULL);
}

bool push(Queue *queue, Message *message) {
    Node* MyNode = (Node *) malloc(sizeof(Node));
    MyNode->Next=NULL;
    MyNode->k = copyMessage(message);

    if(empty(queue)) {
        queue->first = MyNode;
        queue->last = MyNode;
    }
    else {
        queue->last->Next = MyNode;
        queue->last=MyNode;
    }

    queue->size++; 

    if (queue->size == queue->maxSize){
        fullBuffer = true;
    }else{
        fullBuffer = false;
    }
    emptyBuffer = false;
    return true;
}

Node* front(Queue *queue) {
    return queue->first;
}

void pop(Queue *queue) {
    Node* n = queue->first;
    queue->first = queue->first->Next;
    queue->size--;

    if (queue->size == 0)
        emptyBuffer = true;
    else
        emptyBuffer = false;
    if (queue->size != queue->maxSize)
        fullBuffer = false;
    free(n);
}