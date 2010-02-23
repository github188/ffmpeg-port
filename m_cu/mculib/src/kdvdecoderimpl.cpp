#include "kdvdecoderimpl.h"

#include "vid_videounit.h"
#include "mem.h"
#include "log.h"
#include "portabledefine.h"
#include "mcucommon.h"

//#define OUT_WIDTH ( 352 )
//#define OUT_HEIGHT ( 288 )

#define OUT_WIDTH ( 320 )
#define OUT_HEIGHT ( 240 )

/** 静态变量定义及初始化. */
CKDVDecoderImpl::TDecoderNameTable CKDVDecoderImpl::s_tDecoderNameTable;
CMCUMutex CKDVDecoderImpl::s_decoderNameTableLock;

CKDVDecoderImpl * CKDVDecoderImpl::GetDecoder( CBaseCodec::ECodecId eCodecId, LPCTSTR strName /* = NULL */ )
{
	SCOPE_LOCK( s_decoderNameTableLock );

	CKDVDecoderImpl *pDec = FindDecoder( strName );

	// test
// 	if ( pDec )
// 	{
// 		Release( pDec );
// 		pDec = NULL;
// 	}


	if ( NULL == pDec )
	{
		pDec = new CKDVDecoderImpl();
	}
	if ( NULL == pDec )
	{
		Log() << _T( "CKDVDecoderImpl::GetDecoder new CKDVDecoderImpl Fail!" ) << endl;
		return NULL;
	}
	
	BOOL bResult = pDec->Create( eCodecId );
	if ( !bResult )
	{
		Log() << _T( "keda decoder Create Fail! codecid: " ) << eCodecId << endl;
	}

	_ASSERT( bResult );
	if ( !bResult )
	{
		Release( pDec );
		pDec = NULL;
	}

	RegDec( strName, pDec );

	return pDec;
}

void CKDVDecoderImpl::Release( CKDVDecoderImpl *pDecoder )
{
	SCOPE_LOCK( s_decoderNameTableLock );
	for ( TDecoderNameTable::iterator iter = s_tDecoderNameTable.begin(); iter != s_tDecoderNameTable.end(); ++iter )
	{
		if ( iter->second == pDecoder )
		{
			s_tDecoderNameTable.erase( iter );
			break;
		}
	}
	delete pDecoder;
	pDecoder = NULL;
}

CKDVDecoderImpl *CKDVDecoderImpl::FindDecoder( LPCTSTR strName )
{
	SCOPE_LOCK( s_decoderNameTableLock );
	TDecoderNameTable::iterator iter = s_tDecoderNameTable.find( strName );
	if ( iter != s_tDecoderNameTable.end() )
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}

void CKDVDecoderImpl::ReleaseAll()
{
	SCOPE_LOCK( s_decoderNameTableLock );
	for ( TDecoderNameTable::iterator iter = s_tDecoderNameTable.begin(); iter != s_tDecoderNameTable.end(); ++iter )
	{
		delete iter->second;
		iter->second = NULL;
	}
}



void CKDVDecoderImpl::RegDec( LPCTSTR lpName, CKDVDecoderImpl *pDec )
{
	SCOPE_LOCK( s_decoderNameTableLock );

	if ( lpName && pDec )
	{
		s_tDecoderNameTable[ lpName ]  = pDec;
	}		
}

CKDVDecoderImpl::CKDVDecoderImpl(void)
{
	m_pDecParam = NULL;
	m_pvHandle = NULL;
	m_pPicDataBuf = NULL;

	this->m_pCaptureDataBuf = NULL;
	this->m_nCaptureDataBufLen = 0;
}

CKDVDecoderImpl::~CKDVDecoderImpl(void)
{
	Release();
}

BOOL CKDVDecoderImpl::Create( CDecoder::ECodecId eCodec )
{
	SCOPE_LOCK( m_threadSafeLock );

	CODECTYPE eKdvCodecType;
	switch( eCodec )
	{
	case CDecoder::CODEC_H264:
		eKdvCodecType = VCODEC_H264;
		break;
	case CDecoder::CODEC_MPEG4:
		eKdvCodecType = VCODEC_MPEG4;
		break;

	default:
		Log() << _T( "unkown kdv codec Id " ) << eCodec << endl;
		_ASSERT( FALSE );
		return FALSE;
	    break;
	}

	if ( NULL == m_pDecParam )
	{
		m_pDecParam = (TVideoUnitDecParam*)mu_mallocz( sizeof( TVideoUnitDecParam ) );
	}

	if ( NULL == m_pDecParam )
	{
		Log() << _T( "m_pDecParam = (TVideoUnitDecParam*)mu_mallocz mu_mallocz fail!" ) << endl;
		return FALSE;
	}

	m_pDecParam->tDecParam.l32DecMaxWidth = OUT_WIDTH;
	m_pDecParam->tDecParam.l32DecMaxHeight = OUT_HEIGHT;
	m_pDecParam->tDecParam.u32DecType = eKdvCodecType;
	m_pDecParam->tDecParam.l32DecPacketMode = 0;

	TPostProcessParam *ptPostProcessParam = &m_pDecParam->tPostProcessParam;
	ptPostProcessParam->l32OutYUVType = YUV420;	
	ptPostProcessParam->l32OutWidth = OUT_WIDTH;
	ptPostProcessParam->l32OutHeight = OUT_HEIGHT;
	ptPostProcessParam->l32ImageQuality = 0;
	ptPostProcessParam->l32OutIsInterlace = 1;
	ptPostProcessParam->l32OutYStride = OUT_WIDTH;
	ptPostProcessParam->l32OutUVStride = OUT_WIDTH/2;
	ptPostProcessParam->u32Reserved = 0;

	mu_int32 l32Result;
	if( NULL == m_pvHandle )
	{
		// 解码器还没有创建。创建。
		// keda的解码器，H264占内存多，所以需要Open H264的解码。
		// 否则设置成H264的时候会失败。
		CODECTYPE eOldType = (CODECTYPE)m_pDecParam->tDecParam.u32DecType;
		m_pDecParam->tDecParam.u32DecType = VCODEC_H264;
		l32Result = VideoUnitDecoderOpen( &m_pvHandle, m_pDecParam, NULL /*&tIputMemParam*/);
		if(VIDEO_SUCCESS != l32Result)
		{
			Log() << _T("VideoUnitDecoderOpen failure!") << endl;
			PrintErrorInfo( m_pvHandle, l32Result);

			return FALSE;
		}
		else
		{
			Log() << _T( "Craete H264 Decoder success! VideoUnitDecoderOpen" ) << endl;
		}

		m_pDecParam->tDecParam.u32DecType = eOldType;
	}

	// 解码器已经创建，设置参数。
	l32Result = VideoUnitDecoderSetParam( m_pvHandle, m_pDecParam );
	if(VIDEO_SUCCESS != l32Result)
	{
		printf("VideoUnitDecoderSetParam failure!\n");
		PrintErrorInfo( m_pvHandle, l32Result);

		return FALSE;
	}
	else
	{
		Log() << _T( "VideoUnitDecoderSetParam success! codec change to: " ) << m_pDecParam->tDecParam.u32DecType <<endl;
	}

	char as8Version[ 256 ] = {0};
	mu_int32 l32VerLen;
	//获取版本信息
	l32Result = VideoUnitDecoderGetVersion(as8Version,  256, &l32VerLen, VCODEC_VIDUNIT);
	if(VIDEO_SUCCESS == l32Result)
	{
		printf("Vid Decoder version %s\n", as8Version);
	}

	l32Result = VideoUnitDecoderGetVersion(as8Version,  256, &l32VerLen, m_pDecParam->tDecParam.u32DecType);
	if(VIDEO_SUCCESS == l32Result)
	{
		printf("Decoder version %s\n", as8Version);
	}	

	// 为图片缓冲区申请空间.
	mu_int32 lPicBufLen ;
	if(YUV422 == ptPostProcessParam->l32OutYUVType)
	{
		lPicBufLen = ptPostProcessParam->l32OutWidth * ptPostProcessParam->l32OutHeight * 2;
	}
	else
	{
		lPicBufLen = ptPostProcessParam->l32OutWidth * ptPostProcessParam->l32OutHeight +
			(ptPostProcessParam->l32OutWidth * ptPostProcessParam->l32OutHeight >> 1);
	}

	if ( NULL == m_pPicDataBuf )
	{
		m_pPicDataBuf = ( mu_uint8 * )mu_mallocz( lPicBufLen );
		if ( NULL == m_pPicDataBuf )
		{
			Log() << "m_pPicDataBuf = ( mu_uint8 * )mu_mallocz( lPicBufLen ) 申请内存失败！" << endl;
			return FALSE;
		}
	}
	
	return TRUE;
	
}

BOOL CKDVDecoderImpl::Release()
{
	SCOPE_LOCK( m_threadSafeLock );

	//解码结束
	if ( m_pvHandle )
	{
		VideoUnitDecoderClose( m_pvHandle );
	}
	
	mu_freep( &m_pDecParam );

	mu_freep( &m_pPicDataBuf );

	mu_freep( &m_pCaptureDataBuf );
	m_nCaptureDataBufLen = 0;

	return TRUE;
}

BOOL CKDVDecoderImpl::Decode( const CBaseCodec::TStreamPacket& tPkt, CBaseCodec::TVideoPicture& tPicture,
							 CBaseCodec::TVideoFrameInfo *pFrameInfo/* = NULL*/)
{
	SCOPE_LOCK( m_threadSafeLock );

	void *pStreamBuf = tPkt.data;
	mu_uint32 nBufLen = tPkt.datalen;

	if ( NULL == m_pvHandle )
	{
		return FALSE;
	}

	//static mu_uint64 pkttime = 0;
	//int nSpan = tPkt.timeStamp - pkttime;
	//pkttime = tPkt.timeStamp;
	////Log() << _T( "Decode pkt timespan: " ) << nSpan << endl;
	//printf( "Decode pkt timespan: %d \n", nSpan );

	TDecInput tDecoderInput;
	memset( &tDecoderInput, 0, sizeof( tDecoderInput ) );

	tDecoderInput.pu8DecBitstream = (u8*)pStreamBuf;
	tDecoderInput.u32DecBitstreamLen = nBufLen;

	TDecOutput tDecoderOutput;
	memset( &tDecoderOutput, 0, sizeof( tDecoderOutput ) );
	tDecoderOutput.pu8YUV = m_pPicDataBuf;

	mu_int32 l32Result = VideoUnitDecoderProcess( m_pvHandle, &tDecoderInput, &tDecoderOutput);
	if (VIDEO_SUCCESS != l32Result)
	{

		printf("VideoUnitDecoderProcess failure %d!\n", l32Result);
		PrintErrorInfo( m_pvHandle, l32Result);
		return FALSE;
	}

	tPicture.picWidth = OUT_WIDTH;
	tPicture.picHeight = OUT_HEIGHT;
	tPicture.type = CDecoder::PIC_YUV420P;
	tPicture.data[0] = tDecoderOutput.pu8YUV;
	tPicture.linesize[0] = OUT_WIDTH * OUT_HEIGHT;
	tPicture.data[1] = tPicture.data[0] + tPicture.linesize[0];
	tPicture.linesize[1] = tPicture.linesize[0] >> 2;
	tPicture.data[2] = tPicture.data[1] + tPicture.linesize[1];
	tPicture.linesize[2] = tPicture.linesize[0] >> 2;

	// 返回帧信息。
	if ( pFrameInfo )
	{
		CBaseCodec::EFrameType eFt;
		switch( tDecoderOutput.l32DecFrameType )
		{
		case I_PICTURE:
			eFt = CBaseCodec::FRAME_I;
			break;
		case P_PICTURE:
			eFt = CBaseCodec::FRAME_P;
		    break;
		default:
			eFt = CBaseCodec::FRAME_UNKNOW;
		    break;
		}
		pFrameInfo->frameType = eFt;

		pFrameInfo->frameWidth = tDecoderOutput.l32DecWidth;
		pFrameInfo->frameHeight = tDecoderOutput.l32DecHeight;		
		pFrameInfo->frameTimeStamp = tPkt.timeStamp;
	}

#ifdef _DEBUG
// 	static int s_count = 0;
// 	s_count ++;
// 	static CFile s_tmpF;
// 	if ( s_count == 10 )
// 	{
// 		s_tmpF.Open( _T( "\\Storage Card\\yuv.yuv" ), CFile::modeCreate | CFile::modeWrite );
// 		int nLen = 320 * 240 * 3 / 2;
// 		s_tmpF.Write( tDecoderOutput.pu8YUV, nLen );
// 	}

#endif
	

	return TRUE;
}

void CKDVDecoderImpl::PrintErrorInfo(void * pvHandle, mu_int32 l32ResultForAnalyze)
{
	char strErrorText[256];
	l32 l32Result;

	l32Result = VideoUnitDecoderAnalyzeErrorCode(pvHandle, l32ResultForAnalyze, sizeof(strErrorText), strErrorText);
	if(l32Result != VIDEO_SUCCESS)
	{
		sprintf(strErrorText, "unknown ErrCode:0x%x\r\n", l32ResultForAnalyze);
	}
	printf(" ErrorInfo : %s\r\n", strErrorText);
}

BOOL CKDVDecoderImpl::CapturePic( LPCTSTR strPicPath, CEncoder *pEnc )
{
	SCOPE_LOCK( m_threadSafeLock );

	// 获取当前图片。
	CBaseCodec::TVideoPicture tPicture;
	tPicture.picWidth = OUT_WIDTH;
	tPicture.picHeight = OUT_HEIGHT;
	tPicture.type = CDecoder::PIC_YUV420P;
	tPicture.data[0] = this->m_pPicDataBuf;
	tPicture.linesize[0] = OUT_WIDTH * OUT_HEIGHT;
	tPicture.data[1] = tPicture.data[0] + tPicture.linesize[0];
	tPicture.linesize[1] = tPicture.linesize[0] >> 2;
	tPicture.data[2] = tPicture.data[1] + tPicture.linesize[1];
	tPicture.linesize[2] = tPicture.linesize[0] >> 2;

	mu_uint32 nMemSize = pEnc->GetMemSize( tPicture );
	m_pCaptureDataBuf = (mu_uint8 *)mu_fast_realloc( m_pCaptureDataBuf, &m_nCaptureDataBufLen, nMemSize );
	memset( m_pCaptureDataBuf, 0, m_nCaptureDataBufLen );

	mu_uint32 nBufLen = m_nCaptureDataBufLen;
	BOOL bResult = pEnc->Encode( tPicture, m_pCaptureDataBuf, nBufLen );
	if ( bResult )
	{
		// 把缓冲区存起来。
		std::ofstream picFile;
        string strUtf8 = ::UTF16toUTF8( strPicPath );
		picFile.open( strUtf8.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
		if ( picFile.is_open() )
		{
			picFile.write( (char*)m_pCaptureDataBuf, nBufLen );
			picFile.close();			
		}
		else
		{
			Log() << "Capture pic open file fail! file: " << strPicPath << endl;
			bResult = FALSE;
		}		
	}	
	return bResult;
}


