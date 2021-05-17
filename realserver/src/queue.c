#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/queue.h"


bool empty(Queue *queue) {
    if(queue->first==NULL) return true;
    return false;
}

void push(Queue *queue, Node *MyNode) {

     MyNode->Next=NULL;
     if(empty(queue)) {
         queue->first = MyNode;
         queue->last = MyNode;
     }
     else {
         queue->last->Next = MyNode;
         queue->last=MyNode;
     }
}

Node* front(Queue *queue) {
    return queue->first;
}
void pop(Queue *queue) {
    free(queue->first->k);
    queue->first = queue->first->Next;
}