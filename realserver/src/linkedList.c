#include "../include/linkedList.h"
#include <stdlib.h>

LinkedListElement *initLinkedList(pthread_t thread){
    LinkedListElement * newElement = (LinkedListElement *) malloc(sizeof(LinkedListElement));
    if (newElement == NULL)
        return NULL;
    newElement->thread = thread;
    return newElement;

}

LinkedListElement* addElement(LinkedListElement *last, pthread_t thread){
    LinkedListElement * newElement = (LinkedListElement *) malloc(sizeof(LinkedListElement));
    if (newElement == NULL)
        return NULL;
    newElement->thread = thread;
    newElement->next = NULL;
    last->next = newElement;
    return newElement;
}

void freeLinkedList(LinkedListElement *begin){
    LinkedListElement *elem = begin;
    while(elem != NULL){
        LinkedListElement *previous = elem;
        elem = elem->next;
        free(previous);
    }
}