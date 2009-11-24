#include "mcuconfigtest.h"

#include "mcuconfig.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CMcuConfigTest );


CMcuConfigTest::CMcuConfigTest(void)
{
}

CMcuConfigTest::~CMcuConfigTest(void)
{
}

void CMcuConfigTest::TestMCUConfig()
{
	mcu::log << "Run mcu config test!" << endl;

	tstring strOldConfigFile = CConfig::Instance()->GetConfigFilePath();

	mcu::log << "default config file path: " << strOldConfigFile << endl;

#ifdef _WIN32_WCE
	// Set another config path,so we don't overwrite the old config.
	LPCTSTR strUnitTestConfigFile =  _T( "\\unittestconfig.xml" );
#else
    LPCTSTR strUnitTestConfigFile =  _T( "unittestconfig.xml" );
#endif


	CConfig::Instance()->SetConfigFilePath( strUnitTestConfigFile );

	tstring strCaptureDir = _T( "/a\\b/c/def23" ); 
	CPPUNIT_ASSERT ( CConfig::Instance()->SetCaptureDir( strCaptureDir.c_str() ) );
	tstring strRead ;
	CPPUNIT_ASSERT( CConfig::Instance()->GetCaptureDir( strRead ) );

#ifdef _WIN32_WCE
    tstring strTarget = _T( "\\a\\b\\c\\def23\\" );
#else
    tstring strTarget = _T( "/a/b/c/def23/" );
#endif

	CPPUNIT_ASSERT_EQUAL( strTarget, strRead );

	// reset the config file path
	CConfig::Instance()->SetConfigFilePath( strOldConfigFile.c_str() );
}

