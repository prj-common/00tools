// qvod down test
#include <stdio.h>

// website: http://www.bookcool.com/pdf/

#define QVOD_PLAY_PAGE_BUF_SIZE   100*1024
static char buf_s[QVOD_PLAY_PAGE_BUF_SIZE];
int bookcool_down_main()
{
	curl_down("http://www.bookcool.com/pdf/", "app_bookcool\\res\\bookcool.main");

	return 0;
}

int 
bookcool_main_page_handle
(char *name_old, char *name_new, char *prefix, char *back,
char *label_prefix, char *label_back)
{
	FILE *fp_old = fopen(name_old, "rb");
	FILE *fp_new = fopen(name_new, "w+");
	int prefix_len = 0;
	int flag = 0;
	int back_len = 0;
	char *start = NULL;
	char *find = NULL;
	char *start_label = NULL;
	char *find_label = NULL;

	if(NULL == fp_old || NULL == fp_new)
	{
		return -1;
	}
	back_len = bl_strlen(back);
	prefix_len = bl_strlen(prefix);
	if(0 == back_len)
	{
		return -1;
	}

	// buf[QVOD_PLAY_PAGE_BUF_SIZE];
	bl_memset(buf_s, 0, QVOD_PLAY_PAGE_BUF_SIZE);
	fread(buf_s, 1, QVOD_PLAY_PAGE_BUF_SIZE, fp_old);
	start = buf_s;
	find = bl_strstr(start, prefix);
	printf("***\n");
	while(find)
	{
		if(find[prefix_len]>='0' && find[prefix_len]<='9')
		{
			flag = 1;
		}
		start = find;
		find = bl_strstr(start, back);
		if(NULL == find)
		{
			return -1;
		}
		if(find-start > 200)
		{
			start += 100;
			goto find_next;
		}
		start_label = bl_strstr(start, label_prefix);
		if(start_label)
		{
			start_label += bl_strlen(label_prefix);
			find_label = bl_strstr(start_label, label_back);
		}
		if(find_label && flag)
		{
			int len = 0;
			
			*find_label = 0;
			len = bl_strlen(start_label);
			if(len > 0)
			{
				unsigned char *tmp = bl_malloc(len+1);
				if(tmp)
				{
					int i, j=0;
					bl_memset(tmp, 0, len+1);
					for(i=0; i<len; i++)
					{
						if(start_label[i]!='\x20'&&
							start_label[i]!='\xa'&&
							start_label[i]!='\xd')
						{
							tmp[j++] = start_label[i];
						}
					}
					bl_fprintf(fp_new, "DIR%s\n", tmp);
					bl_free(tmp);
				}
			}
		}
		find = find+back_len;
		*find = 0;
		//bl_fprintf(fp_new, "%s\n", start);
		if(flag)
		{
			fprintf(fp_new, "DOWN%s\n", start);
			flag = 0;
		}
		start = find+50;
find_next:
		find = bl_strstr(start, prefix);
	}

	fclose(fp_old);
	fclose(fp_new);
	return 0;
}

int bookcool_down_dir_page(unsigned char *name_r)
{
	FILE *fp = NULL;
	unsigned char buf[1024];
	unsigned char dir[1024];

	fp = fopen(name_r, "r");
	if(NULL == fp)
	{
		printf("file open error\n");
		return -1;
	}

	while(0 == feof(fp))
	{
		bl_memset(buf, 0, 1024);
		//bl_memset(dir, 0, 1024);
		if(fgets (buf, 1024, fp)!= NULL)
		{
			if(buf[0] == 'h')
			{
				buf[bl_strlen(buf)-1] = 0;
				strcat(dir, "\\index.html");
				curl_down(buf, &dir[6]);
				printf("dir: %s\n", dir);
				printf("buf: %s\n", buf);
			}
			else
			{
				bl_memset(dir, 0, 1024);
				buf[bl_strlen(buf)-1] = 0;
				bl_strcpy(dir, "mkdir app_bookcool\\res\\");
				bl_strcat(dir, buf);
				//printf("%s\n", dir);
				system(dir);
			}
			//printf("%s\n", buf);
		}
	}

	fclose(fp);
}

int bookcool_index_page_handle(void)
{
	FILE *fp = NULL;
	FILE *fp_r = NULL;
	FILE *fp_w = NULL;
	unsigned char buf[1024];
	unsigned char dir[1024];
	unsigned char dir_out[1024];
	unsigned char *pdf = NULL;
	unsigned char buf_tmp[1024];

	fp = fopen("app_bookcool\\res\\bookcool.main.handle", "r");
	if(NULL == fp)
	{
		printf("file open error\n");
		return -1;
	}

	while(0 == feof(fp))
	{
		bl_memset(buf, 0, 1024);
		if(fgets (buf, 1024, fp)!= NULL)
		{
			buf[bl_strlen(buf)-1] = 0;
			if(buf[0] == 'D' && buf[1] == 'I' && buf[2] == 'R')
			{
				bl_memset(dir, 0, 1024);
				bl_memset(dir_out, 0, 1024);
				bl_strcpy(dir, "app_bookcool\\res\\");
				bl_strcat(dir, &buf[3]);
				bl_strcpy(dir_out, dir);
				bl_strcat(dir, "\\index.html");
				bl_strcat(dir_out, "\\index_out.html");
				fp_r = fopen(dir, "r");
				fp_w = fopen(dir_out, "w");
				if(fp_r == NULL)
				{
					printf("file open error: %s\n", dir);
					continue;
				}
				if(fp_w == NULL)
				{
					printf("file open error: %s\n", dir_out);
					continue;
				}
				bl_memset(buf_s, 0, QVOD_PLAY_PAGE_BUF_SIZE);
				fread(buf_s, 1, QVOD_PLAY_PAGE_BUF_SIZE, fp_r);
				bl_memset(buf_tmp, 0, 1024);
				pdf = bl_strstr(buf_s, ".pdf");
				while(pdf)
				{
					unsigned char *tmp = NULL;
					unsigned char name_buf[1024];
					unsigned char *start = NULL;
					unsigned char *end = NULL;
					tmp = bl_strstr(&pdf[-44], "http");
					if(tmp && pdf > tmp)
					{
						int i, j=0;
						static unsigned char tmp1[1024];
						int len = 0;
						bl_memset(name_buf, 0, 1024);
						bl_memset(tmp1, 0, 1024);
						bl_memcpy(buf_tmp, tmp, pdf-tmp+4);
						//printf("%s\n", buf_tmp);
						//fprintf(fp_w, "%s\n", buf_tmp);
						start = pdf+6;
						end = bl_strstr(start, "</A>");
						bl_memcpy(name_buf, start, end-start);
						len = bl_strlen(name_buf);
						//bl_memset(tmp, 0, len+1);
						for(i=0; i<len; i++)
						{
							if(name_buf[i]!='\x20'&&
								name_buf[i]!='\xa'&&
								name_buf[i]!='\xd')
							{
								tmp1[j++] = name_buf[i];
							}
						}

						//printf("%s\n", tmp1);
						fprintf(fp_w, "&URL&%s&NAME&%s\n", buf_tmp, tmp1);
					}
					pdf = bl_strstr(&pdf[1], ".pdf");
				}
				// printf("dir: %s\n", dir);
				// printf("dir_out: %s\n", dir_out);
				fclose(fp_r);
				fclose(fp_w);
			}
		}
	}

	fclose(fp);
	return 0;
}

int bookcool_down_pdf()
{
	FILE *fp = NULL;
	FILE *fp_index_out = NULL;
	static unsigned char buf[1024];
	static unsigned char dir[1024];
	static unsigned char dir_index_out[1024];

	fp = fopen("app_bookcool\\res\\bookcool.main.handle", "r");
	if(NULL == fp)
	{
		printf("file open error\n");
		return -1;
	}

	while(0 == feof(fp))
	{
		bl_memset(buf, 0, 1024);
		bl_memset(dir, 0, 1024);
		bl_memset(dir_index_out, 0, 1024);
		if(fgets (buf, 1024, fp)!= NULL)
		{
			buf[bl_strlen(buf)-1] = 0;
			if(buf[0] == 'D' && buf[1] == 'I' && buf[2] == 'R')
			{
				static unsigned char tmp[1024];
				static unsigned char url[1024];
				static unsigned char name[1024];
				//printf("%s\n", buf);
				bl_strcpy(dir, "app_bookcool\\res\\");
				bl_strcat(dir, &buf[3]);
				bl_strcpy(dir_index_out, dir);
				bl_strcat(dir_index_out, "\\index_out.html");
				//printf("%s\n", dir_index_out);
				fp_index_out = fopen(dir_index_out, "r");
				if(NULL == fp_index_out)
				{
					printf("file open error\n");
					continue;
				}
				bl_memset(tmp, 0, 1024);
				bl_memset(url, 0, 1024);
				bl_memset(name, 0, 1024);
				while(0 == feof(fp_index_out))
				{
					if(fgets (tmp, 1024, fp_index_out)!= NULL)
					{
						unsigned char *mark = NULL;
						tmp[bl_strlen(tmp)-1] = 0;
						mark = bl_strstr(tmp, "&NAME&");
						if(mark)
						{
							static unsigned char full_name[1024];
							bl_memset(full_name, 0, 1024);
							bl_memcpy(url, &tmp[5], mark-tmp-5);
							bl_strcpy(name, &mark[6]);
							bl_strcat(name, ".pdf");
							bl_strcpy(full_name, dir);
							bl_strcat(full_name, "\\");
							bl_strcat(full_name, name);
							printf("%s %s\n", url, full_name);
							curl_down(url, full_name);
						}
					}
				}
				fclose(fp_index_out);
			}
		}
	}

	fclose(fp);
	return 0;
}

int bookcool_main()
{
	// bookcool_down_main();
#if 0
	bookcool_main_page_handle("app_bookcool\\res\\bookcool.main", 
		"app_bookcool\\res\\bookcool.main.handle", 
		"http://www.bookcool.com/pdf/", 
		"index.htm",
		"target=_blank>", 
		"</A></TD>");
#endif
	//bookcool_down_dir_page("app_bookcool\\res\\bookcool.main.handle");
	//bookcool_index_page_handle();
	bookcool_down_pdf();
	return 0;
}
