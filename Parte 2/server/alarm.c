#include "alarm.h"
#include<signal.h>
#include <stdlib.h>
#include <stdio.h>
void alarmHandler(){
    printf("Alarm handler \n");
    exit(0);
}

void setupAlarm(){
    signal(SIGALRM, alarmHandler);
}