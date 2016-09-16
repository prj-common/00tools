#ifndef BL_DES_H
#define BL_DES_H

// Single DES Encryption for VNC
int bl_vnc_enc_8byte(unsigned char out_buf[8], 
	unsigned char in_buf[8], unsigned char key[8]);
int bl_vnc_dec_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8]);
int bl_vnc_enc_challenge(unsigned char enc_buf[16], 
	unsigned char challenge[16], unsigned char key[8]);

// Single DES Encrytpion in ECB mode
int bl_des_enc_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8]);
// Single DES Decrytpion in ECB mode
int bl_des_dec_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8]);
// Single DES Encryption in CBC mode
int bl_des_enc_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8], unsigned char iv[8]);
// Single DES Decryption in CBC mode
int bl_des_dec_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[8], unsigned char iv[8]);

// Triple DES Encryption in ECB mode
int bl_des3_enc_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24]);
// Triple DES Decryption in ECB mode
int bl_des3_dec_ecb_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24]);
// Triple DES Encryption in CBC mode
int bl_des3_enc_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24], unsigned char iv[8]);
// Triple DES Decryption in CBC mode
int bl_des3_dec_cbc_8byte(unsigned char out_buf[8],
	unsigned char in_buf[8], unsigned char key[24], unsigned char iv[8]);

#endif

