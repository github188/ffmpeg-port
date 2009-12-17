#include "..\include\ixmlparse.h"

 #ifdef _WIN32_WCE
 /** wince��ʹ��΢���com��xml��������*/
 #define USE_MS_XML_PARSER
#elif defined( __SYMBIAN32__ )

 #else
 /** linux ��ʹ��libxml������ */
 #define USE_LIB_XML
 #endif

#ifdef USE_MS_XML_PARSER
#include "msxmlparser.h"
#endif

#ifdef USE_LIB_XML
#include "LibXmlXmlParser.h"
#endif

IXmlParser::IXmlParser(void)
{
}

IXmlParser::~IXmlParser(void)
{
}

IXmlParser *IXmlParser::CreateNew()
{
#ifdef USE_MS_XML_PARSER
    return new CMsXmlParser();
#endif

#ifdef USE_LIB_XML
    return new CLibXmlXmlParser()
#endif
}
