#ifndef _MD5_
#define _MD5_
#include "utility.h"

typedef struct _md5_ctx MD5_CTX;

struct _md5_ctx
{
    unsigned int state[4];
    unsigned int bits_num[2];
};

void MD_Hash(char *input, size_t size);
#endif

