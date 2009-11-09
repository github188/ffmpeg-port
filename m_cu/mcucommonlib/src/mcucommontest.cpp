#include "MCUCommonTest.h"

#include "winbase.h"
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
	string strResult = UTF16toUTF8( L"" );
	CPPUNIT_ASSERT_EQUAL( strResult, string("") );

	wstring strWR = UTF8toUTF16( "" );
	CPPUNIT_ASSERT_EQUAL( strWR, wstring(L"") );
}

void CMCUCommonTest::testUtfConvert()
{
	wstring strW = L"lzx是大好人!!";

	string strA = UTF16toUTF8( strW );
	wstring strFinalW = UTF8toUTF16( strA );

	CPPUNIT_ASSERT_EQUAL( strW, strFinalW );
}

void CMCUCommonTest::TestGetModulePath()
{
	tstring strPath1 = GetModulePath();

	tstring strPath2;
	TCHAR temp[1024]; 
	::GetModuleFileName( NULL, temp, 1024 );
	strPath2 = temp;

	CPPUNIT_ASSERT_EQUAL( strPath1, strPath2 );
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
	CPPUNIT_ASSERT_EQUAL( tInfo.m_strDirectory, tstring( _T( "\\12345\\abcd\\" ) ) );
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