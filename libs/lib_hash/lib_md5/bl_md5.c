#include <stdio.h>
#include <stdlib.h>
#include "bl.h"
#include "lib_md5.h"
#include "md5_usage.h"
#include "bl_err.h"

int bl_md5_ascii(char digest[MD5_DIGEST_SIZE], char *ascii_string)
{
	MD5CTX  hasher;

	MD5_Initialize(&hasher);
	MD5_Update(&hasher, ascii_string, bl_strlen(ascii_string));
	MD5_Final(digest, &hasher);

	return 0;
}

int bl_md5_file_addr_start(char digest[MD5_DIGEST_SIZE], char *file_name, int address)
{
	FILE* fp = NULL;
	int read_size = 0;
	int buf_size = 0;
	unsigned char *tmp_buf = NULL;
	MD5CTX  hasher;

	bl_assert(digest && file_name);
	fp = fopen(file_name, "rb");
	if(!fp)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}

	MD5_Initialize(&hasher);
	fseek(fp, address, SEEK_SET);
	tmp_buf = bl_get_mem();
	if(NULL == tmp_buf)
	{
		bl_set_errorno(EN_OUT_OF_MEM);
		return -1;
	}
	buf_size = bl_get_mem_size();
	while((read_size = fread(tmp_buf, 1, buf_size, fp))>0) 
	{
		if(read_size <= 0)
		{
			break;
		} 
		MD5_Update(&hasher, tmp_buf, read_size);       
	}

	MD5_Final(digest, &hasher);

	fclose(fp);    
	return 0;
}

int bl_md5_file(char digest[MD5_DIGEST_SIZE], char *file_name)
{
	return bl_md5_file_addr_start(digest, file_name, 0);
}

// test case
// file: md5_test1.txt
// MD5: 5a105e8b9d40e1329780d62ea2265d8a
void md5_test(void)
{
	int i;
	char digest[MD5_DIGEST_SIZE];
	
	bl_md5_file(digest, ".\\lib_md5\\md5_test1.txt");
	printf("file: md5_test1.txt\n");
	printf("md5sums: ");
	for(i=0; i<MD5_DIGEST_SIZE; i++)
	{
		printf("%02X", (unsigned char)digest[i]);
	}
	printf("\n");
}
