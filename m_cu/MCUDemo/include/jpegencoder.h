#pragma once
#include "encoder.h"
#include "kdvencoderimpl.h"

class CJpegEncoder :
	public CEncoder
{
public:
	CJpegEncoder(void);
	virtual ~CJpegEncoder(void);

	/** ��ʼ��. */
	virtual BOOL Init() ;

	/** ����ͼ���jpeg���� */
	virtual BOOL Encode( const TVideoPicture& tVidoPic, void *pStreamBuf, mu_uint32& nBufLen ) ;

	/** �������Ҫ���ڴ�ռ��С�� */
	virtual mu_uint32 GetMemSize( const TVideoPicture& tVideoPic );

private:
	CKDVEncoderImpl m_cKdvEncoder;
};
