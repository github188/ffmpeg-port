#pragma once

#include "unittestcommon.h"

class CMcuConfigTest :
	public CPPUNIT_NS::TestFixture
{	
private:
	CPPUNIT_TEST_SUITE( CMcuConfigTest );
	CPPUNIT_TEST( TestMCUConfig );
	//CPPUNIT_TEST( testUtfConvert );
	//CPPUNIT_TEST( TestGetModulePath );
	//CPPUNIT_TEST( TestParsePath );
	//	CPPUNIT_TEST_EXCEPTION( testAddThrow, IncompatibleMoneyError );
	CPPUNIT_TEST_SUITE_END();


void TestMCUConfig();
public:
	CMcuConfigTest(void);
	virtual ~CMcuConfigTest(void);
};
