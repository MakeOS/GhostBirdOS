# Explorer Loader
# /mbr/Makefile
# makefile for Explorer Loader MBR

# compile all the MBR

	
MBR.bin: MBR.basm
	$(NS) MBR.basm -o MBR.bin
	
all:
	make MBR.bin
	
install:MBR.bin
	$(GFR) -ws MBR.bin $(image) 1
	
clean:
	$(RM) MBR.bin
	
