# Copyright 2013-2014 by Exploerer Developer.
# made by 米油<2@GhostBirdOS.org> BeatMicrosoft<BeatMicrosoft@GhostBirdOS.org> Lab Explorer<1@GhostBirdOS.org>
# Explorer makefile
# version:Beta
# 7/22/2014 8:17 AM


LFLAGS = -m elf_i386


# 编译KERNEL
all:
	cls
	cd arch && make
	cd cpp && make
	cd drivers && make
	cd fs && make
	cd GUI && make
	cd init && make
	cd ipc && make
	cd lib && make

	ld $(LFLAGS) -o kernel.o	-Ttext 0x11000 \
	arch/x86/kernel/_start.o init/main.o arch/x86/kernel/debug.o \
	arch/x86/interrupt/entry.o arch/x86/interrupt/interrupt.o arch/x86/interrupt/handle.o \
	arch/x86/kernel/shell.o lib/vsprintf.o \
	lib/fonts/font.o lib/fonts/simsun.o lib/fonts/standard_font.o \
	drivers/hdd.o drivers/video.o drivers/mouse.o drivers/keyboard.o drivers/i8042.o \
	arch/x86/kernel/i8259.o arch/x86/kernel/i8254.o arch/x86/kernel/cmos.o arch/x86/kernel/time.o \
	arch/x86/kernel/task/switch_to.o arch/x86/kernel/task/task.o arch/x86/kernel/task/init_stack.o \
	arch/x86/kernel/memory.o arch/x86/kernel/kmalloc.o arch/x86/kernel/do_page_fault.o \
	fs/cfs.o fs/fat32/fat32.o \
	arch/x86/Architecture.o arch/x86/io.o arch/x86/CPU/CPU.o \
	arch/x86/syscalls.o \
	cpp/support.o lib/string.o lib/mem.o lib/graphics.o lib/math.o \
	GUI/init.o GUI/manage.o GUI/refresh.o GUI/function.o GUI/gui_api.o \
	GUI/window/lib.o GUI/window/task_bar.o GUI/window/window.o \
	ipc/msg.o
	
	objcopy -R .note -R .comment -S -O binary kernel.o kernel.bin
	
	nasm head.asm -o head.bin
	
	copy /B head.bin + kernel.bin KERNEL
	
# 删除所有临时文件
clean:
	del /s /f /q KERNEL
	del /s /f /q *.o
	del /s /f /q *.bin


# 运行
# 清空屏幕
# 装载映像
# 启动虚拟机运行

run:all
	cls
	WinImage ..\image\Explorer.img KERNEL /i /h /y
	VBoxManage.exe startvm "Ghost Bird 0.02"