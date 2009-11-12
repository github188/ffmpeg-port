#include "unittestmain.h"

#include "log.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

BOOL RunUnitTest()
{
	// Get the top level suite from the registry
	CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CPPUNIT_NS::TextUi::TestRunner runner;
	runner.addTest( suite );

	// Change the default outputter to a compiler error format outputter
	runner.setOutputter( new CPPUNIT_NS::CompilerOutputter( &runner.result(),
		CPPUNIT_NS::stdCOut() ) );
	// Run the test.
	bool wasSucessful = runner.run();

	if( !wasSucessful )
	{
//		AfxMessageBox( _T( "单元测试未通过!请根据错误信息检查代码!" ) );
		mcu::tlog << _T( "单元测试未通过!请根据错误信息检查代码!" ) << endl;
	}
	return wasSucessful;
}
