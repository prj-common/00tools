const int MOD_ADLER = 65521;

#if 0
uint64_t adler32(unsigned char *data, int32_t len) /* where data is the location of the data in physical memory and 
                                                       len is the length of the data in bytes */
{
    uint64_t a = 1, b = 0;
    int32_t index;
 
    /* Process each byte of the data in order */
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
 
    return (b << 16) | a;
}
#endif