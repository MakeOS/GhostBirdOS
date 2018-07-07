@echo off
::Make 0.5.1 for Explorer 0.80 seris
::札喇丰阿
::增加对QEMU Launcher的支持
::支持从其他目录引用
::增加新参数，无人值守一键编译(用于Notepad++)
::  注：make.cmd -auto就可以实现一键编译
set comct=0
%~d0
cd %~dp0
set t=Make 0.5.1
if "%1"=="-auto" (goto auto)
if not "%1"=="" (set comct=%1)
if not exist tmp (md tmp)
echo.>tmp\error.log
set n=Explorer Kernel Project
goto main
:auto
color 0a
set command=
set auto=true
goto compile
:done
title %t% - Done

	echo.
	echo.
	echo.
	echo [Press any key to return]
pause > nul
goto main

:error
cls
title %t% - Error
	color 0c
cls
	echo ** Error
	echo.
	type tmp\error.log|more
goto done


::Main
:main
set /a comct=%comct%+1
color 0f
cls
set command=
title %t% - %n%
    	echo Our Official Website: www.GhostBirdOS.org
	echo You have compiled Explorer for %comct% time(s).
	echo.
	echo Function	Instuction
	echo Enter		LAUNCH Explorer
	if not exist ../qemu/vmlauncher.cmd (echo 2		Start Virtual Machine) else (echo 2		Start VMLauncher)
	echo 3		Delete Temporary Files
	echo 0		Exit
echo.
set /p command="Functions:"
if "%command%"=="" goto compile
if "%command%"=="2" goto run
if "%command%"=="3" goto deltmp
if "%command%"=="0" exit
echo ** 未知命令 任意键返回
pause > nul
goto main
:Compile
cls
title %t% - Compiling Assembly Files...
	taskkill /f /im virtualbox.exe>nul 2>nul
	taskkill /f /im qemu.exe >nul 2>nul
	taskkill /f /im qemu-system-x86_64.exe >nul 2>nul
	taskkill /f /im qemu-system-arm.exe >nul 2>nul

	call asm.cmd -o tmp\head.bin head.asm
	call asm.cmd -f elf arch\x86\kernel\_start.asm -o tmp\_start.o -I "%cd%\include"
	call asm.cmd -f elf arch\x86\io.asm -o tmp\io.o
	call asm.cmd -f elf arch\x86\syscalls.asm -o tmp\syscalls.o
	call asm.cmd -f elf arch\x86\kernel\task\switch_to.asm -o tmp\switch_to.o
	call asm.cmd -f elf arch\x86\kernel\task\init_stack.asm -o tmp\init_stack.o
	call asm.cmd -f elf arch\x86\interrupt\entry.asm -o tmp\entry.o
	call pcc.cmd -c arch\x86\kernel\task\task.c -o tmp\task.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\i8254.c -o tmp\i8254.o -I "%cd%\include"
	call pcc.cmd -c Init\main.c -o tmp\main.o -I "%cd%\include"
	call pcc.cmd -c ipc\msg.c -o tmp\msg.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\debug.c -o tmp\debug.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\shell.c -o tmp\shell.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\cmos.c -o tmp\cmos.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\time.c -o tmp\time.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\interrupt\interrupt.c -o tmp\interrupt.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\interrupt\handle.c -o tmp\handle.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\font.c -o tmp\font.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\simsun.c -o tmp\simsun.o -I "%cd%\include"
	call pcc.cmd -c lib\fonts\standard_font.c -o tmp\standard_font.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\memory.c -o tmp\memory.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\kmalloc.c -o tmp\kmalloc.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\do_page_fault.c -o tmp\do_page_fault.o -I "%cd%\include"
	call pcc.cmd -c drivers\hdd.c -o tmp\hdd.o -I "%cd%\include"
	call pcc.cmd -c drivers\video.c -o tmp\video.o -I "%cd%\include"
	call pcc.cmd -c drivers\i8042.c -o tmp\i8042.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\kernel\i8259.c -o tmp\i8259.o -I "%cd%\include"
	call pcc.cmd -c drivers\mouse.c -o tmp\mouse.o -I "%cd%\include"
	call pcc.cmd -c drivers\keyboard.c -o tmp\keyboard.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\Architecture.c -o tmp\Architecture.o -I "%cd%\include"
	call pcc.cmd -c arch\x86\CPU\cpu.c -o tmp\cpu.o -I "%cd%\include"
	call pcc.cmd -c fs\fat32\fat32.c -o tmp\fat32.o -I "%cd%\include"
	call pcc.cmd -c GUI\window\window.c -o tmp\window.o -I "%cd%\include"
	call pcc.cmd -c GUI\init.c -o tmp\init.o -I "%cd%\include"
	call pcc.cmd -c GUI\manage.c -o tmp\manage.o -I "%cd%\include"
	call pcc.cmd -c GUI\refresh.c -o tmp\refresh.o -I "%cd%\include"
	call pcc.cmd -c GUI\function.c -o tmp\function.o -I "%cd%\include"
	call pcc.cmd -c GUI\gui_api.c -o tmp\gui_api.o -I "%cd%\include"
	call pcc.cmd -c GUI\window\lib.c -o tmp\lib.o -I "%cd%\include"
	call pcc.cmd -c GUI\window\task_bar.c -o tmp\task_bar.o -I "%cd%\include"
	call pcc.cmd -c GUI\window\unit.c -o tmp\unit.o -I "%cd%\include"
	call pcc.cmd -c GUI\window\picture.c -o tmp\picture.o -I "%cd%\include"
	call pcc.cmd -c lib\mem.c -o tmp\mem.o -I "%cd%\include"
	call pcc.cmd -c lib\string.c -o tmp\string.o -I "%cd%\include"
	call pcc.cmd -c lib\graphics.c -o tmp\graphics.o -I "%cd%\include"
	call pcc.cmd -c lib\math.c -o tmp\math.o -I "%cd%\include"
	call pcc.cmd -c lib\vsprintf.c -o tmp\vsprintf.o -I "%cd%\include"
	call p++.cmd -c cpp\support.cpp -o tmp\support.o -I "%cd%\include"
cls
echo √ Compile Kernel - Done
echo -^> Linking .o Files...
	::Link
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
	tmp\msg.o>tmp\error.log
	objcopy -R .note -R .comment -S -O binary tmp\kernel.o tmp\kernel.bin 2>tmp\error.log
	echo.>>tmp\error.log
cls
echo √ Compile Kernel - Done
echo √ Link .o Files - Done
echo -^> Merging File Parts of Kernel...
	::Binary Copy
	copy /B tmp\head.bin + tmp\kernel.bin KERNEL > nul 2>nul
	findstr No "tmp\error.log">nul&&goto Error
	del /f /s /q tmp\error.log >nul 2>nul
cls
title %t% - Writing Kernel...
::Write kernel to Explorer.img
	echo √ Generate Kernel - Done
	echo -^> Writing Kernel...
	echo ** Start Virtual Machine

	WinImage ..\image\Explorer.img KERNEL /i /h /y


:run
cls
::Start Virtual Machine
title %t% - Starting Virtual Machine...
	echo √ Generate Kernel - Done
	echo √ Write Kernel - Done
	echo -^> Starting Virtual Machine...
	taskkill /f /im virtualbox.exe>nul 2>nul
	if not exist ../tools/vmlauncher.cmd (goto vbox)
	REM qemu -system-i386 -fda ../Image/Explorer.img -hda ../Image/FAT32.vhd
	REM call ../qemu/vmlauncher.cmd
	if not "%1"=="-auto" (goto main) else (exit)

:deltmp
cls
del /s /f /q KERNEL>nul 2>nul
del /s /f /q *.o>nul 2>nul
del /s /f /q *.bin>nul 2>nul
	echo Done!
goto done
:vbox
VBoxManage.exe startvm "Ghost Bird 0.02" >nul
if not %errorlevel%==0 (echo ** Failed launching VBOX.&goto done)
if not "%1"=="-auto" (goto main) else (exit)
:eof