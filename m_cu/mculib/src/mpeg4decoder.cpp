
#include "mpeg4decoder.h"
#include "getconfig.h"

CMpeg4Decoder::CMpeg4Decoder(void)
{
	m_bRecording = FALSE;
	m_nBandWidth = 0;
	this->m_pKdvDecoder = NULL;
	m_bHaveIFrame = FALSE;
}

CMpeg4Decoder::~CMpeg4Decoder(void)
{
}

mu_int32 CMpeg4Decoder::InputStreamSync( const TStreamPacket& pkt )
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( NULL == m_pKdvDecoder )
	{
		Log() << _T( "CMpeg4Decoder::InputStreamSync decoder not create!" ) << endl;
		return 0;
	}

	int nBufLeft = pkt.datalen;
	int nReadLen = 0;
	int nFrameCount = 0;
	while ( nBufLeft > 0 )
	{
		int nSinkLen = m_cMediaStreamParse.InputStream( pkt.data + nReadLen, nBufLeft );

		void *pStreamPack;
		mu_uint32 nStreamPackLen;
		if (  m_cMediaStreamParse.GetCurFrame( &pStreamPack, &nStreamPackLen ) )
		{
			// 只有I帧来过，才解码。
			if ( !m_bHaveIFrame )
			{
				// 判断是否是I帧。
				BOOL bIsIFrame = IsIFrameMPG4( (u8*)pStreamPack, nStreamPackLen );
				if ( bIsIFrame )
				{
					m_bHaveIFrame = TRUE;
				}				
			}

			if ( m_bHaveIFrame )
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
			
		}

		nReadLen += nSinkLen;
		nBufLeft -= nSinkLen;
	}
	return nFrameCount;
}

BOOL CMpeg4Decoder::Init( int nBandWidth )
{
	SCOPE_LOCK( this->m_threadSafeLock );
	this->m_nBandWidth = nBandWidth;

	if ( NULL == m_pKdvDecoder )
	{
		m_pKdvDecoder = CKDVDecoderImpl::GetDecoder( CBaseCodec::CODEC_MPEG4, MAIN_KEDA_DECODER_NAME );
	}

	return ( NULL != m_pKdvDecoder );
}

BOOL CMpeg4Decoder::CapturePic(LPCTSTR strPicPath, CEncoder *pEnc)
{	
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( m_pKdvDecoder )
	{
		return m_pKdvDecoder->CapturePic( strPicPath, pEnc );
	}
	else
	{
		Log() << _T( "CMpeg4Decoder::CapturePic decoder not created!" ) << endl;
		return FALSE;
	}
}

BOOL CMpeg4Decoder::StartRecord( LPCTSTR strFileName )
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( m_bRecording )
	{
		return FALSE;
	}

	BOOL bResult = m_cFFRecoder.Init( CBaseCodec::CODEC_MPEG4, m_nBandWidth, strFileName );

	m_bRecording = bResult;

	return bResult;
}

BOOL CMpeg4Decoder::StopRecord()
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( !m_bRecording )
	{
		return FALSE;
	}

	BOOL bResult = m_cFFRecoder.CloseFile();

	m_bRecording = FALSE;

	return bResult;
}

BOOL CMpeg4Decoder::IsRecording() const
{
	SCOPE_LOCK( m_threadSafeLock );
	return m_bRecording;
}