#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/queue.h"
#include "../include/common.h"

Message **intArray;
int max;
int front = 0;
int rear = -1;
int itemCount = 0;

void queue_init(int size){
    max = size;
    intArray = (Message **) malloc(sizeof(Message *) * size);
}


bool queue_isEmpty() {
   return itemCount == 0;
}

bool queue_isFull() {
   return itemCount == max;
}

int queue_size() {
   return itemCount;
}  

void queue_push(Message *data) {

   if(!queue_isFull()) {
	
      if(rear == max-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
   }
}

Message* queue_pop() {
   Message * data = intArray[front++];
	
   if(front == max) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}