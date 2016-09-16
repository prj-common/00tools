int bl_utf8_to_cp936_str(unsigned char *cp936, unsigned char *utf8)
{
    int utf8_start = 0;
    int cp936_start = 0;
    int utf8_ch_size = 0;
    int cp936_ch_size = 0;
    unsigned short ucs2 = 0;
    unsigned short cp936_ch = 0;

    while(utf8[utf8_start])
    {
        utf8_ch_size = bl_utf8_to_ucs2_char(&ucs2, &utf8[utf8_start]);
        cp936_ch_size = bl_ucs2_to_cp936_char(&cp936_ch, ucs2);
        if(1 == cp936_ch_size)
        {
            cp936[cp936_start] = cp936_ch;
        }
        else
        {
            cp936[cp936_start] = cp936_ch>>8;
            cp936[cp936_start+1] = cp936_ch&0xff;
        }
        utf8_start += utf8_ch_size;
        cp936_start += cp936_ch_size;
    }

    return 0;
}

int bl_chcode_test()
{
    unsigned char utf8[256];
    unsigned char cp936[256];

    memset(cp936, 0, 256);
    bl_utf8_to_cp936_str(cp936, "\xd0\x90\xd0\x91\xd0\x92");
    printf("%s\n", cp936);
    
    return 0;
}

