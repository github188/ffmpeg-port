#include "unittestmain.h"

#include "log.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#ifdef _WIN32_WCE
#include "mcucommontest.h"
#include "xmlparsetest.h"
#include "mcuconfigtest.h"
#endif

BOOL RunUnitTest()
{
    // vc ���ӵ�ʱ��Ĭ�ϵĻ�ѵ�Ԫ���Ե�cppȥ����
    // �޸�����ѡ����Ч����ʱ����������һ�£�ǿ�����ӡ�
#ifdef _WIN32_WCE
    CMCUCommonTest tmp0;
    CXMLParseTest tmp1;
    CMcuConfigTest tmp2;

#endif

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
//		AfxMessageBox( _T( "��Ԫ����δͨ��!����ݴ�����Ϣ������!" ) );
		mcu::tlog << _T( "��Ԫ����δͨ��!����ݴ�����Ϣ������!" ) << endl;
	}
	return wasSucessful;
}
