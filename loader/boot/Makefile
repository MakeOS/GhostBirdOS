# Explorer Loader
# /boot/Makefile
# makefile for Explorer Loader Boot

# compile all the Boot
all:
	make setup.bin
	make boota.o
	make bootc.o
	make VI.o
	make storage.o
	make fs.o
	make MMU.o
	cd lib && make all
	make boot.o
	make boot.bin
	make BOOT
# compile files
	
BOOT:setup.bin boot.bin
	copy /B setup.bin + boot.bin BOOT >nul
	
boot.bin:boot.o
	objcopy -R .note -R .comment -S -O binary boot.o boot.bin
	
boot.o:boota.o bootc.o lib/font.o VI.o lib/vsprintf.o lib/mem.o lib/string.o storage.o fs.o MMU.o
	ld -o boot.o	-Ttext 0x11000 boota.o bootc.o lib/font.o VI.o lib/vsprintf.o lib/mem.o lib/string.o storage.o fs.o MMU.o

setup.bin:setup.asm
	nasm setup.asm -o setup.bin
	
boota.o:boota.asm
	nasm -f elf boota.asm -o boota.o
	
VI.o:VI.c lib/stdarg.h lib/stdlib.h lib/stdbool.h lib/string.h lib/mem.h ../configure.h VI.h head.h
	gcc -c VI.c -o VI.o
	
bootc.o:bootc.c head.h lib/stdlib.h ../configure.h ../problem.h lib/mem.h storage.h VI.h fs.h MMU.h
	gcc -c bootc.c -o bootc.o
	
storage.o:storage.c storage.h VI.h lib/stdlib.h lib/stdbool.h MMU.h ../configure.h ../problem.h head.h lib/mem.h lib/color.h
	gcc -c storage.c -o storage.o
	
fs.o:fs.c ../problem.h lib/stdbool.h lib/string.h lib/stdlib.h MMU.h storage.h fs.h VI.h
	gcc -c fs.c -o fs.o
	
MMU.o:MMU.c lib/stdlib.h ../configure.h ../problem.h MMU.h
	gcc -c MMU.c -o MMU.o
	