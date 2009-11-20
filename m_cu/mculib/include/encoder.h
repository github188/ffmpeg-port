#pragma once
#include "basecodec.h"
#include "scopelock.h"

class CEncoder :
	public CBaseCodec
{
protected:
	CEncoder(void);
	virtual ~CEncoder(void);

public:
	/** ����������. */
	static CEncoder *CreateEncoder( ECodecId eCodecId, LPCTSTR strName = NULL );
	/** �ͷ�. */
	static void Release( CEncoder *pDecoder );
	/** �������ֲ��ҽ�����. */
	static CEncoder *FindEncoder( LPCTSTR strName );
	/** �ͷ�����. */
	static void ReleaseAll();

	/** ��ʼ��. */
	virtual BOOL Init() = 0;

	/** ����ͼ��ɱ��������� */
	virtual BOOL Encode( const TVideoPicture& tVidoPic, void *pStreamBuf, mu_uint32& nBufLen ) = 0;

	/** �������Ҫ���ڴ�ռ��С�� */
	virtual mu_uint32 GetMemSize( const TVideoPicture& tVideoPic ) = 0;
	
protected:
	static void RegDec( LPCTSTR lpName, CEncoder *pEnc );
	static void UnRegDec( CEncoder * pEnc );

private:
	/** ����������-������ӳ���. */
	typedef std::map< tstring, CEncoder * > TEncoderNameTable;
	static TEncoderNameTable s_tEncoderNameTable;
	/** Ϊ����������ӳ��������֤�̰߳�ȫ����. */
	static CMCUMutex s_encoderNameTableLock;
};
