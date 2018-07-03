	echo off
	title Explorer Kernel Project
	set /a Compile_count=1

:UI
	cls
	echo Copyright 2013-2015 by Lab Explorer Developers.
	echo Our website:www.GhostBirdOS.org
	echo Function	Information
	echo 1	Start for %Compile_count% compile,load and run
	echo 2	Compile only
	echo 3	Load Explorer.img from KERNEL
	echo 4	Run only
	echo 6	Clean the temporary file
	echo 7	Read help
	echo 9	Restart
	echo 0	exit
	set /p command="Choose[1,2,3,4,5,6,7,8,9,0]:"
::????
	if "%command%"=="" goto Compile
	if "%command%"=="1" goto Compile
	if "%command%"=="2" goto Compile
	if "%command%"=="3" goto load_KERNEL
	if "%command%"=="4" goto run
	if "%command%"=="7" goto help
	if "%command%"=="6" goto file_del
	if "%command%"=="9" %0
	if "%command%"=="0" exit
	echo The instruction is not support,Plase press any key to return.
	pause > nul
	goto UI
	
:Compile
::????
	echo Compiling Explorer...
	nasm -o tmp\head.bin head.asm
	nasm -f elf arch\x86\kernel\_start.asm -o tmp\_start.o -I "%cd%\include"
	nasm -f elf arch\x86\io.asm -o tmp\io.o
	nasm -f elf arch\x86\syscalls.asm -o tmp\syscalls.o
	nasm -f elf arch\x86\kernel\task\switch_to.asm -o tmp\switch_to.o
	nasm -f elf arch\x86\kernel\task\init_stack.asm -o tmp\init_stack.o
	nasm -f elf arch\x86\interrupt\entry.asm -o tmp\entry.o
	gcc -c arch\x86\kernel\task\task.c -o tmp\task.o -I "%cd%\include"
	gcc -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	gcc -c Init\main.c -o tmp\main.o -I "%cd%\include"
	gcc -c ipc\msg.c -o tmp\msg.o -I "%cd%\include"
	gcc -c arch\x86\kernel\debug.c -o tmp\debug.o -I "%cd%\include"
	gcc -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	gcc -c arch\x86\kernel\cmos.c -o tmp\cmos.o -I "%cd%\include"
	gcc -c arch\x86\kernel\time.c -o tmp\time.o -I "%cd%\include"
	gcc -c arch\x86\interrupt\interrupt.c -o tmp\interrupt.o -I "%cd%\include"
	gcc -c arch\x86\interrupt\handle.c -o tmp\handle.o -I "%cd%\include"
	gcc -c lib\fonts\font.c -o tmp\font.o -I "%cd%\include"
	gcc -c lib\fonts\simsun.c -o tmp\simsun.o -I "%cd%\include"
	gcc -c lib\fonts\standard_font.c -o tmp\standard_font.o -I "%cd%\include"
	gcc -c arch\x86\kernel\memory.c -o tmp\memory.o -I "%cd%\include"
	gcc -c arch\x86\kernel\kmalloc.c -o tmp\kmalloc.o -I "%cd%\include"
	gcc -c arch\x86\kernel\do_page_fault.c -o tmp\do_page_fault.o -I "%cd%\include"
	gcc -c drivers\hdd.c -o tmp\hdd.o -I "%cd%\include"
	gcc -c drivers\video.c -o tmp\video.o -I "%cd%\include"
	gcc -c drivers\i8042.c -o tmp\i8042.o -I "%cd%\include"
	gcc -c arch\x86\kernel\i8259.c -o tmp\i8259.o -I "%cd%\include"
	gcc -c drivers\mouse.c -o tmp\mouse.o -I "%cd%\include"
	gcc -c drivers\keyboard.c -o tmp\keyboard.o -I "%cd%\include"
	gcc -c arch\x86\Architecture.c -o tmp\Architecture.o -I "%cd%\include"
	gcc -c arch\x86\CPU\cpu.c -o tmp\cpu.o -I "%cd%\include"
	gcc -c fs\fat32\fat32.c -o tmp\fat32.o -I "%cd%\include"
	gcc -S fs\fat32\fat32.c -o tmp\fat32.asm -I "%cd%\include"
	gcc -c GUI\window\window.c -o tmp\window.o -I "%cd%\include"
	gcc -c GUI\init.c -o tmp\init.o -I "%cd%\include"
	gcc -c GUI\manage.c -o tmp\manage.o -I "%cd%\include"
	gcc -c GUI\refresh.c -O3 -o tmp\refresh.o -I "%cd%\include"
	gcc -c GUI\function.c -o tmp\function.o -I "%cd%\include"
	gcc -c GUI\gui_api.c -o tmp\gui_api.o -I "%cd%\include"
	gcc -c GUI\window\lib.c -o tmp\lib.o -I "%cd%\include"
	gcc -c GUI\window\task_bar.c -o tmp\task_bar.o -I "%cd%\include"
	gcc -c GUI\window\unit.c -o tmp\unit.o -I "%cd%\include"
	gcc -c GUI\window\picture.c -o tmp\picture.o -I "%cd%\include"
	gcc -c lib\mem.c -o tmp\mem.o -I "%cd%\include"
	gcc -c lib\string.c -o tmp\string.o -I "%cd%\include"
	gcc -c lib\graphics.c -o tmp\graphics.o -I "%cd%\include"
	gcc -c lib\math.c -o tmp\math.o -I "%cd%\include"
	gcc -c lib\vsprintf.c -o tmp\vsprintf.o -I "%cd%\include"
	gcc -c cpp\support.cpp -o tmp\support.o -I "%cd%\include"
	gcc -c script\script.cpp -o tmp\script.o -I "%cd%\include"
	
	echo Linking...
	::??????
	ld -o tmp\kernel.o	-Ttext 0x11000^
	tmp\_start.o tmp\main.o tmp\debug.o^
	tmp\entry.o tmp\interrupt.o tmp\handle.o^
	tmp\shell.o tmp\vsprintf.o^
	tmp\font.o tmp\simsun.o tmp\standard_font.o^
	tmp\hdd.o tmp\video.o tmp\mouse.o tmp\keyboard.o tmp\i8042.o^
	tmp\i8259.o tmp\i8254.o tmp\cmos.o tmp\time.o^
	tmp\switch_to.o tmp\task.o tmp\init_stack.o^
	tmp\memory.o tmp\kmalloc.o tmp\do_page_fault.o tmp\fat32.o^
	tmp\Architecture.o tmp\io.o tmp\cpu.o^
	tmp\syscalls.o^
	tmp\support.o tmp\string.o tmp\mem.o tmp\graphics.o tmp\math.o^
	tmp\init.o tmp\manage.o tmp\refresh.o tmp\function.o tmp\window.o tmp\gui_api.o^
	tmp\lib.o tmp\task_bar.o tmp\unit.o tmp\picture.o^
	tmp\msg.o tmp\script.o
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin

	::?????
	copy /B tmp\head.bin + tmp\kernel.bin KERNEL > nul
	
	echo Compile completely
	set /a Compile_count=%Compile_count%+1
	if "%command%"=="1" goto load_KERNEL
	echo Press any key to return.
	pause > nul
	goto UI
	
:load_KERNEL
	echo Starting WinImage...
::??????Explorer.img
	WinImage ..\image\Explorer.img KERNEL /i /h /y
	echo load completely.
	if "%command%"=="1" goto run
	goto UI
	
:run
::?????
	echo Starting VirtualBox...
	VBoxManage.exe startvm "Ghost Bird 0.02"
	goto UI
	
:help
::??????
	cls
	echo Ghost Bird OS Compilation Help
	echo.
	echo.
	echo Copyright 2013-2015 by Lab Explorer Developers.
	echo Nothing
	echo Press any key to return.
	pause > nul
	goto UI

:file_del
	del /a /f /q KERNEL
	del /s /f /q *.o
	del /s /f /q *.bin
	goto UI
	
:Init_vironment
	echo Create new Virtual machine...
	VBoxManage createvm --name "Ghost Bird 0.02" --register
	
	
	pause
	goto UI
	