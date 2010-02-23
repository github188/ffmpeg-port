
#include "mcuconfig.h"

#include "mcucommon.h"
#include "encrypt.h"
#include "log.h"


CConfig *CConfig::s_instance = 0;

/** 相关常量定义。 */
static LPCTSTR CONFIG_FILE_NAME = _T( "mcucfg.xml" );
static LPCTSTR CONFIG_BK_FILE = _T( "mcucfgbk.xml" );
static LPCTSTR CONFIG_SECTION_CONFIG_NAME = _T( "config" );

static LPCTSTR CONFIG_ENTRY_SERVER_URL = _T( "server" );
static LPCTSTR CONFIG_ENTRY_USER = _T( "user" );
static LPCTSTR CONFIG_ENTRY_PW = _T( "password" );
static LPCTSTR CONFIG_ENTRY_STREAMTYPE = _T( "stream" );
static LPCTSTR CONFIG_ENTRY_LOGIN_DLG = _T( "logindlg" );
static LPCTSTR CONFIG_ENTRY_IS_SAVE = _T( "issave" );
static LPCTSTR CONFIG_ENTRY_REGION = _T( "region" );
static LPCTSTR CONFIG_ENTRY_PTZ_SPEED = _T( "ptzspeed" );
static LPCTSTR CONFIG_ENTRY_LEN_SPEED = _T( "lenspeed" );
static LPCTSTR CONFIG_ENTRY_CAPTURE_DIR = _T( "capturedir" );
static LPCTSTR CONFIG_ENTRY_CAPTURE_DIR_TYPE = _T( "capturedirtype" );
static LPCTSTR CONFIG_ENTRY_APP_TITLE = _T( "apptitle" );
static LPCTSTR CONFIG_ENTRY_MIN_SPACE = _T( "minspace" );
static LPCTSTR CONFIG_ENTRY_WEBPAGE_ZOOM_LEVEL = _T( "webpagezoom" );
static LPCTSTR CONFIG_ENTRY_WEBPAGE_HISTORY_BACK = _T( "webpagehistoryback" );
static LPCTSTR CONFIG_ENTRY_SEND_NAT_PACKET = _T( "sendnatpacket" );


static LPCTSTR CONFIG_DEFAULT_SERVER = _T( "" );
static LPCTSTR CONFIG_DEFAULT_REGION = _T( "" );
static const BOOL CONFIG_DEFAULT_LOGIN_DIALOG = TRUE;
static const BOOL CONFIG_DEFAULT_IS_SAVE = TRUE;
static const int CONFIG_DEFAULT_PTZ_SPEED = 5;
static const int CONFIG_DEFAULT_LEN_SPEED = 5;
static const int CONFIG_DEFAULT_WEBPAGE_ZOOM_LEVEL = 100;
static const int CONFIG_DEFAULT_WEBPAGE_HISTORY_BACK = 2;
static const BOOL CONFIG_DEFAULT_SEND_NAT_PACKET = TRUE;

#ifdef _WIN32_WCE
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_RAM = _T( "\\My Documents\\M_CU\\" );
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_CARD = _T( "\\Storage Card\\M_CU\\" );
#elif defined( __SYMBIAN32__)
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_RAM = _T( "c:\\M_CU\\pic\\" );
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_CARD = _T( "e:\\M_CU\\pic\\" );
#else
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_RAM = _T( "/media/mmc1/M_CU/" );
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_CARD = _T( "/media/mmc1/M_CU/" );
#endif

static const int CONFIG_DEFAULT_MIN_SPACE = MIN_STORAGE_SPACE;


CConfig::CConfig(void)
{
    this->m_pXmlParser = IXmlParser::CreateNew();

	tstring strXmlFile = this->GetDefaultConfigFilePath();
	BOOL bResult = this->m_pXmlParser->LoadXML( strXmlFile.c_str(), CONFIG_SECTION_CONFIG_NAME);

//	Log() << _T( "config load xml: " ) << strXmlFile << _T( " result: ") << bResult << endl;
	_ASSERT( bResult );

    m_pBkCfgxmlParser = NULL;
    this->m_pBkCfgxmlParser = IXmlParser::CreateNew();

    tstring strPresetFile = this->GetBkCfgFilePath();

    bResult = this->m_pBkCfgxmlParser->LoadXML( strPresetFile.c_str(), CONFIG_SECTION_CONFIG_NAME );
//    Log() << _T( "config load preset: " ) << strPresetFile << _T( " result: " ) << bResult << endl;

//	bResult = this->m_pXmlParser->SetCurRootElement( CONFIG_SECTION_CONFIG_NAME );
//	Log() << _T( "config set config root: result: ") << bResult << endl;
}

CConfig::~CConfig(void)
{
	if ( this->m_pXmlParser )
	{
		delete m_pXmlParser;
		m_pXmlParser = NULL;
	}
}

void CConfig::SetConfigFilePath( LPCTSTR strCfgFilePath )
{
	if ( m_pXmlParser )
	{
		this->m_pXmlParser->LoadXML( strCfgFilePath, CONFIG_SECTION_CONFIG_NAME );
	}
	else
	{
		Log() << _T( "SetConfigFilePath xml parse is null!" ) << endl;
	}
	
//	this->m_pXmlParser->SetCurRootElement( CONFIG_SECTION_CONFIG_NAME );
}

tstring CConfig::GetConfigFilePath( ) const
{
	if ( m_pXmlParser )
	{
		return this->m_pXmlParser->GetXMLPath();
	}
	else
	{
		Log() << _T( "GetConfigFilePath xml parse is null!" ) << endl;
		return _T( "" );
	}	
}

tstring CConfig::GetDefaultConfigFilePath() const
{
#ifdef _WIN32
	tstring strPath = ::GetAppDir();
	return ( strPath + CONFIG_FILE_NAME ).c_str();
#elif defined ( __SYMBIAN32__ )
	
	return _T( "e:\\mcu\\mcucfg.xml" );
#else
	tstring strPath = _T( "/usr/share/mcu/" );
	if( !IsFileExist( strPath.c_str() ) )
	{
		MakeDir( strPath.c_str() );
	}
	return  ( strPath + CONFIG_FILE_NAME ).c_str();
#endif
}

tstring CConfig::GetBkCfgFilePath() const
{
#ifdef _WIN32
    tstring strPath = ::GetAppDir();
    return ( strPath + CONFIG_BK_FILE ).c_str();
#elif defined ( __SYMBIAN32__ )
	
	return _T( "e:\\mcu\\mcucfgbk.xml" );
#else
    tstring strPath = _T( "/usr/share/mcu/" );
    if( !IsFileExist( strPath.c_str() ) )
    {
        MakeDir( strPath.c_str() );
    }
    return  ( strPath + CONFIG_BK_FILE ).c_str();
#endif
}


CConfig *CConfig::Instance()
{
	if ( s_instance == 0 )
	{
		s_instance = new CConfig();
	}
	return s_instance;
}

void CConfig::Release()
{
	if ( s_instance )
	{
		delete s_instance;
		s_instance = 0;
	}
}

BOOL CConfig::GetServer( tstring& strUrl )
{
	strUrl = this->ReadConfig( CONFIG_ENTRY_SERVER_URL, CONFIG_DEFAULT_SERVER );
	return !strUrl.empty();
}

BOOL CConfig::SetServer( LPCTSTR strUrl )
{
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_SERVER_URL, strUrl );
	return bResult;
}

//BOOL CConfig::GetServerFullSvrUrl( LPTSTR ServerFullStr)
//{
//    BOOL bret = FALSE;
//    tstring strServer;
//    CConfig::Instance()->GetServer( strServer );
//
//    LPCTSTR ServerStr = strServer.c_str();
//    //LPCTSTR ServerStr = L"http:adf//172.16.160.24:8080/adfadf/adfadf/adfadf/";
//
//    TCHAR ServerPath[1024];
//    wcscpy( (LPTSTR)ServerPath, ServerStr);
//    LPCTSTR lpcopy = _wcslwr( (LPTSTR)ServerPath);
//
//    LPCTSTR lpProtocol = L"http://";
//    LPTSTR lpRet = wcsstr(ServerPath, lpProtocol);
//    if(lpRet != NULL && lpRet == lpcopy)
//    {
//        LPCTSTR lpRight = (LPCTSTR)ServerPath + wcslen(lpProtocol); 
//        LPTSTR lpRet2 = wcschr(lpRight,'/');
//        if((lpRet2 != NULL) && (lpRet2 > lpRet) && (lpRet2 - lpRet)>7 )
//        {
//            //符合格式 "http://server/..."
//            wcsncpy(ServerFullStr, ServerStr, (lpRet2 - lpRet) );
//            //wcsncpy(ServerFullStr, ServerStr, (lpRet2 - lpRet) + 1);
//            bret = TRUE;
//        }
//        else
//        {
//            //符合格式 "htpp://server" 需要补
//            //swprintf(ServerFullStr, L"%s%s", ServerStr, L"/");
//            wcscpy(ServerFullStr, ServerStr);
//            bret = TRUE;
//        }
//    }
//    else 
//    {
//        Log() << _T( "Not right http url! " ) << endl;
//    }
//    return bret;
//}

//BOOL CConfig::GetServerFullUrl( tstring& strServerFullStr )
//{
//    BOOL bret = FALSE;
//    tstring strServer;
//    CConfig::Instance()->GetServer( strServer );
//
//    LPCTSTR ServerStr = strServer.c_str();
//    //LPCTSTR ServerStr = L"http:adf//172.16.160.24:8080/adfadf/adfadf/adfadf/";
//
//    TCHAR ServerPath[1024];
//    wcscpy( (LPTSTR)ServerPath, ServerStr);
//    LPCTSTR lpcopy = _wcslwr( (LPTSTR)ServerPath);
//
//    LPCTSTR lpProtocol = L"http://";
//    LPTSTR lpRet = wcsstr(ServerPath, lpProtocol);
//    if(lpRet != NULL && lpRet == lpcopy)
//    {
//        LPCTSTR lpRight = (LPCTSTR)ServerPath + wcslen(lpProtocol); 
//        LPTSTR lpRet2 = wcschr(lpRight,'/');
//        if((lpRet2 != NULL) && (lpRet2 > lpRet) && (lpRet2 - lpRet)>7 )
//        {
//            //符合格式 "http://server/..."
//            //wcscpy(ServerFullStr, ServerStr);
//            strServerFullStr = strServer;
//            bret = TRUE;
//        }
//        else
//        {
//            //符合格式 "htpp://server" 需要补/
//           // swprintf(ServerFullStr, L"%s%s", ServerStr, L"/");
//            strServerFullStr = strServer + _T( "/" );
//            bret = TRUE;
//        }
//    }
//    else 
//    {
//        Log() << _T( "Not right http url!" ) << endl;
//    }
//    return bret;
//}

BOOL CConfig::GetLoginInfo( tstring& strUserId, tstring& strPw, EStreamType& eStreamType )
{
	strUserId = this->ReadConfig( CONFIG_ENTRY_USER, _T("") );
	eStreamType = (EStreamType)this->ReadConfig( CONFIG_ENTRY_STREAMTYPE, STREAM_1X );

	

	// 读出密码，解密。
	tstring strEncPwHex = this->ReadConfig( CONFIG_ENTRY_PW, _T( "" ) );

	strPw = _T( "" );

	// 加密的密文写入配置文件后肯定是32的倍数。
	if ( !strEncPwHex.empty() && strEncPwHex.size() % 32 == 0 )
	{
		// 把16进制数字的字符串恢复成密文。
		// 转成大写后才能与‘0’做差求出代表的数字。
		int nEncDataLen = strEncPwHex.size() / 2;
		unsigned char *pEncDataBuf = new unsigned char[ nEncDataLen ];
		for ( int i=0; i<nEncDataLen; ++i )
		{
			TCHAR nHigh = strEncPwHex[2*i];
			if( nHigh >= 'a' && nHigh <= 'z' )
			{
				nHigh += ( 'A' - 'a' );
			}
			TCHAR nLow = strEncPwHex[ 2*i + 1 ];
			if( nLow >= 'a' && nLow <= 'z' )
			{
				nLow += ( 'A' - 'a' );
			}
			
			// 已经转换成大写。
			unsigned char cHigh = nHigh > '9' ? ( 10 + nHigh - 'A' ) : ( nHigh - '0' );
			unsigned char cLow = nLow> '9' ? ( 10 + nLow - 'A' ) : ( nLow - '0' );
			pEncDataBuf[i] = ( cHigh << 4 ) + cLow;

			//		Log() << pEncDataBuf[i];
		}

		//	Log() << endl;

		// 解密。
		tean_dec( s_EncKey, (char*)pEncDataBuf, nEncDataLen );

		strPw = (LPCTSTR)pEncDataBuf;

		delete[] pEncDataBuf;
	}
	


	return TRUE;
}

BOOL CConfig::SetLoginInfo( LPCTSTR strUserId, LPCTSTR strPw, EStreamType eStreamType )
{
	BOOL bResult = TRUE;
	bResult &= this->WriteConfig( CONFIG_ENTRY_USER, strUserId );
	bResult &= this->WriteConfig( CONFIG_ENTRY_STREAMTYPE, eStreamType );

	// 加密密码。
	int nPasswordByteLen = ( _tcslen( strPw ) + 1 ) * sizeof( strPw[0] );
	// 补齐。
	const int nEncLenUnit = 16;
	int nBufByteLen = ( nPasswordByteLen / nEncLenUnit + 1 ) * nEncLenUnit;
	unsigned char *pDateBuf = new unsigned char[ nBufByteLen ];
	_tcscpy( (LPTSTR)pDateBuf, strPw );

	tean_enc( s_EncKey, (char*)pDateBuf, nBufByteLen );

	// 将加密后的密码看作16进制数字写入配置文件。
	tstring strSavePw;
    
	for ( int i=0; i<nBufByteLen; ++i )
	{
        tstringstream ssSavePw;
		//		Log() << pDateBuf[i] ;
		//TCHAR tmpBuf[10] = {0};
		//_stprintf( tmpBuf,  _T("%02X" ), pDateBuf[i] );
        ssSavePw << setbase( 16 ) << (void*)(int)pDateBuf[i];
        tstring strTmp = ssSavePw.str();
        strTmp = strTmp.substr( strTmp.length() -2 );
		strSavePw += strTmp;
	}

	
//	Log() << endl;




	bResult &= this->WriteConfig( CONFIG_ENTRY_PW, strSavePw.c_str() );

	delete[] pDateBuf;
	pDateBuf = NULL;

	return ( TRUE == bResult);
}

BOOL CConfig::GetLoginInterface(BOOL &bLoginDlg)
{
	bLoginDlg = ( TRUE == this->ReadConfig( CONFIG_ENTRY_LOGIN_DLG, CONFIG_DEFAULT_LOGIN_DIALOG ) );
	return TRUE;
}

BOOL CConfig::SetLoginInterface(BOOL bLoginDlg)
{
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_LOGIN_DLG, bLoginDlg );
	return bResult;
}

BOOL CConfig::GetIsSave( BOOL& bIsSave )
{
	bIsSave = ( TRUE == this->ReadConfig( CONFIG_ENTRY_IS_SAVE, CONFIG_DEFAULT_IS_SAVE ) );
	return TRUE;
}

BOOL CConfig::SetIsSave( BOOL bIsSave )
{
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_IS_SAVE, bIsSave );
	return ( TRUE == bResult );
}

BOOL CConfig::GetRegionId( tstring& strRegionId )
{
	strRegionId = this->ReadConfig( CONFIG_ENTRY_REGION, CONFIG_DEFAULT_REGION );	//AfxGetApp()->GetProfileString( CONFIG_SECTION_CONFIG_NAME, CONFIG_ENTRY_REGION, CONFIG_DEFAULT_REGION );
	return ( !strRegionId.empty() );
}

BOOL CConfig::SetReginId( LPCTSTR strRegionId )
{
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_REGION, strRegionId );// AfxGetApp()->WriteProfileString( CONFIG_SECTION_CONFIG_NAME, CONFIG_ENTRY_REGION, strRegionId.c_str());
	return bResult;
}

BOOL CConfig::GetPtzSpeed( int& nPtzSpeed )
{
	nPtzSpeed = ( this->ReadConfig( CONFIG_ENTRY_PTZ_SPEED, CONFIG_DEFAULT_PTZ_SPEED ) );
	return TRUE;
}
BOOL CConfig::SetPtzSpeed( int nPtzSpeed )
{
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_PTZ_SPEED, nPtzSpeed );
	return ( bResult );
}

BOOL CConfig::GetLenSpeed( int& nLenSpeed )
{
//	nLenSpeed = ( AfxGetApp()->GetProfileInt( CONFIG_SECTION_CONFIG_NAME, CONFIG_ENTRY_LEN_SPEED, CONFIG_DEFAULT_LEN_SPEED ) );
	nLenSpeed = this->ReadConfig( CONFIG_ENTRY_LEN_SPEED, CONFIG_DEFAULT_LEN_SPEED );
	return TRUE;
}

BOOL CConfig::SetLenSpeed( int nLenSpeed )
{
//	BOOL bResult = AfxGetApp()->WriteProfileInt( CONFIG_SECTION_CONFIG_NAME, CONFIG_ENTRY_LEN_SPEED, nLenSpeed );
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_LEN_SPEED, nLenSpeed );
	return bResult;
}

BOOL CConfig::GetCaptureDir( tstring& strCaptureDir )
{
	strCaptureDir = this->ReadConfig( CONFIG_ENTRY_CAPTURE_DIR, _T("") );

	if ( strCaptureDir.empty() )
	{
		// 配置文件中没有记录，返回默认配置。
		// 如果手机有存储卡，优先返回存储卡位置。
		BOOL bHasCard = ::HasStorageCard();
		if( bHasCard )
		{
			strCaptureDir = CONFIG_DEFAULT_CAPTURE_DIR_CARD;
		}
		else
		{
			strCaptureDir = CONFIG_DEFAULT_CAPTURE_DIR_RAM;
		}
	}

    // 标准化文件夹。
    NormalizeDir( strCaptureDir );
//	Log() << _T( "CConfig::GetCaptureDir [" ) << strCaptureDir << _T( "]" )<< endl;

	return ( !strCaptureDir.empty() );
}

BOOL CConfig::SetCaptureDir( LPCTSTR strCaptureDir )
{
	BOOL bResult = this->WriteConfig( CONFIG_ENTRY_CAPTURE_DIR, strCaptureDir );
	return bResult;
}

BOOL CConfig::GetIsCaptureDirCustom( BOOL& bCustom )
{
	bCustom = this->ReadConfig( CONFIG_ENTRY_CAPTURE_DIR_TYPE, FALSE );
	return TRUE;
}

BOOL CConfig::SetIsCaptureDirCustom( BOOL bCustom )
{
	return this->WriteConfig( CONFIG_ENTRY_CAPTURE_DIR_TYPE, bCustom );
}

BOOL CConfig::GetAppTitle( tstring& strAppTitle )
{
	tstring strAppName = ::GetAppName();
	strAppTitle = this->ReadConfig( CONFIG_ENTRY_APP_TITLE, strAppName.c_str() );
	return ( !strAppTitle.empty() );
}

BOOL CConfig::GetMinStorageSpace( int& nMinSpace )
{
	nMinSpace = this->ReadConfig( CONFIG_ENTRY_MIN_SPACE, CONFIG_DEFAULT_MIN_SPACE );
	return TRUE;
}

BOOL CConfig::GetWebpageZoomLevel( float& fZoom )
{
	int nZoomNum = this->ReadConfig( CONFIG_ENTRY_WEBPAGE_ZOOM_LEVEL, CONFIG_DEFAULT_WEBPAGE_ZOOM_LEVEL );
	fZoom = (float)nZoomNum / 100;
	return TRUE;
}

BOOL CConfig::SetWebpageZoomLevel( const float fZoom )
{
	int nZoomNum = int( fZoom * 100 );
	return this->WriteConfig( CONFIG_ENTRY_WEBPAGE_ZOOM_LEVEL, nZoomNum );
}

BOOL CConfig::SetWebpageHistoryBackStep( const int nStep )
{
	return this->WriteConfig( CONFIG_ENTRY_WEBPAGE_HISTORY_BACK, nStep );
}

BOOL CConfig::GetWebpageHistoryBackStep( int& nStep )
{
	nStep = this->ReadConfig( CONFIG_ENTRY_WEBPAGE_HISTORY_BACK, CONFIG_DEFAULT_WEBPAGE_HISTORY_BACK );
	return TRUE;
}

BOOL CConfig::GetIsSendNatPacket( BOOL& bSendnat )
{
    bSendnat = this->ReadConfig( CONFIG_ENTRY_SEND_NAT_PACKET, CONFIG_DEFAULT_SEND_NAT_PACKET );
    return TRUE;
}


int CConfig::ReadConfig( LPCTSTR strCfgEntryName, int nDefault )
{
	tstringstream ssDef;
	ssDef << nDefault;
	tstring strValue = this->ReadConfig( strCfgEntryName, ssDef.str().c_str() );

	tstringstream ssTmp;
    ssTmp << strValue;

    int nValue = nDefault;
    ssTmp >> nValue;

	return nValue;
}

BOOL CConfig::WriteConfig( LPCTSTR strCfgEntryName, int nValue )
{
	tstringstream ssValue;
	ssValue << nValue;

	return this->WriteConfig( strCfgEntryName, ssValue.str().c_str() );
}


tstring CConfig::ReadConfig( LPCTSTR strCfgEntryName, LPCTSTR strDefault )
{
	tstring strResult;

    if ( m_pXmlParser == NULL )
    {
        return _T( "" );
    }
	strResult = this->m_pXmlParser->GetElementValue( strCfgEntryName );
	if ( strResult.empty() && m_pBkCfgxmlParser )
	{
        strResult = this->m_pBkCfgxmlParser->GetElementValue( strCfgEntryName );
        if ( strResult.empty() )
        {
            strResult = strDefault;
        }		
	}
	

	return strResult;
}

BOOL CConfig::WriteConfig( LPCTSTR strCfgEntryName, LPCTSTR strValue )
{
	BOOL bResult = FALSE;

    if ( m_pXmlParser )
    {
        bResult = this->m_pXmlParser->SetElementValue( strCfgEntryName, strValue );

        if ( m_pBkCfgxmlParser )
        {
            this->m_pBkCfgxmlParser->SetElementValue( strCfgEntryName, strValue );
        }        
    }
	
	return bResult;

}

