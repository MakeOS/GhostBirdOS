#include <stddef.h>
#include <memory.h>
#include <lib/string.h>
#include <lib/mem.h>

#ifdef __cplusplus  
	extern "C"
	{
		void printk(const char* format, ...);
		void test_C_plus_plus(void);
	}
#endif  

void *operator new (size_t size)
{
	void *ptr;
	ptr = (void *) kmalloc(size, 0);
	if (!ptr) return ptr;
	memset(ptr, '\0', size);
	return ptr;
}

void *operator new[](size_t size)
{
	void *ptr;
	ptr = (void *) kmalloc(size, 0);
	if (!ptr) return ptr;
	memset(ptr, '\0', size);
	return ptr;
}

void operator delete(void* p)
{
	kfree(p);
}

void test_C_plus_plus(void)
{
	int i; 
	char *a = new char[6];
	a[0] = 'a';
	a[1] = 'b';
	a[2] = 'c';
	a[3] = '\0';
	printk("it is made by C++.\n word:%s.\n", a);
	delete a;
}