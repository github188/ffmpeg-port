#include "mcucommontest.h"

#ifdef _WIN32_WCE
#include "winbase.h"
#endif
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CMCUCommonTest );



CMCUCommonTest::CMCUCommonTest(void)
{
}

CMCUCommonTest::~CMCUCommonTest(void)
{
}

void CMCUCommonTest::testEmptyInput()
{
#ifdef _WIN32_WCE
	string strResult = UTF16toUTF8( L"" );
	CPPUNIT_ASSERT_EQUAL( strResult, string("") );

	wstring strWR = UTF8toUTF16( "" );
	CPPUNIT_ASSERT_EQUAL( strWR, wstring(L"") );
#endif
}

void CMCUCommonTest::testUtfConvert()
{
#ifdef _WIN32_WCE
	wstring strW = L"lzx是大好人!!";

	string strA = UTF16toUTF8( strW );
	wstring strFinalW = UTF8toUTF16( strA );

	CPPUNIT_ASSERT_EQUAL( strW, strFinalW );
#endif
}

void CMCUCommonTest::TestGetModulePath()
{
#ifdef _WIN32_WCE
	tstring strPath1 = GetModulePath();

	tstring strPath2;
	TCHAR temp[1024]; 
	::GetModuleFileName( NULL, temp, 1024 );
	strPath2 = temp;

	CPPUNIT_ASSERT_EQUAL( strPath1, strPath2 );
#endif
}

void CMCUCommonTest::TestParsePath()
{
	LPCTSTR strPath = _T( "\\12345/abcd\\thinkingl.keda.com" );
	TFileNameInfo tInfo = ParsePath( strPath );

	TFileNameInfo tInfo2;
	CPPUNIT_ASSERT( ParsePath( strPath, tInfo2 ) );

	// 两种方式的结果必须一样.
	CPPUNIT_ASSERT_EQUAL( tInfo, tInfo2 );

	CPPUNIT_ASSERT_EQUAL( tInfo.m_strBaseName, tstring(_T( "thinkingl.keda" )) );
#ifdef _WIN32_WCE
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strDirectory, tstring( _T( "\\12345\\abcd\\" ) ) );
#else
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strDirectory, tstring( _T( "/12345/abcd/" ) ) );
#endif
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strExtName, tstring( _T( "com" ) ) );
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strFileName, tstring( _T( "thinkingl.keda.com" ) ) );
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strFilePath, tstring( strPath ) );

	LPCTSTR strPath2 = _T( "abc" );
	CPPUNIT_ASSERT( ParsePath( strPath2, tInfo ) );

	tInfo2 = ParsePath( strPath2 );
	CPPUNIT_ASSERT_EQUAL( tInfo2, tInfo );

	CPPUNIT_ASSERT_EQUAL( tInfo.m_strBaseName, tstring( strPath2 ) );
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strDirectory, tstring( _T( "" ) ) );
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strExtName, tstring( _T( "" ) ) );
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strFileName, tstring( strPath2 ) );
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strFilePath, tstring( strPath2 ) );
}

void CMCUCommonTest::TestEnumFolder()
{
	LPCTSTR strFolder = _T( "." );
	TFileList tAllFile = EnumAllFile( strFolder );
	Log() << _T( "Enum all file of folder: " ) << strFolder << endl;
	for ( size_t i=0; i<tAllFile.size(); ++i )
	{
		Log() << tAllFile[i] << endl;
	}

	strFolder = _T( "/media/mmc1/M_CU" );
	tAllFile = EnumAllFile( strFolder );
	Log() << _T( "Enum all file of folder: " ) << strFolder << endl;
	for ( size_t i=0; i<tAllFile.size(); ++i )
	{
		Log() << tAllFile[i] << endl;
	}

	strFolder = _T( "\\Storage Card\\" );
	tAllFile = EnumAllFile( strFolder );
	Log() << _T( "Enum all file of folder: " ) << strFolder << endl;
	for ( size_t i=0; i<tAllFile.size(); ++i )
	{
		Log() << tAllFile[i] << endl;
	}

}

void CMCUCommonTest::TestDelFile()
{
	LPCTSTR strFile = _T( "/media/mmc1/M_CU/tempdeltest.test" );
	ofstream fTest;
	fTest.open( strFile );
	fTest << "test";
	fTest.close();

	BOOL bExist = IsFileExist( strFile );
	if ( bExist )
	{
		BOOL bResult = DelFile( strFile );
		CPPUNIT_ASSERT( bResult );
		bExist = IsFileExist( strFile );
		CPPUNIT_ASSERT( !bExist );
	}
	else
	{
		Log() << _T( "Unittest DelFile create temp file fail! file: " ) << strFile << endl;
	}
}

void CMCUCommonTest::TestNormalizeDir()
{
    tstring strDir = _T( "/root/test" );

    NormalizeDir( strDir );

#ifdef _WIN32_WCE
    CPPUNIT_ASSERT_EQUAL( tstring( _T( "\\root\\test\\" ) ), strDir );
#else
    CPPUNIT_ASSERT_EQUAL( tstring( _T( "/root/test/" ) ), strDir );
#endif
}

void CMCUCommonTest::TestTimeStr()
{
    // 数字0，应该对应1970年1月1日 00：00：00 。但我们是东八区，时区转换后提前8小时。
    // 于是测试数字选择为 3600 × 8 + 6，我们东八区应该正好是1970/1/1 00：16：06.
    const __time64_t TestTime = 3600 * 8 + 6;
    tstring strTime = TimeToStr( TestTime );

    CPPUNIT_ASSERT_EQUAL( tstring( _T( "19700101160006" ) ), strTime );

    __time64_t tTime = StrToTime( strTime.c_str() );

    CPPUNIT_ASSERT_EQUAL( TestTime, tTime );
}

void CMCUCommonTest::TestStringToUrl()
{
    LPCTSTR str1 = _T( "#%&+ end" );
    tstring strUrl = StringToUrl( str1 );

    LPCTSTR strTar = _T( "%23%25%26%2b%20end" );
    CPPUNIT_ASSERT_EQUAL( tstring( strTar ), strUrl );

    str1 = _T( "http://abc# d13=我是神#%&+ end\n" );

    strUrl = StringToUrl( str1 );

    // 00000002h: 11 62 2F 66 5E 79                               ; .b/f^y
    
    strTar = _T( "http://abc%23%20d13=%11%62%2f%66%5e%79%23%25%26%2b%20end" );
    CPPUNIT_ASSERT_EQUAL( tstring( strTar ), strUrl );
}



