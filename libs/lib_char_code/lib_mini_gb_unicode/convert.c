#include <string.h>
#include "code_type.h"

#define UNKNOW_ENCODE_CHAR 0xffff

// gb2312 to ucs2
extern const key_index_t g_chset_gb2312_to_ucs2_key_LSB_index[];
extern const unsigned char g_chset_gb2312_to_ucs2_key_MSB[];
extern const unsigned short g_chset_gb2312_to_ucs2_table[];

// ucs2 to gb2312
extern const key_index_t g_chset_ucs2_to_gb2312_key_LSB_index[];
extern const unsigned char g_chset_ucs2_to_gb2312_key_MSB[];
extern const unsigned short g_chset_ucs2_to_gb2312_table[];

static int mmi_chset_binary_search(
                    const unsigned char key,
                    const unsigned char *lookup_table,
                    int start,
                    int end)
{
	while (start <= end)
    {
        int mid = (start + end) / 2;

        if (key > lookup_table[mid])
        {
            start = mid + 1;
        }
        else if (key < lookup_table[mid])
        {
            end = mid - 1;
        }
        else    /* key == target */
        {
            return mid;
        }
    }

    return -1;
}

unsigned short mmi_chset_wchar_to_ucs2(unsigned short encode_char)
{
    const key_index_t *key_LSB_index_table = g_chset_gb2312_to_ucs2_key_LSB_index;
    const unsigned char *key_MSB_table = g_chset_gb2312_to_ucs2_key_MSB;
    const unsigned short *ucs2_table = g_chset_gb2312_to_ucs2_table;

    unsigned char key_LSB = (unsigned char) (encode_char & 0xff);
    unsigned char key_MSB;
    int start = key_LSB_index_table[key_LSB].start;
    int end;
    int index;

    if (start < 0)  /* key LSB not found in index table */
    {
        return UNKNOW_ENCODE_CHAR;
    }

    key_MSB = (unsigned char) (encode_char >> 8);
    end = key_LSB_index_table[key_LSB].end;
    if ((index = mmi_chset_binary_search(key_MSB, key_MSB_table, start, end)) < 0) /* key MSB not found */
    {
        return UNKNOW_ENCODE_CHAR;
    }

    return ucs2_table[index];
}

// gb2312 string to ucs2 string
unsigned short mmi_chset_text_to_ucs2_str_ex(
            unsigned char *dest,
            int dest_size,
            unsigned char *src
)
{
    const key_index_t *key_LSB_index_table = g_chset_gb2312_to_ucs2_key_LSB_index;
    const unsigned char *key_MSB_table = g_chset_gb2312_to_ucs2_key_MSB;
    const unsigned short *ucs2_table = g_chset_gb2312_to_ucs2_table;

    unsigned char key_LSB;
    unsigned char key_MSB;
    int start;
    int end;
    int pos = 0;
    int index;
    unsigned char *src_end = src + strlen((char*)src);

    while (*(src) != 0) /* I do not know how we assume that this is end of data.. Currently we are taking only chinese encodings for
                           the optimized algo so we can take it as 2 bytes */
    {
        if (*(src) <= 0x7f)
        {
            *(dest + pos) = *src;
            pos++;
            *(dest + pos) = 0;
            pos++;
            src += 1;
        }
        else
        {
            key_LSB = (unsigned char) (*(src));
            start = key_LSB_index_table[key_LSB].start;

            if (start < 0)
            {
                if (src_end - src <= 1)
                {
                    /* can't encoding the character. The data may be segmented. */
                    break;
                }
                else
                {
                    dest[pos] = (unsigned char)UNKNOW_ENCODE_CHAR;
                    dest[pos + 1] = (unsigned char) (UNKNOW_ENCODE_CHAR>> 8);
                }
            }
            else
            {
                key_MSB = (unsigned char) (*(src + 1));
                end = key_LSB_index_table[key_LSB].end;
                if ((index = mmi_chset_binary_search(key_MSB, key_MSB_table, start, end)) < 0)  /* key MSB not found */
                {
                    if (src_end - src <= 1)
                    {
                        /* can't encoding the character. The data may be segmented. */
                        break;
                    }
                    else
                    {
                        dest[pos] = (unsigned char)UNKNOW_ENCODE_CHAR;
                        dest[pos + 1] = (unsigned char)(UNKNOW_ENCODE_CHAR>> 8);
                    }
                }
                else
                {
                    dest[pos] = (unsigned char) ucs2_table[index];
                    dest[pos + 1] = (unsigned char) (ucs2_table[index] >> 8);

                }
            }
            src += 2;
            pos += 2;
        }
        if (pos >= dest_size - 2)
        {
            break;
        }
    }
    dest[pos] = 0;
    dest[pos + 1] = 0;
    return pos + 2;
}

// ucs2 string to gb2312 string
unsigned short mmi_chset_ucs2_to_text_str_ex(
            unsigned char *dest,
            int dest_size,
            unsigned char *src
            )
{
    const key_index_t *key_LSB_index_table = g_chset_ucs2_to_gb2312_key_LSB_index;
    const unsigned char *key_MSB_table = g_chset_ucs2_to_gb2312_key_MSB;
    const unsigned short *encode_table = g_chset_ucs2_to_gb2312_table;
    unsigned char key_LSB;
    unsigned char key_MSB;
    int start;
    int end;
    int index;
    int pos = 0;

    while (*src || *(src + 1))
    {
        if (*src <= 0x7f && *(src + 1) == 0)
        {
            *(dest + pos) = *(src);
            pos++;
        }
        else
        {
            /*
             * The original conversion tables of mmi_chset_ucs2_to_text_str() seem to assume the src is big-endian,
             * but the src should be little-endian. We need to change the byte order.
             */
            key_LSB = (unsigned char) (*(src + 1));
            start = key_LSB_index_table[key_LSB].start;
            if (start < 0)
            {
                dest[pos] = (unsigned char) (UNKNOW_ENCODE_CHAR >> 8);
                dest[pos + 1] = (unsigned char) UNKNOW_ENCODE_CHAR;
            }
            else
            {
                key_MSB = (unsigned char) (*(src));
                end = key_LSB_index_table[key_LSB].end;
                if ((index = mmi_chset_binary_search(key_MSB, key_MSB_table, start, end)) < 0)  /* key MSB not found */
                {
                    dest[pos] = (unsigned char)(UNKNOW_ENCODE_CHAR >> 8);
                    dest[pos + 1] = (unsigned char)UNKNOW_ENCODE_CHAR;
                }
                else
                {
                    dest[pos] = (unsigned char) (encode_table[index] >> 8);
                    dest[pos + 1] = (unsigned char) encode_table[index];
                }

            }
            pos += 2; 
        }
        src += 2;

        if (pos >= dest_size - 2)
        {
            break;
        }
    }
    dest[pos] = 0;
    dest[pos + 1] = 0;
    return pos + 2;
}

