//���ܽ��ܺ���

#include "mcudefine.h"


#ifndef _Encrypt_h_
#define _Encrypt_h_


/** tea�����㷨��
*	����Կѭ�����ܡ�
*	��Կ���ȱ�����128bit(16���ֽ�)��
*	�����ܵ����ݳ��ȱ�����16�ֽڵı�����ֱ����ԭʼ���ݻ��������������ġ�
*/
int MU_DECLSPEC tean_enc(const char *k/*128 bit key*/, char *v/*data and buffer*/,int len/*data size,must be n*16*/);
int MU_DECLSPEC tean_dec(const char *k, char *v,int len);

int MU_DECLSPEC tean_enc(const unsigned char *key, char *v, int len );
int MU_DECLSPEC tean_dec(const unsigned char *key, char *v, int len );


// int tean_enc(const uint128& key, char *v, int len );
// int tean_dec(const uint128& key, char *v, int len );

#endif