#include <stdio.h>
#include <stdlib.h> 
#include "assert.h"
#include "code_type.h"

int print_gbcode_str(unsigned char *str);
int gb_to_unicode_test(void);
int unicode_to_gb_test(void);
int print_unicode_str(unsigned short *str);

int chcode_test()
{
	printf("gb2312 to unicode test...\n");
	gb_to_unicode_test();
	
	printf("\n");
	printf("unicode to gb2312 test...\n");
	unicode_to_gb_test();

	return 0;
}

int unicode_to_gb_test(void)
{
	unsigned short unicode_str[] = {0x4f60, 0x597d, 0x6c49, 0x5b57, 0x7f16, 0x7801, 0x6d4b, 0x8bd5, 0};
	unsigned short gb_str[64];

	memset(gb_str, 0, 64*2);
	mmi_chset_ucs2_to_text_str_ex(gb_str, 64, unicode_str);
	printf((unsigned char *)gb_str);
	printf("\n");
	print_unicode_str(unicode_str);
	print_gbcode_str(gb_str);
	printf("\n");
	return 0;
}

int gb_to_unicode_test(void)
{
	unsigned short gb_code = 0xbaba; // 0xbaba "ºº" µÄGBÂë
	unsigned short unicode = 0;
	unsigned char gb_str[] = "ÄãºÃºº×Ö±àÂë²âÊÔ";
	unsigned short unicode_str[64];
	int i = 0;

	printf("ºº\n"); 
	printf("[GB]%x\n", gb_code);
	unicode = mmi_chset_wchar_to_ucs2((unsigned short)gb_code);
	printf("[U]%x\n\n", unicode);

	memset(unicode_str, 0, 64*2);
	mmi_chset_text_to_ucs2_str_ex(unicode_str, 64, gb_str);
	printf("%s\n", gb_str);

	print_gbcode_str(gb_str);
	print_unicode_str(unicode_str);

	return 0;
}

int print_gbcode_str(unsigned char *str)
{
	unsigned char gb_byte = 0;
	unsigned short gb_2byte = 0;

	assert(str);
	printf("[G]");
	while(*str)
	{
		unsigned char cur_byte = 0;
		unsigned char *cur_p = str;
		cur_byte = *str++;
		
		if(cur_byte == 0)
		{
			assert(0);
			break;
		}
		else if(cur_byte&0x80) // 2byte
		{
			str++;
			gb_2byte = *(unsigned short *)cur_p;
			printf("%x ", gb_2byte);
		}
		else
		{
			gb_byte = *cur_p;
			printf("%x ", gb_byte);
		}
	}
	printf("\n");
	
	return 0;
}

int print_unicode_str(unsigned short *str)
{
	assert(str);
	printf("[U]");
	while(*str)
	{
		printf("%x ", *str++);
	}
	printf("\n");
}

