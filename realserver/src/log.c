#include "../include/log.h"
#include<stdio.h>
#include "../include/utils.h"

static char * operationsStr[] = {"IWANT", "RECVD", "TSKEX", "TSKDN", "GOTRS", "2LATE", "CLOSD", "GAVUP", "FAILD"};

void writeLog(Message *msg, Operation op ){
    fprintf(stdout, "%d ; %d ; %d ; %d ; %ld ; %d ; %s\n", getInst(), msg->rid, msg->tskload, msg->pid, msg->tid, msg->tskres, operationsStr[op]);
}

