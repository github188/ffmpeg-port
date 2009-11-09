
#include "stdafx.h"
#include "mcucommon.h"




string UTF16toUTF8( wstring strUTF16 )
{
	if ( strUTF16.empty() )
	{
		return "";
	}

	int nLen8 = WideCharToMultiByte( CP_UTF8, 0, strUTF16.c_str(), strUTF16.length(), 0, 0, 0, 0 );
	if ( 0 == nLen8 )
	{
		return "";
	}

	std::vector<char> vTemp( nLen8+1, 0 );
	WideCharToMultiByte( CP_UTF8, 0, strUTF16.c_str(), strUTF16.length(), &vTemp[0], nLen8, 0, 0 );

	return &vTemp[0];
}

wstring UTF8toUTF16( string strUTF8 )
{
	if ( strUTF8.empty() )
	{
		return L"";
	}

	std::vector< wchar_t > vTemp( strUTF8.length()+1, 0 );
	MultiByteToWideChar( CP_UTF8, 0, strUTF8.c_str(), strUTF8.length(), &vTemp[0], strUTF8.length() );

	return &vTemp[0];
}

tstring GetModulePath( HMODULE hm /*= NULL*/ )
{
	std::vector< wchar_t > vTemp( MAX_PATH+1, 0 );
	::GetModuleFileName( hm, &vTemp[0], MAX_PATH );

	return &vTemp[0];
}

bool TFileNameInfo::operator ==( const TFileNameInfo& tAnother ) const
{
	if ( this == &tAnother )
	{
		return true;
	}
	return( tAnother.m_strFilePath == this->m_strFilePath 
		&& tAnother.m_strFileName == this->m_strFileName
		&& tAnother.m_strExtName == this->m_strExtName
		&& tAnother.m_strDirectory == this->m_strDirectory
		&& tAnother.m_strBaseName == this->m_strBaseName );
}


TFileNameInfo ParsePath( LPCTSTR strPath )
{
	TFileNameInfo tInfo;
	ParsePath( strPath, tInfo );
	return tInfo;
}

BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo )
{
	tFileNameInfo.m_strFilePath = strPath;

	CString strAppPath = strPath;

	strAppPath.Replace( _T( '/' ), _T( '\\' ) );

	int nIndex = strAppPath.ReverseFind( _T( '\\' ) );
	if( nIndex >=0 )
	{
		tFileNameInfo.m_strFileName = (LPCTSTR)strAppPath.Right( strAppPath.GetLength() - nIndex - 1 );
		tFileNameInfo.m_strDirectory = (LPCTSTR)strAppPath.Left( nIndex + 1 );
	}	
	else
	{
		tFileNameInfo.m_strFileName = (LPCTSTR)strAppPath;
		tFileNameInfo.m_strDirectory = _T( "" );
	}

	CString strAppName = tFileNameInfo.m_strFileName.c_str();
	nIndex = strAppName.ReverseFind( _T( '.' )  );
	if( nIndex >= 0 )
	{
		tFileNameInfo.m_strBaseName = (LPCTSTR)strAppName.Left( nIndex );
		tFileNameInfo.m_strExtName = (LPCTSTR)strAppName.Right( strAppName.GetLength() - nIndex - 1 );
	}
	else
	{
		tFileNameInfo.m_strBaseName = (LPCTSTR)strAppName;
		tFileNameInfo.m_strExtName = _T( "" );
	}
	return TRUE;
}

tstring GetAppName()
{
	return ParsePath( GetModulePath().c_str() ).m_strBaseName;
}

tstring GetAppDir()
{
	return ParsePath( GetModulePath().c_str() ).m_strDirectory;
}

int GetScreenLong()
{
	int nScreenWidth = ::GetScreenWidth();
	int nScreenHeight = ::GetScreenHeight();
	int nScreenLong = max( nScreenWidth, nScreenHeight );
	return nScreenLong;
}

int GetScreenShort()
{
	int nScreenWidth = ::GetScreenWidth();
	int nScreenHeight = ::GetScreenHeight();
	return min( nScreenWidth, nScreenHeight );
}

int GetScreenWidth()
{
	return ::GetSystemMetrics( SM_CXSCREEN );
}

int GetScreenHeight()
{
	return ::GetSystemMetrics( SM_CYSCREEN );
}

/* char -> wchar_t */
BOOL MBtoWC(const char* srcStr, wchar_t * dstStr, int len )
{
	int length;

	length = MultiByteToWideChar(CP_ACP, 0, 
		srcStr, -1, NULL, 0);

	if ( length < len )
	{
		MultiByteToWideChar(CP_ACP, 0,
			srcStr, -1, dstStr, length);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* wchar_t -> char */
BOOL WCtoMB(const wchar_t* srcStr, char *dstStr, int len )
{
	int charlength;

	charlength = WideCharToMultiByte(CP_ACP, 0, srcStr,
		-1, NULL, 0, NULL, NULL);

	if( charlength < len )
	{
		WideCharToMultiByte(CP_ACP, 0, srcStr,
			-1, dstStr, charlength, NULL, NULL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

tstring TimeToStr( __time64_t nTime )
{
	CTime time( nTime );
	CString strTime;// = now.Format( _T( "%04Y%02M%02d%02H%02m%02S.jpg" ) );

	//tm localTime;
	//time.GetLocalTm( &localTime );
	

	strTime.Format( _T( "%04d%02d%02d%02d%02d%02d" ), time.GetYear(), time.GetMonth(), 
		time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

	return (LPCTSTR)strTime;

}

__time64_t StrToTime( LPCTSTR lpTime )
{
	CString strTime = lpTime;
	SYSTEMTIME tTime;
	memset( &tTime, 0, sizeof( tTime ) );

	if ( strTime.GetLength() >= 4 )
	{
		CString strYear = strTime.Left( 4 );
		tTime.wYear = (WORD)_ttoi( strYear );

		strTime = strTime.Right( strTime.GetLength() - 4 );
	}

	if ( strTime.GetLength() >= 2 )
	{
		CString strTemp = strTime.Left( 2 );
		tTime.wMonth = (WORD)_ttoi( strTemp );

		strTime = strTime.Right( strTime.GetLength() - 2 );
	}

	if ( strTime.GetLength() >= 2 )
	{
		CString strTemp = strTime.Left( 2 );
		tTime.wDay = (WORD)_ttoi( strTemp );

		strTime = strTime.Right( strTime.GetLength() - 2 );
	}

	if ( strTime.GetLength() >= 2 )
	{
		CString strTemp = strTime.Left( 2 );
		tTime.wHour = (WORD)_ttoi( strTemp );

		strTime = strTime.Right( strTime.GetLength() - 2 );
	}

	if ( strTime.GetLength() >= 2 )
	{
		CString strTemp = strTime.Left( 2 );
		tTime.wMinute = (WORD)_ttoi( strTemp );

		strTime = strTime.Right( strTime.GetLength() - 2 );
	}

	if ( strTime.GetLength() >= 2 )
	{
		CString strTemp = strTime.Left( 2 );
		tTime.wSecond = (WORD)_ttoi( strTemp );

		strTime = strTime.Right( strTime.GetLength() - 2 );
	}

	if( ( tTime.wYear >= 1900 ) && ( tTime.wMonth >= 1 && tTime.wMonth <= 12 ) && ( tTime.wDay >= 1 && tTime.wDay <= 31 ) 
		&& ( tTime.wHour >= 0 && tTime.wHour <= 23 ) && ( tTime.wMinute >= 0 && tTime.wMinute <= 59 ) 
		&& ( tTime.wSecond >= 0 && tTime.wSecond <= 59 ) )
	{
		try
		{
			return CTime( tTime ).GetTime();
		}
		catch (CException* e)
		{
			e;
			return 0;
		}
		catch( ... )
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	
	
}

BOOL HasStorageCard()
{
	LPCTSTR strCardPath = _T( "\\Storage Card\\" );

	// ÊÇ·ñ´æÔÚ´æ´¢¿¨¡£
	DWORD dwFA = ::GetFileAttributes( strCardPath );
	
	return dwFA != -1;
}