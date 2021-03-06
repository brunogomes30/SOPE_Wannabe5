all: xmod xmod.stat xmod.asm xmod.prec

xmod: xmod.c
	cc -o xmod xmod.c
xmod.stat: xmod.c
	cc -static -o xmod.stat xmod.c
xmod.asm: xmod.c
	cc -S -o xmod.asm xmod.c
xmod.prec: xmod.c
	cc -E -o xmod.prec xmod.c
clean:
	rm -f xmod xmod.stat xmod.asm xmod.prec
