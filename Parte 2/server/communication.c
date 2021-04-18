#include "communications.h"

void initFIFO(char *myfifo){
    mkfifo(myfifo,0777);
}
void writeToFIFO(){

}
void readFromFIFO(){
    char arr1[500];
    FD fd = open(myfifo, O_RDONLY);
    read(fd, arr1, sizeof(arr1));
}