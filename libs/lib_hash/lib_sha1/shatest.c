/*
 *  shatest.c
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *****************************************************************************
 *  $Id: shatest.c 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This file will exercise the SHA1 class and perform the three
 *      tests documented in FIPS PUB 180-1.
 *
 *  Portability Issues:
 *      None.
 *
 */

#include <stdio.h>
#include <string.h>
#include "sha1.h"

int sha1_main_test()
{
    SHA1Context sha;
    int i;
	unsigned char data[16] = "123456789";

    SHA1Reset(&sha);
    SHA1Input(&sha, data, 9);

    if (!SHA1Result(&sha))
    {
        printf("calculate error\n");
    }
    else
    {
    	printf("data: %s\n", data);
		printf("sha1: ");
        for(i = 0; i < 5 ; i++)
        {
            printf("%X ", sha.Message_Digest[i]);
        }
		printf("\n");
    }

    return 0;
}
