CC = gcc

BIN := bin/
INCLUDE := include/
SRC := src/

RM = rm -f

flags := -c -Wall

xmod : ${BIN}auxXmod.o ${BIN}logFile.o ${BIN}xmod.o
	cc -Wall -o  xmod ${BIN}auxXmod.o ${BIN}logFile.o  ${BIN}xmod.o 

${BIN}auxXmod.o : ${SRC}auxXmod.c ${INCLUDE}auxXmod.h ${INCLUDE}logFile.h
	cc  $(flags) ${SRC}auxXmod.c -o ${BIN}auxXmod.o

${BIN}logFile.o : ${SRC}logFile.c ${INCLUDE}logFile.h
	cc $(flags) ${SRC}logFile.c -o ${BIN}logFile.o
	
${BIN}xmod.o: ${SRC}xmod.c ${INCLUDE}xmod.h ${INCLUDE}auxXmod.h
	cc  $(flags) ${SRC}xmod.c -o ${BIN}xmod.o 

clean: 
	$(RM) xmod ${BIN}auxXmod.o ${BIN}logFile.o ${BIN}xmod.o