// Explorer Memory Management Unit
// /boot/MMU.c

#include "../configure.h"
#include "../problem.h"
#include "MMU.h"
#include "VI.h"
#include "lib/stdlib.h"

/**未分配内存指针*/
unsigned long unalloc_ptr = mem_start;
/**不可回收内存分配函数*/
void *bmalloc(unsigned long size)
{
	void *retval;
	
	/**若有足够的内存供分配，则成功分配，返回已经分配的内存地址*/
	if ((unalloc_ptr + size) < (mem_start + mem_size))
	{
		retval = (void *)unalloc_ptr;
		unalloc_ptr += size;
		return retval;
	/**空间不足*/
	}else{
		warning(WARN_NO_MEM, "The available memory for the boot program is not enough.");
		return NULL;
	}
}
