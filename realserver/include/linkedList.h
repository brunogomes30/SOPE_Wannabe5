#ifndef SERVER_INCLUDE_LINKEDLIST_H_
#define SERVER_INCLUDE_LINKEDLIST_H_
#include <pthread.h>

typedef struct LinkedListElement LinkedListElement;

struct LinkedListElement
{
    pthread_t thread;
    LinkedListElement *next;
};

/**
 * @brief 
 * Initializes a threads linked list
 * @param thread 
 */
LinkedListElement *initLinkedList(pthread_t thread);

/**
 * @brief 
 * Adds new element to the linked list
 * @param last 
 * @param thread 
 */
LinkedListElement *addElement(LinkedListElement *last, pthread_t thread);

/**
 * @brief 
 * Deletes linked list and frees memory
 * @param begin 
 */
void freeLinkedList(LinkedListElement *begin);

#endif //SERVER_INCLUDE_LINKEDLIST_H_