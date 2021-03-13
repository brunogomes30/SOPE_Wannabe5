CC=gcc
RM=rm -f
BIN:=bin/
INCLUDE:=include/
SRC:=src/

xmod : ${BIN}auxXmod.o ${BIN}logFile.o ${BIN}xmod.o
	cc -Wall -o  xmod ${BIN}auxXmod.o ${BIN}logFile.o  ${BIN}xmod.o 

${BIN}auxXmod.o : ${SRC}auxXmod.c ${INCLUDE}auxXmod.h ${INCLUDE}logFile.h
	cc  -c -Wall ${SRC}auxXmod.c -o ${BIN}auxXmod.o

${BIN}logFile.o : ${SRC}logFile.c ${INCLUDE}logFile.h
	cc -c -Wall ${SRC}logFile.c -o ${BIN}logFile.o
	
${BIN}xmod.o: ${SRC}xmod.c ${INCLUDE}xmod.h ${INCLUDE}auxXmod.h
	cc  -c -Wall ${SRC}xmod.c -o ${BIN}xmod.o 

clean: 
	rm xmod ${BIN}auxXmod.o ${BIN}logFile.o ${BIN}xmod.o