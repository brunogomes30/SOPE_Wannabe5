all: s c

s: server.o lib.o src/delay.c include/delay.h
	gcc -Wall -DDELAY=0 -o s src/delay.c lib.o server.o -pthread

c: src/client.c include/common.h src/*.c 
	gcc -Wall -o c src/*.c -pthread

# server.o: server.c common.h
# 	gcc -Wall -c -o server.o server.c

# lib.o: lib.c lib.h
# 	gcc -Wall -c -o lib.o lib.c

clean:
	rm -f s c
