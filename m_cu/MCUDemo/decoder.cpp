#include "StdAfx.h"
#include "Decoder.h"

#include "h264decoder.h"
#include "mpeg4decoder.h"

/** 静态变量定义及初始化. */
CDecoder::TDecoderNameTable CDecoder::s_tDecoderNameTable;
CSDLMutex CDecoder::s_decoderNameTableLock;

CDecoder::PictrueCallback CDecoder::m_pPicCallBack = NULL;
void * CDecoder::m_picCallbackParam = NULL;

CDecoder::CDecoder(void)
{
}

CDecoder::~CDecoder(void)
{
	SCOPE_LOCK( s_decoderNameTableLock );
}

BOOL CDecoder::SetPictueCallback( PictrueCallback pFun, void *param )
{	
	SCOPE_LOCK( s_decoderNameTableLock );

	m_pPicCallBack = pFun;
	m_picCallbackParam = param;
	return TRUE;
}

CDecoder * CDecoder::CreateDecoder( ECodecId eCodecId, int nBandWidth, LPCTSTR strName /* = NULL */ )
{
	CDecoder *pDec = NULL;
	switch( eCodecId )
	{
	case CODEC_H264:
		pDec = new CH264Decoder();
		break;
	case CODEC_MPEG4:
		pDec = new CMpeg4Decoder(); 
		break;	
	default:
		mcu::tlog << _T( "unknown codec! " ) << endl;
		_ASSERT( FALSE );
		return 0;
	    break;
	}

	BOOL bResult = pDec->Init( nBandWidth );
	if ( !bResult )
	{
		mcu::tlog << _T( "decoder Init Fail! bandwidth: " ) << nBandWidth << endl;
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

void CDecoder::Release( CDecoder *pDecoder )
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

CDecoder *CDecoder::FindDecoder( LPCTSTR strName )
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

void CDecoder::ReleaseAll()
{
	SCOPE_LOCK( s_decoderNameTableLock );
	for ( TDecoderNameTable::iterator iter = s_tDecoderNameTable.begin(); iter != s_tDecoderNameTable.end(); ++iter )
	{
		delete iter->second;
		iter->second = NULL;
	}
	s_tDecoderNameTable.empty();
}



void CDecoder::RegDec( LPCTSTR lpName, CDecoder *pDec )
{
	SCOPE_LOCK( s_decoderNameTableLock );

	if ( lpName && pDec )
	{
		s_tDecoderNameTable[ lpName ]  = pDec;
	}		
}

void CDecoder::OnDecodeResult( BOOL bSuccess, TVideoPicture *pPic, TVideoFrameInfo* pFrameInfo )
{
	SCOPE_LOCK( s_decoderNameTableLock );
	if ( bSuccess && this->m_pPicCallBack )
	{
		m_pPicCallBack( pPic, pFrameInfo, this->m_picCallbackParam );
	}
}

BOOL CDecoder::CapturePic( LPCTSTR strPicPath )
{
	if ( NULL == strPicPath )
	{
		 return FALSE;
	}

	LPCTSTR CAPTURE_ENCODER_NAME = _T( "cap_jpeg_enc" );
	CEncoder *pEnc = CEncoder::FindEncoder( CAPTURE_ENCODER_NAME );
	if ( NULL == pEnc )
	{
		pEnc = CEncoder::CreateEncoder( CODEC_JPEG, CAPTURE_ENCODER_NAME );
	}

	if ( pEnc )
	{
		return this->CapturePic( strPicPath, pEnc );
	}	
		
	return FALSE;
}