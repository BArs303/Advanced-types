#ifndef _SHA256_
#define _SHA256_

#include "utility.h"
#include <math.h>

#define SHA256_INITIAL_BLOCK_SIZE 64
#define SHA256_WORKSPACE_SIZE 64

struct _sha256_ctx_
{
    unsigned char block[SHA256_INITIAL_BLOCK_SIZE];
    unsigned int state[8];
    unsigned int length[2];
};

typedef struct _sha256_ctx_ SHA256_CTX;

void sha256_hash(char *input, size_t length);

#endif
