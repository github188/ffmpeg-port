#include "StdAfx.h"
#include "Config.h"

#include "mcudef.h"
#include "mcucommon.h"
#include "encrypt.h"


//#include <objsafe.h>

CConfig *CConfig::s_instance = 0;

/** 相关常量定义。 */
static LPCTSTR CONFIG_FILE_NAME = _T( "mcucfg.xml" );
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


static LPCTSTR CONFIG_DEFAULT_SERVER = _T( "http://218.242.128.205/msp/login.php" );
static LPCTSTR CONFIG_DEFAULT_REGION = _T( "admin.zj" );
static const BOOL CONFIG_DEFAULT_LOGIN_DIALOG = TRUE;
static const BOOL CONFIG_DEFAULT_IS_SAVE = TRUE;
static const int CONFIG_DEFAULT_PTZ_SPEED = 5;
static const int CONFIG_DEFAULT_LEN_SPEED = 5;
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_RAM = _T( "\\My Documents\\M_CU\\" );
static LPCTSTR CONFIG_DEFAULT_CAPTURE_DIR_CARD = _T( "\\Storage Card\\M_CU\\" );

static const int CONFIG_DEFAULT_MIN_SPACE = MIN_STORAGE_SPACE;


CConfig::CConfig(void)
{

	BOOL bResult = this->LoadXML();
	_ASSERT( bResult );
}

CConfig::~CConfig(void)
{
}





BOOL CConfig::LoadXML()
{
	COleVariant vXmlFile( this->GetXMLFilePath().c_str() );
	VARIANT_BOOL vSuccess;

	HRESULT hr = this->m_pXMLDoc.CoCreateInstance(__uuidof (DOMDocument));
	m_pXMLDoc->put_validateOnParse(VARIANT_FALSE);
	m_pXMLDoc->put_resolveExternals(VARIANT_FALSE);
	m_pXMLDoc->put_preserveWhiteSpace(VARIANT_FALSE);
	hr = m_pXMLDoc->load(vXmlFile,&vSuccess);
	
	// 载入失败时可能是文件不存在。保存一个出来。
	BOOL bSuccess = ( S_OK == hr );
	if ( !bSuccess )
	{
		bSuccess = this->SaveXML();
	}

	// 获取config节点。
	if ( bSuccess )
	{
		IXMLDOMNodePtr pCfgNode;
		IXMLDOMNodeListPtr pNodeList;
		hr = m_pXMLDoc->getElementsByTagName( XML_TEXT( CONFIG_SECTION_CONFIG_NAME ), &pNodeList );
		long lCount = 0;
		if ( pNodeList )
		{
			pNodeList->get_length( &lCount );
		}
		if ( lCount > 0 )
		{			
			_ASSERT( lCount == 1 );
			pNodeList->get_item( 0, &pCfgNode );
		}
		else
		{
			// 没有这个节点，创建新的。
			IXMLDOMElementPtr pNewEle;
			hr = m_pXMLDoc->createElement( XML_TEXT( CONFIG_SECTION_CONFIG_NAME ), &pNewEle );
			bSuccess = ( S_OK == hr );
			if ( bSuccess )
			{
				hr = m_pXMLDoc->appendChild( pNewEle, &pCfgNode );
				bSuccess = ( S_OK == hr );
				this->SaveXML();
			}
		}

		m_pXMLConfigNode = pCfgNode;

	}

	return bSuccess;
}

BOOL CConfig::SaveXML()
{
	COleVariant vXmlFile( this->GetXMLFilePath().c_str() );
	HRESULT hr = this->m_pXMLDoc->save( vXmlFile );
	return ( S_OK == hr );
}

tstring CConfig::GetXMLFilePath()
{
	tstring strPath = ::GetAppDir();
	return ( strPath + CONFIG_FILE_NAME ).c_str();
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

BOOL CConfig::GetServerFullSvrUrl( LPTSTR ServerFullStr)
{
	BOOL bret = FALSE;
	tstring strServer;
	CConfig::Instance()->GetServer( strServer );

	LPCTSTR ServerStr = strServer.c_str();
	//LPCTSTR ServerStr = L"http:adf//172.16.160.24:8080/adfadf/adfadf/adfadf/";

	TCHAR ServerPath[1024];
	wcscpy( (LPTSTR)ServerPath, ServerStr);
	LPCTSTR lpcopy = _wcslwr( (LPTSTR)ServerPath);

	LPCTSTR lpProtocol = L"http://";
	LPTSTR lpRet = wcsstr(ServerPath, lpProtocol);
	if(lpRet != NULL && lpRet == lpcopy)
	{
		LPCTSTR lpRight = (LPCTSTR)ServerPath + wcslen(lpProtocol); 
		LPTSTR lpRet2 = wcschr(lpRight,'/');
		if((lpRet2 != NULL) && (lpRet2 > lpRet) && (lpRet2 - lpRet)>7 )
		{
			//符合格式 "http://server/..."
			wcsncpy(ServerFullStr, ServerStr, (lpRet2 - lpRet) );
			//wcsncpy(ServerFullStr, ServerStr, (lpRet2 - lpRet) + 1);
			bret = TRUE;
		}
		else
		{
			//符合格式 "htpp://server" 需要补
			//swprintf(ServerFullStr, L"%s%s", ServerStr, L"/");
			wcscpy(ServerFullStr, ServerStr);
			bret = TRUE;
		}
	}
	else 
	{
		mcu::tlog << L"不是合法的HTTP地址" << endl;
	}
	return bret;
}

BOOL CConfig::GetServerFullUrl( LPTSTR ServerFullStr)
{
	BOOL bret = FALSE;
	tstring strServer;
	CConfig::Instance()->GetServer( strServer );

	LPCTSTR ServerStr = strServer.c_str();
	//LPCTSTR ServerStr = L"http:adf//172.16.160.24:8080/adfadf/adfadf/adfadf/";

	TCHAR ServerPath[1024];
	wcscpy( (LPTSTR)ServerPath, ServerStr);
	LPCTSTR lpcopy = _wcslwr( (LPTSTR)ServerPath);

	LPCTSTR lpProtocol = L"http://";
	LPTSTR lpRet = wcsstr(ServerPath, lpProtocol);
	if(lpRet != NULL && lpRet == lpcopy)
	{
		LPCTSTR lpRight = (LPCTSTR)ServerPath + wcslen(lpProtocol); 
		LPTSTR lpRet2 = wcschr(lpRight,'/');
		if((lpRet2 != NULL) && (lpRet2 > lpRet) && (lpRet2 - lpRet)>7 )
		{
			//符合格式 "http://server/..."
			wcscpy(ServerFullStr, ServerStr);
			bret = TRUE;
		}
		else
		{
			//符合格式 "htpp://server" 需要补/
			swprintf(ServerFullStr, L"%s%s", ServerStr, L"/");
			bret = TRUE;
		}
	}
	else 
	{
		mcu::tlog << L"不是合法的HTTP地址" << endl;
	}
	return bret;
}


BOOL CConfig::GetLoginInfo( tstring& strUserId, tstring& strPw, EStreamType& eStreamType )
{
	strUserId = this->ReadConfig( CONFIG_ENTRY_USER, _T("") );
	eStreamType = (EStreamType)this->ReadConfig( CONFIG_ENTRY_STREAMTYPE, STREAM_1X );

	

	// 读出密码，解密。
	CString strEncPwHex = this->ReadConfig( CONFIG_ENTRY_PW, _T( "" ) ).c_str();

	strPw = _T( "" );

	// 加密的密文写入配置文件后肯定是32的倍数。
	if ( !strEncPwHex.IsEmpty() && strEncPwHex.GetLength() % 32 == 0 )
	{
		// 把16进制数字的字符串恢复成密文。
		// 转成大写后才能与‘0’做差求出代表的数字。
		strEncPwHex.MakeUpper();
		int nEncDataLen = _tcslen( strEncPwHex ) / 2;
		unsigned char *pEncDataBuf = new unsigned char[ nEncDataLen ];
		for ( int i=0; i<nEncDataLen; ++i )
		{
			// 已经转换成大写。
			unsigned char cHigh = strEncPwHex[2*i] > '9' ? ( 10 + strEncPwHex[2*i] - 'A' ) : ( strEncPwHex[2*i] - '0' );
			unsigned char cLow = strEncPwHex[ 2*i + 1 ] > '9' ? ( 10 + strEncPwHex[2*i+1] - 'A' ) : ( strEncPwHex[2*i+1] - '0' );
			pEncDataBuf[i] = ( cHigh << 4 ) + cLow;

			//		mcu::tlog << pEncDataBuf[i];
		}

		//	mcu::tlog << endl;

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
	CString strSavePw;
	for ( int i=0; i<nBufByteLen; ++i )
	{
		//		mcu::tlog << pDateBuf[i] ;
		CString strTmp;
		strTmp.Format( _T( "%02X" ), pDateBuf[i] );
		strSavePw += strTmp;
	}

	
//	mcu::tlog << endl;




	bResult &= this->WriteConfig( CONFIG_ENTRY_PW, strSavePw );

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
	return ( strRegionId.empty() );
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
	return ( strCaptureDir.empty() );
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
	return strAppTitle.empty();
}

BOOL CConfig::GetMinStorageSpace( int& nMinSpace )
{
	nMinSpace = this->ReadConfig( CONFIG_ENTRY_MIN_SPACE, CONFIG_DEFAULT_MIN_SPACE );
	return TRUE;
}

int CConfig::ReadConfig( LPCTSTR strCfgEntryName, int nDefault )
{
	tstringstream ssDef;
	ssDef << nDefault;
	tstring strValue = this->ReadConfig( strCfgEntryName, ssDef.str().c_str() );

	int nValue = _ttoi( strValue.c_str() );

	return nValue;
}

tstring CConfig::ReadConfig( LPCTSTR strCfgEntryName, LPCTSTR strDefault )
{
	IXMLDOMNodeListPtr pNodeList;
	IXMLDOMNodePtr pNode;

	tstring strResult;

	HRESULT hr = this->m_pXMLConfigNode->getElementsByTagName( XML_TEXT( strCfgEntryName ), &pNodeList );
	long lCount = 0;
	if ( pNodeList )
	{
		pNodeList->get_length( &lCount );
	}
	if ( lCount > 0 )
	{
		// 找到了。
		pNodeList->get_item( 0, &pNode );
	}

	if ( pNode )
	{
		BSTR strValue;
		pNode->get_text( &strValue );
		strResult = strValue;
	}
	else
	{
		strResult = strDefault;
	}

	return strResult;
}

BOOL CConfig::WriteConfig( LPCTSTR strCfgEntryName, LPCTSTR strValue )
{
	IXMLDOMNodeListPtr pNodeList;
	IXMLDOMNodePtr pNode;

	tstring strResult;

	HRESULT hr = this->m_pXMLConfigNode->getElementsByTagName( XML_TEXT( strCfgEntryName ), &pNodeList );
	long lCount = 0;
	if ( pNodeList )
	{
		pNodeList->get_length( &lCount );
	}
	if ( lCount > 0 )
	{
		// 找到了。
		pNodeList->get_item( 0, &pNode );
	}
	else
	{
		IXMLDOMElementPtr pEle;
		hr = this->m_pXMLDoc->createElement( XML_TEXT( strCfgEntryName ), &pEle );
		if ( S_OK == hr )
		{
			this->m_pXMLConfigNode->appendChild( pEle, &pNode );
		}
	}

	if ( pNode )
	{
		hr = pNode->put_text( XML_TEXT( strValue ) );
	}

	// 写完保存。
	this->SaveXML();

	return ( S_OK == hr );
}

BOOL CConfig::WriteConfig( LPCTSTR strCfgEntryName, int nValue )
{
	tstringstream ssValue;
	ssValue << nValue;

	return this->WriteConfig( strCfgEntryName, ssValue.str().c_str() );
}