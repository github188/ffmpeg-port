#pragma once

/** 
*	�������ڡ�
*/

#include "mcucommon.h" 
#include "xmlparse.h"

LPCTSTR HELP_FILE_PATH;


class CVirtualHelpWnd
{
public:
	CVirtualHelpWnd();
	~CVirtualHelpWnd();

protected:

private:
	CXmlParse m_helpXMLParser;
};


