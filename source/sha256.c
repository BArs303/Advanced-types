#include "sha256.h"

/* 
 * This file contains implementation of sha256 algorithm described in RFC 4634
 */

#define ROTATE_RIGHT(a, s) ((a) >> (s)  | (a) << (32 - (s)))

#define SHA256_SIGMA0(a)\
(ROTATE_RIGHT(a, 7) ^ ROTATE_RIGHT(a, 18) ^ ((a) >> 3))

#define SHA256_SIGMA1(a)\
(ROTATE_RIGHT(a, 17) ^ ROTATE_RIGHT(a, 19) ^ ((a) >> 10))




static SHA256_CTX* sha256_init_ctx(size_t length);
static void sha256_update(SHA256_CTX *ctx, unsigned int workspace[]);
static void sha256_transform(SHA256_CTX *ctx, unsigned int workspace[]);
static void print_sha256_table();

void sha256_hash(char *input, size_t length)
{
    SHA256_CTX *ctx;
    unsigned int workspace[SHA256_WORKSPACE_SIZE];
    unsigned int remainder;

    ctx = sha256_init_ctx(length);

    for(size_t p = 0; p < length / SHA256_INITIAL_BLOCK_SIZE; p += SHA256_INITIAL_BLOCK_SIZE)
    {
        /*printf("sha256 cycle\n");*/
        memcpy(ctx->block, input + p, SHA256_INITIAL_BLOCK_SIZE);
        sha256_update(ctx, workspace);
        sha256_transform(ctx, workspace);
    }

    remainder = length % SHA256_INITIAL_BLOCK_SIZE;
    /*printf("remainder %d\n", remainder);*/
    memset(ctx->block, 0, SHA256_INITIAL_BLOCK_SIZE);
    memcpy(ctx->block, input + (length - remainder), remainder);
    ctx->block[remainder] = 0x80;

    if(remainder >=  56)
    {
        sha256_update(ctx, workspace);
        sha256_transform(ctx, workspace);
        memset(ctx->block, 0, SHA256_INITIAL_BLOCK_SIZE);
    }

    ctx->block[56] = ctx->length[1] >> 24 & 0xFF;
    ctx->block[57] = ctx->length[1] >> 24 & 0xFF;
    ctx->block[58] = ctx->length[1] >> 16 & 0xFF;
    ctx->block[59] = ctx->length[1] >>  8 & 0xFF;

    ctx->block[60] = ctx->length[0] >> 24 & 0xFF;
    ctx->block[61] = ctx->length[0] >> 16 & 0xFF;
    ctx->block[62] = ctx->length[0] >>  8 & 0xFF;
    ctx->block[63] = ctx->length[0]       & 0xFF;
    sha256_update(ctx, workspace);
    sha256_transform(ctx, workspace);

    for(int i = 0; i < 8; i++)
    {
        printf("result %d %032b %X\n", i, ctx->state[i], ctx->state[i]);
    }

}

static SHA256_CTX* sha256_init_ctx(size_t length)
{
    SHA256_CTX *ctx;
    ctx = malloc(sizeof(SHA256_CTX));
    /* initial values for hash computing */
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;

    /* convert bytes to bits */
    ctx->length[0] = (length & 0xFFFFFFFF) << 3;
    ctx->length[1] = length >> 29;

    return ctx;
}

static void sha256_update(SHA256_CTX *ctx, unsigned int workspace[])
{
   /* copy initial block to buffer in reverse byte order*/
   for(int i = 0; i < 16; i++)
    {
        workspace[i]  = ctx->block[4*i]   << 24;
        workspace[i] += ctx->block[4*i+1] << 16; 
        workspace[i] += ctx->block[4*i+2] << 8;
        workspace[i] += ctx->block[4*i+3];
        /*printf("w%02d %032b\n", i, workspace[i]);*/
    }

    /* 
        expand the workspace to 64 words 
        based on the first 512 bits copied
        from the initial buffer 
    */

    for(int i = 0; i < 64 - 16; i++)
    {
        workspace[16+i] = workspace[i] + SHA256_SIGMA0(workspace[i+1]) + workspace[9+i] + SHA256_SIGMA1(workspace[14+i]);
        /*printf("w%d %032b\n", i+16, workspace[16+i]);*/
    }

}

static void sha256_transform(SHA256_CTX *ctx, unsigned int workspace[])
{
    unsigned int temp[3];
    unsigned int h[8];
    /* see print 256 table function for details */
    static const unsigned int k[64] = {
        0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 
        0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5, 
        0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 
        0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174, 
        0xE49B69C1, 0xEFBE4786, 0xFC19DC6, 0x240CA1CC, 
        0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA, 
        0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 
        0xC6E00BF3, 0xD5A79147, 0x6CA6351, 0x14292967, 
        0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 
        0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85, 
        0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 
        0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070, 
        0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 
        0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3, 
        0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 
        0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2 
    };

    memcpy(h, ctx->state, 4*8);

    for(int i = 0; i < SHA256_WORKSPACE_SIZE; i++)
    {
        temp[0] = 
            ROTATE_RIGHT(h[0], 2) ^
            ROTATE_RIGHT(h[0], 13) ^
            ROTATE_RIGHT(h[0], 22);

        temp[1] =
            ROTATE_RIGHT(h[4], 6) ^
            ROTATE_RIGHT(h[4], 11) ^
            ROTATE_RIGHT(h[4], 25);

        temp[2] = (h[0] & h[1]) ^ (h[0] & h[2]) ^ (h[1] & h[2]);
        temp[0] += temp[2];

        /* bitwise not (~) has higher priority than and (&) */
        temp[2] = (h[4] & h[5]) ^ (~h[4] & h[6]);
        temp[1] = temp[1] + h[7] + k[i] + workspace[i] + temp[2];

        h[7] = h[6];
        h[6] = h[5];
        h[5] = h[4];
        h[4] = h[3] + temp[1];
        h[3] = h[2];
        h[2] = h[1];
        h[1] = h[0];
        h[0] = temp[0] + temp[1];
    }

    ctx->state[0] += h[0];
    ctx->state[1] += h[1];
    ctx->state[2] += h[2];
    ctx->state[3] += h[3];
    ctx->state[4] += h[4];
    ctx->state[5] += h[5];
    ctx->state[6] += h[6];
    ctx->state[7] += h[7];
}

static void print_sha256_table()
{
    double num;
    unsigned long *bits;
    unsigned short exponent;
    int i, j;

    for(i = 2, j = 0; j < 64; i++)
    {
        if(is_prime(i))
        {
            num = cbrt(i);
            bits = (unsigned long*)&num;
            /* see IEEE 754 for details */
            exponent = ((*bits & 0x7FF0000000000000) >> 52) - 1023;
            *bits = *bits >> (20 - exponent) & 0xFFFFFFFF;
            /* bits contains first 32 bits from fraction part of cubic root of i */
            printf("0x%lX, ", *bits);
            j++;
            if(j % 4 == 0)
                printf("\n");
        }
    }
}
