# Explorer Loader
# /boot/Makefile
# makefile for Explorer Loader Boot

# compile all the Boot
all:
	make font.o
	make vsprintf.o
	make mem.o
	make string.o

# compile files
font.o:font.c font.h
	gcc -c font.c -o font.o
	
vsprintf.o:vsprintf.c string.h stdlib.h stdarg.h mem.h
	gcc -c vsprintf.c -o vsprintf.o
	
mem.o:mem.c mem.h
	gcc -c mem.c -o mem.o
	
string.o:string.c string.h mem.h
	gcc -c string.c -o string.o
	