#ifndef QUEUE_H
#define QUEUE_H
#include "../include/common.h"
#include <stdbool.h>
/**
 * @brief 
 * 
 * @param size 
 */
void queue_init(int size);


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool queue_isEmpty();

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool queue_isFull();

/**
 * @brief 
 * 
 * @return int 
 */
int queue_size();

/**
 * @brief 
 * 
 */
void queue_push(Message *);

/**
 * @brief 
 * 
 * @return Message* 
 */
Message * queue_pop();

typedef struct Node Node;


struct Node {
    Message *k;
    Node Next;
};

typedef struct {
    Node first,last;

    void push(Node MyNode) {
        MyNode->Next=NULL;
        if(empty()) {
            first=MyNode;
            last=MyNode;
        }
        else {
            last->Next = MyNode;
            last=MyNode;
        }
    }
    Node front() {
        return first;
    }
    void pop() {
        free(first->k);
        first=first->Next;
    }
    bool empty() {
        if(first==NULL) return true;
        return false;
    }
} Queue;


#endif //QUEUE_H