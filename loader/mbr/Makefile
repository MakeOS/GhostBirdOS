# Explorer Loader
# /mbr/Makefile
# makefile for Explorer Loader MBR

# compile all the MBR
all:
	make MBR.bin
	
MBR.bin: MBR.asm
	nasm MBR.asm -o MBR.bin
	