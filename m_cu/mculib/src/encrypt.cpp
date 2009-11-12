

#include "encrypt.h"

#define ROUNDS 16
#define DELTA 0x9e3779b9 /* sqr(5)-1 * 2^31 */

static void tean(unsigned int *k, unsigned int *v, long N);

/**********************************************************
Input values: 	k[4]	128-bit key
v[2]    64-bit plaintext block
Output values:	v[2]    64-bit ciphertext block 
**********************************************************/

void tean(unsigned int *k, unsigned int *v, long N)
{
	unsigned int y=v[0], z=v[1];
	unsigned int limit,sum=0;

	if(N>0)	/* ENCRYPT */		
	{
		limit=DELTA*N;

		while(sum!=limit)
		{
			y+=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];
			sum+=DELTA;
			z+=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];
		}
	}
	else	/* DECRYPT */
	{
		sum=DELTA*(-N);

		while(sum)
		{
			z-=((y<<4)^(y>>5)) + (y^sum) + k[(sum>>11)&3];
			sum-=DELTA;
			y-=((z<<4)^(z>>5)) + (z^sum) + k[sum&3];
		}
	}
	v[0]=y; v[1]=z;
}

//int tean_enc(const uint128& key, char *v, int len )
//{
//	const char *k = (const char*) key.GetData() ;
//	return tean_enc( k, v, len );
//}

int tean_enc(const char *k, char *v,int len)
{
	if(k==0||v==0||len<=0)return 1;
	if((len%16)!=0)return 2;

	for(len-=16;len>0;len-=16)	//从后往前循环加密
	{
		tean((unsigned int*)(v+len-16),(unsigned int*)(v+len),ROUNDS);
		tean((unsigned int*)(v+len-16),(unsigned int*)(v+len+8),ROUNDS);
	}

	//加密最前端的16字节
	tean((unsigned int*)k,(unsigned int*)v,ROUNDS);
	tean((unsigned int*)k,(unsigned int*)(v+8),ROUNDS);

	return 0;
}

int tean_enc(const unsigned char *k, char *v, int len )
{
	return tean_enc( (const char *) k, v, len );
}

int tean_dec(const unsigned char *k, char *v,int len)
{
	return tean_dec( (const char *)k, v, len );
}

//int tean_dec(const uint128& key, char *v, int len )
//{
//	return tean_dec( key.GetData(), v, len );
//}

int tean_dec(const char *k, char *v,int len)
{

	if(k==0||v==0||len<=0)return 1;
	if((len%16)!=0)return 2;
	
	//解密最前端的16字节单元
	tean((unsigned int*)k,(unsigned int*)v,-ROUNDS);
	tean((unsigned int*)k,(unsigned int*)(v+8),-ROUNDS);
	len-=16;
	
	for(;len>0;v+=16,len-=16)	//从前往后循环解密
	{
		tean((unsigned int*)v,(unsigned int*)(v+16),-ROUNDS);
		tean((unsigned int*)v,(unsigned int*)(v+16+8),-ROUNDS);
	}
	
	return 0;
}
