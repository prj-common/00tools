#include <stdio.h>
#include <stdlib.h>
#include <bl.h>
int bl_get_utf8_size(unsigned char byte)
{
    if ( (byte & 0x80) == 0x0) // 1byte
    { 
        return 1;
    } 
    else if ((byte & 0xE0) == 0xC0) // 2btye
    { 
        return 2;
    }  
    else if ((byte & 0xF0) == 0xE0) // 3byte
    { 
        return 3;
    } 
    else if ((byte & 0xF8) == 0xF0) // 4byte
    { 
        return 4;
    } 
    else if ((byte & 0xFC) == 0xF8) // 5byte
    { 
        return 5;
    } 
    else if ((byte & 0xFE) == 0xFC) // 6byte
    {
        return 6;
    }
    else
    {
        return 0;
    }
}

int bl_utf8_to_ucs2_char(unsigned short* dst, const unsigned char *src)
{
    char b1, b2, b3;
    int utfbytes = bl_get_utf8_size(*src);
    unsigned char *dst8 = NULL;

    dst8 = (unsigned char*)dst;
    *dst = 0x0;
    switch (utfbytes)
    {
        case 1: // 1byte utf8 encode character
            *dst = *src;
            utfbytes = 1;
            break;
        case 2: // 2byte utf8 encode character
            b1 = *src; 
            b2 = *(src + 1);
            if ((b2 & 0xC0) != 0x80)
            {
                return -1;
            }
            *dst8     = (b1 << 6) + (b2 & 0x3F);
            *(dst8+1) = (b1 >> 2) & 0x07;
            break;
        case 3: // 3byte utf8 encode character
            b1 = *src;
            b2 = *(src + 1);
            b3 = *(src + 2);
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
            {
                return -1;
            }
            *dst8     = (b2 << 6) + (b3 & 0x3F);
            *(dst8+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
#if 0
        case 4:
            b1 = *src;
            b2 = *(src + 1);
            b3 = *(src + 2);
            b4 = *(src + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *dst     = (b3 << 6) + (b4 & 0x3F);
            *(dst+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(dst+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;  
        case 5:  
            b1 = *src;  
            b2 = *(src + 1);  
            b3 = *(src + 2);  
            b4 = *(src + 3);  
            b5 = *(src + 4);  
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )  
                return 0;  
            *dst     = (b4 << 6) + (b5 & 0x3F);  
            *(dst+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);  
            *(dst+2) = (b2 << 2) + ((b3 >> 4) & 0x03);  
            *(dst+3) = (b1 << 6);  
            break;  
        case 6:  
            b1 = *src;  
            b2 = *(src + 1);  
            b3 = *(src + 2);  
            b4 = *(src + 3);  
            b5 = *(src + 4);  
            b6 = *(src + 5);  
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)  
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)  
                    || ((b6 & 0xC0) != 0x80) )  
                return 0;  
            *dst     = (b5 << 6) + (b6 & 0x3F);  
            *(dst+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);  
            *(dst+2) = (b3 << 2) + ((b4 >> 4) & 0x03);  
            *(dst+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);  
            break;
#endif            
        default:
            return -1;
            break;
    }  

    return utfbytes;  
}

int bl_ucs2_to_utf8_char(unsigned char *utf8, unsigned short ucs2)
{
	unsigned int utf8_uint;
	if(ucs2 < 0x80) // 1byte utf8
	{
		utf8_uint = ucs2;
        utf8[0] = utf8_uint&0xff;
        
		return 1;
	}
	if(ucs2 < 0x800) // 2byte utf8
	{
		utf8_uint = (0xc0 | (ucs2 >> 6)) << 8
			| (0x80 | (ucs2 & 0x3f));
        utf8[1] = utf8_uint&0xff;
        utf8[0] = (utf8_uint>>8)&0xff;

		return 2;
	}
	else //if(ucs2 < 0x10000) // 3byte utf8
	{
		utf8_uint = (0xe0 | (ucs2>>12)) << 16
			| (0x80 | (ucs2 >> 6 & 0x3f)) << 8
			| (0x80 | (ucs2 & 0x3f));
        utf8[2] = utf8_uint&0xff;
        utf8[1] = (utf8_uint>>8)&0xff;
        utf8[0] = (utf8_uint>>16)&0xff;
        
		return 3;
	}
}

int bl_utf8_to_ucs2_str(unsigned short *dst, unsigned char *src)
{
    int src_index = 0;
    int dst_index = 0;
    int cur_len = 0;

    bl_assert(dst && src);

    while(src[src_index])
    {
        cur_len = bl_utf8_to_ucs2_char(&dst[dst_index], &src[src_index]);
        if(cur_len < 0)
        {
            return -1;
        }
        src_index += cur_len;
        dst_index++;
    }
    
    return 0;
}

#define  TEST_UCS2_LEN  256

// test data
//U+0500	d4 80	CYRILLIC CAPITAL LETTER KOMI DE
//U+0501	d4 81	CYRILLIC SMALL LETTER KOMI DE
//U+0502	d4 82	CYRILLIC CAPITAL LETTER KOMI DJE
//U+0503	d4 83	CYRILLIC SMALL LETTER KOMI DJE
//U+0504	d4 84	CYRILLIC CAPITAL LETTER KOMI ZJE
//U+0505	d4 85	CYRILLIC SMALL LETTER KOMI ZJE
//U+0506	d4 86	CYRILLIC CAPITAL LETTER KOMI DZJE

// U+F901	¸ü	ef a4 81	CJK COMPATIBILITY IDEOGRAPH-F901
// U+F904	»¬	ef a4 84	CJK COMPATIBILITY IDEOGRAPH-F904
// U+F905	´®	ef a4 85	CJK COMPATIBILITY IDEOGRAPH-F905
// U+F906	¾ä	ef a4 86	CJK COMPATIBILITY IDEOGRAPH-F906

int utf8_to_ucs2_test()
{
    static unsigned short a[TEST_UCS2_LEN];
    int i = 0;

    memset(a, 0, TEST_UCS2_LEN*2);
    bl_utf8_to_ucs2_str(a, "\xd0\x90\x40\xef\xa4\x81\x50\xd4\x80");
    while(a[i])
    {
        printf("0x%x\n", a[i]);
        i++;
    }
    
    return 0;
}

int ucs2_to_utf8_test()
{
    unsigned short ucs2 = 0xf901;
    unsigned char utf8[128];

    memset(utf8, 0, 128);
    bl_ucs2_to_utf8_char(utf8, ucs2);
    printf("%x %x %x %x\n", utf8[0], utf8[1], utf8[2], utf8[3]);

    return 0;
}

int main_utf8_test()
{
	utf8_to_ucs2_test();
    // ucs2_to_utf8_test();

    return 0;
}

