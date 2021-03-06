/*
*	Example of threads in action
*/
#include <sys/stat.h>
#include<stdio.h>
#include<stdlib.h>

// 777
// -us

int xmod(char *path, char *modeStr){
	char first = modeStr[0];
	if(first == '0'){
		unsigned mode = strtol(modeStr, 0, 8);
		printf("path = %s ; mode = %d\n", path, mode );
		if(chmod(path, mode) != 0){
			printf("Error\n");
			return 1;
		}
	} else if(first == 'u' || first == 'g' || first == 'o' || first == 'a'){
		char operator = modeStr[1];
		unsigned mode = 0;
		int i = 1;
		while(modeStr[++i] != 0){
			switch(modeStr[i]){
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
					printf("Invalid permission - %c\n", modeStr[i]);
					return -1;
			}
		}
		unsigned mask;
		switch(first){
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
				printf("Invalid type of user - %c\n", first);
				return -1;
		}

		unsigned previousMode;
		struct stat *fileInfo;
		stat(path, fileInfo);
		previousMode = fileInfo->st_mode;
		switch(operator){
			case '-':
				mode = previousMode - (previousMode & mode);
				break;
			case '+':
				mode |= previousMode;
				break;
			case '=':
				mode = (previousMode & mask) | mode; 
				break;
			default:
				printf("Invalid operator - %c\n", operator);
				return -1;
		}
		if(chmod(path, mode) != 0){
			printf("Error chmod \n");
			return -1;
		}
	} else {
		printf("Invalid input\n");
		return -1;
	}
	return 0;
}

int symbolicChmod(){

}




int main(int nargs, char *args[]) {
	printf("Teste\n");
	if(nargs > 2){
		printf("nargs > 2\n");
		for(int i = 0; i < nargs - 2; i++){
			//
		}
		// -ux
		char *modeStr = args[nargs - 2];
		char *path = args[nargs - 1];
		xmod(path, modeStr);

		
	}
	return 0;	// will not run this!
}
