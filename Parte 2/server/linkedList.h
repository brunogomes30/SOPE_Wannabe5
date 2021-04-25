#include<pthread.h>

typedef struct LinkedListElement LinkedListElement;

struct LinkedListElement {
    pthread_t thread;
    LinkedListElement *next;
};

LinkedListElement *initLinkedList(pthread_t thread);

LinkedListElement* addElement(LinkedListElement *last, pthread_t thread);

void freeLinkedList(LinkedListElement *begin);