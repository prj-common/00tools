#include "bl_file.h"
#include "bl_err.h"
#include "bl_err.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#define S_ISREG(x)	(((x)&S_IFREG) == S_IFREG)

#define BL_FILE_CPY_SIZE  32768
static unsigned char file_cpy_buf[BL_FILE_CPY_SIZE];

// get file size
int  bl_get_file_size( char *file_name) // get file size
{
	FILE *fp;
	int  length;

	if((fp = fopen(file_name, "rb"))==NULL)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	fclose(fp);
	return (length);
}

// get file size by extend method
int bl_get_file_size_ex(char *file_name)
{
	int fd = -1;
	int size = -1;
	struct stat file_stat;

	fd = open(file_name, O_RDONLY|O_BINARY);

	if(fstat(fd, &file_stat) == -1)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}
	if(S_ISREG(file_stat.st_mode))
	{
		size = file_stat.st_size;
	}
	
	return size;
}

// file extract
int  bl_file_extract(char *new_name, char *old_name, int start_file_addr, int size)
{ 
	FILE *fp_new = NULL;
	FILE *fp_old = NULL;
	int old_file_len = 0;
	int read_size = 0;
	int write_size = 0;
	char *new_file_buf;

	old_file_len = bl_get_file_size(old_name);
	
	if(old_file_len < 0)
	{
		return old_file_len;
	}

	if((fp_new = fopen(new_name, "wb"))==NULL)
	{
		return -1;
	}

	if((fp_old = fopen(old_name, "rb"))==NULL)
	{
		fclose(fp_new);
		return -1;
	}

	fseek(fp_old, start_file_addr, SEEK_SET);
	new_file_buf = (char *)bl_malloc(size);
	if(new_file_buf == NULL)
	{
		fclose(fp_new);
		fclose(fp_old);
		return -1;
	}



	read_size = fread(new_file_buf, 1, size, fp_old);
	write_size = fwrite(new_file_buf, 1, read_size, fp_new);

	fclose(fp_new);
	fclose(fp_old);
	free(new_file_buf);
	return write_size;
}

int bl_file_append(char *file_name,char *append_file_name) 
{
	FILE *fp = NULL;
	FILE *fp_append = NULL;
	char *append_file_buf;
	int size = 0;

	size = bl_get_file_size(append_file_name);
	if(size <= 0)
	{
		return -1;
	}
	
	append_file_buf=(char *)bl_malloc(size);
	if((fp=fopen(file_name,"ab"))==NULL)
	{
		return -1;
	}
	
	if((fp_append = fopen(append_file_name,"rb"))==NULL)
	{
		return -1;
	}

	fseek(fp_append, 0, SEEK_SET);
	fread(append_file_buf, size, 1, fp_append);
	fwrite(append_file_buf, size, 1, fp);

	free(append_file_buf);
	fclose(fp);
	fclose(fp_append);

	return 0;
}     

int bl_fcpy_bf(char *dst_fname, char *buf, int buf_len, char *src_fname)
{
	FILE *fp_w = NULL, *fp_r = NULL;
	int read_size = 0;

	fp_r = fopen(src_fname, "rb");
	if(NULL == fp_r)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}
	fp_w = fopen(dst_fname, "wb");
	if(NULL == fp_w)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}

	fwrite(buf, 1, buf_len, fp_w);
    while((read_size = fread(file_cpy_buf, 1, BL_FILE_CPY_SIZE, fp_r))>0) 
    {
        if(read_size <= 0)
        {
            break;
        }
        fwrite(file_cpy_buf, 1, read_size, fp_w);
    }

	fclose(fp_r);
	fclose(fp_w);
	return 0;
}

int bl_fcpy_ff(char *dst_fname, char *src1_fname, char *src2_fname)
{
	return 0;
}

int bl_fpart_addr_start(char *dst_fname, int address, char *src_fname)
{
	FILE *fp_w = NULL, *fp_r = NULL;
	int read_size = 0;

	fp_r = fopen(src_fname, "rb");
	if(NULL == fp_r)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}
	fp_w = fopen(dst_fname, "wb");
	if(NULL == fp_w)
	{
		bl_set_errorno(EN_FILE_NOT_EXIST);
		return -1;
	}

	fseek(fp_r, address, SEEK_SET);
    while((read_size = fread(file_cpy_buf, 1, BL_FILE_CPY_SIZE, fp_r))>0) 
    {
        if(read_size <= 0)
        {
            break;
        }
        fwrite(file_cpy_buf, 1, read_size, fp_w);
    }

	fclose(fp_r);
	fclose(fp_w);
	return 0;
}

int bl_line_file_part(char *file_name, char *out_main_file_name, char *out_file_name_suffix,  
	char *out_dir, int part_line_num)
{
	FILE *fr = NULL;
	FILE *fw = NULL;
	char out_file_name[1024];
	char line_buf[1024];
	int count = 0;
	int cur_part = 0;
	char cmd[1024];

	fr = fopen(file_name, "r");
	if(NULL == fr)
	{
		printf("open read file error!\n");
		return -1;
	}

	memset(cmd, 0, 1024);
	sprintf(cmd, "mkdir %s", out_dir);
	system(cmd);

	count = part_line_num;
	memset(line_buf, 0, 1024);
	while (fgets(line_buf, 1024, fr) != NULL)
	{
		if(count == part_line_num)
		{
			memset(out_file_name, 0, 1024);
			strcpy(out_file_name, out_dir);
			strcat(out_file_name, "\\");
			strcat(out_file_name, out_main_file_name);
			sprintf(out_file_name, "%s_%d.%s", out_file_name, cur_part, out_file_name_suffix);
			//printf(out_file_name);
			cur_part++;
			fw = fopen(out_file_name, "w");
			if(NULL == fw)
			{
				fclose(fr);
				printf("open write file error!\n");
				return -1;
			}			
		}

		//printf("%d\n", count);
		//printf("%s", line_buf);
		fprintf(fw, "%s", line_buf);
		memset(line_buf, 0, 1024);

		count--;
		if(count == 0)
		{
			count = part_line_num;
			fclose(fw);
		}
	}

	fclose(fr);
	return 0;
}

int bl_line_file_part_test(int argc, char **argv)
{
	int part_num = 0;

	if(argc != 6)
	{
		printf("usage: part <file name> <out main file name> <suffix> <out dir> <part num>\n");
		return -1;
	}
	part_num = atoi(argv[5]);

	return bl_line_file_part(argv[1], argv[2], argv[3], argv[4], part_num);
}

void bl_file_test(void)
{
#if 0
	int size = -1;

	size = -1;
	size = bl_get_file_size("main.c");
	printf("main.c size: %d\n", size);

	size = -1;
	size = bl_get_file_size_ex("main.c");
	printf("main.c size: %d\n", size);
#endif
	bl_fcpy_bf("test1_new", "\x12\x34\x56\x78", 4, "test1");
	bl_fpart_addr_start("test1_new_new", 4, "test1_new");
}
