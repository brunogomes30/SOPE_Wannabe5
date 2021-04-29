#ifndef LOG_H
#define LOG_H
#include "common.h"

typedef enum
{
    IWANT,
    RECVD,
    TSKEX,
    TSKDN,
    GOTRS,
    LATE,
    CLOSD,
    GAVUP,
    FAILD
} Operation;

/**
 * @brief 
 * Register action in the stdout
 * @param msg 
 * @param op 
 */
void writeLog(Message *msg, Operation op);

#endif
