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
	/** 创建解码器. */
	static CEncoder *CreateEncoder( ECodecId eCodecId, LPCTSTR strName = NULL );
	/** 释放. */
	static void Release( CEncoder *pDecoder );
	/** 根据名字查找解码器. */
	static CEncoder *FindEncoder( LPCTSTR strName );
	/** 释放所有. */
	static void ReleaseAll();

	/** 初始化. */
	virtual BOOL Init() = 0;

	/** 编码图像成编码后的数据 */
	virtual BOOL Encode( const TVideoPicture& tVidoPic, void *pStreamBuf, mu_uint32& nBufLen ) = 0;

	/** 编码后需要的内存空间大小。 */
	virtual mu_uint32 GetMemSize( const TVideoPicture& tVideoPic ) = 0;
	
protected:
	static void RegDec( LPCTSTR lpName, CEncoder *pEnc );
	static void UnRegDec( CEncoder * pEnc );

private:
	/** 解码器名称-解码器映射表. */
	typedef std::map< tstring, CEncoder * > TEncoderNameTable;
	static TEncoderNameTable s_tEncoderNameTable;
	/** 为解码器名称映射表加锁保证线程安全的锁. */
	static CMCUMutex s_encoderNameTableLock;
};
