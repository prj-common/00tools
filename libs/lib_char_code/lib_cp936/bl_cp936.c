#include "bl_cp936.h"
#include "bl_cp936_tab.h"
#include <stdio.h>
#include <stdlib.h>

int bl_make_ucs2_to_cp936_tab()
{
    bl_cp936_to_ucs2_t *tmp = NULL;
    bl_cp936_to_ucs2_t t;
    int i = 0;
    int pos = 0;
    int n = sizeof(bl_cp936_to_ucs2_tab)/sizeof(bl_cp936_to_ucs2_t);
    
    tmp = malloc(sizeof(bl_cp936_to_ucs2_tab));
    tmp[0] = bl_cp936_to_ucs2_tab[0];
    for(pos = 1; pos < n; pos++)
    {
        t = bl_cp936_to_ucs2_tab[pos];
        for(i = pos-1; (i >= 0)&&(tmp[i].code_ucs2 > t.code_ucs2); i--)
        {
            tmp[i+1] = tmp[i];
        }
        tmp[i+1] = t;
    }
    for(i=0; i<n; i++)
    {
        printf("/* %5d */", i);
        printf(" {0x%04X, 0x%04X},", tmp[i].code_cp936, tmp[i].code_ucs2);
        if(i>0 && (tmp[i].code_ucs2 <= tmp[i-1].code_ucs2))
        {
            printf("/*error!*/\n");
        }
        else
        {
            printf("\n");
        }
    }

    free(tmp);

	return 0;
}

int bl_cp936_to_ucs2_char(unsigned short *ucs2, unsigned short cp936)
{
    int low = 0;
    int high = sizeof(bl_cp936_to_ucs2_tab)/sizeof(bl_cp936_to_ucs2_t)-1;
    int mid = 0;
    int mid_val = 0;

    while(low <= high)
    {
        mid = (low+high)/2;
        mid_val = bl_cp936_to_ucs2_tab[mid].code_cp936;
        if(mid_val < cp936)
        {
            low = mid+1;
        }
        else if(mid_val > cp936)
        {
            high = mid-1;
        }
        else
        {
            *ucs2 = bl_cp936_to_ucs2_tab[mid].code_ucs2;
            return 0;
        }
    }
    *ucs2 = -1;
    return -1;
}

int bl_ucs2_to_cp936_char(unsigned short *cp936, unsigned short ucs2)
{
    int low = 0;
    int high = sizeof(bl_ucs2_to_cp936_tab)/sizeof(bl_cp936_to_ucs2_t)-1;
    int mid = 0;
    int mid_val = 0;

    while(low <= high)
    {
        mid = (low+high)/2;
        mid_val = bl_ucs2_to_cp936_tab[mid].code_ucs2;
        if(mid_val < ucs2)
        {
            low = mid+1;
        }
        else if(mid_val > ucs2)
        {
            high = mid-1;
        }
        else
        {
            *cp936 = bl_ucs2_to_cp936_tab[mid].code_cp936;
            if(*cp936 < 0x80)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }
    *cp936 = -1;
    return -1;
}

int cp936_test()
{
    unsigned short ucs2 = 0x0410;
    unsigned short cp936 = 0;
    
	//bl_make_ucs2_to_cp936_tab();
    bl_ucs2_to_cp936_char(&cp936, ucs2);
    printf("%x\n", cp936);
    return 0;
}

