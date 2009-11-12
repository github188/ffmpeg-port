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

	// Ω‚ŒˆURL°£
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


	if ( !strPtzAddr.empty() && !strPtzPort.empty() && !strStreamType.empty() && !strPuId.empty() )
	{
		mcu::tlog << _T( "Is url with Param! " ) << strUrl << endl;
		CMCUSession::Instance()->CurVideoSession()->PtzIP( strPtzAddr );
		CMCUSession::Instance()->CurVideoSession()->PtzPort( _ttoi( strPtzPort.c_str() ) );
		CMCUSession::Instance()->CurVideoSession()->PuId( strPuId );
		CMCUSession::Instance()->CurVideoSession()->StreamType( (EStreamType)_ttoi( strStreamType.c_str() ) );
		CMCUSession::Instance()->CurVideoSession()->PuName( strPuName );

		CMCUSession::Instance()->CurVideoSession()->PtzControl( bPtzControl );

		eType = UrlHttpWithParam;
	}	
	else
	{
//		mcu::tlog << _T( "Not right url! " ) << strUrl << endl;
	}


	ctstring strRtspUrlToken = _T( "rtsp://" );
	tstring strRtspUrl = strUrl;
	
	if ( 0 == CompareNoCase( strRtspUrlToken, strRtspUrl.substr( 0, strRtspUrlToken.length() )) )
	{
		//  «rtsp°£
		mcu::tlog << _T( "rtsp url: " ) << strUrl << endl;

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

