//加密解密函数

#include "mcudefine.h"


#ifndef _Encrypt_h_
#define _Encrypt_h_


/** tea加密算法。
*	用密钥循环加密。
*	密钥长度必须是128bit(16个字节)。
*	被加密的数据长度必须是16字节的倍数。直接在原始数据缓冲区上生成密文。
*/
int MU_DECLSPEC tean_enc(const char *k/*128 bit key*/, char *v/*data and buffer*/,int len/*data size,must be n*16*/);
int MU_DECLSPEC tean_dec(const char *k, char *v,int len);

int MU_DECLSPEC tean_enc(const unsigned char *key, char *v, int len );
int MU_DECLSPEC tean_dec(const unsigned char *key, char *v, int len );


// int tean_enc(const uint128& key, char *v, int len );
// int tean_dec(const uint128& key, char *v, int len );

#endif