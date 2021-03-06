#include <string.h>
#include <sys/time.h>

#include "../include/auxXmod.h"

u_int8_t getFlags(int nargs, char *args[]){
    u_int8_t flags = 0;
    for (int i = 1; i < nargs - 2; i++){
        char *flagStr = args[i];

        if (flagStr[0] != '-' || flagStr[2] != 0) {
            fprintf(stderr, "Invalid flag %s", flagStr);
            return -1;
        }
        switch (flagStr[1]){
        case 'v':
            flags |= VERB_FLAG;
            break;
        case 'c':
            flags |= CHANG_FLAG;
            break;
        case 'R':
            flags |= REC_FLAG;
            break;
        default:
            printf("Invalid flag %s", flagStr);
            return -1;
        }
    }
    return flags;
}

void getSymbolic(mode_t mode, char *output) {

    char fullMode[] = "rwxrwxrwx";

    for(int i=0;i<9;i++){
        if(!(mode & (1 << i)))
            fullMode[8-i] = '-';
    }
    
    snprintf(output, sizeof(char) * 10, "%s", fullMode);
}

u_int64_t timeDifferenceMS(XmodData *processData){

    struct timeval t1;
    gettimeofday(&t1, NULL);

    return (t1.tv_sec - processData->startTime.tv_sec) * 1000 + (t1.tv_usec - processData->startTime.tv_usec) / 1000;
}

void getArgStr(int nargs, char* args[], char* logMsg){
    //printf("%s\n",logMsg);
    strcpy(logMsg, "");
    for (unsigned i = 1; i < nargs; i++) {
		strcat(logMsg, args[i]);
		if (i < nargs - 1)
			strcat(logMsg, " ");
	}
    //printf("%s\n",logMsg);
}