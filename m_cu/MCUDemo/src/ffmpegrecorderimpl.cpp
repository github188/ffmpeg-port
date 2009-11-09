#include "StdAfx.h"
#include "ffmpegrecorderimpl.h"

#include "avformat.h"
#include "avstring.h"
#include "mcucommon.h"
#include "mem.h"
#include "getconfig.h"

extern "C"
{
	static int mov_write_header(AVFormatContext *s);

	extern AVOutputFormat tgp_muxer;

	extern URLProtocol file_protocol;

	extern AVInputFormat mov_demuxer;

	void av_freep(void *ptr);
};

// Ĭ��û��ע�����
BOOL CFFMpegRecorderImpl::s_bFileProtocolReg = FALSE;

CFFMpegRecorderImpl::CFFMpegRecorderImpl(void)
{
	m_pavFormatContext = NULL;
	m_pavStream = NULL;
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
	SCOPE_LOCK( this->m_threadSafeLock );

	BOOL bResult = TRUE;

	this->m_nBandWidth = nBandWidth;
	this->m_strRecordFilePath = strFileName;

	mcu::tlog << _T( "codec: " ) << eCodec << "bandwidth: " << nBandWidth << "file: " << strFileName << endl;

	// ע���ļ���дЭ�顣
	if ( !s_bFileProtocolReg )
	{
		av_register_protocol( &file_protocol );
		s_bFileProtocolReg = TRUE;
	}
	

	m_pavFormatContext = avformat_alloc_context();

	if ( NULL == m_pavFormatContext )
	{
		mcu::tlog << "m_pavFormatContext = avformat_alloc_context()" << endl;
	}

	bResult &= ( NULL != m_pavFormatContext );

	// 3gp�ĸ�ʽ�����
	if ( m_pavFormatContext )
	{
		m_pavFormatContext->oformat = &tgp_muxer;

		char szFileName[1024] = {0};
		WCtoMB( strFileName, szFileName, 1024 );

		// �洢
		av_strlcpy( m_pavFormatContext->filename, szFileName, 1024 );

		// ��Ŀ���ļ���
		bResult &= (url_fopen(&m_pavFormatContext->pb, szFileName, URL_WRONLY) == 0);

		if ( !bResult )
		{
			mcu::tlog << _T( "create record file fail! file:" ) << strFileName << endl;
		}

		m_pavStream = av_new_stream( m_pavFormatContext, 0 ); 

		if ( m_pavStream )
		{
			CodecID ffCodecId = this->GetFFCodecId( eCodec );
			m_pavStream->codec->codec_id = ffCodecId;
			m_pavStream->codec->codec_type = CODEC_TYPE_VIDEO;
		}
		else
		{
			mcu::tlog << _T( "m_pavStream = av_new_stream( m_pavFormatContext, 0 ) fail!" ) << endl;
		}
	}	

	bResult &= ( NULL != m_pavStream );
	
	return bResult;
}

BOOL CFFMpegRecorderImpl::WriteFrame( mu_uint8 *buf, int len, const CBaseCodec::TVideoFrameInfo& tFrameInfo )
{
	SCOPE_LOCK( this->m_threadSafeLock );

//	OutputDebugString( _T( "WriteFrame 1\n" ) );

	if ( !( m_pavStream && m_pavFormatContext ) )
	{
		mcu::tlog << _T( "record write frame stream and formatcontext is null!" ) << endl;
		return FALSE;
	}

//	mcu::tlog << _T( "WriteFrame call!" ) << endl;

	

	AVCodecContext *pavCodec = m_pavStream->codec;
	if ( !pavCodec )
	{
		mcu::tlog << _T( "WriteFrame AVCodecContext is NULL!" ) << endl;
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
						// H264����Ļ����븽����ϢҪ��������I֡��
						//pavCodec->extradata = mu_malloc( len );
						//if ( pavCodec->extradata == NULL )
						//{
						//	mcu::tlog << _T( "pavCodec->extradata = mu_malloc �����ڴ�ʧ�ܣ�" ) << endl;
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
								mcu::tlog << _T( "pps: " ) ;
								for ( int k=0; k<tH264Cfg.l32PpsLen[i]; k++ )
								{
									mcu::tlog << (void*)(unsigned char)tH264Cfg.pu8Pps[i][k] << _T( " " );
								}
							}

							mcu::tlog << endl;

							for ( int i=0; i< tH264Cfg.l32SpsNum; ++i )
							{
								mcu::tlog << _T( "sps: " ) ;
								for ( int k=0; k<tH264Cfg.l32SpsLen[i]; k++ )
								{
									mcu::tlog << (void*)(unsigned char)tH264Cfg.pu8Sps[i][k] << _T( " " );
								}
							}

							// Ӧ������ֻ��һ��pps��sps��
							if ( tH264Cfg.l32PpsNum != 1 || tH264Cfg.l32SpsNum != 1 )
							{
								mcu::tlog << _T( "Get H264 PPS or SPS num != 1 !" ) << endl;
								return FALSE;
							}
							else
							{
								// ��PPS��SPS��Ϣ���� pavCodec->extradata
								// ���ݽṹ�� pps���ȣ�32bit ��pps���� sps���ȣ�32bit��sps���ݡ�
								int nBuffLen = 4 + tH264Cfg.l32PpsLen[0] + 4 + tH264Cfg.l32SpsLen[0];
								pavCodec->extradata = mu_malloc( nBuffLen );
								if ( !pavCodec->extradata )
								{
									mcu::tlog << _T( "pavCodec->extradata = mu_malloc �����ڴ�ʧ�ܣ�" ) << endl;
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
							mcu::tlog << _T( "Get H264 Config fail! return: " ) << lResult << endl;
							return FALSE;
						}
					}
					break;
				case CODEC_ID_MPEG4:
					{
						// MPEG4 Ҫ�Ȼ�ȡConfig��Ϣ��
						TMpeg4ConfigInfo tMpeg4Cfg;
						if( VIDEO_SUCCESS == GetMpeg4Config( buf, len, &tMpeg4Cfg ) )
						{
							pavCodec->extradata = mu_malloc( tMpeg4Cfg.l32ConfigLen );
							if ( NULL == pavCodec->extradata )
							{
								mcu::tlog << _T( "pavCodec->extradata = mu_malloc �����ڴ�ʧ�ܣ�" ) << endl;
								return FALSE;
							}
							pavCodec->extradata_size = tMpeg4Cfg.l32ConfigLen;
							memcpy( pavCodec->extradata, tMpeg4Cfg.pu8Config, tMpeg4Cfg.l32ConfigLen );
						}
						else
						{
							mcu::tlog << _T( "get mpeg4 config info fail!" ) << endl;
							return FALSE;
						}
					}				
					break;
				default:
					return FALSE;	// ��֧�֡�
					break;
				}


				// ��������codec��Ϣ��
				pavCodec->width = tFrameInfo.frameWidth;
				pavCodec->height = tFrameInfo.frameHeight;
				pavCodec->bit_rate = m_nBandWidth << 10;	// ��ʱ����д��β������֮ǰ�������¼�����ʵ�����ʡ�
				pavCodec->time_base.num = 1;

				// ��ʱд��ʼ�տ̶ȡ�
				pavCodec->time_base.den = 1000000; //m_nTimeScale;	//

				// ���濪ʼʱ�䡣
				m_timeBegin = this->GetTimeStamp( tFrameInfo.frameTimeStamp );//tFrameInfo.frameTimeStamp.tv_sec * 1000 + tFrameInfo.frameTimeStamp.tv_usec;

				// 3gp ����Ҫ�����־�ġ�
				if ( m_pavFormatContext->oformat->flags & AVFMT_GLOBALHEADER )
				{
					pavCodec->flags |= CODEC_FLAG_GLOBAL_HEADER;
				}

				// ����ʲôҲû�У�ҲҪ����һ�²�����;
				if( av_set_parameters( m_pavFormatContext, NULL) != 0 )
				{
					mcu::tlog << "av set paramenters error!" << endl;
					return FALSE;
				}

				if ( NULL == m_pavFormatContext->priv_data )
				{
					mcu::tlog << _T( "priv_data is null after set param!" ) << endl;
					return FALSE;
				}

				if( av_write_header( m_pavFormatContext ) != NULL )
				{
					mcu::tlog << "av write header error !" << endl;
					return FALSE;
				}
			}
		}

		if ( pavCodec->extradata != NULL )
		{
			// �Ѿ���ʼ����codec������Ϣ������д���ˡ�
			AVPacket avPkt;
			av_init_packet( &avPkt );
			avPkt.data = buf;
			avPkt.size = len;
			//		avPkt.pts = tFrameInfo.frameTimeStamp;
			mu_uint64 nPktTime = this->GetTimeStamp( tFrameInfo.frameTimeStamp ); // tFrameInfo.frameTimeStamp.tv_sec * 1000 + tFrameInfo.frameTimeStamp.tv_usec;
			avPkt.pts = nPktTime - m_timeBegin;

			//////////////////////////////////////////////////////////////////////////
			// !��ʱ����10֡ÿ����¼�ơ�
			//		avPkt.pts = m_nFrameCount * 100;

			// �ؼ�֡��
			if ( tFrameInfo.frameType == CBaseCodec::FRAME_I )
			{
				avPkt.flags |= PKT_FLAG_KEY;;
			}

			if( m_pavFormatContext->streams == NULL || m_pavFormatContext->streams[ avPkt.stream_index ] == NULL )
			{
				mcu::tlog << "write frame fail! streams is null! " << endl;
				return FALSE;
			}

			if( 0 != av_write_frame( m_pavFormatContext, &avPkt ) )
			{
				mcu::tlog << "write frame fail!" << endl;
				return FALSE;
			}

			// ͳ�����ʡ�
			this->m_timeEnd = nPktTime;
			this->m_nDataSize += len;
			this->m_nFrameCount ++;
		}
	}
	catch ( ... ) 
	{
		mcu::tlog << _T( "Wirte frame crash!" ) << endl;
		return FALSE;
	}

//	OutputDebugString( _T( "WriteFrame 2\n" ) );
	
	return TRUE;
}

BOOL CFFMpegRecorderImpl::CloseFile()
{
	SCOPE_LOCK( this->m_threadSafeLock );

//	OutputDebugString( _T( "close file 1 \n" ) );

	BOOL bResult = TRUE;

	if (  !( m_pavFormatContext && m_pavFormatContext->priv_data ) )
	{
		mcu::tlog << _T( "Close file data invalid!" ) << endl;
		bResult = FALSE;
	}

	if ( !( m_pavStream && m_pavStream->codec && m_pavStream->codec->extradata ) )
	{
		mcu::tlog << _T( "Close file stream data invalid!" ) << endl;
		bResult = FALSE;
	}

	if ( !( m_pavFormatContext && m_pavFormatContext->pb && m_pavFormatContext->pb->buffer ) )
	{
		mcu::tlog << _T( "Close file buffer data invalid!" ) << endl;
		bResult = FALSE;
	}
	
	if ( bResult )
	{
		try
		{		
			// ���û��д����������,����Ϊ¼��ʧ��.
			if ( 0 == m_nFrameCount || 0 == m_nDataSize )
			{
				bResult = FALSE;
			}
			else
			{
//				OutputDebugString( _T( "close file 2 \n" ) );
				bResult = ( 0 == av_write_trailer( m_pavFormatContext ) );		
			}
						
		}
		catch ( ... )
		{
			mcu::tlog << _T( "Close file Crash!" ) << endl;
		}
	}	

//	OutputDebugString( _T( "close file 3 \n" ) );
	this->Release();

	if ( !bResult )
	{
		// ¼��ʧ��,ɾ���ļ�.
		BOOL bDel = ::DeleteFile( m_strRecordFilePath.c_str() );
		mcu::tlog << _T( "Record fail! file " ) << m_strRecordFilePath <<  _T( "del result " ) << bDel << endl;
	}

//	OutputDebugString( _T( "close file 4 \n" ) );

	return bResult;
}

BOOL CFFMpegRecorderImpl::Release()
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if ( m_pavFormatContext && m_pavFormatContext->pb )
	{
		url_fclose( m_pavFormatContext->pb );
	}	

	if ( m_pavFormatContext && m_pavFormatContext->priv_data )
	{
		av_freep( &m_pavFormatContext->priv_data );
	}

	if ( m_pavStream && m_pavStream->codec )
	{
		av_freep( &( m_pavStream->codec->extradata ) );
	}

	av_freep( &m_pavFormatContext );
	av_freep( &m_pavStream );

	this->m_timeBegin = 0;
	this->m_timeEnd = 0;
	this->m_nDataSize = 0;
	this->m_nFrameCount = 0;

	return TRUE;
}

CodecID CFFMpegRecorderImpl::GetFFCodecId( CBaseCodec::ECodecId eMUCodecId ) const
{
	CodecID ffId;
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

mu_uint64 CFFMpegRecorderImpl::GetTimeStamp( const timeval &time ) const
{
	return time.tv_sec * 1000000 + time.tv_usec;
}