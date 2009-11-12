#pragma once
#include "encoder.h"
#include "kdvencoderimpl.h"

class CJpegEncoder :
	public CEncoder
{
public:
	CJpegEncoder(void);
	virtual ~CJpegEncoder(void);

	/** 初始化. */
	virtual BOOL Init() ;

	/** 编码图像成jpeg数据 */
	virtual BOOL Encode( const TVideoPicture& tVidoPic, void *pStreamBuf, mu_uint32& nBufLen ) ;

	/** 编码后需要的内存空间大小。 */
	virtual mu_uint32 GetMemSize( const TVideoPicture& tVideoPic );

private:
	CKDVEncoderImpl m_cKdvEncoder;
};
