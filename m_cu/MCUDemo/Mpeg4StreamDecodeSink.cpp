#include "StdAfx.h"
#include "Mpeg4StreamDecodeSink.h"
#ifdef USE_FFMPEG
#include "avcodec.h"
#endif

CMpeg4StreamDecodeSink::CMpeg4StreamDecodeSink( UsageEnvironment& env, unsigned bufferSize, int nBandWidth )
:MediaSink( env )
{
	m_nBandWidth = nBandWidth;

	m_pBuffer = new unsigned char[ bufferSize ];
	memset( m_pBuffer, 0, bufferSize );

	m_pData = m_pBuffer ;
	m_nDataBufferSize = bufferSize;
	m_nDataLen = 0;

	m_pDecoder = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if ( m_pDecoder )
	{
		CDecoder::Release( m_pDecoder );
	}

	

	// 暂时写死时钟刻度。
	m_pDecoder = CDecoder::CreateDecoder( CDecoder::CODEC_MPEG4, m_nBandWidth, MAIN_DECODER_NAME );	
	if ( NULL == m_pDecoder )
	{
		// 出错了！
		mcu::tlog << _T( "创建Mpeg4解码器失败!" ) << endl;
		AfxMessageBox( _T( "无法创建MPEG4解码器!" ) );
	}
	_ASSERT( m_pDecoder );

#ifdef USE_FFMPEG
	m_decoderId = CreateDecoder( CODEC_ID_MPEG4 );
	if ( InvalidDecoder == m_decoderId )
	{
		// 出错了！
		mcu::tlog << _T( "创建Mpeg4解码器失败!" ) << endl;
		AfxMessageBox( _T( "无法创建MPEG4解码器!" ) );
	}
#endif
}

CMpeg4StreamDecodeSink::~CMpeg4StreamDecodeSink(void)
{
	if ( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = 0;
	}

	if ( m_pDecoder )
	{
		CDecoder::Release( m_pDecoder );
	}

#ifdef USE_FFMPEG
	if ( InvalidDecoder != m_decoderId )
	{
		DestroyDecoder( m_decoderId );
	}
#endif
}

CMpeg4StreamDecodeSink *CMpeg4StreamDecodeSink::CreateNew( UsageEnvironment& env, unsigned bufferSize, int nBandWidth )
{
	return new CMpeg4StreamDecodeSink( env, bufferSize, nBandWidth );
}

void CMpeg4StreamDecodeSink::afterGettingFrame(void* clientData, 
											  unsigned frameSize, 
											  unsigned numTruncatedBytes, 
											  timeval presentationTime, 
											  unsigned durationInMicroseconds)
{
	CMpeg4StreamDecodeSink *pThis = (CMpeg4StreamDecodeSink*)clientData;
	if ( pThis )
	{
		pThis->afterGettingFrame1( frameSize, presentationTime );
	}
}

void CMpeg4StreamDecodeSink::afterGettingFrame1(unsigned frameSize, timeval presentationTime)
{
	//if ( !log_file )
	//{
	//	log_file = fopen(".\\lzxlog.txt", "w");
	//}
	//if ( log_file )
	//{
	//	//		fprintf( log_file, "afterGettingFrame1\n" );
	//}
	// 处理数据。
	m_nDataLen = frameSize + ( m_pData - m_pBuffer );
	m_timeStamp = presentationTime;
	DecodeData();

	continuePlaying();
}

Boolean CMpeg4StreamDecodeSink::continuePlaying()
{
	if (fSource == NULL) return False;

	fSource->getNextFrame( m_pData, m_nDataBufferSize,
		afterGettingFrame, this,
		onSourceClosure, this);

	return True;
}


Boolean CMpeg4StreamDecodeSink::DecodeData()
{

	// 	static CFile file;
	// 	file.Open( _T( "" ), CFile::modeCreate | CFile::modeReadWrite );
	// 	file.Write( m_pData, m_nDataLen );

	if ( m_pDecoder )
	{
		CBaseCodec::TStreamPacket tPkt;
		tPkt.data = m_pBuffer;
		tPkt.datalen = m_nDataLen;
		tPkt.timeStamp = m_timeStamp;
		m_pDecoder->InputStreamSync( tPkt );
	}

#ifdef USE_FFMPEG
	if( InvalidDecoder == m_decoderId )
	{
		return FALSE;
	}
	InputData( m_decoderId, m_pBuffer, m_nDataLen );
#endif

	memset( m_pData, 0, m_nDataLen );

	

	return true;
}

