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
	mcu::tlog << _T( "Enum all file of folder: " ) << strFolder << endl;
	for ( size_t i=0; i<tAllFile.size(); ++i )
	{
		mcu::tlog << tAllFile[i] << endl;
	}

	strFolder = _T( "/media/mmc1/M_CU" );
	tAllFile = EnumAllFile( strFolder );
	mcu::tlog << _T( "Enum all file of folder: " ) << strFolder << endl;
	for ( size_t i=0; i<tAllFile.size(); ++i )
	{
		mcu::tlog << tAllFile[i] << endl;
	}

	strFolder = _T( "\\Storage Card\\" );
	tAllFile = EnumAllFile( strFolder );
	mcu::tlog << _T( "Enum all file of folder: " ) << strFolder << endl;
	for ( size_t i=0; i<tAllFile.size(); ++i )
	{
		mcu::tlog << tAllFile[i] << endl;
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
		mcu::tlog << _T( "Unittest DelFile create temp file fail! file: " ) << strFile << endl;
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

