#include "virtualconfigwnd.h"
#include "mcuconfig.h"
#include "log.h"
#include "mcucommon.h"


CVirtualConfigWnd::CVirtualConfigWnd(void)
{
}

CVirtualConfigWnd::~CVirtualConfigWnd(void)
{
}

BOOL CVirtualConfigWnd::ReadConfig( TConfigInfo& tConfInfo )
{
	BOOL bResult = TRUE;
	bResult &= CConfig::Instance()->GetServer( tConfInfo.m_strServerUrl );
	bResult &= CConfig::Instance()->GetRegionId( tConfInfo.m_strRegion );
	bResult &= CConfig::Instance()->GetCaptureDir( tConfInfo.m_strSavePath );
	bResult &= CConfig::Instance()->GetIsCaptureDirCustom( tConfInfo.m_bCustomSavePath );
	bResult &= CConfig::Instance()->GetPtzSpeed( tConfInfo.m_nPtzSpeed );
	bResult &= CConfig::Instance()->GetLenSpeed( tConfInfo.m_nLenSpeed );

	tstring strPicFolder = tConfInfo.m_strSavePath; 
	if ( !strPicFolder.empty() )
	{
		if ( !IsFileExist( strPicFolder.c_str() ) )
		{
			MakeDir( strPicFolder.c_str() );
			if( !IsFileExist( strPicFolder.c_str() ) )
			{
				mcu::tlog << _T( "CVirtualConfigWnd::ReadConfig MakeDir failed. folder: " ) << strPicFolder << endl;
				tConfInfo.m_strSavePath.clear();
			}
			else
			{
				mcu::tlog << _T( "CVirtualConfigWnd::ReadConfig makedir :" ) << strPicFolder << endl;
			}
		}
	}

	return bResult;
}

BOOL CVirtualConfigWnd::SaveConfig( const TConfigInfo& tConfInfo )
{
	BOOL bResult = TRUE;
	bResult &= CConfig::Instance()->SetServer( tConfInfo.m_strServerUrl.c_str() );
	bResult &= CConfig::Instance()->SetReginId( tConfInfo.m_strRegion.c_str() );
	bResult &= CConfig::Instance()->SetCaptureDir( tConfInfo.m_strSavePath.c_str() );
	bResult &= CConfig::Instance()->SetIsCaptureDirCustom( tConfInfo.m_bCustomSavePath );
	bResult &= CConfig::Instance()->SetPtzSpeed( tConfInfo.m_nPtzSpeed );
	bResult &= CConfig::Instance()->SetLenSpeed( tConfInfo.m_nLenSpeed );

	return bResult;
}


