/*
*	Example of threads in action
*/
#include <sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>

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


/**
 * @brief 
 * 
 * @param path 
 * @param modeStr 
 * @param flags 100 - v ; 010 - c ; 001 - R
 * @return int 
 */
int xmod(char *path, char *modeStr, short flags){
	printf("Path = %s\n", path);
	char first = modeStr[0];
	mode_t mode;
	mode_t previousMode;
	
	struct stat *fileInfo = (struct stat *) malloc(sizeof(struct stat));
	if(stat(path, fileInfo) != 0){
		printf("Error stat() \n");
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
		printf("Invalid type of user - %c\n", first);
		return -1;
	}
	
	if(chmod(path, mode) != 0){
		printf("Error\n");
		return  -1;
	}


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
		if(dir!=NULL) {
			while((dent=readdir(dir))!=NULL){
				
				if( !strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) continue;; 
				int id = fork();
				switch(id){
					case 0:
						// Child
						xmod(dent->d_name, modeStr, flags);
						break;
					case -1:
					 	//ERRO
						return -1;
					default:
						kill(id);
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
			printf("Invalid type of user - %c\n", modeStr[0]);
			return -1;
	}
	mask += 0100000;
	//Get new mode
	switch(operator){
		case '-':
			*newMode = previousMode - (previousMode & mode);
			break;
		case '+':
			*newMode |= previousMode;
			break;
		case '=':
			*newMode = (previousMode & mask) | mode; 
			break;
		default:
			printf("Invalid operator - %c\n", operator);
			return -1;
	}
}




int main(int nargs, char *args[]) {

	short flags = 0;
	if(nargs > 2){
		for(int i = 1; i < nargs - 2; i++){
			char * flag = args[i];
			if(flag[0] != '-' || flag[2] != 0){
				printf("Invalid flag %s", flag);
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
	return 0;
}
