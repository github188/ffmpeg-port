#pragma once


#include "cppunit/TestFixture.h"
#include "mcucommon.h"
#include "cppunit/extensions/HelperMacros.h"
#include "log.h"

// The function below could be prototyped as:
// inline std::ostream &operator <<( std::ostream &os, const Money &value )
// If you know that you will never compile on a platform without std::ostream
// (such as embedded vc++ 4.0; though even that platform you can use STLPort)
#if defined( UNICODE ) || defined ( _UNICODE )
inline CPPUNIT_NS::OStream &operator <<( CPPUNIT_NS::OStream &os, const wstring &value )
{
	mcu::log << value;

	std::wstringstream sswMsg;
	sswMsg << _T( "output wstring addr=" ) <<  (void *)value.c_str() << _T( " < value=\"" ) << value << _T( "\" >" ) <<endl;
	mcu::log << sswMsg.str();

	os << "cpp unit don't surpport wstring !" << endl;
	return os << "wstring < value has show above!! please search \"addr=" << value.c_str() << "\" >";
}
#endif

