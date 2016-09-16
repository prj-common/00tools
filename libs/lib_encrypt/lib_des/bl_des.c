#include "des.h"
#include <stdio.h>
#include <assert.h>

/*
 * DES and 3DES test vectors from:
 *
 * http://csrc.nist.gov/groups/STM/cavp/documents/des/tripledes-vectors.zip
 */

// bit reversal of byte
#define BIT_FLIP_BYTE(x) do{        \
    x = (x&0x55)<<1 | (x&0xaa)>>1;  \
    x = (x&0x33)<<2 | (x&0xcc)>>2;  \
    x = (x&0x0f)<<4 | (x&0xf0)>>4;  \
}while(0)

// bit reversal of 2byte
#define BIT_FLIP_2BYTE(x) do{                    \
    x = (x & 0x5555) <<  1 | (x & 0xAAAA) >>  1; \
    x = (x & 0x3333) <<  2 | (x & 0xCCCC) >>  2; \
    x = (x & 0x0F0F) <<  4 | (x & 0xF0F0) >>  4; \
    x = (x & 0x00FF) <<  8 | (x & 0xFF00) >>  8; \
}while(0)

// bit reversal of 4byte
#define BIT_FLIP_4BYTE(x) do{                            \
    x = (x & 0x55555555) <<  1 | (x & 0xAAAAAAAA) >>  1; \
    x = (x & 0x33333333) <<  2 | (x & 0xCCCCCCCC) >>  2; \
    x = (x & 0x0F0F0F0F) <<  4 | (x & 0xF0F0F0F0) >>  4; \
    x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8; \
    x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16; \
}while(0)

#if 0
void bit_test(void)
{
	unsigned int i = 0x12345678;
	unsigned short s = 0x1234;
	unsigned char c = 0x12;
    printf("bit test\n");

	// byte
	printf("number: %02x\n", c);
	print_byte(c);
	BIT_FLIP_BYTE(c);
	printf("after reversal\n");
	print_byte(c);

	// short
	printf("number: %04x\n", s);
	print_2byte(s);
	BIT_FLIP_2BYTE(s);
	printf("after reversal\n");
	print_2byte(s);
	
	// int
	printf("number: %08x\n", i);
	print_4byte(i);
	BIT_FLIP_4BYTE(i);
	printf("after reversal\n");
	print_4byte(i);	
}
#endif

// Single DES Encryption for VNC
int bl_vnc_enc_8byte(unsigned char out_buf[8], 
	unsigned char in_buf[8], unsigned char key[8])
{
	int i;
	des_context ctx;
	
    for(i=0; i<8; i++)
    {
        BIT_FLIP_BYTE(key[i]);
    }
	
    des_setkey_enc( &ctx, (unsigned char *) key);
    des_crypt_ecb( &ctx, in_buf, out_buf);
	
	return 0;
}

int bl_vnc_dec_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8])
{
	int i;
	des_context ctx;
	
    for(i=0; i<8; i++)
    {
        BIT_FLIP_BYTE(key[i]);
    }
	
    des_setkey_dec( &ctx, (unsigned char *) key);
    des_crypt_ecb( &ctx, in_buf, out_buf);

	return 0;
}


int bl_vnc_enc_challenge(unsigned char enc_buf[16], 
	unsigned char challenge[16], unsigned char key[8])
{
	int i;
    des_context ctx;

	bl_vnc_enc_8byte(enc_buf, challenge, key);
	bl_vnc_enc_8byte(&enc_buf[8], &challenge[8], key);

    return 0;
}

// Single DES Encrytpion in ECB mode
int bl_des_enc_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8])
{
    des_context ctx;
	
    des_setkey_enc( &ctx, (unsigned char *) key);
    des_crypt_ecb( &ctx, in_buf, out_buf);
	
	return 0;
}

// Single DES Decrytpion in ECB mode
int bl_des_dec_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8])
{
    des_context ctx;

    des_setkey_dec(&ctx, (unsigned char *) key);
    des_crypt_ecb(&ctx, in_buf, out_buf);

	return 0;
}

// Single DES Encryption in CBC mode
int bl_des_enc_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8], unsigned char iv[8])
{
    des_context ctx;
	
    des_setkey_enc( &ctx, (unsigned char *) key );
    des_crypt_cbc( &ctx, DES_ENCRYPT, 8, iv, in_buf, out_buf );

	return 0;
}

// Single DES Decryption in CBC mode
int bl_des_dec_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8], unsigned char iv[8])
{
    des_context ctx;
	
    des_setkey_dec( &ctx, (unsigned char *) key );
    des_crypt_cbc( &ctx, DES_DECRYPT, 8, iv, in_buf, out_buf );

	return 0;
}

// Triple DES Encryption in ECB mode
int bl_des3_enc_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24])
{
    des3_context ctx;

	des3_set3key_enc(&ctx, (unsigned char *)key);
    des3_crypt_ecb( &ctx, in_buf, out_buf );

	return 0;
}

// Triple DES Decryption in ECB mode
int bl_des3_dec_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24])
{
    des3_context ctx;

	des3_set3key_dec(&ctx, (unsigned char *)key);
    des3_crypt_ecb( &ctx, in_buf, out_buf );

	return 0;
}

// Triple DES Encryption in CBC mode
int bl_des3_enc_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24], unsigned char iv[8])
{
    des3_context ctx;

	des3_set3key_enc(&ctx, (unsigned char *)key);
    des3_crypt_cbc( &ctx, DES_ENCRYPT, 8, iv, in_buf, out_buf );

	return 0;
}

// Triple DES Decryption in CBC mode
int bl_des3_dec_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24], unsigned char iv[8])
{
    des3_context ctx;

	des3_set3key_dec(&ctx, (unsigned char *)key);
    des3_crypt_cbc( &ctx, DES_DECRYPT, 8, iv, in_buf, out_buf );

	return 0;
}

