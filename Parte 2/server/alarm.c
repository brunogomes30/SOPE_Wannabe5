#include "alarm.h"
#include<signal.h>
#include <stdlib.h>
#include <stdio.h>


void alarmHandler(){
    printf("Alarm handler \n");
    clientTimeOut = 1;
}

void setupAlarm(){
    signal(SIGALRM, alarmHandler);
}