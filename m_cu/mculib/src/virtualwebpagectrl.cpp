#include "virtualwebpagectrl.h"
#include "urlparse.h"
#include "mcusession.h"
#include "mcucommon.h"
#include "mcuconfig.h"

CVirtualWebpageCtrl::CVirtualWebpageCtrl(void)
{
}

CVirtualWebpageCtrl::~CVirtualWebpageCtrl(void)
{
}

CVirtualWebpageCtrl::EUrlType CVirtualWebpageCtrl::OnLoadPage( LPCTSTR strUrl )
{
	EUrlType eType = UrlNormal;

	if( NULL == strUrl )
	{
		return eType;
	}

	// ����URL��
	CUrlParse urlParser;
	urlParser.Parse( strUrl );
	tstring strPtzAddr, strPtzPort, strStreamType, strPuId, strPuName;
	int bPtzControl;
	urlParser.ParamValue( _T( "VauPtzAdd" ), strPtzAddr );
	urlParser.ParamValue( _T( "VauPtzPort" ), strPtzPort );
	urlParser.ParamValue( _T( "StreamingType" ), strStreamType );
	urlParser.ParamValue( _T( "PuId-ChannelNo" ), strPuId );
	urlParser.ParamValue( _T( "PuProperty" ), bPtzControl );
	urlParser.ParamValue( _T( "PuName" ), strPuName );


	if ( !strPtzAddr.empty() && !strPtzPort.empty() && !strPuId.empty() )
	{
		Log() << _T( "Is url with Param! " ) << strUrl << endl;
		CMCUSession::Instance()->CurVideoSession()->PtzIP( strPtzAddr );
		tstringstream ssTmp;
		ssTmp << strPtzPort;
		int nPort = 0;
		ssTmp >> nPort;
		CMCUSession::Instance()->CurVideoSession()->PtzPort( nPort );
		CMCUSession::Instance()->CurVideoSession()->PuId( strPuId );
		
		int nStreamType = 0;
		ssTmp.clear();
		ssTmp << strStreamType;
		ssTmp >> nStreamType;
		CMCUSession::Instance()->CurVideoSession()->StreamType( (EStreamType)nStreamType );
		CMCUSession::Instance()->CurVideoSession()->PuName( strPuName.c_str() );

		CMCUSession::Instance()->CurVideoSession()->PtzControl( bPtzControl );

		eType = UrlHttpWithParam;
	}	
	else
	{
//		Log() << _T( "Not right url! " ) << strUrl << endl;
	}


	ctstring strRtspUrlToken = _T( "rtsp://" );
	tstring strRtspUrl = strUrl;
	
	if ( 0 == CompareNoCase( strRtspUrlToken, strRtspUrl.substr( 0, strRtspUrlToken.length() )) )
	{
		// ��rtsp��
		Log() << _T( "rtsp url: " ) << strUrl << endl;

		eType = UrlRtsp;

		CMCUSession::Instance()->CurVideoSession()->RtspUrl( strUrl );

		this->OpenPlayer( CMCUSession::Instance()->CurVideoSession() );

		int nHistoryBackStep;
		CConfig::Instance()->GetWebpageHistoryBackStep( nHistoryBackStep );
		this->HistoryBack( nHistoryBackStep );
	}

	return eType;
	//if ( strUrl.Left( _tcslen( lpRtspUrlToken ) ).CompareNoCase( lpRtspUrlToken ) == 0 );
}

float CVirtualWebpageCtrl::GetZoomLevel() const
{
	float fResult = 1.0;
	CConfig::Instance()->GetWebpageZoomLevel( fResult );
	return fResult;
}

