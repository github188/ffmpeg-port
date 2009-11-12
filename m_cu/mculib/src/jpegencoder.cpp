
#include "jpegencoder.h"

CJpegEncoder::CJpegEncoder(void)
{
}

CJpegEncoder::~CJpegEncoder(void)
{
}

BOOL CJpegEncoder::Init()
{
	return this->m_cKdvEncoder.Create( CBaseCodec::CODEC_JPEG );
}

BOOL CJpegEncoder::Encode( const TVideoPicture& tVidoPic, void *pStreamBuf, mu_uint32& nBufLen )
{
	return m_cKdvEncoder.Encode( tVidoPic, pStreamBuf, nBufLen );
}

mu_uint32 CJpegEncoder::GetMemSize( const TVideoPicture& tVideoPic )
{
	return m_cKdvEncoder.GetMemSize( tVideoPic );
}