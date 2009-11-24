#include "virtualloginwnd.h"
#include "mcuconfig.h"
#include "mcusession.h"

CVirtualLoginWnd::CVirtualLoginWnd( )
{
}

CVirtualLoginWnd::~CVirtualLoginWnd( )
{
}

BOOL CVirtualLoginWnd::Login( const TLoginInfo& tLoginInfo, ELoginType eLoginType )
{
	ELoginInfoErrorCode eErrCode = this->CheckInvalid( tLoginInfo, eLoginType );
	if( NoError != eErrCode )
	{
		this->AssertError( eErrCode );
		return FALSE;
	}

	tstringstream ssServerFullUrl;
	tstring strServer;
	CConfig::Instance()->GetServer( strServer );

	ssServerFullUrl << strServer;

	tstring strRegionId;
	CConfig::Instance()->GetRegionId( strRegionId );
	tstringstream ssAccount;
	ssAccount << tLoginInfo.m_strUserId << _T( "@" ) << strRegionId;

	ssServerFullUrl << _T( "?" );
	ssServerFullUrl << _T( "Account=" ) << ssAccount.str() << _T( "&" );
	ssServerFullUrl << _T( "Password=" ) << tLoginInfo.m_strPassword << _T( "&" );
	ssServerFullUrl << _T( "StreamingType=" ) << int( tLoginInfo.m_eStreamType )  << _T( "&" );
	ssServerFullUrl << _T( "LoginType=" ) << (int)eLoginType;

	// ±£´æ.
	CMCUSession::Instance()->UserId( ssAccount.str() );
	CMCUSession::Instance()->Password( tLoginInfo.m_strPassword );

	this->SaveConfig( tLoginInfo );

	mcu::log << _T( "mculib login url: " ) << ssServerFullUrl.str() << endl;
	this->OpenWebpage( ssServerFullUrl.str().c_str() );

	return TRUE;
}

BOOL CVirtualLoginWnd::SaveConfig( const TLoginInfo& tLoginInfo )
{
	CConfig::Instance()->SetIsSave( tLoginInfo.m_bSaveLoginInfo );
	CConfig::Instance()->SetLoginInfo( tLoginInfo.m_strUserId.c_str(),
		tLoginInfo.m_strPassword.c_str(), tLoginInfo.m_eStreamType );
	return TRUE;
}

BOOL CVirtualLoginWnd::ReadConfig( TLoginInfo& tLoginInfo )
{
	BOOL bSaveLoginInfo;
	CConfig::Instance()->GetIsSave( bSaveLoginInfo );
	tLoginInfo.m_bSaveLoginInfo = bSaveLoginInfo;

	tstring strUser, strPw;
	EStreamType eStreamType;
	CConfig::Instance()->GetLoginInfo( strUser, strPw, eStreamType );

	tLoginInfo.m_strUserId = strUser;
	tLoginInfo.m_strPassword = strPw;
	tLoginInfo.m_eStreamType = eStreamType;
	return TRUE;
}

CVirtualLoginWnd::ELoginInfoErrorCode CVirtualLoginWnd::CheckInvalid( const TLoginInfo& tLoginInfo, ELoginType eLoginType )
{
	if( eLoginType == LOGIN_NORMAL )
	{
		if ( tLoginInfo.m_strUserId.empty() )
		{
			return ErrorUserIdEmpty;
		}
		if ( tLoginInfo.m_strPassword.empty() )
		{
			return ErrorPasswordEmpty;
		}

		tstring strRegion;
		CConfig::Instance()->GetRegionId( strRegion );
		if ( strRegion.empty() )
		{
			return ErrorRegionEmpty;
		}
	}
	
	
	tstring strServer;
	CConfig::Instance()->GetServer( strServer );
	if ( strServer.empty() )
	{
		return ErrorServerUrlEmpty;
	}



	return NoError;
}