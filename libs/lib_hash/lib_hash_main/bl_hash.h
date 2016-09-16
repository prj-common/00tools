#ifndef __BL_HASH_H__
#define __BL_HASH_H__

#define BL_DIGEST_BYTE_MAX_LEN  128
#define BL_HASH_CNTXT_MAX_LEN   128

struct bl_hash_type;
typedef int (*hash_init_fun)(struct bl_hash_type *hash);
typedef int (*hash_update_fun)(struct bl_hash_type *hash, unsigned char *data, int size);
typedef int (*hash_final_fun)(struct bl_hash_type *hash);
typedef struct bl_hash_type
{
	hash_init_fun hash_init;
	hash_update_fun hash_update;
	hash_final_fun hash_final;

	unsigned char digest[BL_DIGEST_BYTE_MAX_LEN];
	int digest_len;
	unsigned char hash_cntxt[BL_HASH_CNTXT_MAX_LEN];
}bl_hash_t;

#define BL_HASH_MD5 0

#endif

