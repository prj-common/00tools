#ifndef __BL_H__
#define __BL_H__

#include <stdio.h>
#include <assert.h>

#define  bl_assert assert

// c lib
void *bl_malloc(unsigned int size);
void bl_free(void *ptr);
char *bl_strcpy(char *dst, char *src);
char *bl_strcat(char *dst, char *src);
char *bl_memcpy(char *dst, char *src, int n);
int bl_strlen(char *str);
char *bl_strstr(char *str, char *match);
char *bl_strchr(char *str, int match);
void *bl_memset(void *s, int c, unsigned int n);
void bl_reverse_str(char *s);
int  bl_atoi(const char *s);
void bl_itoa(int n, char *s);
int bl_fprintf(FILE *fp, const char *fmt, ...);
#ifdef WIN32
int bl_utf8_to_gb(char *dst_buf, int dst_buf_len, char *src_buf);
#endif

#endif

