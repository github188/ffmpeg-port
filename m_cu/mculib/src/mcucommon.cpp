
#include "mcucommon.h"
#include <algorithm>
#include "log.h"

#ifdef __GNUC__
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

#ifdef __SYMBIAN32__
#include <utf.h>
#endif



string UTF16toUTF8( wstring strUTF16 )
{
if ( strUTF16.empty() )
{
	return "";
}
#if defined( _WIN32_WCE )
	int nLen8 = WideCharToMultiByte( CP_UTF8, 0, strUTF16.c_str(), strUTF16.length(), 0, 0, 0, 0 );
	if ( 0 == nLen8 )
	{
		return "";
	}
	std::vector<char> vTemp( nLen8+1, 0 );
	WideCharToMultiByte( CP_UTF8, 0, strUTF16.c_str(), strUTF16.length(), &vTemp[0], nLen8, 0, 0 );

	return &vTemp[0];
#elif defined( __SYMBIAN32__ )
	int nMaxLen8 = 2 * strUTF16.length() ;
	
	std::vector< TUint8 > vTmp( nMaxLen8 + 1, 0 );
	
	TPtr8 pUtf8( &vTmp[0], vTmp.size() );
	//pUtf8.AppendFill( 0, nMaxLen8 + 1 );
	TPtrC16 pUtf16( (TUint16*)strUTF16.c_str() );
	
	CnvUtfConverter::ConvertFromUnicodeToUtf8( pUtf8, pUtf16 );
	
	string strRet = (char*)&vTmp[0];
	return strRet;
#endif 
}



wstring UTF8toUTF16( string strUTF8 )
{
	if ( strUTF8.empty() )
	{
		return L"";
	}

	std::vector< wchar_t > vTemp( strUTF8.length()+1, 0 );
	
#if defined ( _WIN32_WCE )
	MultiByteToWideChar( CP_UTF8, 0, strUTF8.c_str(), strUTF8.length(), &vTemp[0], strUTF8.length() );
#elif defined( __SYMBIAN32__ )
	TPtr16 pUtf16( (TUint16*)&vTemp[0], vTemp.size() );
	TPtrC8 pUtf8( (TUint8*)strUTF8.c_str() );
	
	CnvUtfConverter::ConvertToUnicodeFromUtf8 ( pUtf16, pUtf8 );
#endif
	return &vTemp[0];
}

#ifdef _WIN32_WCE
tstring GetModulePath( HMODULE hm /*= NULL*/ )
{
	std::vector< wchar_t > vTemp( MAX_PATH+1, 0 );
	::GetModuleFileName( hm, &vTemp[0], MAX_PATH );

	return &vTemp[0];
}
#endif

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

	tstring strAppPath = strPath;

#if defined( _WIN32_WCE ) || defined( __SYMBIAN32__ )
	const TCHAR chDirToken = '\\';
	const TCHAR chWrongDirToken = '/';
#else
	const TCHAR chDirToken = '/';
	const TCHAR chWrongDirToken = '\\';
#endif

	replace( strAppPath.begin(), strAppPath.end(), chWrongDirToken, chDirToken );


	int nIndex = strAppPath.rfind( chDirToken );
	if( nIndex >=0 )
	{	
		tFileNameInfo.m_strFileName = strAppPath.substr( nIndex + 1 );
		tFileNameInfo.m_strDirectory = strAppPath.substr( 0, nIndex + 1 );
	}	
	else
	{
		tFileNameInfo.m_strFileName = strAppPath;
		tFileNameInfo.m_strDirectory = _T( "" );
	}

	tstring strAppName = tFileNameInfo.m_strFileName;
	nIndex = strAppName.rfind( '.' );
	if( nIndex >= 0 )
	{
		tFileNameInfo.m_strBaseName = strAppName.substr( 0, nIndex );
		tFileNameInfo.m_strExtName = strAppName.substr( nIndex + 1);
	}
	else
	{
		tFileNameInfo.m_strBaseName = strAppName;
		tFileNameInfo.m_strExtName = _T( "" );
	}
	return TRUE;
}


tstring GetAppName()
{
#ifdef _WIN32_WCE
	return ParsePath( GetModulePath().c_str() ).m_strBaseName;
#else
	return _T( "" );
#endif
}

tstring GetAppDir()
{
#ifdef _WIN32_WCE
	return ParsePath( GetModulePath().c_str() ).m_strDirectory;
#else
	return _T( "" );
#endif
}

#ifdef _WIN32_WCE

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
#endif
__time64_t StrToTime( LPCTSTR lpTime )
{
#if defined( _WIN32_WCE )
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
#elif defined( __SYMBIAN32__ )
	return 0;
#endif
	
	
}




BOOL HasStorageCard()
{
#ifdef _WIN32_WCE

	LPCTSTR strCardPath = _T( "\\Storage Card\\" );
#elif defined( __SYMBIAN32__ )
	LPCTSTR strCardPath = _T( "e:\\" );
#else
	LPCTSTR strCardPath = _T( "/media/mmc1" );
#endif
	return IsFileExist( strCardPath );
}

BOOL IsFileExist( LPCTSTR strPath )
{
#ifdef _WIN32
	DWORD dwFA = ::GetFileAttributes( strPath );
	return dwFA != -1;
#else	
	BOOL bExist = ( _taccess( strPath, 0 ) == 0 ); 	
	return bExist;
#endif
}

BOOL MakeDir( LPCTSTR strDir )
{
#ifdef _WIN32_WCE
	return CreateDirectory( strDir, NULL );
#else
	return ( 0 == _tmkdir( strDir, S_IRWXU ) );
#endif
}



BOOL DelFile( LPCTSTR strFile )
{
#ifdef _WIN32_WCE
	return ::DeleteFile( strFile );
#else
	int nResult = _tunlink( strFile );
	return ( 0 == nResult );
#endif
}

int CompareNoCase(const string& s, const string& s2)
{
	string::const_iterator   p   =   s.begin();   
	string::const_iterator   p2   =   s2.begin();   
	while(p!=s.end()   &&   p2!=s2.end())   
	{   
		if(toupper(*p)!=toupper(*p2))   
		{
			return   ( toupper(*p) < toupper(*p2) )  ?   -1:1;   
		}
		++p;   
		++p2;   
	}   
	return   (s2.size()   ==   s.size())?0:(s.size()<s2.size())?-1:1;   
}

int CompareNoCase(const wstring&   s,   const   wstring&   s2)
{
	wstring::const_iterator   p   =   s.begin();   
	wstring::const_iterator   p2   =   s2.begin();   
	while(p!=s.end()   &&   p2!=s2.end())   
	{   
		if(toupper(*p)!=toupper(*p2))   
		{
			return   ( toupper(*p) < toupper(*p2) )  ?   -1:1;   
		}
		++p;   
		++p2;   
	}   
	return   (s2.size()   ==   s.size())?0:(s.size()<s2.size())?-1:1;   
}

mu_uint64 GetTick()
{
#ifdef _WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday( &tv, NULL );
	return (mu_uint64)tv.tv_sec * 1000000 + tv.tv_usec;
#endif
}

__time64_t GetCurTime()
{	
#ifdef _WIN32
	return _time64( NULL );
#else
	return time( NULL );
#endif	
}

tstring TimeToStr( __time64_t nTime )
{
	tstring strTimeResult;

	tm localTime;
	if( GetLocalTime( nTime, &localTime ) )
	{
		tstringstream ssTime;
		ssTime << setfill( _T( '0' ) ) << setw(4) << ( localTime.tm_year + 1900 );
		ssTime << setfill( _T( '0' ) ) << setw(2) << localTime.tm_mon + 1 <<setfill( _T( '0' ) ) <<  setw(2) << localTime.tm_mday;
		ssTime << setfill( _T( '0' ) ) << setw(2) << localTime.tm_hour << setfill( _T( '0' ) ) <<  setw(2) << localTime.tm_min 
            << setfill( _T( '0' ) ) << setw(2) << localTime.tm_sec;
		strTimeResult = ssTime.str();
	}
	return strTimeResult;
}

BOOL GetLocalTime( const time_t time, tm * plocalTime )
{
	BOOL bResult;
#ifdef _WIN32
	bResult = ( 0 == _localtime64_s( plocalTime, &time ) );
#else
	tm *ptm = localtime( &time );
	if( ptm )
	{
		*plocalTime = *ptm;
	}
	bResult = ( NULL != ptm );
#endif
	return bResult;
}

TFileList EnumAllFile( LPCTSTR strFolder )
{
	TFileList tFiles;
#ifdef _WIN32
#elif !defined( __SYMBIAN32__ )
	DIR * pDir = opendir( strFolder );
	if( NULL != pDir )
	{
		dirent tDirEnt;
		dirent *pFileResult;
//		Log() << _T( "Open dir and enum all file: " ) << strFolder  << endl;
		while( TRUE )
		{			
			int nResult = readdir_r( pDir, &tDirEnt, &pFileResult );
			if( pFileResult == NULL )
			{
				break;
			}
			tstring strFilePath = strFolder;
			if( strFilePath.length() > 1 && strFilePath[ strFilePath.length() - 1 ] != '/' ) 
			{
				strFilePath += "/";
			}
			strFilePath += pFileResult->d_name;
			tFiles.push_back( strFilePath );
//			Log() << _T( "find file: " ) << pFileResult->d_name << _T( " i-num: " ) << pFileResult->d_ino << endl;
		}

		closedir( pDir );
	}
	else
	{
		Log() << _T( "EnumAllFile open dir fail! dir: " ) << strFolder << endl;
	}

#endif
	return tFiles;
}


void NormalizeDir( tstring& strFolder )
{
    if ( strFolder.empty() )
    {
        return;
    }

#if defined( _WIN32_WCE )
    const TCHAR chDirToken = '\\';
    const TCHAR chWrongDirToken = '/';
#else
    const TCHAR chDirToken = '/';
    const TCHAR chWrongDirToken = '\\';
#endif

    replace( strFolder.begin(), strFolder.end(), chWrongDirToken, chDirToken );

    if ( strFolder.at( strFolder.size() - 1 ) != chDirToken )
    {
        strFolder += chDirToken;
    }

}

tstring StringToUrl( LPCTSTR strStr )
{
    tstringstream ssRet;
    if ( NULL == strStr )
    {
        return ssRet.str();
    }

    while( *strStr )
    {
        TCHAR nChar = /*ntohs*/( *strStr );
        if ( (mu_uint16)nChar > 127 )
        {
            // 中文。
 //           ssRet << nChar;
            
            mu_uint8 cHigh = 0xff & ( nChar >> 8 );
            mu_uint8 cLow = 0xff & nChar;

            ssRet << _T( "%" ) << setbase( 16 ) << (unsigned int)(cLow) ;
            ssRet << _T( "%" ) << setbase( 16 ) << (unsigned int)(cHigh);

        }
        else
        {
            // 英文特殊符号。
            switch( nChar )
            {
                case _T( '#' ):
                    ssRet << _T( "%23" );
            	break;
                case _T( '%' ):
                    ssRet << _T( "%25" );
                break;
                case _T( '&' ):
                    ssRet << _T( "%26" );
                break;
                case _T( '+' ):
                    ssRet << _T( "%2b" );
                break;
                case _T( ' ' ):
                    ssRet << _T( "%20" );
                break;
                case _T( '\n' ):
                    ssRet << _T( "\0a" );
                break;
                
            default:
                ssRet << nChar;
                break;
            }


        }

        ++strStr;
    }

    return ssRet.str();
}

mu_uint64 GetDirFreeSpace( LPCTSTR strDirPath )
{
#if defined( _WIN32_WCE )
    ULARGE_INTEGER nFreeSpaceToCaller, nTotalSpace, nFreeSpace;

    BOOL bResult = ::GetDiskFreeSpaceEx( strDirPath, &nFreeSpaceToCaller, &nTotalSpace, &nFreeSpace );
    if ( bResult )
    {
        return nFreeSpaceToCaller.QuadPart;
    }
    else
    {
        Log() << _T( "GetDiskFreeSpaceEx Fail! " ) << endl;
        return 0;
    }
#endif
}

