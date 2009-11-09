#pragma once

#include "basecodec.h"
#include "ScopeLock.h"
#include "mcudefine.h"

/** keda������.
*	��װkeda��������ʵ��.
*	��������������ֶ�keda����������ļ�������. */

extern "C" 
{ 
	typedef struct TVideoUnitEncParam TVideoUnitEncParam; 
}

class CKDVEncoderImpl
{
public:
	CKDVEncoderImpl(void);
	virtual ~CKDVEncoderImpl(void);


	/** ����. */
	BOOL Create( CBaseCodec::ECodecId eCodec );

	/** ����. */
	BOOL Release();

	/** ����һ֡. 
	*	����ͼ��,�������������. 
	*	
	*/
	BOOL Encode( const CBaseCodec::TVideoPicture& tPicture, void *pStramBuf, mu_uint32 & nBufLen );


	/** �������Ҫ���ڴ�ռ��С�� */
	virtual mu_uint32 GetMemSize( const CBaseCodec::TVideoPicture& tVideoPic );
private:
	void PrintErrorInfo(void * pvHandle, mu_int32 l32ResultForAnalyze);
private:
	TVideoUnitEncParam *m_pEncParam;

	/** ���������. */
	void * m_pvHandle;

	/** �̰߳�ȫ. 
	*	keda�����ӿ�ֻ�ܵ��̷߳���.
	*/
	CSDLMutex m_threadSafeLock;

	/** �ڲ�������.�洢���������� */
	mu_uint8 *m_pDataBuf;
	/** ��������С�� */
	mu_uint32 m_nDataBufLen;

	/** ͼƬ�������� */
	mu_uint8 *m_pPicBuf;
	/** ͼƬ��������С��*/
	mu_uint32 m_nPicBufLen;
};
