#include "log.h"
#include<stdio.h>
#include "utils.h"
void writeLog(Message msg, Operation op ){
    fprintf(stdout, "%d ; %d ; %d ; %d ; %d ; %s\n", getInst(), msg.rid, msg.tskload, msg.pid, msg.tskres, operationsStr[op]);
}