; Explorer Loader - MBR
; /mbr/MBR.asm
; MBR
; 编译后用二进制编辑工具将其写入虚拟映像或者实体映像的MBR扇区前446字节
; 先将自身移动至0x1000:0处，再加载剩下的127个保留扇区到MBR搬移后的新地
; 址之后将控制权交给之后0x1000:0x200处的剩下127个扇区

	; 正常运作的时候会在0x1000:0处开始
	org		0x0
	; 跳过Explorer MBR数据区
	jmp		start						; 占3Bytes 的跳转指令
	
	; 下面的数据格式十分重要，不能轻易改变
	driver		db 0					; Explorer MBR驱动器号，便于MBR找到自己所处磁盘
	signature	db "Explorer"			; Explorer MBR签名，便于引导程序找到驱动器
	
; MBR引导开始的地方，在存储
start:
	; 移动自身到0x1000:0去，由于这里程序较短，为相对地址跳转，不会出现意外
	mov		cx,512 / 2
	mov		si,$$				; 当前MBR的开始偏移地址为目标开始偏移地址+0x7c00
	mov		di,0x0000
	mov		ax,cs
	mov		ds,ax
	mov		ax,0x1000
	mov		es,ax
.moveloop:
	mov		ax,[ds:si + 0x7c00]
	mov		[es:si],ax
	add		si,2
	loop	.moveloop

	; 跳转至0x1000:load
	jmp		0x1000:load
	
load:
	; 关中断
	cli
	
	; 初始化寄存器
	mov		ax,cs
	mov		ds,ax
	mov		es,ax
	
	; 初始化堆栈，堆栈覆盖原来的MBR
	xor		ax,ax
	mov		ss,ax
	mov		sp,0x7e00
	
	; 先保存自身的驱动器号
	mov		[ds:driver],dl
	
	; 输出信息
	mov		si,str_move_success
	call	function.print
	
	; 加载之后的127个扇区，组成64KB占领0x1000:0~0x1000:0xffff的内存
	mov		ah,0x42
	mov		dl,[ds:driver]
	mov		si,disk_addr_pkt
	int		13h
	; 当读取错误时，会跳到.err_readboot处输出信息后停机
	jc		.err_readboot
	
	mov		si,str_suc_readboot
	call	function.print
	
	; 跳转至Boot
	jmp		0x1000:0x200

; 读取错误处理程序，显示错误信息之后空转
.err_readboot:
	mov		si,str_err_readboot
	call	function.print
	call	function.halt

; MBR 内置功能库
function:
; 输出字符串，入口参数：SI = 字符串首地址
.print:
	mov		ah,0x0e				;AH = 0x0e INT 10h 显示单个字符中断
	mov		al,[ds:si]
	cmp		al,0x0000			;判断是否是空字符
	jz		.finish				;若是空字符NUL，则反回
	int		10h
	inc		si
	jmp		.print
	
; 停机功能
.halt:
	hlt
	jmp		.halt
	
.finish:
	ret
	
; Boot地址包
disk_addr_pkt:
	packet_size		db  16			; 磁盘参数包的尺寸，必须为10H
	reserved		db  0			; 保留，必须为零
	block_count		dw  127			; 传输的扇区数
	buffer_addr		dd  0x10000200	; 内存缓冲区地址（段：偏移）
	block_num		dq  1			; 起始绝对扇区号（即起始扇区的LBA号码）

; 移动成功字符串
str_move_success:	db	"move MBR into 0x1000:0 successfully.", 10, 13, 0x00

; 读取Boot成功
str_suc_readboot:	db	"Read boot area successfully!", 10, 13, 0x00

; 读取Boot错误
str_err_readboot:	db	"Read boot area unsuccessfully!", 10, 13, "Please Press The key Ctrl+Alt+Delet to reboot", 10, 13, 0x00
	
	;填充MBR大小至446字节
	times	446-($-$$)	db	0
	
