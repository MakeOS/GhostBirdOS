# Explorer Loader
# /mbr/Makefile
# makefile for Explorer Loader MBR

# compile all the MBR

	
MBR.bin: MBR.asm
	$(AS) MBR.asm -o MBR.bin
	
all:
	make MBR.bin
	
install:MBR.bin
	$(gfr) -ws MBR.bin $(image) 1
	
	