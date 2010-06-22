
#include "kdvencoderimpl.h"

#include "vid_videounit.h"
#include "mem.h"
#include "log.h"

CKDVEncoderImpl::CKDVEncoderImpl(void)
{
	this->m_pEncParam = NULL;
	m_pvHandle = NULL;
	this->m_pDataBuf = NULL;
	this->m_nDataBufLen = 0;

	this->m_pPicBuf = NULL;
	this->m_nPicBufLen = 0;
}

CKDVEncoderImpl::~CKDVEncoderImpl(void)
{
	this->Release();
}

BOOL CKDVEncoderImpl::Create( CBaseCodec::ECodecId eCodec )
{
	SCOPE_LOCK( m_threadSafeLock );

	CODECTYPE eKdvCodecType;
	switch( eCodec )
	{
	case CBaseCodec::CODEC_JPEG:
		eKdvCodecType = VCODEC_JPEG;
		break;

	default:
		Log() << _T( "unkown kdv encoder id, kdv encoder create fail!" ) << endl;
		_ASSERT( FALSE );
		return FALSE;
		break;
	}

	if ( NULL == this->m_pEncParam )
	{
		m_pEncParam = (TVideoUnitEncParam*)mu_mallocz( sizeof( TVideoUnitEncParam ) );
	}

	if( NULL == m_pEncParam )
	{
		Log() << "m_pEncParam = (TVideoUnitEncParam*)mu_mallocz 申请内存失败！" << endl;
		return FALSE;
	}

	const int conQuality = 80;
	const int INPUT_IMG_WIDHT = OUT_IMG_WIDTH;
	const int INPUT_IMG_HEIGHT = OUT_IMG_HEIGHT;

	m_pEncParam->tEncParam.u32EncType = eKdvCodecType;
	m_pEncParam->tEncParam.l32EncQuality = conQuality;
	m_pEncParam->tEncParam.l32EncWidth = INPUT_IMG_WIDHT;
	m_pEncParam->tEncParam.l32EncHeight = INPUT_IMG_HEIGHT;

	m_pEncParam->tPreProcessParam.l32SrcYUVType = YUV420;
	m_pEncParam->tPreProcessParam.l32SrcWidth = INPUT_IMG_WIDHT;
	m_pEncParam->tPreProcessParam.l32SrcHeight = INPUT_IMG_HEIGHT;
	m_pEncParam->tPreProcessParam.l32SrcIsInterlace = 1;
	m_pEncParam->tPreProcessParam.l32SrcYStride = m_pEncParam->tPreProcessParam.l32SrcWidth;
	m_pEncParam->tPreProcessParam.l32SrcUVStride = m_pEncParam->tPreProcessParam.l32SrcYStride >> 1;

	m_pEncParam->tPreProcessParam.l32DstYUVType = YUV420;
	m_pEncParam->tPreProcessParam.l32DstWidth = INPUT_IMG_WIDHT;
	m_pEncParam->tPreProcessParam.l32DstHeight = INPUT_IMG_HEIGHT;
	m_pEncParam->tPreProcessParam.l32DstIsInterlace = 1;
	m_pEncParam->tPreProcessParam.l32DstYStride = m_pEncParam->tPreProcessParam.l32DstWidth;
	m_pEncParam->tPreProcessParam.l32DstUVStride = m_pEncParam->tPreProcessParam.l32DstYStride >> 1;

	m_pEncParam->tPreProcessParam.l32ImageQuality = 0;
	

	
	// 创建编码器。
	mu_int32 l32Result;
	l32Result = ::VideoUnitEncoderOpen( &this->m_pvHandle, m_pEncParam, NULL );
	if( VIDEO_SUCCESS != l32Result )
	{
		printf( "VideoUnitEncoderOpen fail!\n" );
		PrintErrorInfo( m_pvHandle, l32Result );
		return FALSE;
	}

	// 打印版本。
	char as8Version[ 256 ] = {0};
	mu_int32 l32VerLen;
	//获取版本信息
	l32Result = VideoUnitEncoderGetVersion(as8Version,  256, &l32VerLen, VCODEC_VIDUNIT);
	if(VIDEO_SUCCESS == l32Result)
	{
		printf("Vid Encoder version %s\n", as8Version);
	}

	return TRUE;
}

BOOL CKDVEncoderImpl::Encode( const CBaseCodec::TVideoPicture& tPicture, void *pStramBuf, mu_uint32 & nBufLen )
{
	SCOPE_LOCK( m_threadSafeLock );

	if ( NULL == m_pvHandle )
	{
		return FALSE;
	}

	// 编码后图像应该同编码前一样。重新设置编码参数。
	this->m_pEncParam->tEncParam.l32EncWidth = tPicture.picWidth;
	this->m_pEncParam->tEncParam.l32EncHeight = tPicture.picHeight;

	m_pEncParam->tPreProcessParam.l32SrcWidth = tPicture.picWidth;
	m_pEncParam->tPreProcessParam.l32SrcHeight = tPicture.picHeight;

	m_pEncParam->tPreProcessParam.l32SrcYStride = m_pEncParam->tPreProcessParam.l32SrcWidth;
	m_pEncParam->tPreProcessParam.l32SrcUVStride = m_pEncParam->tPreProcessParam.l32SrcYStride >> 1;

	m_pEncParam->tPreProcessParam.l32DstWidth = tPicture.picWidth;
	m_pEncParam->tPreProcessParam.l32DstHeight = tPicture.picHeight;

	m_pEncParam->tPreProcessParam.l32DstYStride = m_pEncParam->tPreProcessParam.l32DstWidth;
	m_pEncParam->tPreProcessParam.l32DstUVStride = m_pEncParam->tPreProcessParam.l32DstYStride >> 1;

	mu_int32 l32Result;
	l32Result = ::VideoUnitEncoderSetParam( this->m_pvHandle, this->m_pEncParam );
	if( VIDEO_SUCCESS != l32Result )
	{
		printf( "VideoUnitEncoderSetParam fail!\n" );
		PrintErrorInfo( m_pvHandle, l32Result );
		return FALSE;
	}

	// 获取编码需要空间，申请内存。
	mu_int32 l32MemSize = this->GetMemSize( tPicture );
	this->m_pDataBuf = (mu_uint8 *)mu_fast_realloc( this->m_pDataBuf, &m_nDataBufLen, l32MemSize );
	memset( m_pDataBuf, 0, m_nDataBufLen );
	
	// 编码。

	// 将要编码的图片copy进图片缓冲区。
	mu_int32 nPicSize = tPicture.linesize[0] + tPicture.linesize[1] + tPicture.linesize[2] + tPicture.linesize[3];
	this->m_pPicBuf = (mu_uint8 *)mu_fast_realloc( this->m_pPicBuf, &m_nPicBufLen, nPicSize );
	memset( m_pPicBuf, 0, m_nPicBufLen );
	int nCopyCout = 0;
	for ( int i=0; i<4; ++i )
	{
		memcpy( m_pPicBuf + nCopyCout, tPicture.data[i], tPicture.linesize[i] );
		nCopyCout += tPicture.linesize[i];
	}

	// 准备编码输入参数。
	TEncInput tIn;
	memset( &tIn, 0, sizeof( tIn ) );
	tIn.pu8YUV = m_pPicBuf;
	tIn.u32EncSetKeyFrame = 1;
	
	// 编码输出参数。
	TEncOutput tOut;
	memset( &tOut, 0, sizeof( tOut ) );
	tOut.pu8EncBitstream = m_pDataBuf;
	tOut.l32EncBitstreamLen = m_nDataBufLen;
	l32Result = ::VideoUnitEncoderProcess( this->m_pvHandle, &tIn, &tOut );
	if( VIDEO_SUCCESS != l32Result )
	{
		printf( "VideoUnitEncoderProcess fail!\n" );
		PrintErrorInfo( m_pvHandle, l32Result );
		return FALSE;
	}

	if ( nBufLen < m_nDataBufLen )
	{
		return FALSE;
	}

	nBufLen = tOut.l32EncBitstreamLen;
	memcpy( pStramBuf, tOut.pu8EncBitstream, nBufLen );

	return TRUE;
}

mu_uint32 CKDVEncoderImpl::GetMemSize( const CBaseCodec::TVideoPicture& tVideoPic )
{
	return 100 << 10;

	mu_int32 l32Result = VIDEO_SUCCESS;
	// 获取编码需要空间，申请内存。
	mu_int32 l32MemSize = 100 << 10;	// 暂时定为100k
	//	l32Result = VideoUnitEncoderGetMemSize( this->m_pEncParam, &l32MemSize );
	if( VIDEO_SUCCESS != l32Result )
	{
		printf( "VideoUnitEncoderGetMemSize fail!\n" );
		PrintErrorInfo( m_pvHandle, l32Result );
		return FALSE;
	}
}

BOOL CKDVEncoderImpl::Release()
{
	SCOPE_LOCK( m_threadSafeLock );

	//解码结束
	if ( m_pvHandle )
	{
		VideoUnitEncoderClose( m_pvHandle );
	}

	mu_freep( &this->m_pEncParam );

	mu_freep( &this->m_pDataBuf );
	m_nDataBufLen = 0;

	mu_freep( &this->m_pPicBuf );
	m_nPicBufLen = 0;


	return TRUE;
}

void CKDVEncoderImpl::PrintErrorInfo(void * pvHandle, mu_int32 l32ResultForAnalyze)
{
	char strErrorText[256];
	l32 l32Result;

	l32Result = VideoUnitEncoderAnalyzeErrorCode(pvHandle, l32ResultForAnalyze, sizeof(strErrorText), strErrorText);
	if(l32Result != VIDEO_SUCCESS)
	{
		sprintf(strErrorText, "unknown ErrCode:0x%x\r\n", l32ResultForAnalyze);
	}
	printf(" ErrorInfo : %s\r\n", strErrorText);
}