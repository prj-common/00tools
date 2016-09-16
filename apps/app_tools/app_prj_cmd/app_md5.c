#include <stdio.h>
#include <stdlib.h>
#include "md5_usage.h"
#include "app_common.h"

int app_md5_handle_file(int argc, char *argv[])
{
	unsigned char digest[MD5_DIGEST_SIZE];
	int i;
	int ret = 0;

	bl_memset(digest, 0, MD5_DIGEST_SIZE);
	ret = bl_md5_file(digest, argv[3]);
	if(ret < 0)
	{
		printf("%s\n", bl_get_cur_errstr());
		return -1;
	}

	printf("file: %s\n", argv[3]);
	printf("md5: ");

	for(i=0; i<MD5_DIGEST_SIZE; i++)
	{
		printf("%02X", digest[i]);
	}
	printf("\n");

	return 0;
}

// argv[3]: ascii string
int app_md5_handle_ascii(int argc, char *argv[])
{
	unsigned char digest[MD5_DIGEST_SIZE];
	int i;
	int ret = 0;

	bl_memset(digest, 0, MD5_DIGEST_SIZE);
	ret = bl_md5_ascii(digest, argv[3]);
	if(ret < 0)
	{
		printf("%s\n", bl_get_cur_errstr());
		return -1;
	}

	printf("string: %s\n", argv[3]);
	printf("md5: ");

	for(i=0; i<MD5_DIGEST_SIZE; i++)
	{
		printf("%02X", digest[i]);
	}
	printf("\n");

	return 0;
	return 0;
}

// argv[3]: hex string
int app_md5_handle_hex(int argc, char *argv[])
{
	printf("todo\n");

	return 0;
}

int fmd5_check_handle(int argc, char *argv[])
{
	unsigned char digest_old[MD5_DIGEST_SIZE];
	unsigned char digest_new[MD5_DIGEST_SIZE];
	FILE *fp = NULL;
	int i;
	int ret = 0;

	bl_memset(digest_old, 0, MD5_DIGEST_SIZE);
	bl_memset(digest_new, 0, MD5_DIGEST_SIZE);

	ret = bl_md5_file_addr_start(digest_new, argv[2], MD5_DIGEST_SIZE);
	if(ret < 0)
	{
		printf("%s\n", bl_get_cur_errstr());
		return -1;
	}
	fp = fopen(argv[2], "rb");
	if(NULL == fp)
	{
		printf("file open error: %s\n", argv[2]);
		return -1;
	}
	fread(digest_old, 1, MD5_DIGEST_SIZE, fp);

	for(i=0; i<MD5_DIGEST_SIZE; i++)
	{
		if(digest_new[i] != digest_old[i])
		{
			printf("file check error\n");
			return -1;
		}
	}

	printf("file check ok\n");
	printf("md5: ");
	for(i=0; i<MD5_DIGEST_SIZE; i++)
	{
		printf("%02X", digest_new[i]);
	}
	printf("\n");

	return 0;
}

int fmd5_convert_handle(int argc, char *argv[])
{
	unsigned char digest[MD5_DIGEST_SIZE];
	int ret = 0;

	bl_memset(digest, 0, MD5_DIGEST_SIZE);
	ret = bl_md5_file(digest, argv[3]);
	if(ret < 0)
	{
		printf("%s\n", bl_get_cur_errstr());
		return -1;
	}
	ret = bl_fcpy_bf(argv[2], digest, MD5_DIGEST_SIZE, argv[3]);
	if(ret < 0)
	{
		printf("%s\n", bl_get_cur_errstr());
		return -1;
	}

	printf("file convert ok\n");
	printf("\n");

	return 0;
}

int fmd5_revert_handle(int argc, char *argv[])
{
	int ret = 0;

	ret = bl_fpart_addr_start(argv[2], MD5_DIGEST_SIZE, argv[3]);
	if(ret < 0)
	{
		printf("%s\n", bl_get_cur_errstr());
		return -1;
	}
	
	printf("file revert ok\n");
	return 0;
}

int app_md5_main(int argc, char *argv[])
{
	if(argc < 4)
	{
		app_libtools_print_help_prompt();
		return 0;
	}

	if(bl_strcmp(argv[1], "-md5"))
	{
		app_libtools_print_help_prompt();
		return 0;
	}

	if(!bl_strcmp(argv[2], "-f"))
	{
		return app_md5_handle_file(argc, argv);
	} 
	if(!bl_strcmp(argv[2], "-a"))
	{
		return app_md5_handle_ascii(argc, argv);
	} 
	if(!bl_strcmp(argv[2], "-h"))
	{
		return app_md5_handle_hex(argc, argv);
	} 	
	else
	{
		app_libtools_print_help_prompt();
		return 0;
	}

	return 0;
}

