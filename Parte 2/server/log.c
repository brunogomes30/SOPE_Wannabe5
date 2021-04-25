#include "log.h"
#include<stdio.h>
#include "utils.h"

static char * operationsStr[] = {"IWANT", "RECVD", "TSKEX", "TSKDN", "GOTRS", "2LATE", "CLOSD", "GAVUP", "FAILD"};

void writeLog(Message *msg, Operation op ){
    fprintf(stdout, "%d ; %d ; %d ; %d ; %d ; %s\n", getInst(), msg->rid, msg->tskload, msg->pid, msg->tskres, operationsStr[op]);
}

