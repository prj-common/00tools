#ifndef __BASE64__
#define __BASE64__
/*
 * Encode a buffer into base64 format
 */
int base64_encode( unsigned char *dst, int *dlen, 
	const unsigned char *src, int slen );

/*
 * Decode a base64-formatted buffer
 */
int base64_decode( unsigned char *dst, int *dlen,
                   const unsigned char *src, int slen );
#endif