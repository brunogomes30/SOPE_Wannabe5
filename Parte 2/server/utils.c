#include "utils.h"
#include<time.h>
#include<stdlib.h>
int getRandomNumber(int lower, int upper) {
    if(upper > lower)
        return (rand() % (upper - lower)) + lower;
    else 
        return -1;
}

int getRandom(){
    return getRandomNumber(1, 10);
}

int getInst(){
    return time(0);
}

bool isNumber(char *str){
    for(unsigned i=0; str[i]!= 0; i++){
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}