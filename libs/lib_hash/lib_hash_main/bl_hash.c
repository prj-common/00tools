#include "bl_hash.h"
#include <lib_md5.h>

static int md5_init(PMD5CTX pCtx)
{
	MD5_Initialize(pCtx);
	return 0;
}

static int md5_reset(PMD5CTX pCtx)
{
	MD5_Reset(pCtx);
	return 0;
}

static int md5_update(PMD5CTX pCtx, const void* pData, unsigned int lNumOfBytes)
{
	MD5_Update(pCtx, pData, lNumOfBytes); 
	return 0;
}

static int bl_hash_md5_init(bl_hash_t *hash)
{
	bl_memset(hash, 0, sizeof(bl_hash_t));
	
}

int bl_hash_init(int type, bl_hash_t *hash)
{
	if(BL_HASH_MD5 == type)
	{
		bl_hash_md5_init(hash);
		return 0;
	}

	return -1;
}

int bl_hash_test(void)
{
	bl_hash_t hash;
	return 0;
}

