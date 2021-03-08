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
#include "../include/xmod.h"


static int *nModif, *nTotal;
int *stop;

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

void sigintHandler(int signal){
	printf("%d ; ; %d ; %d\n", getpid(), *nTotal, *nModif);
	*stop = 1;
}

/**
 * @brief 
 * 
 * @param path 
 * @param modeStr 
 * @param flags 100 - v ; 010 - c ; 001 - R
 * @return int 
 */
int xmod(char *path, char *modeStr, short flags){

	if(*stop){
		printf("Queres continuar? (y or n)\n");
		char input;
		scanf("%c", &input);
		*stop = input != 'y';
		if(input == 'n'){
			kill(0, SIGKILL);
		}
	}

	printf("Path = %s\n", path);
	*nTotal = *nTotal + 1;
	char first = modeStr[0];
	mode_t mode;
	mode_t previousMode;
	signal(SIGINT, sigintHandler);


	sleep(1);
	struct stat *fileInfo = (struct stat *) malloc(sizeof(struct stat));
	if(stat(path, fileInfo) != 0){
		fprintf(stderr,"Error stat() %s\n", path);
		return -1;
	}
	previousMode = fileInfo->st_mode;
	previousMode &= 0777;
	if(first == '0'){
		mode = strtol(modeStr, 0, 8);
	} else if(first == 'u' || first == 'g' || first == 'o' || first == 'a'){
		mode = previousMode;
		symbolicChmod(modeStr, &mode);

	} else {
		fprintf(stderr,"Invalid type of user - %c\n", first);
		return -1;
	}
	
	if(chmod(path, mode) != 0){
		fprintf(stderr,"Error\n");
		return  -1;
	}


	if(previousMode != mode) *nModif = *nModif + 1;
	if(flags & 0x110){
		
		char previousModeS[10], modeS[10];
		getSymbolic(previousMode, previousModeS);
		getSymbolic(mode, modeS);
		previousMode &= 0777;
		if(previousMode != mode){
			printf("modo de '%s' alterado de %04o (%s) para %04o (%s)\n", path, previousMode, previousModeS, mode, modeS);
		} else if(flags & 0x100){
			printf("modo de '%s' mantido como %04o (%s)\n", path, mode, modeS);
		}
	}

	if(flags & 0x001 && S_ISDIR(fileInfo->st_mode)){
		//Recursive
		
		DIR *dir;
		struct dirent *dent;
		dir = opendir(path);   //this part
		int *status = 0;
		if(dir!=NULL) {
			while((dent=readdir(dir))!=NULL){
				
				if( !strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) continue;; 
				char nextPath[250];
				int id = fork();
				switch(id){
					case 0:
						// Child
						strcpy(nextPath, path);
						strcat(nextPath, "/");
						strcat(nextPath, dent->d_name);
						xmod(nextPath, modeStr, flags);
						exit(0);
						
					case -1:
					 	//ERRO
						return -1;
					default:
						wait(status);
						//kill(id);
						break;
						//PAI/Mãe/Parente
				}
				
			}
			closedir(dir);
		}
		
	}

	return 0;
}

int symbolicChmod(char *modeStr, mode_t *newMode){
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


int main(int nargs, char *args[]) {
	
	nTotal =  mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	nModif = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	stop = mmap(NULL, sizeof(*stop), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	short flags = 0;
	if(nargs > 2){
		for(int i = 1; i < nargs - 2; i++){
			char * flag = args[i];
			if(flag[0] != '-' || flag[2] != 0){
				fprintf(stderr,"Invalid flag %s", flag);
				return -1;
			}
			switch(flag[1]){
				case 'v':
					flags |= 0x100;
					break;
				case 'c':
					flags |= 0x010;
					break;
				case 'R':
					flags |= 0x001;
					break;
				default:
					printf("Invalid flag %s", flag);
					return -1;
			}
		}
		// -ux
		char *modeStr = args[nargs - 2];
		char *path = args[nargs - 1];
		xmod(path, modeStr, flags);

		
	}
	printf("\n\t\tFINAL nModif = %d, nTotal = %d\n", *nModif, *nTotal);
	
	return 0;
}
