;内存可分配区间表(128KB)
mem_map:
	.offset		equ	0x0
	.segment	equ	0x3000
	.address	equ	0x30000
;内核内存可分配区间表(128KB)
k_mem_map:
	.address	equ	0x50000
	
;全局描述符表(64KB)
GDT_addr		equ	0x60000
GDT_size		equ	65536
