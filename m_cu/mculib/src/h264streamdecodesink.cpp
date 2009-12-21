//#include "StdAfx.h"
#include "h264streamdecodesink.h"
#include "mcudefine.h"
#include "log.h"


CH264StreamDecodeSink::CH264StreamDecodeSink( UsageEnvironment& env, unsigned bufferSize, int nBandWidth )
: MediaSink( env )
{
	// 缓存码率.
	m_nBandWidth = nBandWidth;

	m_pBuffer = new unsigned char[ bufferSize ];
	memset( m_pBuffer, 0, bufferSize );
	const unsigned char start_code[4] = {0x00, 0x00, 0x00, 0x01};

	m_pData = m_pBuffer + sizeof( start_code );
	memcpy( m_pBuffer, start_code, sizeof( start_code ) );

	m_nDataBufferSize = bufferSize - sizeof( start_code );
	m_nDataLen = 0;

#ifdef USE_FFMPEG
	m_decoderId = CreateDecoder( CODEC_ID_H264 );
	if ( InvalidDecoder == m_decoderId )
	{
		// 出错了！
		Log() << _T( "创建H264解码器失败!" ) << endl;
		AfxMessageBox( _T( "无法创建H264解码器!" ) );
	}
#endif

	m_pDecoder = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if( m_pDecoder )
	{
		CDecoder::Release( m_pDecoder );
		m_pDecoder = NULL;
	}

	// 
	m_pDecoder = CDecoder::CreateDecoder( CDecoder::CODEC_H264, m_nBandWidth, MAIN_DECODER_NAME );
	if ( NULL == m_pDecoder )
	{
		// 出错了！
		Log() << _T( "Create H264 Decoder fail!!" ) << endl;
#ifdef _WIN32_WCE
		AfxMessageBox( _T( "无法创建H264解码器!" ) );
#endif
	}
//	_ASSERT( m_pDecoder );

	
}

CH264StreamDecodeSink::~CH264StreamDecodeSink(void)
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

CH264StreamDecodeSink *CH264StreamDecodeSink::CreateNew( UsageEnvironment& env, unsigned bufferSize, int nBandWidth )
{
	return new CH264StreamDecodeSink( env, bufferSize, nBandWidth );
}

void CH264StreamDecodeSink::afterGettingFrame(void* clientData, 
											  unsigned frameSize, 
											  unsigned numTruncatedBytes, 
											 timeval presentationTime, 
												unsigned durationInMicroseconds)
{
	CH264StreamDecodeSink *pThis = (CH264StreamDecodeSink*)clientData;
	if ( pThis )
	{
		pThis->afterGettingFrame1( frameSize, presentationTime );
	}
}

void CH264StreamDecodeSink::afterGettingFrame1(unsigned frameSize, timeval presentationTime)
{
//	if ( !log_file )
//	{
//		log_file = fopen(".\\lzxlog.txt", "w");
//	}
//	if ( log_file )
//	{
////		fprintf( log_file, "afterGettingFrame1\n" );
//	}
	// 处理数据。
	m_nDataLen = frameSize + ( m_pData - m_pBuffer );

	m_timeStamp = presentationTime;

	DecodeData();

	continuePlaying();
}

Boolean CH264StreamDecodeSink::continuePlaying()
{
	if (fSource == NULL) return False;

	fSource->getNextFrame( m_pData, m_nDataBufferSize,
		afterGettingFrame, this,
		onSourceClosure, this);

	return True;
}


Boolean CH264StreamDecodeSink::DecodeData()
{
	//char kkkk[ 4096 ] = {0};
	//memcpy( kkkk, m_pBuffer, m_nDataLen );
	//memset( kkkk, 0, m_nDataLen );

// 	static CFile file;
// 	file.Open( _T( "" ), CFile::modeCreate | CFile::modeReadWrite );
// 	file.Write( m_pData, m_nDataLen );

	if ( m_pDecoder )
	{
		CBaseCodec::TStreamPacket tPkt;
		tPkt.data = m_pBuffer;
		tPkt.datalen = m_nDataLen;
//		tPkt.timeStamp = m_timeStamp;
		// thinkingl
		// 暂时写在这里。
		tPkt.timeStamp = m_timeStamp.tv_sec * 1000000 + m_timeStamp.tv_usec;

	//	Log() << _T( "Receive video data!" ) << endl;

		m_pDecoder->InputStreamSync( tPkt );
	}

	memset( m_pData, 0, m_nDataLen );
	return true;
}

