#include "md5.h"


#define MD5BufferSize 64

#define S11 7
#define S12 12
#define S13 17
#define S14 22

#define S21 5
#define S22 9
#define S23 14
#define S24 20

#define S31 4
#define S32 11
#define S33 16
#define S34 23

#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* MD5 auxiliary functions */
#define F(X, Y, Z) ((X) & (Y) | ~(X) & (Z))
#define G(X, Y, Z) ((X) & (Z) | (Y) & ~(Z))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ (X | ~(Z)))

#define ROTATE_LEFT(a, s) ((a) << (s) | (a) >> (32 - (s)))
/* Round 1 transformation */
#define FF(a, b, c, d, x, s, t) \
(a) += F(b, c, d) + (x) + (t); \
(a) = ROTATE_LEFT((a), (s)); \
(a) += (b);

/* Round 2 transformation */
#define GG(a, b, c, d, x, s, t) \
(a) += G(b, c, d) + (x) + (t); \
(a) = ROTATE_LEFT((a), (s)); \
(a) += (b);

/* Round 3 transformation */
#define HH(a, b, c, d, x, s, t) \
(a) += H(b, c, d) + (x) + (t); \
(a) = ROTATE_LEFT((a), (s)); \
(a) += (b);

/* Round 4 transformation */
#define II(a, b, c, d, x, s, t) \
(a) += I(b, c, d) + (x) + (t); \
(a) = ROTATE_LEFT((a), (s)); \
(a) += (b);

static void MD5Transform(unsigned int state[4], unsigned int x[16]);
static MD5_CTX* MD5Init(size_t length);

void foo(char *str, size_t length)
{
    MD5_CTX *ctx;
    unsigned char buffer[MD5BufferSize];
    unsigned int *x;
    unsigned int remainder;
    
    ctx = MD5Init(length);
    x = (unsigned int*)buffer;

    for(size_t i = 0; i < length / MD5BufferSize; i += MD5BufferSize)
    {
        memcpy(buffer, str + i, MD5BufferSize);
        MD5Transform(ctx->state, x);
    }

    memset(buffer, 0, MD5BufferSize);
    remainder = length % MD5BufferSize;
    memcpy(buffer, str, remainder);
    buffer[remainder] = 0x80;

    if(remainder >= 56)
    {
        MD5Transform(ctx->state, x);
        memset(buffer, 0, MD5BufferSize);
    }

    x[14] = ctx->bits_num[0];
    x[15] = ctx->bits_num[1];
    MD5Transform(ctx->state, x);

    char *result;
    result = (char*)ctx->state;
    for(int i = 0; i < 16; i++)
    {
        printf("%hhX", result[i]);
    }
    printf("\n");

}

static MD5_CTX* MD5Init(size_t length)
{
    MD5_CTX *result;
    result = malloc(sizeof(MD5_CTX));
    /* MD5 initial values */
    result->state[0] = 0x67452301;
    result->state[1] = 0xEFCDAB89;
    result->state[2] = 0x98BADCFE;
    result->state[3] = 0x10325476;
    /* 
     * Convert bytes to bits 
     * MD5 uses only last 64 bits from the actual number of bits
     * They are stored as two 4 bytes words 
     */
    result->bits_num[0] = (length & 0xFFFFFFFF) << 3;
    result->bits_num[1] = (length >> 29 & 0xFFFFFFFF);
    return result;
}

static void MD5Transform(unsigned int state[4], unsigned int x[16])
{
    unsigned int a, b, c, d;

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];

    /* 
        Last values in each function calculated as integer part of abs(sin(i)) * (1<<31)
        where i in radians [1, 64]
     */

    /* Round 1 */
    FF(a, b, c, d, x[0], S11, 0xD76AA478)
    FF(d, a, b, c, x[1], S12, 0xE8C7B756)
    FF(c, d, a, b, x[2], S13, 0x242070DB)
    FF(b, c, d, a, x[3], S14, 0xC1BDCEEE)

    FF(a, b, c, d, x[4], S11, 0xF57C0FAF)
    FF(d, a, b, c, x[5], S12, 0x4787C62A)
    FF(c, d, a, b, x[6], S13, 0xA8304613)
    FF(b, c, d, a, x[7], S14, 0xFD469501)

    FF(a, b, c, d, x[ 8], S11, 0x698098D8)
    FF(d, a, b, c, x[ 9], S12, 0x8B44F7AF)
    FF(c, d, a, b, x[10], S13, 0xFFFF5BB1)
    FF(b, c, d, a, x[11], S14, 0x895CD7BE)

    FF(a, b, c, d, x[12], S11, 0x6B901122)
    FF(d, a, b, c, x[13], S12, 0xFD987193)
    FF(c, d, a, b, x[14], S13, 0xA679438E)
    FF(b, c, d, a, x[15], S14, 0x49B40821)

    /* Round 2 */
    GG(a, b, c, d, x[ 1], S21, 0xF61E2562)
    GG(d, a, b, c, x[ 6], S22, 0xC040B340)
    GG(c, d, a, b, x[11], S23, 0x265E5A51)
    GG(b, c, d, a, x[ 0], S24, 0xE9B6C7AA)

    GG(a, b, c, d, x[ 5], S21, 0xD62F105D)
    GG(d, a, b, c, x[10], S22,  0x2441453)
    GG(c, d, a, b, x[15], S23, 0xD8A1E681)
    GG(b, c, d, a, x[ 4], S24, 0xE7D3FBC8)

    GG(a, b, c, d, x[ 9], S21, 0x21E1CDE6)
    GG(d, a, b, c, x[14], S22, 0xC33707D6)
    GG(c, d, a, b, x[ 3], S23, 0xF4D50D87)
    GG(b, c, d, a, x[ 8], S24, 0x455A14ED)

    GG(a, b, c, d, x[13], S21, 0xA9E3E905)
    GG(d, a, b, c, x[ 2], S22, 0xFCEFA3F8)
    GG(c, d, a, b, x[ 7], S23, 0x676F02D9)
    GG(b, c, d, a, x[12], S24, 0x8D2A4C8A)

    /* Round 3 */
    HH(a, b, c, d, x[ 5], S31, 0xFFFA3942)
    HH(d, a, b, c, x[ 8], S32, 0x8771F681)
    HH(c, d, a, b, x[11], S33, 0x6D9D6122)
    HH(b, c, d, a, x[14], S34, 0xFDE5380C)

    HH(a, b, c, d, x[ 1], S31, 0xA4BEEA44)
    HH(d, a, b, c, x[ 4], S32, 0x4BDECFA9)
    HH(c, d, a, b, x[ 7], S33, 0xF6BB4B60)
    HH(b, c, d, a, x[10], S34, 0xBEBFBC70)

    HH(a, b, c, d, x[13], S31, 0x289B7EC6)
    HH(d, a, b, c, x[ 0], S32, 0xEAA127FA)
    HH(c, d, a, b, x[ 3], S33, 0xD4EF3085)
    HH(b, c, d, a, x[ 6], S34, 0x4881D05)

    HH(a, b, c, d, x[ 9], S31, 0xD9D4D039)
    HH(d, a, b, c, x[12], S32, 0xE6DB99E5)
    HH(c, d, a, b, x[15], S33, 0x1FA27CF8)
    HH(b, c, d, a, x[ 2], S34, 0xC4AC5665)

    /* Round 4 */
    II(a, b, c, d, x[ 0], S41, 0xF4292244)
    II(d, a, b, c, x[ 7], S42, 0x432AFF97)
    II(c, d, a, b, x[14], S43, 0xAB9423A7)
    II(b, c, d, a, x[ 5], S44, 0xFC93A039)

    II(a, b, c, d, x[12], S41, 0x655B59C3)
    II(d, a, b, c, x[ 3], S42, 0x8F0CCC92)
    II(c, d, a, b, x[10], S43, 0xFFEFF47D)
    II(b, c, d, a, x[ 1], S44, 0x85845DD1)

    II(a, b, c, d, x[ 8], S41, 0x6FA87E4F)
    II(d, a, b, c, x[15], S42, 0xFE2CE6E0)
    II(c, d, a, b, x[ 6], S43, 0xA3014314)
    II(b, c, d, a, x[13], S44, 0x4E0811A1)

    II(a, b, c, d, x[ 4], S41, 0xF7537E82)
    II(d, a, b, c, x[11], S42, 0xBD3AF235)
    II(c, d, a, b, x[ 2], S43, 0x2AD7D2BB)
    II(b, c, d, a, x[ 9], S44, 0xEB86D391)

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}
