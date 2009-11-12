
#include "h264decoder.h"

CH264Decoder::CH264Decoder(void)
{
	m_bRecording = FALSE;
	m_nBandWidth = 0;
	this->m_pKdvDecoder = NULL;
}

CH264Decoder::~CH264Decoder(void)
{
}

mu_int32 CH264Decoder::InputStreamSync( const TStreamPacket& pkt )
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( NULL == m_pKdvDecoder )
	{
		mcu::tlog << _T( "CH264Decoder::InputStreamSync decoder not create!" ) << endl;
		return 0;
	}

//#define TIMESTAMP_DEBUG
#ifdef TIMESTAMP_DEBUG
	mcu::tlog << _T( "timestamp: s:" ) << pkt.timeStamp.tv_sec << _T( "u: " )<< pkt.timeStamp.tv_usec << endl;
#endif

	int nBufLeft = pkt.datalen;
	int nReadLen = 0;
	int nFrameCount = 0;
	while ( nBufLeft > 0 )
	{
		int nSinkLen = m_cH264StreamParse.InputStream( pkt.data + nReadLen, nBufLeft );

		void *pStreamPack;
		mu_uint32 nStreamPackLen;
		if ( m_cH264StreamParse.GetCurFrame( &pStreamPack, &nStreamPackLen ) )
		{
			// 解码.			
			TVideoFrameInfo tFrameInfo;
			TVideoPicture tPic;
			
			TStreamPacket tFramePacket;
			tFramePacket.data = (mu_uint8*)pStreamPack;
			tFramePacket.datalen = nStreamPackLen;
			tFramePacket.timeStamp = pkt.timeStamp;
			BOOL bDecodeResult = m_pKdvDecoder->Decode( tFramePacket, tPic, &tFrameInfo );
	
			// 录像。
			if ( bDecodeResult && m_bRecording )
			{							
				BOOL bWriteSuc = this->m_cFFRecoder.WriteFrame( (mu_uint8 *)pStreamPack, nStreamPackLen, tFrameInfo );				
			}

			nFrameCount ++;

			// 调用基类处理.
			this->OnDecodeResult( bDecodeResult, &tPic, &tFrameInfo );
		}

		nReadLen += nSinkLen;
		nBufLeft -= nSinkLen;
	}
	return nFrameCount;
}

BOOL CH264Decoder::Init( int nBandWidth )
{
	SCOPE_LOCK( this->m_threadSafeLock );

	this->m_nBandWidth = nBandWidth;

	if ( NULL == m_pKdvDecoder )
	{
		m_pKdvDecoder = CKDVDecoderImpl::GetDecoder( CBaseCodec::CODEC_H264, MAIN_KEDA_DECODER_NAME );
	}

	return ( NULL != m_pKdvDecoder );
}

BOOL CH264Decoder::CapturePic(LPCTSTR strPicPath, CEncoder *pEnc)
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( m_pKdvDecoder )
	{
		return m_pKdvDecoder->CapturePic( strPicPath, pEnc );
	}
	else
	{
		mcu::tlog << _T( "CH264Decoder::CapturePic decoder not created!" ) << endl;
		return FALSE;
	}
}

BOOL CH264Decoder::StartRecord( LPCTSTR strFileName )
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( m_bRecording )
	{
		return FALSE;
	}
	
	BOOL bResult = m_cFFRecoder.Init( CBaseCodec::CODEC_H264, m_nBandWidth, strFileName );
	
	m_bRecording = bResult;

	return bResult;
}

BOOL CH264Decoder::StopRecord()
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( !m_bRecording )
	{
		return FALSE;
	}

	m_bRecording = FALSE;

	BOOL bResult = m_cFFRecoder.CloseFile();
//	_ASSERT( bResult );

	return bResult;
}

BOOL CH264Decoder::IsRecording() const
{
	SCOPE_LOCK( m_threadSafeLock );

	return m_bRecording;
}