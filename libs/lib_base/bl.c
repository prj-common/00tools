#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "bl.h"

#if 0
void bl_assert(int a)
{
	assert(a);
}
#endif

// c lib
void *bl_malloc(unsigned int size)
{
	void *ptr = NULL;
	
	ptr = malloc(size);
	if(ptr)
	{
		memset(ptr, 0, size);
	}
	
	return ptr;
}

void bl_free(void *ptr)
{
	free(ptr);
}

int bl_strcmp(char *dst, char *src)
{
	return strcmp(dst, src);
}

char *bl_strcpy(char *dst, char *src)
{
	return strcpy(dst, src);
}

char *bl_strcat(char *dst, char *src)
{
	return strcat(dst, src);
}

char *bl_memcpy(char *dst, char *src, int n)
{
	return memcpy(dst, src, n);
}

int bl_strlen(char *str)
{
	return strlen(str);
}

char *bl_strstr(char *str, char *match)
{
	return strstr(str, match);
}

char *bl_strchr(char *str, int match)
{
	return strchr(str, match);
}

void *bl_memset(void *s, int c, unsigned int n)
{
	return memset(s, c, n);
}

void bl_reverse_str(char *s)
{
	int c;
	char *t;

	for (t = s + (strlen(s) - 1); s < t; s++, t-- )
	{
		c = *s;
		*s = *t;
		*t = c;
	}
}

int  bl_atoi(const char *s)
{
	return atoi(s);
}

void bl_itoa(int n, char *s)
{
	int sign;
	char *t = s;

	if ((sign = n) < 0)
	{
		n = -n;
	}
	
	do
	{
		*s++ = n % 10 + '0';
	} while ((n /= 10) >0);

	if (sign < 0)
	{
		*s++ = '-';
	}
	*s = '\0';
	bl_reverse_str(t);
}

int bl_fprintf(FILE *fp, const char *fmt, ...)
{
	static char printf_buf[1024*4];
	va_list args;
	int i;

	va_start(args, fmt);
	if(fp == stdout)
	{
		memset(printf_buf, 0, 1024*4);
		i = vsprintf(printf_buf, fmt, args);
		printf(printf_buf);
	}
	else
	{
		i=vfprintf(fp,fmt,args);
	}
	va_end(args);

	return i;
}

// string to int
// hex char to int
int bl_hex_chr_to_int(unsigned int c)
{
	if((c>='0') && (c<='9'))
	{
		return c - '0';
	}

	if(c>='a' && c<='z')
	{
		c &= ~0x20;
	}

	return c-'A'+10;
}

// hex string to int
// skip blank
static int str_skip_blank(unsigned char *buf)
{
	unsigned char *s = buf;
	unsigned char *e = buf;
	unsigned int cur_char = 0;

	bl_assert(buf);
	cur_char = *e;
	while(cur_char)
	{
		if(cur_char == ' ')
		{
			e++;
		}
		else
		{
			*s++ = *e++;
		}
		cur_char = *e;
	}
	*s = 0;

	return 0;
}

int bl_hex_str_to_int(unsigned char *buf)
{
	unsigned int cur_char = 0;
	int ret = 0;
	
	str_skip_blank(buf);
	if(!strncmp(buf, "0x", 2))
	{
		buf += 2;	
	}
	cur_char = *buf;
	while(cur_char)
	{
		int cur = 0;
		cur = bl_hex_chr_to_int(cur_char);
		ret = (ret<<4) + cur;
		cur_char = *(++buf);
	}

	return ret;
}

#ifdef WIN32
/* windows �ַ�����ת������

API ���� MultiByteToWideChar 
��һ������Ϊ����ҳ
�ڶ�������Ϊѡ�һ���� 0 �Ϳ�����
����������Ϊ ANSI �ַ����ĵ�ַ, ����ַ����ǵ�һ������ָ�������Ե� ANSI �ַ��� (AnsiString)
���ĸ�����Ϊ ANSI �ַ����ĳ��ȣ������ -1, �ͱ�ʾ���� 0 ��Ϊ���������ַ���
���������Ϊת�����ɵ� unicode �ַ��� (WideString) �ĵ�ַ, ���Ϊ NULL, ���Ǵ���������ɵ��ַ����ĳ���
����������Ϊת�����ɵ� unicode �ַ������������

API ����WideCharToMultiByte����˵��
��һ��������ָ��Ҫת���ɵ��ַ�������ҳ�����������κ��Ѿ���װ�Ļ�ϵͳ�Դ����ַ�������Ҳ����ʹ��������ʾ����ҳ֮һ��
    CP_ACP ��ǰϵͳANSI����ҳ
    CP_MACCP ��ǰϵͳMacintosh����ҳ
    CP_OEMCP ��ǰϵͳOEM����ҳ��һ��ԭʼ�豸������Ӳ��ɨ����
    CP_SYMBOL Symbol����ҳ.
    CP_THREAD_ACP ��ǰ�߳�ANSI����ҳ������Windows 2000���Ժ�汾���Ҳ�������ʲô
    CP_UTF7 UTF-7�����ô�ֵʱlpDefaultChar��lpUsedDefaultChar������ΪNULL
    CP_UTF8 UTF-8�����ô�ֵʱlpDefaultChar��lpUsedDefaultChar������ΪNULL
�ڶ���������ָ����δ���û��ת�����ַ���������˲������������еĸ���һЩ���Ҷ��ǰ�����Ϊ0��
����������: ��ת���Ŀ��ַ���?
���ĸ���������ת�����ַ����ĳ��ȣ�-1��ʾת�����ַ�����β��
���������: ����ת��������´��Ļ�����?
����������: �����������С?
���߸�����: ָ���ַ���ָ��?
�ڰ˸����������ر�����ָ�룬���Ա����Ƿ�ʹ�ù�Ĭ���ַ���һ����Ϊ0

*/

int bl_utf8_to_gb(char *dst_buf, int dst_buf_len, char *src_buf)
{
	static char tmp_buf[1024*4];
	int len;

	memset(tmp_buf, 0, 1024*4);
	MultiByteToWideChar(CP_UTF8, 0, src_buf, -1, tmp_buf, 1024*4);
	len = WideCharToMultiByte(CP_ACP, 0, tmp_buf, -1, dst_buf, dst_buf_len, NULL, NULL);
	return len;
}
#endif

int bl_sub_str_remove_tailer(char *dst_str, char *match)
{
	char *start = NULL;

	start = bl_strstr(dst_str, match);
	if(NULL == start)
	{
		return -1;
	}
	
	if(start)
	{
		*start = 0;
	}

	return 0;
}

int bl_sub_str_remove(char *dst_str, char *match)
{
	char *start = NULL;
	char *find = NULL;
	char *start_back = NULL;
	char *end = NULL;
	int match_len = 0;

	match_len = bl_strlen(match);
	if(0 == match_len)
	{
		return -1;
	}
	start = dst_str;
	find = bl_strstr(start, match);
	if(NULL == find)
	{
		return -1;
	}

	while(find)
	{
		start_back = start = find;
		find = find+match_len;
		while(*find)
		{
			*start++ = *find++;
		}
		*start = *find;
		start = start_back;
		find = bl_strstr(start, match);
	}

	return 0;
}

int bl_sub_str_replace(char *dst, char *src, 
	char *match, char *replace, int len)
{
	char *start = NULL;
	char *find = NULL;
	int tmp_len = 0;
	int match_len = 0;
	int replace_len = 0;
	int count = 0;

	*dst = 0;
	start = src;
	match_len = bl_strlen(match);
	replace_len = bl_strlen(replace);
	
	find = bl_strstr(start, match);
	while(find && count < len)
	{
		tmp_len = find-start;
		count += tmp_len;
		if(count >= len)
		{
			break;
		}
		if(tmp_len > 0)
		{
			bl_memcpy(dst, start, tmp_len);
			dst+= tmp_len;
		}
		count += replace_len;
		if(count >= len)
		{
			break;
		}
		bl_memcpy(dst, replace, replace_len);
		dst += replace_len;
		find += match_len;
		start = find;
		find = bl_strstr(start, match);
	}

	if(count >= len)
	{
		return 0;
	}
	if(*start != 0)
	{
		tmp_len = bl_strlen(start);
		count += tmp_len;
		if(count >= len)
		{
			return 0;
		}
		bl_memcpy(dst, start, tmp_len);
		dst[tmp_len] = 0;
	}
	else
	{
		*dst = 0;
	}

	return 0;
}

int bl_test_utf8_to_gb(void)
{
	char dst_buf[1024];
	char *src_buf = "\xe7\x8b\x90\xe5\x81\x87\x7c\x3a\x2f";

	bl_utf8_to_gb(dst_buf, 100, src_buf);
	printf("%s\n", dst_buf);

	return 0;
}

int bl_test_sub_str(void)
{
	char dst_buf[1024];
	char *src = "hello_world_hello_world_hello_hello_test_123";
	char *match = "hello";
	char *replace = "world";

	bl_sub_str_replace(dst_buf, src, match, replace, 1024);
	printf("%s\n", dst_buf);
	bl_sub_str_remove(dst_buf, "world");
	printf("%s\n", dst_buf);
}

// test code
int bl_test(void)
{
	bl_test_utf8_to_gb();
	bl_test_sub_str();
	return 0;
}

