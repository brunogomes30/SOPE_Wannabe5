#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include "../include/xmod.h"
#include "../include/auxXmod.h"
#include "../include/macros.h"

XmodData processData;

void stopProcesses(){

	printf("Do you want to continue? (y or n)\n");
	char input;
	do{
		scanf("%c", &input);

		if (input == 'n'){

			writeLog(getpid(), SIGNAL_SENT, "SIGKILL : 0", &processData);
			kill(0, SIGKILL);
		}

	} while (input != 'y' && input != 'n');

	writeLog(getpid(), SIGNAL_SENT, "SIGCONT", &processData);
	kill(0, SIGCONT);
}

void contHandler(int sig){}

void sigintHandler(int sig){

	printf("%d ; %s ; %d ; %d\n", getpid(), processData.currentDirectory, processData.nTotal, processData.nModif);
	writeLog(getpid(), SIGNAL_RECV, "SIGINT", &processData);

	if (getpid() == getpgrp()){

		stopProcesses();}
	else{

		//Wait for signal from first process
		signal(SIGCONT, contHandler);
		sigset_t wset;
		sigemptyset(&wset);
		sigaddset(&wset, SIGCONT);
		int sig;
		sigwait(&wset, &sig);

		writeLog(getpid(), SIGNAL_RECV, "SIGCONT", &processData);
	}
}

/**
 * @brief 
 * 
 * @param path 
 * @param modeStr 
 * @param flags 
 * @return int 
 */
int xmod(char *path, char *modeStr, u_int8_t flags, mode_t previousMode)
{
	printf("in xmod\n");
	processData.nTotal++;
	char first = modeStr[0];
	mode_t mode;

	signal(SIGINT, sigintHandler);

	previousMode &= ALL_PERMS;

	if (first == '0'){   //numerical mode

		mode = strtol(modeStr, 0, 8);
	}
	else if (first == 'u' || first == 'g' || first == 'o' || first == 'a'){ //symbolic mode
		mode = previousMode;
		symbolicXmod(modeStr, &mode);
	}
	else{
		fprintf(stderr, "xmod: Invalid type of user - %c\n", first);
		return -1;
	}

	previousMode &= ALL_PERMS;

	char previousModeS[10], modeS[10];
	getSymbolic(previousMode, previousModeS);
	getSymbolic(mode, modeS);


	if (chmod(path, mode) != 0) {
		fprintf(stderr, "failed to change mode of '%s' from %04o (%s) to %04o (%s)\n", path, previousMode, previousModeS, mode, modeS);
		return -1;
	}

	if (previousMode != mode)
		processData.nModif = processData.nModif + 1;
	
	
	if (flags & (CHANG_FLAG | VERB_FLAG)){
		
		if (previousMode != mode){
			printf("mode of '%s' changed from %04o (%s) to %04o (%s)\n", path, previousMode, previousModeS, mode, modeS);
		}
		else if (flags & VERB_FLAG){
			printf("mode of '%s' retained as %04o (%s)\n", path, mode, modeS);
		}
	}

	if (previousMode != mode){
		char logMsg[500];
		sprintf(logMsg, "%s : %04o : %04o", path, previousMode, mode);
		writeLog(getpid(), FILE_MODF, logMsg, &processData);
	}
		printf("out xmod\n");

	return 0;
}

int symbolicXmod(char *modeStr, mode_t *newMode){
		printf("in SYMBxmod\n");

	char operator= modeStr[1];
	mode_t mode = 0, previousMode = *newMode;
	
	//Permissions start at modeStr[2]
	int i = SYMB_PERM_START;

	//Get permissions
	while (modeStr[i] != 0){
		switch (modeStr[i++])
		{
		case 'r':
			mode += READ_MASK;
			break;
		case 'w':
			mode += WRITE_MASK;
			break;
		case 'x':
			mode += EXE_MASK;
			break;
		default:
			printf("Invalid permission - %c\n", modeStr[i]);
			return -1;
		}
	}

	unsigned mask;

	//Get type of users
	switch (modeStr[0])
	{
	case 'u':
		mode *= USER_POS;
		mask = USER_MASK;
		break;
	case 'g':
		mode *= GROUP_POS;
		mask = GROUP_MASK;
		break;
	case 'o':
		mask = OTHER_MASK;
		break;
	case 'a':
		mask = ALL_MASK;
		mode = mode + mode * GROUP_POS + mode * USER_POS;
		break;
	default:
		fprintf(stderr, "Invalid type of user - %c\n", modeStr[0]);
		return -1;
	}

	mask += FILE_MASK;  //regular files have bit 1 at pos5

	//Get new mode
	switch (operator)
	{
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
		fprintf(stderr, "xmod: Invalid operator - %c\n", operator);
		return -1;
	}
			printf("OUT SYMBxmod\n");

	return 0;
}

void goThroughDirectory(char *path, int nargs, char *args[], u_int8_t flags){

	bool hasLog = getenv("LOG_FILENAME") != NULL;
	char logMsg[1024];
	char *modeStr = args[nargs - 2];

	
	DIR *dir;
	struct dirent *dent;
	dir = opendir(processData.currentDirectory);

	if (dir != NULL) {

		while ((dent = readdir(dir)) != NULL) {

			if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
				continue;
			
			char nextPath[250];
			strcpy(nextPath, processData.currentDirectory);
			strcat(nextPath, "/");
			strcat(nextPath, dent->d_name);
			struct stat *nextFileInfo = (struct stat *)malloc(sizeof(struct stat));

			printf("before statError\n");
			int statErrorCode = stat(nextPath, nextFileInfo);

			if (statErrorCode != 0){
				if (statErrorCode == EACCES) fprintf("xmod: cannot read directory '%s': Permission denied\n", nextPath);
				else fprintf(stderr, "xmod: cannot access  '%s': No such file or directory\n", nextPath);
				continue;
			}
						printf("after statError\n");

				
			xmod(nextPath, modeStr, flags, nextFileInfo->st_mode);

			if (S_ISDIR(nextFileInfo->st_mode)) {
				printf("before fork\n");
				int id = fork();

				switch (id){
				case 0:
					strcpy(args[nargs - 1], nextPath);
					execvp("./xmod", args);
					exit(0);
				case -1:
					return -1;
				default:
					if (hasLog) {
						strcat(logMsg, "/");
						strcat(logMsg, dent->d_name);
						writeLog(getpid(), PROC_CREAT, logMsg, &processData);
					}
					break;
				}
				printf("after fork()\n");
			}
		}
		closedir(dir);

		pid_t wpid;
		int *status = 0;
		printf("before waiting\n");
		//Parent must wait for all children to finnish process
		while ((wpid = wait(&status)) > 0){
			sprintf(logMsg, "%d", status);
			if(hasLog)
				writeLog(wpid, PROC_EXIT, logMsg, &processData);
		}
		printf("waited for child\n");
	}
}

int main(int nargs, char *args[]) {

	processData.nTotal = 0;
	processData.nModif = 0;
	bool hasLog = getenv("LOG_FILENAME") != NULL;

	//Check if is first born process 
	if ((getpgrp() == getpid()) && hasLog) {
		initLog(&processData);
	}
	else if (hasLog) { //read first process starting time 

		char *startTimeStr = getenv("xmodStartTime");
		struct timeval startTime;

		sscanf(startTimeStr, "%lu %lu", &startTime.tv_sec, &startTime.tv_usec);
		processData.startTime = startTime;
	}

	char *logMsg = (char *)malloc(sizeof(char) * 500);

	for (unsigned i = 1; i < nargs; i++) {

		strcat(logMsg, args[i]);
		if (i < nargs - 1)
			strcat(logMsg, " ");
	}
	if (hasLog)
		writeLog(getpid(), PROC_CREAT, logMsg, &processData);

	if (nargs > 2){

		u_int8_t flags = getFlags(nargs, args);
		char *modeStr = args[nargs - 2];

		char path[250];
		strcpy(path, args[nargs - 1]);
		processData.currentDirectory = path;
		struct stat *fileInfo = (struct stat *)malloc(sizeof(struct stat));
		int statErrorCode = stat(processData.currentDirectory, fileInfo);

		if (statErrorCode != 0){
			if (statErrorCode == EACCES)
				fprintf("xmod: cannot read directory '%s': Permission denied\n", processData.currentDirectory);
			else
				fprintf(stderr, "xmod: cannot access  '%s': No such file or directory\n", processData.currentDirectory);
			return -1;
		}

		//Check if first born process
		if (getpgrp() == getpid()){
			xmod(processData.currentDirectory, modeStr, flags, fileInfo->st_mode);
		}
		if (flags & REC_FLAG && S_ISDIR(fileInfo->st_mode)) {
			printf("before goThroughDirectory\n");
			goThroughDirectory(path, nargs, args, flags);
			printf("after goThroughDirectory\n");
		}
	}

	//Testing purposes
	printf("\n\t\tFINAL nModif = %d, nTotal = %d\n", processData.nModif, processData.nTotal);
	return 0;
}
