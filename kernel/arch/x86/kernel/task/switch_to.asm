;Copyright 2013-2015 by Explorer Developers.
;made by Lab Explorer Developers<1@GhostBirdOS.org>
;Explorer x86 function switch to
;Explorer/arch/x86/kernel/task/switch_to.asm
;version:Alpha
;1/11/2015 1:02 AM

%define TASK_STACK	8

global	switch_to
extern	current
extern	printk
extern	print_mem
	
[bits 32]
;切换任务函数
;void switch_to(union task* id);
;参数：id为要切换的任务的id(联合体首地址)
;id:[ESP+4]
switch_to:
	mov		eax,[esp+4]
	mov		ecx,[current]
	;将current指向新任务
	mov		[current],eax
	;保存相关信息
	pushfd
	push	ebp
	;pushad
	mov		[ecx+TASK_STACK],esp
	;切换堆栈
	mov		esp,[eax+TASK_STACK]
	;还原信息
	pop		ebp
	popfd
	ret
	