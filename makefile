#c compiler
CC := cc

#directories
BIN := ./bin
SRC := ./src
INCLUDE := ./include


all: xmod xmod.stat xmod.asm xmod.prec


xmod: $(SRC)/xmod.c $(INCLUDE)/xmod.h
	$(CC) -o xmod $(SRC)/xmod.c
xmod.stat: $(SRC)/xmod.c $(INCLUDE)/xmod.h
	$(CC) -static -o $(BIN)/xmod.stat $(SRC)/xmod.c
xmod.asm: $(SRC)/xmod.c $(INCLUDE)/xmod.h
	$(CC) -S -o $(BIN)/xmod.asm $(SRC)/xmod.c
xmod.prec: $(SRC)/xmod.c $(INCLUDE)/xmod.h
	$(CC) -E -o $(BIN)/xmod.prec $(SRC)/xmod.c
clean:
	rm -f $(BIN)/xmod $(BIN)/xmod.stat $(BIN)/xmod.asm $(BIN)/xmod.prec
	