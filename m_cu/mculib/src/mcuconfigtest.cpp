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
	mcu::tlog << "Run mcu config test!" << endl;

	tstring strOldConfigFile = CConfig::Instance()->GetConfigFilePath();

	mcu::tlog << "default config file path: " << strOldConfigFile << endl;

	// Set another config path,so we don't overwrite the old config.
	LPCTSTR strUnitTestConfigFile =  _T( "unittestconfig.xml" );
	CConfig::Instance()->SetConfigFilePath( strUnitTestConfigFile );

	tstring strCaptureDir = _T( "/a\\b/c/def23\\" ); 
	CPPUNIT_ASSERT ( CConfig::Instance()->SetCaptureDir( strCaptureDir.c_str() ) );
	tstring strRead ;
	CPPUNIT_ASSERT( CConfig::Instance()->GetCaptureDir( strRead ) );
	CPPUNIT_ASSERT_EQUAL( strRead, strCaptureDir );

	// reset the config file path
	CConfig::Instance()->SetConfigFilePath( strOldConfigFile.c_str() );
}

