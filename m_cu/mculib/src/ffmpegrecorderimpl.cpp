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
};

// Ĭ��û��ע�����
BOOL CFFMpegRecorderImpl::s_bFileProtocolReg = FALSE;

CFFMpegRecorderImpl::CFFMpegRecorderImpl(void)
{
#ifdef _WIN32_WCE

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
	BOOL bResult = TRUE;
#ifdef _WIN32_WCE


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
#ifdef _UNICODE
		WCtoMB( strFileName, szFileName, 1024 );
#else
		strcpy( szFileName, strFileName );
#endif

		// �洢
		av_strlcpy( m_pavFormatContext->filename, szFileName, 1024 );

		// ��Ŀ���ļ���
		bResult &= (url_fopen(&m_pavFormatContext->pb, szFileName, URL_WRONLY) == 0);

		if ( !bResult )
		{
			mcu::tlog << "create record file fail! file:" << szFileName << endl;
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
			mcu::tlog << "m_pavStream = av_new_stream( m_pavFormatContext, 0 ) fail!" << endl;
		}
	}	

	bResult &= ( NULL != m_pavStream );
#else
	bResult = FALSE;
#endif
	return bResult;
}

BOOL CFFMpegRecorderImpl::WriteFrame( mu_uint8 *buf, int len, const CBaseCodec::TVideoFrameInfo& tFrameInfo )
{
#ifdef _WIN32_WCE

	if ( !( m_pavStream && m_pavFormatContext ) )
	{
		return FALSE;
	}
	AVCodecContext *pavCodec = m_pavStream->codec;
	if ( pavCodec->extradata == NULL )
	{
		if ( tFrameInfo.frameType == CBaseCodec::FRAME_I )
		{
			switch( pavCodec->codec_id )
			{
			case CODEC_ID_H264:

				// H264����Ļ����븽����ϢҪ��������I֡��
				pavCodec->extradata = mu_malloc( len );
				pavCodec->extradata_size = len;
				memcpy( pavCodec->extradata, buf, len );

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
			m_timeBegin = /*this->GetTimeStamp*/( tFrameInfo.frameTimeStamp );//tFrameInfo.frameTimeStamp.tv_sec * 1000 + tFrameInfo.frameTimeStamp.tv_usec;

			// 3gp ����Ҫ�����־�ġ�
			if ( m_pavFormatContext->oformat->flags & AVFMT_GLOBALHEADER )
			{
				pavCodec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			}

			// ����ʲôҲû�У�ҲҪ����һ�²�����;
			if( av_set_parameters( m_pavFormatContext, NULL) != 0 )
			{
				cout << "av set paramenters error!" << endl;
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
		// �Ѿ���ʼ����codec������Ϣ������д���ˡ�
		AVPacket avPkt;
		av_init_packet( &avPkt );
		avPkt.data = buf;
		avPkt.size = len;
//		avPkt.pts = tFrameInfo.frameTimeStamp;
		mu_uint64 nPktTime = /*this->GetTimeStamp*/( tFrameInfo.frameTimeStamp ); // tFrameInfo.frameTimeStamp.tv_sec * 1000 + tFrameInfo.frameTimeStamp.tv_usec;
		avPkt.pts = nPktTime - m_timeBegin;

		//////////////////////////////////////////////////////////////////////////
		// !��ʱ����10֡ÿ����¼�ơ�
//		avPkt.pts = m_nFrameCount * 100;

		// �ؼ�֡��
		if ( tFrameInfo.frameType == CBaseCodec::FRAME_I )
		{
			avPkt.flags |= PKT_FLAG_KEY;;
		}

		if( 0 != av_write_frame( m_pavFormatContext, &avPkt ) )
		{
			cout << "write frame fail!" << endl;
			return FALSE;
		}

		// ͳ�����ʡ�
		this->m_timeEnd = nPktTime;
		this->m_nDataSize += len;
		this->m_nFrameCount ++;
	}

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CFFMpegRecorderImpl::CloseFile()
{
	BOOL bResult = FALSE;
#ifdef _WIN32_WCE
	if ( !( m_pavStream && m_pavFormatContext ) )
	{
		return FALSE;
	}
	
	
	// ���û��д����������,����Ϊ¼��ʧ��.
	if ( 0 == m_nFrameCount || 0 == m_nDataSize )
	{
		bResult = FALSE;
	}
	else
	{
		// �������ʡ�
//		mu_uint64 nTimeSpan = this->m_timeEnd - this->m_timeBegin;
//		this->m_pavStream->codec->bit_rate = (int)( this->m_nDataSize * 1000 / nTimeSpan );

		bResult = ( 0 == av_write_trailer( m_pavFormatContext ) );
		
	}

	this->Release();

	if ( !bResult )
	{
		// ¼��ʧ��,ɾ���ļ�.
		BOOL bDel = ::DelFile( m_strRecordFilePath.c_str() );
		mcu::tlog << _T( "Record fail! file " ) << m_strRecordFilePath <<  _T( "del result " ) << bDel << endl;
	}
#else
	bResult = FALSE;
#endif
	return bResult;
}

BOOL CFFMpegRecorderImpl::Release()
{
#ifdef _WIN32_WCE
	if ( m_pavFormatContext && m_pavFormatContext->pb )
	{
		url_fclose( m_pavFormatContext->pb );
	}	

	if ( m_pavStream && m_pavStream->codec )
	{
		mu_freep( &( m_pavStream->codec->extradata ) );
	}

	mu_freep( &m_pavFormatContext );
	mu_freep( &m_pavStream );
#endif
	this->m_timeBegin = 0;
	this->m_timeEnd = 0;
	this->m_nDataSize = 0;
	this->m_nFrameCount = 0;

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