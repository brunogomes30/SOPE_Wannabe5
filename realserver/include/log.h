#ifndef SERVER_INCLUDE_LOG_H_
#define SERVER_INCLUDE_LOG_H_
#include "../include/common.h"

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

#endif //SERVER_INCLUDE_LOG_H_
