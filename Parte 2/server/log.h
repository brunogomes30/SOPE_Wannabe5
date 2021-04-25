#ifndef LOG_H
#define LOG_H
#include "common.h"

typedef enum {IWANT, RECVD, TSKEX, TSKDN, GOTRS, LATE, CLOSD, GAVUP, FAILD} Operation;

void writeLog(Message *msg, Operation op);

#endif
