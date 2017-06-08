# Explorer Loader
# /Makefile
# makefile for Explorer Loader

all:
	cd mbr && make all
	cd boot && make all
	
run:all
# this line can't run in Linux
	VBoxManage.exe startvm "Explorer"
	
clean:
	del /a /f /q boot\BOOT
	del /a /f /q mbr\MBR.bin
	del /s /f /q *.o
	del /s /f /q *.bin
	
	
# part of functions
	




	