all: test test.stat test.asm test.prec

test: test.c
	cc -Wall -o test test.c
test.stat: test.c
	cc -Wall -static -o test.stat test.c
test.asm: test.c
	cc -Wall -S -o test.asm test.c
test.prec: test.c
	cc -Wall -E -o test.prec test.c
clean:
	rm -f test test.stat test.asm test.prec
