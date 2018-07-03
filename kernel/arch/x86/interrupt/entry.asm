;Copyright 2013-2015 by Explorer Developers.
;Lab Explorer Developers<1@GhostBirdOS.org>
;Explorer Interruption entry
;Explorer/arch/x86/interrupt/entry.asm
;version:Alpha
;1/2/2015 9:47 AM: created

[section .text]

;全局函数及全局变量
extern interrupt_handle				;位于handle.c中
extern do_syscalls					;位于syscalls.c中
global int_0						;0号中断执行函数
global int_1						;1号中断执行函数
global int_2						;2号中断执行函数
global int_3						;3号中断执行函数
global int_4						;4号中断执行函数
global int_5						;5号中断执行函数
global int_6						;6号中断执行函数
global int_7						;7号中断执行函数
global int_8						;8号中断执行函数
global int_9						;9号中断执行函数
global int_10						;10号中断执行函数
global int_11						;11号中断执行函数
global int_12						;12号中断执行函数
global int_13						;13号中断执行函数
global int_14						;14号中断执行函数
global int_15						;15号中断执行函数
global int_16						;16号中断执行函数
global int_17						;17号中断执行函数
global int_18						;18号中断执行函数
global int_19						;19号中断执行函数
global int_20						;20号中断执行函数
global int_21						;21号中断执行函数
global int_22						;22号中断执行函数
global int_23						;23号中断执行函数
global int_24						;24号中断执行函数
global int_25						;25号中断执行函数
global int_26						;26号中断执行函数
global int_27						;27号中断执行函数
global int_28						;28号中断执行函数
global int_29						;29号中断执行函数
global int_30						;30号中断执行函数
global int_31						;31号中断执行函数
global int_32						;32号中断执行函数
global int_33						;33号中断执行函数
global int_34						;34号中断执行函数
global int_35						;35号中断执行函数
global int_36						;36号中断执行函数
global int_37						;37号中断执行函数
global int_38						;38号中断执行函数
global int_39						;39号中断执行函数
global int_40						;40号中断执行函数
global int_41						;41号中断执行函数
global int_42						;42号中断执行函数
global int_43						;43号中断执行函数
global int_44						;44号中断执行函数
global int_45						;45号中断执行函数
global int_46						;46号中断执行函数
global int_47						;47号中断执行函数
global int_48						;48号中断执行函数
global int_49						;49号中断执行函数
global int_50						;50号中断执行函数

;中断开始宏
%macro interrupt_start 1
	push	dword 0x00000000		;错误代码
	push	dword %1				;中断号
	pushad							;保护通用寄存器
	jmp		interrupt_entry			;统一执行
%endmacro

;中断开始(CPU自动压入错误号)宏
%macro interrupt_start_errnum 1
	push	dword %1				;中断号
	pushad							;保护通用寄存器
	jmp		interrupt_entry			;统一执行
%endmacro

;中断开始(系统调用)宏
%macro interrupt_start_syscall 1
	push	dword 0x00000000		;错误代码
	push	dword %1				;中断号
	pushad							;保护通用寄存器
	jmp		do_syscalls				;执行
%endmacro

[bits 32]

;中断执行函数入口
int_0:	interrupt_start 0			;0号中断
int_1:	interrupt_start 1			;1号中断
int_2:	interrupt_start 2			;2号中断
int_3:	interrupt_start 3			;3号中断
int_4:	interrupt_start 4			;4号中断
int_5:	interrupt_start 5			;5号中断
int_6:	interrupt_start 6			;6号中断
int_7:	interrupt_start 7			;7号中断
int_8:	interrupt_start_errnum 8	;8号中断
int_9:	interrupt_start 9			;9号中断
int_10:	interrupt_start_errnum 10	;10号中断
int_11:	interrupt_start_errnum 11	;11号中断
int_12:	interrupt_start_errnum 12	;12号中断
int_13:	interrupt_start_errnum 13	;13号中断
int_14:	interrupt_start_errnum 14	;14号中断
int_15:	interrupt_start 15			;15号中断
int_16:	interrupt_start 16			;16号中断
int_17:	interrupt_start_errnum 17	;17号中断
int_18:	interrupt_start 18			;18号中断
int_19:	interrupt_start 19			;19号中断
int_20:	interrupt_start 20			;20号中断
int_21:	interrupt_start 21			;21号中断
int_22:	interrupt_start 22			;22号中断
int_23:	interrupt_start 23			;23号中断
int_24:	interrupt_start 24			;24号中断
int_25:	interrupt_start 25			;25号中断
int_26:	interrupt_start 26			;26号中断
int_27:	interrupt_start 27			;27号中断
int_28:	interrupt_start 28			;28号中断
int_29:	interrupt_start 29			;29号中断
int_30:	interrupt_start 30			;30号中断
int_31:	interrupt_start 31			;31号中断
int_32:	interrupt_start 32			;32号中断
int_33:	interrupt_start 33			;33号中断
int_34:	interrupt_start 34			;34号中断
int_35:	interrupt_start 35			;35号中断
int_36:	interrupt_start 36			;36号中断
int_37:	interrupt_start 37			;37号中断
int_38:	interrupt_start 38			;38号中断
int_39:	interrupt_start 39			;39号中断
int_40:	interrupt_start 40			;40号中断
int_41:	interrupt_start 41			;41号中断
int_42:	interrupt_start 42			;42号中断
int_43:	interrupt_start 43			;43号中断
int_44:	interrupt_start 44			;44号中断
int_45:	interrupt_start 45			;45号中断
int_46:	interrupt_start 46			;46号中断
int_47:	interrupt_start 47			;47号中断
int_48:	interrupt_start 48			;48号中断
int_49:	interrupt_start 49			;49号中断
int_50:	interrupt_start_syscall	50	;50号中断(系统调用)

	
	
interrupt_entry:
	call	interrupt_handle		;调用中断处理函数
	popad							;还原通用寄存器
	add		esp,8					;恢复栈
	iretd							;中断反回
