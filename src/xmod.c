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
#include <time.h>
#include "../include/xmod.h"
#include "../include/auxXmod.h"
#include "../include/macros.h"

XmodData processData;

void stopProcesses()
{

	sleep(1);
	printf("Do you want to continue? (y or n)\n");
	char input;
	do
	{
		scanf("%c", &input);

		if (input == 'n')
		{

			writeLog(getpid(), SIGNAL_SENT, "SIGUSR1: 10", &processData);
			kill(0, SIGUSR1);
		}

	} while (input != 'y' && input != 'n');

	kill(0, SIGCONT);
	writeLog(getpid(), SIGNAL_SENT, "SIGCONT: 18", &processData);
}

void usrHandler(int signal)
{
	writeLog(getpid(), SIGNAL_RECV, "SIGUSR1: 10", &processData);
	writeLog(getpid(), SIGNAL_SENT, "SIGKILL: 9", &processData);
	kill(getpid(), SIGKILL);
}

void genericSignalHandler(int signal)
{
	if (getenv("LOG_FILENAME") != NULL)
	{
		char buffer[20];
		snprintf(buffer, sizeof(buffer), "%d", signal);
		writeLog(getpid(), SIGNAL_RECV, buffer, &processData);
	}
}

void sigintHandler(int sig)
{

	printf("%d ; %s ; %d ; %d\n", getpid(), processData.currentDirectory, processData.nTotal, processData.nModif);
	writeLog(getpid(), SIGNAL_RECV, "SIGINT: 2", &processData);

	if (getpid() == getpgrp())
	{
		stopProcesses();
	}
	else
	{
		writeLog(getpid(), SIGNAL_SENT, "SIGSTOP: 19", &processData);
		writeLog(getpid(), SIGNAL_RECV, "SIGSTOP: 19", &processData);
		kill(getpid(), SIGSTOP);
		writeLog(getpid(), SIGNAL_RECV, "SIGCONT: 18", &processData);
	}

	struct sigaction sigintAction, old;
	sigset_t smask;
	if (sigemptyset(&smask) == -1)
		perror("sigsetfunctions");

	sigintAction.sa_handler = sigintHandler;
	sigintAction.sa_mask = smask;
	sigintAction.sa_flags = 0;

	if (sigaction(SIGINT, &sigintAction, &old) == -1)
		perror("sigaction");

	sigaction(SIGINT, &old, NULL);

	return;
}

void receiveSignal()
{
	struct sigaction new, sigintAction, sigusrAction;
	sigset_t smask;

	// prepare struct sigaction
	if (sigemptyset(&smask) == -1)
		perror("sigsetfunctions");

	new.sa_handler = genericSignalHandler;
	new.sa_mask = smask;
	new.sa_flags = SA_RESTART;

	sigintAction.sa_handler = sigintHandler;
	sigintAction.sa_mask = smask;
	sigintAction.sa_flags = SA_RESTART;

	sigusrAction.sa_handler = usrHandler;
	sigusrAction.sa_mask = smask;
	sigusrAction.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &sigintAction, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGUSR1, &sigusrAction, NULL) == -1)
		perror("sigaction");

	if (sigaction(SIGHUP, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGQUIT, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGSEGV, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGUSR2, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGPIPE, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGALRM, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGTERM, &new, NULL) == -1)
		perror("sigaction");
	if (sigaction(SIGCHLD, &new, NULL) == -1)
		perror("sigaction");
}

/**
 * @brief 
 * 
 * @param path 
 * @param modeStr 
 * @param flags 
 * @return int 
 */
int xmod(char *path, char *modeStr, int8_t flags, mode_t previousMode)
{
	processData.nTotal++;
	char first = modeStr[0];
	mode_t mode;

	previousMode &= ALL_PERMS;
	if (first == '0')
	{ //numerical mode
		mode = strtol(modeStr, 0, 8);
	}
	else if (first == 'u' || first == 'g' || first == 'o' || first == 'a')
	{
		//Symbolic mode
		mode = previousMode;
		int symbolicExitCode = symbolicXmod(modeStr, &mode) != 0;
		if (symbolicExitCode != 0)
		{
			return symbolicExitCode;
		}
	}
	else
	{
		fprintf(stderr, "xmod: Invalid mode - %s\n", modeStr);
		return MODE_ERROR;
	}

	previousMode &= ALL_PERMS;

	char previousModeS[10], modeS[10];
	getSymbolic(previousMode, previousModeS);
	getSymbolic(mode, modeS);

	if (chmod(path, mode) != 0)
	{
		fprintf(stderr, "failed to change mode of '%s' from %04o (%s) to %04o (%s)\n", path, previousMode, previousModeS, mode, modeS);
		return DEFAULT_ERROR;
	}

	if (previousMode != mode)
		processData.nModif = processData.nModif + 1;

	if (flags & (CHANG_FLAG | VERB_FLAG))
	{

		if (previousMode != mode)
		{
			printf("mode of '%s' changed from %04o (%s) to %04o (%s)\n", path, previousMode, previousModeS, mode, modeS);
		}
		else if (flags & VERB_FLAG)
		{
			printf("mode of '%s' retained as %04o (%s)\n", path, mode, modeS);
		}
	}

	if (previousMode != mode)
	{
		char logMsg[500];
		snprintf(logMsg, sizeof(logMsg), "%s : %04o : %04o", path, previousMode, mode);
		writeLog(getpid(), FILE_MODF, logMsg, &processData);
	}

	return 0;
}

int symbolicXmod(char *modeStr, mode_t *newMode)
{

	char operator= modeStr[1];
	mode_t mode = 0, previousMode = *newMode;

	//Permissions start at modeStr[2]
	int i = SYMB_PERM_START;
	bool r_flag = false;
	bool w_flag = false;
	bool x_flag = false;
	//Get permissions
	while (modeStr[i] != 0)
	{
		switch (modeStr[i++])
		{
		case 'r':
			if (r_flag)
				break;
			mode += READ_MASK;
			r_flag = true;
			break;
		case 'w':
			if (w_flag)
				break;
			mode += WRITE_MASK;
			w_flag = true;
			break;
		case 'x':
			if (x_flag)
				break;
			mode += EXE_MASK;
			x_flag = true;
			break;
		default:
			printf("Invalid permission - %c\n", modeStr[i]);
			return PERMISSION_ERROR;
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
		return TYPE_ERROR;
	}

	mask += FILE_MASK; //regular files have bit 1 at pos5

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
		return OPERATOR_ERROR;
	}

	return 0;
}

void goThroughDirectory(char *path, int nargs, char *args[], int8_t flags)
{

	bool hasLog = getenv("LOG_FILENAME") != NULL;
	char logMsg[1024];
	char *modeStr = args[nargs - 2];

	DIR *dir;
	struct dirent *dent;
	dir = opendir(processData.currentDirectory);

	if (dir != NULL)
	{
		while ((dent = readdir(dir)) != NULL)
		{

			if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
				continue;

			char nextPath[500];
			snprintf(nextPath, sizeof(nextPath), "%s/%s", processData.currentDirectory, dent->d_name);
			struct stat nextFileInfo;

			int statErrorCode = stat(nextPath, &nextFileInfo);

			if (statErrorCode != 0)
			{
				if (statErrorCode == EACCES)
					fprintf(stderr, "xmod: cannot read directory '%s': Permission denied\n", nextPath);
				else
					fprintf(stderr, "xmod: cannot access  '%s': No such file or directory\n", nextPath);
				continue;
			}

			xmod(nextPath, modeStr, flags, nextFileInfo.st_mode);

			if (S_ISDIR(nextFileInfo.st_mode))
			{
				int id = fork();

				switch (id)
				{
				case 0:
					snprintf(args[nargs - 1], sizeof(nextPath), "%s", nextPath);
					execvp("./xmod", args);
					exit(0);
				case -1:
					return;
				default:
					if (hasLog)
					{
						char logMsgArgs[1024];
						getArgStr(nargs, args, logMsgArgs);
						snprintf(logMsg, sizeof(logMsgArgs) + sizeof(dent->d_name), "%s/%s", logMsgArgs, dent->d_name);
						writeLog(getpid(), PROC_CREAT, logMsg, &processData);
					}
					break;
				}
			}
		}
		closedir(dir);

		pid_t wpid;
		int status = 0;
		//Parent must wait for all children to finish process
		while ((wpid = wait(&status)) > 0)
		{
			snprintf(logMsg, sizeof(logMsg), "%d", status);
			if (hasLog)
				writeLog(wpid, PROC_EXIT, logMsg, &processData);
		}
	}
}

int main(int nargs, char *args[])
{
	processData.nTotal = 0;
	processData.nModif = 0;
	bool hasLog = getenv("LOG_FILENAME") != NULL;
	char buffer[50];
	receiveSignal();

	//Check if is first born process
	if ((getpgrp() == getpid()) && hasLog)
	{
		gettimeofday(&processData.startTime, NULL);
		snprintf(buffer, sizeof(char) * 50, "xmodStartTime=%lu %lu", processData.startTime.tv_sec, processData.startTime.tv_usec);
		putenv(buffer);
		initLog(&processData);
	}
	else if (hasLog)
	{
		//read first process starting time
		char *startTimeStr = getenv("xmodStartTime");
		struct timeval startTime;

		sscanf(startTimeStr, "%lu %lu", &startTime.tv_sec, &startTime.tv_usec);
		processData.startTime = startTime;
	}

	char logMsg[1024];
	if (hasLog)
	{

		getArgStr(nargs, args, logMsg);
		writeLog(getpid(), PROC_CREAT, logMsg, &processData);
	}

	if (nargs > 2)
	{
		int8_t flags = getFlags(nargs, args);
		if (flags == -1)
		{
			return -1;
		}
		char *modeStr = args[nargs - 2];

		char path[250];
		snprintf(path, sizeof(path), "%s", args[nargs - 1]);
		processData.currentDirectory = path;
		struct stat fileInfo;
		int statErrorCode = stat(processData.currentDirectory, &fileInfo);

		if (statErrorCode != 0)
		{
			if (statErrorCode == EACCES)
				fprintf(stderr, "xmod: cannot read directory '%s': Permission denied\n", processData.currentDirectory);
			else
				fprintf(stderr, "xmod: cannot access  '%s': No such file or directory\n", processData.currentDirectory);
			return -1;
		}

		//Check if first born process
		if (getpgrp() == getpid())
		{
			int xmodExitCode = xmod(processData.currentDirectory, modeStr, flags, fileInfo.st_mode);

			if (xmodExitCode < DEFAULT_ERROR)
			{
				// if true, the flags or mode is wrong, so the program should be terminated
				return xmodExitCode;
			}
		}
		if (flags & REC_FLAG && S_ISDIR(fileInfo.st_mode))
		{
			goThroughDirectory(path, nargs, args, flags);
		}
	}

	return 0;
}
