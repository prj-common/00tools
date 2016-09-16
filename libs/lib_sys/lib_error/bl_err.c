#include "bl_err.h"
#include <stdio.h>

int bl_errorno = 0;

unsigned char *err_msg_tab[] = {
	"no error",               // EN_OK
	"file not exist",         // EN_FILE_NOT_EXIST
	"out of memory",          // EN_OUT_OF_MEM
	"error end"               // EN_MAX
};

int bl_set_errorno(int errorno)
{
	bl_errorno = errorno;

	return 0;
}

unsigned char *bl_get_cur_errstr(void)
{
	return err_msg_tab[bl_errorno];
}

void bl_err_test(void)
{
	int tab_size = 0;
	int i;

	tab_size = sizeof(err_msg_tab) / sizeof(unsigned char *);
	printf("tab_size = %d\n", tab_size);
	for(i=0; i<tab_size; i++)
	{
		printf("%s\n", err_msg_tab[i]);
	}

	bl_errorno = 1;
	printf("errorno: %d, string: %s\n", bl_errorno, bl_get_cur_errstr());
}

