CC=gcc
RM=rm -f

xmod : auxXmod.o logFile.o xmod.o
	cc -Wall -o  xmod auxXmod.o logFile.o  xmod.o
auxXmod.o : src/auxXmod.c include/auxXmod.h include/logFile.h
	cc  -c -Wall src/auxXmod.c 
logFile.o : src/logFile.c include/logFile.h
	cc -c -Wall src/logFile.c
xmod.o: src/xmod.c include/xmod.h include/auxXmod.h
	cc  -c -Wall src/xmod.c 

clean: 
	rm xmod auxXmod.o logFile.o xmod.o