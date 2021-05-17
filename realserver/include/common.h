#ifndef SERVER_INCLUDE_COMMON_H_
#define SERVER_INCLUDE_COMMON_H_
#include<pthread.h>
typedef struct {
	int rid; 										// request id
	pid_t pid; 										// process id
	pthread_t tid;									// thread id
	int tskload;									// task load
	int tskres;										// task result
} Message;
#endif // SERVER_INCLUDE_COMMON_H_
