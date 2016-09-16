#ifndef __BL_MEM_H__
#define __BL_MEM_H__

#define BL_USE_STATIC_MEM
unsigned char *bl_mem_init(void);
void bl_mem_release(void);
int bl_get_mem_size(void);
unsigned char *bl_get_mem(void);

#endif

