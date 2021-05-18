#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/queue.h"
#include <semaphore.h>
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

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
    sem_init(&(queue->empty), 0, 0);
    sem_init(&(queue->full), 0, maxSize);
    int valueEmpty, valueFull;
    sem_getvalue(&queue->empty, &valueEmpty);
    sem_getvalue(&queue->full, &valueFull);
    return queue;
}

bool emptyBuffer(Queue *queue) {
    return (queue->first == NULL);
}

bool push(Queue *queue, Message *message) {
    
    sem_wait(&(queue->full));
    pthread_mutex_lock(&queueMutex);
    Node* MyNode = (Node *) malloc(sizeof(Node));
    MyNode->Next=NULL;
    MyNode->k = copyMessage(message);

    if(emptyBuffer(queue)) {
        queue->first = MyNode;
        queue->last = MyNode;
    }
    else {
        queue->last->Next = MyNode;
        queue->last=MyNode;
    }

    queue->size++; 
    pthread_mutex_unlock(&queueMutex);
    sem_post(&(queue->empty));
    return true;
}

Message* pop(Queue *queue) {
    struct timespec timeout;
    if (clock_gettime(CLOCK_REALTIME, &timeout) == -1)
    {
        /* handle error */
        return NULL;
    }

    timeout.tv_sec += 1;
    
    if(sem_timedwait(&queue->empty, &timeout) == -1){
        return NULL;
    }
    //sem_wait(&queue->empty);
    pthread_mutex_lock(&queueMutex);
    Node* n = queue->first;
    Message * m = (Message *) malloc(sizeof(Message));
    *m = n->k;
    queue->first = queue->first->Next;
    queue->size--;
    pthread_mutex_unlock(&queueMutex);
    sem_post(&queue->full);
    free(n);
    return m;
}

void destroyQueue(Queue * queue){
    sem_destroy(&(queue->empty));
    sem_destroy(&(queue->full));
    free(queue);
}