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

	/** ���Կ��ַ�������.*/
	void testEmptyInput();

	/** �����ַ���ת��.utf16->utf8->utf16 �����Ƿ��֮ǰ��һ��. */
	void testUtfConvert();

	/** ���Ի�ȡģ���ļ���. */
	void TestGetModulePath();

	/** �����ļ�·������.*/
	void TestParsePath();

	/** �����ļ����ļ�ö�١� */
	void TestEnumFolder();
	
	/** ����ɾ���ļ��� */
	void TestDelFile();
};
