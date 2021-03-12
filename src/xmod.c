/*
*	Example of threads in action
*/
#include <sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/mman.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<time.h>
#include<fcntl.h>
#include <stdbool.h>
#include "../include/xmod.h"

XmodData processData;


void stopProcesses() {
	printf("Do you want to continue? (y or n)\n");
	char input;
	do{
		scanf("%c", &input);
		if (input == 'n') {
			writeLog(getpid(), SIGNAL_SENT, "SIGKILL : 0");
			kill(0, SIGKILL);
		}
	} while(input != 'y' && input != 'n');
	writeLog(getpid(), SIGNAL_SENT, "SIGCONT");
	kill(0, SIGCONT);
}

void getSymbolic(mode_t mode, char *output){
	char *symbols = "rwx";

	int i = 8, j = 2;
	while(mode){
		if(mode & 1){
			output[i--] = symbols[j];
			
		} else {
			output[i--] = '-';
		}
		mode = mode >> 1;
		j--;
		if(j < 0) j = 2; 
	}
	output[9] = 0;
}

void contHandler(int sig){
	//writeLog(getpid(), SIGNAL_RECV, "SIGCONT");
}

void sigintHandler(int sig){
	printf("%d ; %s ; %d ; %d\n", getpid(), processData.currentDirectory, processData.nTotal, processData.nModif);
	writeLog(getpid(), SIGNAL_RECV, "SIGINT");
	if(getpid() == getpgrp()) {
		stopProcesses();
	} else {
		//Wait for signal from first process

		signal(SIGCONT, contHandler);
		sigset_t wset;
		sigemptyset(&wset);
		sigaddset(&wset, SIGCONT);
		int sig;
		sigwait(&wset, &sig);
		writeLog(getpid(), SIGNAL_RECV, "SIGCONT");
	
	}
}

/**
 * @brief 
 * 
 * @param path 
 * @param modeStr 
 * @param flags 100 - v ; 010 - c ; 001 - R
 * @return int 
 */
int xmod(char *path, char *modeStr, u_int8_t flags, mode_t previousMode)
{
	//printf("Path = %s\n", path);
	processData.nTotal++;
	char first = modeStr[0];
	mode_t mode;
	signal(SIGINT, sigintHandler);


	//sleep(1);
	previousMode &= 0777;
	if(first == '0'){
		mode = strtol(modeStr, 0, 8);
	} else if(first == 'u' || first == 'g' || first == 'o' || first == 'a'){
		mode = previousMode;
		symbolicXmod(modeStr, &mode);

	} else {
		fprintf(stderr,"Invalid type of user - %c\n", first);
		return -1;
	}
	
	if(chmod(path, mode) != 0){
		fprintf(stderr,"Error\n");
		return  -1;
	}


	if(previousMode != mode) processData.nModif = processData.nModif + 1;
	char previousModeS[10], modeS[10];
	previousMode &= 0777;
	if(flags & 0b110){
		getSymbolic(previousMode, previousModeS);
		getSymbolic(mode, modeS);

		if(previousMode != mode){
			printf("modo de '%s' alterado de %04o (%s) para %04o (%s)\n", path, previousMode, previousModeS, mode, modeS);
		} else if(flags & 0b100){
			printf("modo de '%s' mantido como %04o (%s)\n", path, mode, modeS);
		}
	}
	if(previousMode != mode){
		char msg[500];
		sprintf(msg, "%s : %04o : %04o", path, previousMode, mode);
		writeLog(getpid(), FILE_MODF, msg);
	}

	return 0;
}

int symbolicXmod(char *modeStr, mode_t *newMode){
	char operator = modeStr[1];
	mode_t mode = 0, previousMode = *newMode; 
	int i = 2;
	//Get permissions
	while(modeStr[i] != 0){
		switch(modeStr[i++]){
			case 'r':
				mode += 4;
				break;
			case 'w':
				mode += 2;
				break;
			case 'x':
				mode += 1;
				break;
			default:
				printf("Invalid permission - %c, fullString = %s\n", modeStr[i], modeStr);
				return -1;
		}
	}
	unsigned mask;

	//Get type of users
	switch(modeStr[0]){
		case 'u':
			mode *= 64;
			mask = 63;
			break;
		case 'g':
			mode *= 8;
			mask = 7 + 511 - 63;
			break;
		case 'o':
			mask = 511 - 7;
			break;
		case 'a':
			mask = 0;
			mode = mode + mode*8 + mode*64;
			break;
		default:
			fprintf(stderr,"Invalid type of user - %c\n", modeStr[0]);
			return -1;
	}
	mask += 0100000;
	//Get new mode
	switch(operator){
		case '-':
			*newMode = previousMode - (previousMode & mode);
			break;
		case '+':
			*newMode |= mode;
			break;
		case '=':
			*newMode = (previousMode & mask) | mode; 
			break;
		default:
			fprintf(stderr,"Invalid operator - %c\n", operator);
			return -1;
	}
	return 0;
}



long timedifference_msec() {
	struct timeval t1;
	gettimeofday(&t1, NULL);
	
	return (t1.tv_sec - processData.startTime.tv_sec) * 1000 + (t1.tv_usec - processData.startTime.tv_usec) / 1000;
}

void writeLog(int pid, enum logEvent event, char * msg){
	if(getenv("LOG_FILENAME") != NULL){
		FILE *file = fopen(getenv("LOG_FILENAME"), "a+");
		if(file  == NULL){
			fprintf(stderr, "Fopen error\n");
			return;
		}	
		fprintf(file, "%lu ; %d ; %s ; %s\n", timedifference_msec(), pid, eventsStr[event], msg);
		fclose(file);
	}
}

void initLog(){
	gettimeofday(&processData.startTime, NULL);
	printf("Time = %lu %lu\n", processData.startTime.tv_sec, processData.startTime.tv_usec);
	char *buffer = (char *)malloc(sizeof(char) * 50);
	sprintf(buffer, "xmodStartTime= %lu %lu", processData.startTime.tv_sec, processData.startTime.tv_usec);
	putenv(buffer);
	FILE *file = fopen(getenv("LOG_FILENAME"), "w+");
	if(file == NULL){
		fprintf(stderr, "Fopen error\n");
		return;
	}
	fclose(file);
}


u_int8_t getFlags(int nargs, char*args[]){
	u_int8_t flags = 0;
	for (int i = 1; i < nargs - 2; i++) {
		char *flagStr = args[i];
		if (flagStr[0] != '-' || flagStr[2] != 0) {
			fprintf(stderr, "Invalid flag %s", flagStr);
			return -1;
		}
		switch (flagStr[1]) {
		case 'v':
			flags |= 0b100;
			break;
		case 'c':
			flags |= 0b010;
			break;
		case 'R':
			flags |= 0b001;
			break;
		default:
			printf("Invalid flag %s", flagStr);
			return -1;
		}
	}
	return flags;
}

int main(int nargs, char *args[]) {
	processData.nTotal = 0;
	processData.nModif = 0;
	char *msg[1024];
	bool hasLog = getenv("LOG_FILENAME") != NULL;
	 if ((getpgrp() == getpid()) && hasLog ) {
		 initLog();
	}
	else if(hasLog) {
		char *msg = getenv("xmodStartTime");
		struct timeval startTime;
		
		sscanf(msg, "%lu %lu", &startTime.tv_sec, &startTime.tv_usec);
		processData.startTime = startTime;
	}
	char *logInfo = (char *) malloc(sizeof(char) * 500);
	for(unsigned i=1; i<nargs; i++){
		strcat(logInfo, args[i]);
		if(i < nargs - 1) 
			strcat(logInfo, " ");
	}
	if(hasLog)
		writeLog(getpid(), PROC_CREAT, logInfo);

	if(nargs > 2){
		u_int8_t flags = getFlags(nargs, args);
		// -ux
		char *modeStr = args[nargs - 2];

		
		char path[250];
		strcpy(path,args[nargs-1]);
		processData.currentDirectory = path;
		struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
		if (stat(processData.currentDirectory, fileInfo) != 0)
		{
			fprintf(stderr, "Error stat() %s\n", processData.currentDirectory);
			return -1;
		}

		//First process
		if(getpgrp() == getpid()) {
			xmod(processData.currentDirectory, modeStr, flags, fileInfo->st_mode);
		}
		if (flags & 0b001 && S_ISDIR(fileInfo->st_mode)) {
			//TODO:: Bia
			//Recursive
			DIR *dir;
			struct dirent *dent;
			dir = opendir(processData.currentDirectory);
			int *status = 0;
			if(dir!=NULL) {

				while((dent=readdir(dir))!=NULL){
					if( !strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) continue;; 
					char nextPath[250];
					strcpy(nextPath, processData.currentDirectory);
					strcat(nextPath, "/");
					strcat(nextPath, dent->d_name);
					struct stat *nextFileInfo = (struct stat *) malloc(sizeof(struct stat));
					if(stat(nextPath, nextFileInfo) != 0)
						fprintf(stderr,"Error stat() %s\n", nextPath);
					xmod(nextPath, modeStr, flags, nextFileInfo->st_mode);
					if(S_ISDIR(nextFileInfo->st_mode)){
						//char msg[1024];
						int id = fork();
						switch(id){
							case 0:
								// Child
								strcpy(args[nargs - 1], nextPath);
								execvp("./xmod", args);
								exit(0);
							case -1:
								//ERRO
								return -1;
							default:
								//Create Process Log
								if(hasLog) {
									strcat(logInfo, "/");
									strcat(logInfo, dent->d_name);
									writeLog(getpid(), PROC_CREAT, logInfo);
								}
								//sprintf(msg, "%ld %ld", startTime.tv_sec, startTime.tv_usec); APAGAR
								wait(status);
								
								//Process exit log
								if(hasLog) {
									//char msg[15];
									sprintf(msg, "%d", status);
									writeLog(id, PROC_EXIT, msg);
								}
								break;
						}
					}
				}
				closedir(dir);
			}
		}
	}
	printf("\n\t\tFINAL nModif = %d, nTotal = %d\n", processData.nModif, processData.nTotal);
	return 0;
}
