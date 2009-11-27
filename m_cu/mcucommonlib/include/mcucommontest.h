#pragma once

#include "cppunit/TestFixture.h"

#include "mcucommon.h"
#include "cppunit/extensions/HelperMacros.h"
#include "log.h"

// The function below could be prototyped as:
// inline std::ostream &operator <<( std::ostream &os, const Money &value )
// If you know that you will never compile on a platform without std::ostream
// (such as embedded vc++ 4.0; though even that platform you can use STLPort)
inline CPPUNIT_NS::OStream &operator <<( CPPUNIT_NS::OStream &os, const wstring &value )
{
	mcu::log << value;

//	std::wstringstream sswMsg;
//	sswMsg << _T( "output wstring addr=" ) <<  (void *)value.c_str() << _T( " < value=\"" ) << value << _T( "\" >" ) <<endl;
//	tcout << sswMsg.str();

    return	os << "cpp unit don't surpport wstring !output to mcu::log!" << endl;
//	return os << "wstring < value has show above!! please search \"addr=" << value.c_str() << "\" >";
}

inline CPPUNIT_NS::OStream &operator <<( CPPUNIT_NS::OStream &os, const TFileNameInfo &value )
{
	value;
	return os << "cpp unit don't surpport wstring ! so I can't output TFileNameInfo" << endl;	
}

class CMCUCommonTest :
	public CPPUNIT_NS::TestFixture
{
private:
	CPPUNIT_TEST_SUITE( CMCUCommonTest );
	CPPUNIT_TEST( testEmptyInput );
	CPPUNIT_TEST( testUtfConvert );
	CPPUNIT_TEST( TestGetModulePath );
	CPPUNIT_TEST( TestParsePath );
//	CPPUNIT_TEST_EXCEPTION( testAddThrow, IncompatibleMoneyError );
	CPPUNIT_TEST_SUITE_END();
public:
	CMCUCommonTest(void);
	~CMCUCommonTest(void);

	/** 测试空字符串输入.*/
	void testEmptyInput();

	/** 测试字符窗转换.utf16->utf8->utf16 后检查是否和之前的一样. */
	void testUtfConvert();

	/** 测试获取模块文件名. */
	void TestGetModulePath();

	/** 测试文件路径解析.*/
	void TestParsePath();
	
};
