#include <stdio.h>
#include <stdlib.h>
#include "app_common.h"

int main_prj(int argc, char *argv[])
{
	if(argc == 1)
	{
		app_libtools_print_help_prompt();
		return 0;
	}

	if(argc == 2)
	{
		if(!bl_strcmp(argv[1], "-help") || !bl_strcmp(argv[1], "-h"))
		{
			app_libtools_help();
			return 0;
		}
		else if(!bl_strcmp(argv[1], "-version") || !bl_strcmp(argv[1], "-v"))
		{
			app_libtools_show_version();
			return 0;
		}
		else
		{
			app_libtools_print_help_prompt();
			return 0;
		}
	}

	if(!bl_strcmp(argv[1], "-crc"))
	{
		app_crc_main(argc, argv);
	}
	else if(!bl_strcmp(argv[1], "-md5"))
	{
		app_md5_main(argc, argv);
	}
	else
	{
		app_libtools_print_help_prompt();
	}
	
	return 0;
}
