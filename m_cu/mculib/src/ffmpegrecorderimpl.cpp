#include "ffmpegrecorderimpl.h"

#include "avformat.h"
#include "avstring.h"
#include "mcucommon.h"
#include "mem.h"
#include "getconfig.h"
#include "log.h"

extern "C"
{
	static int mov_write_header(AVFormatContext *s);

	extern AVOutputFormat tgp_muxer;

	extern URLProtocol file_protocol;

	extern AVInputFormat mov_demuxer;

	void av_freep(void *ptr);
};

// 默认没有注册过。
BOOL CFFMpegRecorderImpl::s_bFileProtocolReg = FALSE;

CFFMpegRecorderImpl::CFFMpegRecorderImpl(void)
{
#if !defined( __SYMBIAN32__ )
	m_pavFormatContext = NULL;
	m_pavStream = NULL;
	
#endif

	this->m_timeBegin = 0;
	this->m_timeEnd = 0;
	this->m_nDataSize = 0;
	this->m_nFrameCount = 0;
	this->m_nBandWidth = 0;
}

CFFMpegRecorderImpl::~CFFMpegRecorderImpl(void)
{
	this->Release();
}

BOOL CFFMpegRecorderImpl::Init( CBaseCodec::ECodecId eCodec, int nBandWidth, LPCTSTR strFileName )
{
#if !defined( __SYMBIAN32__ )
	
	SCOPE_LOCK( this->m_threadSafeLock );

	BOOL bResult = TRUE;



	this->m_nBandWidth = nBandWidth;
	this->m_strRecordFilePath = strFileName;

	Log() << _T( "codec: " ) << eCodec << "bandwidth: " << nBandWidth << "file: " << strFileName << endl;

	// 注册文件读写协议。
	if ( !s_bFileProtocolReg )
	{
		av_register_protocol( &file_protocol );
		s_bFileProtocolReg = TRUE;
	}
	

	m_pavFormatContext = avformat_alloc_context();

	if ( NULL == m_pavFormatContext )
	{
		Log() << "m_pavFormatContext = avformat_alloc_context()" << endl;
	}

	bResult &= ( NULL != m_pavFormatContext );

	// 3gp的格式输出。
	if ( m_pavFormatContext )
	{
		m_pavFormatContext->oformat = &tgp_muxer;

		char szFileName[1024] = {0};
#ifdef _UNICODE
		WCtoMB( strFileName, szFileName, 1024 );
#else
		strcpy( szFileName, strFileName );
#endif

		// 存储
		av_strlcpy( m_pavFormatContext->filename, szFileName, 1024 );

		// 打开目标文件。
		bResult &= (url_fopen(&m_pavFormatContext->pb, szFileName, URL_WRONLY) == 0);

		if ( !bResult )
		{
			Log() << "create record file fail! file:" << szFileName << endl;
		}

		m_pavStream = av_new_stream( m_pavFormatContext, 0 ); 

		if ( m_pavStream )
		{
			CodecID ffCodecId = (CodecID)this->GetFFCodecId( eCodec );
			m_pavStream->codec->codec_id = ffCodecId;
			m_pavStream->codec->codec_type = CODEC_TYPE_VIDEO;
		}
		else
		{
			Log() << "m_pavStream = av_new_stream( m_pavFormatContext, 0 ) fail!" << endl;
		}
	}	

	bResult &= ( NULL != m_pavStream );

	return bResult;
	
#else
	return FALSE;
#endif
}

BOOL CFFMpegRecorderImpl::WriteFrame( mu_uint8 *buf, int len, const CBaseCodec::TVideoFrameInfo& tFrameInfo )
{
#if !defined( __SYMBIAN32__ )
	
	SCOPE_LOCK( this->m_threadSafeLock );


	if ( !( m_pavStream && m_pavFormatContext ) )
	{
		return FALSE;
	}
	AVCodecContext *pavCodec = m_pavStream->codec;
	if ( !pavCodec )
	{
		Log() << _T( "WriteFrame AVCodecContext is NULL!" ) << endl;
		return FALSE;
	}

	try
	{
		if ( pavCodec->extradata == NULL )
		{
			if ( tFrameInfo.frameType == CBaseCodec::FRAME_I )
			{
				switch( pavCodec->codec_id )
				{
				case CODEC_ID_H264:
					{
						// H264编码的话编码附加信息要传入整个I帧。
						//pavCodec->extradata = mu_malloc( len );
						//if ( pavCodec->extradata == NULL )
						//{
						//	Log() << _T( "pavCodec->extradata = mu_malloc 申请内存失败！" ) << endl;
						//	return FALSE;
						//}
						//pavCodec->extradata_size = len;
						//memcpy( pavCodec->extradata, buf, len );

						TH264ConfigInfo tH264Cfg;
						l32 lResult = GetH264Config( buf, len, &tH264Cfg );
						if( VIDEO_SUCCESS == lResult )
						{
							for ( int i=0; i< tH264Cfg.l32PpsNum; ++i )
							{
								Log() << _T( "pps: " ) ;
								for ( int k=0; k<tH264Cfg.l32PpsLen[i]; k++ )
								{
									Log() << (void*)(unsigned char)tH264Cfg.pu8Pps[i][k] << _T( " " );
								}
							}
							Log() << endl;

							for ( int i=0; i< tH264Cfg.l32SpsNum; ++i )
							{
								Log() << _T( "sps: " ) ;
								for ( int k=0; k<tH264Cfg.l32SpsLen[i]; k++ )
								{
									Log() << (void*)(unsigned char)tH264Cfg.pu8Sps[i][k] << _T( " " );
								}
							}
                            Log() << endl;

							// 应该有且只有一个pps，sps。
							if ( tH264Cfg.l32PpsNum != 1 || tH264Cfg.l32SpsNum != 1 )
							{
								Log() << _T( "Get H264 PPS or SPS num != 1 !" ) << endl;
								return FALSE;
							}
							else
							{
								// 将PPS和SPS信息填入 pavCodec->extradata
								// 数据结构： pps长度（32bit ）pps数据 sps长度（32bit）sps数据。
								int nBuffLen = 4 + tH264Cfg.l32PpsLen[0] + 4 + tH264Cfg.l32SpsLen[0];
								pavCodec->extradata = mu_malloc( nBuffLen );
								if ( !pavCodec->extradata )
								{
									Log() << _T( "pavCodec->extradata = mu_malloc 申请内存失败！" ) << endl;
									return FALSE;
								}

								((char*)(pavCodec->extradata))[ nBuffLen -1 ] = 0;

								pavCodec->extradata_size = nBuffLen;
								int nWriteCursor = 0;

								int nPPSLen = tH264Cfg.l32PpsLen[0];
								memcpy( (char*)pavCodec->extradata + nWriteCursor, &nPPSLen, sizeof( int ) );
								nWriteCursor += 4;
								
								memcpy( (char*)pavCodec->extradata + nWriteCursor, tH264Cfg.pu8Pps[0], nPPSLen );
								nWriteCursor += nPPSLen;
								
								int nSpsLen = tH264Cfg.l32SpsLen[0];
								memcpy( (char*)pavCodec->extradata + nWriteCursor, &nSpsLen, sizeof( int ) );
								nWriteCursor += 4;

								memcpy( (char*)pavCodec->extradata + nWriteCursor, tH264Cfg.pu8Sps[0], nSpsLen );
								nWriteCursor += nSpsLen;

							}

						}
						else
						{
							Log() << _T( "Get H264 Config fail! return: " ) << lResult << endl;
							return FALSE;
						}
					}
					break;
				case CODEC_ID_MPEG4:
					{
						// MPEG4 要先获取Config信息。
						TMpeg4ConfigInfo tMpeg4Cfg;
						if( VIDEO_SUCCESS == GetMpeg4Config( buf, len, &tMpeg4Cfg ) )
						{
							pavCodec->extradata = mu_malloc( tMpeg4Cfg.l32ConfigLen );
							if ( NULL == pavCodec->extradata )
							{
								Log() << _T( "pavCodec->extradata = mu_malloc 申请内存失败！" ) << endl;
								return FALSE;
							}
							pavCodec->extradata_size = tMpeg4Cfg.l32ConfigLen;
							memcpy( pavCodec->extradata, tMpeg4Cfg.pu8Config, tMpeg4Cfg.l32ConfigLen );
						}
						else
						{
							Log() << _T( "get mpeg4 config info fail!" ) << endl;
							return FALSE;
					}
				}				
			    break;
			default:
				return FALSE;	// 不支持。
			    break;
			}
			

			// 填入其它codec信息。
			pavCodec->width = tFrameInfo.frameWidth;
			pavCodec->height = tFrameInfo.frameHeight;
			pavCodec->bit_rate = m_nBandWidth << 10;	// 临时，在写入尾部数据之前，会重新计算真实的码率。
			pavCodec->time_base.num = 1;

			// 暂时写死始终刻度。
			pavCodec->time_base.den = 1000000; //m_nTimeScale;	//

			// 保存开始时间。
			m_timeBegin = /*this->GetTimeStamp*/( tFrameInfo.frameTimeStamp );//tFrameInfo.frameTimeStamp.tv_sec * 1000 + tFrameInfo.frameTimeStamp.tv_usec;

			// 3gp 是需要这个标志的。
			if ( m_pavFormatContext->oformat->flags & AVFMT_GLOBALHEADER )
			{
				pavCodec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			}

			// 就算什么也没有，也要设置一下参数。;
			if( av_set_parameters( m_pavFormatContext, NULL) != 0 )
			{
				cout << "av set paramenters error!" << endl;
				return FALSE;
			}
				if ( NULL == m_pavFormatContext->priv_data )
				{
					Log() << _T( "priv_data is null after set param!" ) << endl;
					return FALSE;
				}
			if( av_write_header( m_pavFormatContext ) != 0 )
			{
				cout << "av write header error !" << endl;
				return FALSE;
			}
		}
	}
	
	if ( pavCodec->extradata != NULL )
	{
		// 已经初始化了codec附加信息，可以写入了。
		AVPacket avPkt;
		av_init_packet( &avPkt );
		avPkt.data = buf;
		avPkt.size = len;
//		avPkt.pts = tFrameInfo.frameTimeStamp;
		mu_uint64 nPktTime = /*this->GetTimeStamp*/( tFrameInfo.frameTimeStamp ); // tFrameInfo.frameTimeStamp.tv_sec * 1000 + tFrameInfo.frameTimeStamp.tv_usec;
		avPkt.pts = nPktTime - m_timeBegin;

		//////////////////////////////////////////////////////////////////////////
		// !暂时按照10帧每秒来录制。
//		avPkt.pts = m_nFrameCount * 100;

		// 关键帧。
		if ( tFrameInfo.frameType == CBaseCodec::FRAME_I )
		{
			avPkt.flags |= PKT_FLAG_KEY;;
		}
			if( m_pavFormatContext->streams == NULL || m_pavFormatContext->streams[ avPkt.stream_index ] == NULL )
			{
				Log() << "write frame fail! streams is null! " << endl;
				return FALSE;
			}
		if( 0 != av_write_frame( m_pavFormatContext, &avPkt ) )
		{
				Log() << "write frame fail!" << endl;
			return FALSE;
		}

		// 统计码率。
		this->m_timeEnd = nPktTime;
		this->m_nDataSize += len;
		this->m_nFrameCount ++;
	}
	}
	catch ( ... ) 
	{
		Log() << _T( "Wirte frame crash!" ) << endl;
		return FALSE;
	}

//	OutputDebugString( _T( "WriteFrame 2\n" ) );
	
	return TRUE;
	
#else
	return FALSE;
#endif
	
}

BOOL CFFMpegRecorderImpl::CloseFile()
{
#if !defined( __SYMBIAN32__ )
	
	SCOPE_LOCK( this->m_threadSafeLock );

//	OutputDebugString( _T( "close file 1 \n" ) );

	BOOL bResult = TRUE;

	if (  !( m_pavFormatContext && m_pavFormatContext->priv_data ) )
	{
		Log() << _T( "Close file data invalid!" ) << endl;
		bResult = FALSE;
	}

	if ( !( m_pavStream && m_pavStream->codec && m_pavStream->codec->extradata ) )
	{
		Log() << _T( "Close file stream data invalid!" ) << endl;
		bResult = FALSE;
	}

	if ( !( m_pavFormatContext && m_pavFormatContext->pb && m_pavFormatContext->pb->buffer ) )
	{
		Log() << _T( "Close file buffer data invalid!" ) << endl;
		bResult = FALSE;
	}
	
	if ( bResult )
	{
		try
		{		
			// 如果没有写入码流数据,则认为录像失败.
			if ( 0 == m_nFrameCount || 0 == m_nDataSize )
			{
				bResult = FALSE;
			}
			else
			{
//				OutputDebugString( _T( "close file 2 \n" ) );
                Log() << _T( "av_write_trailer call!" ) << endl;
				bResult = ( 0 == av_write_trailer( m_pavFormatContext ) );		
                Log() << _T( "av_write_trailer return!" ) << endl;
			}
						
		}
		catch ( ... )
		{
			Log() << _T( "Close file Crash!" ) << endl;
		}
	}	

//	OutputDebugString( _T( "close file 3 \n" ) );
	this->Release();

	if ( !bResult )
	{
		// 录像失败,删除文件.
		BOOL bDel = ::DelFile( m_strRecordFilePath.c_str() );
		Log() << _T( "Record fail! file " ) << m_strRecordFilePath <<  _T( "del result " ) << bDel << endl;
	}

//	OutputDebugString( _T( "close file 4 \n" ) );

	return bResult;
#else
	return FALSE;
#endif
}

BOOL CFFMpegRecorderImpl::Release()
{
#if	!defined( __SYMBIAN32__ )
			
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( m_pavFormatContext && m_pavFormatContext->pb )
	{
		url_fclose( m_pavFormatContext->pb );
	}	

	if ( m_pavFormatContext && m_pavFormatContext->priv_data )
	{
		av_freep( &m_pavFormatContext->priv_data );
	}


	av_freep( &m_pavFormatContext );
	av_freep( &m_pavStream );

	this->m_timeBegin = 0;
	this->m_timeEnd = 0;
	this->m_nDataSize = 0;
	this->m_nFrameCount = 0;

    Log() << _T( "CFFMpegRecorderImpl::Release call over!" ) << endl;

#endif
    
	return TRUE;
}

mu_int32 CFFMpegRecorderImpl::GetFFCodecId( CBaseCodec::ECodecId eMUCodecId ) const
{
	mu_int32 ffId;
	switch( eMUCodecId )
	{
	case CBaseCodec::CODEC_H264:
		ffId = CODEC_ID_H264;
		break;
	case CBaseCodec::CODEC_MPEG4:
		ffId = CODEC_ID_MPEG4;
		break;
	default:
		ffId = CODEC_ID_NONE;
		break;
	}

	return ffId;
}

//mu_uint64 CFFMpegRecorderImpl::GetTimeStamp( const timeval &time ) const
//{
//	return time.tv_sec * 1000000 + time.tv_usec;
//}
