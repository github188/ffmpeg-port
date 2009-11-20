
#include "encoder.h"
#include "jpegencoder.h"
#include "log.h"

CEncoder::TEncoderNameTable CEncoder::s_tEncoderNameTable;
CMCUMutex CEncoder::s_encoderNameTableLock;

CEncoder::CEncoder(void)
{
}

CEncoder::~CEncoder(void)
{
}

CEncoder * CEncoder::CreateEncoder( ECodecId eCodecId, LPCTSTR strName /* = NULL */ )
{
	CEncoder *pEec = NULL;
	switch( eCodecId )
	{
	case CODEC_JPEG:
		pEec = new CJpegEncoder();
		break;
	default:
		mcu::tlog << _T( "Unknown encoder!" ) << eCodecId << endl;
		_ASSERT( FALSE );
		return 0;
		break;
	}

	BOOL bResult = pEec->Init();
	if ( !bResult )
	{
		mcu::tlog << _T( "Encoder init Fail!" ) << endl;
	}
	
	_ASSERT( bResult );
	if ( !bResult )
	{
		Release( pEec );
		pEec = NULL;
	}

	RegDec( strName, pEec );


	return pEec;
}


void CEncoder::RegDec( LPCTSTR lpName, CEncoder *pEnc )
{
	SCOPE_LOCK( s_encoderNameTableLock );

	if ( lpName && pEnc )
	{
		s_tEncoderNameTable[ lpName ]  = pEnc;
	}		
}

void CEncoder::Release( CEncoder *pEncoder )
{
	SCOPE_LOCK( s_encoderNameTableLock );
	for ( TEncoderNameTable::iterator iter = s_tEncoderNameTable.begin(); iter != s_tEncoderNameTable.end(); ++iter )
	{
		if ( iter->second == pEncoder )
		{
			s_tEncoderNameTable.erase( iter );
			break;
		}
	}
	delete pEncoder;
	pEncoder = NULL;
}

CEncoder *CEncoder::FindEncoder( LPCTSTR strName )
{
	SCOPE_LOCK( s_encoderNameTableLock );
	TEncoderNameTable::iterator iter = s_tEncoderNameTable.find( strName );
	if ( iter != s_tEncoderNameTable.end() )
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}