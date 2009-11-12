#pragma once

#include "unittestcommon.h"

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
	CPPUNIT_TEST( TestEnumFolder );
	CPPUNIT_TEST( TestDelFile );
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

	/** 测试文件夹文件枚举。 */
	void TestEnumFolder();
	
	/** 测试删除文件。 */
	void TestDelFile();
};
