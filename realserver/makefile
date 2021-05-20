all: s 

s: src/server.c include/common.h src/*.c  
	gcc -Wall -o s src/*.c -pthread

clean:
	rm -f s 
