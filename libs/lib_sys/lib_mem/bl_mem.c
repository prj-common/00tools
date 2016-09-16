#include "bl_mem.h"
#include "bl.h"
#include <stdio.h>

#define  BL_MEM_SIZE  (1024*1024*10)
static unsigned char *mem_ptr = NULL;

#ifdef BL_USE_STATIC_MEM
static unsigned char buf[BL_MEM_SIZE];
#endif

unsigned char *bl_mem_init(void)
{
#ifdef BL_MEM_SIZE
	mem_ptr = buf;
#else
	mem_ptr = bl_malloc(BL_MEM_SIZE);
#endif
	return mem_ptr;
}

void bl_mem_release(void)
{
#ifdef BL_MEM_SIZE
#else
	if(mem_ptr)
	{
		bl_free(mem_ptr);
		mem_ptr = NULL;
	}
#endif
}

int bl_get_mem_size(void)
{
	return BL_MEM_SIZE;
}

unsigned char *bl_get_mem(void)
{
	return mem_ptr;
}

