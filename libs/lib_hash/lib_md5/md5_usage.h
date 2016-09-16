#ifndef  __MD5_USAGE_H__
#define  __MD5_USAGE_H__

#define  MD5_DIGEST_SIZE  16

// calculate file md5 digest
int libmd5_file(char digest[MD5_DIGEST_SIZE], char *file_name);

#endif

