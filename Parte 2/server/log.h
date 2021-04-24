#ifndef LOG_H
#define LOG_H
#include "common.h"

static char * operationsStr[] = {"IWANT", "RECVD", "TSKEX", "TSKDN", "GOTRS", "2LATE", "CLOSD", "GAVUP", "FAILD"};
typedef enum {IWANT, RECVD, TSKEX, TSKDN, GOTRS, LATE, CLOSD, GAVUP, FAILD} Operation;

void writeLog(Message *msg, Operation op);

#endif
