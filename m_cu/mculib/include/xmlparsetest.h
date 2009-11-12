#pragma once

#include "cppunit/TestFixture.h"

#include "mcucommon.h"
#include "cppunit/extensions/HelperMacros.h"
#include "log.h"

class CXMLParseTest :
	public CPPUNIT_NS::TestFixture
{
private:
	CPPUNIT_TEST_SUITE( CXMLParseTest );
	CPPUNIT_TEST( TestXMLWriteRead );
	//CPPUNIT_TEST( testUtfConvert );
	//CPPUNIT_TEST( TestGetModulePath );
	//CPPUNIT_TEST( TestParsePath );
	//	CPPUNIT_TEST_EXCEPTION( testAddThrow, IncompatibleMoneyError );
	CPPUNIT_TEST_SUITE_END();


	void TestXMLWriteRead();
public:
	CXMLParseTest(void);
	virtual ~CXMLParseTest(void);
};
