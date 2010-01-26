#include "..\include\ixmlparse.h"

 #ifdef _WIN32_WCE
 /** wince下使用微软的com的xml解析器。*/
 #define USE_MS_XML_PARSER
#elif defined( __SYMBIAN32__ )
#define USE_S60_XML_PARSER 1
 #else
 /** linux 下使用libxml解析。 */
 #define USE_LIB_XML
 #endif

#ifdef USE_MS_XML_PARSER
#include "msxmlparser.h"
#endif

#ifdef USE_LIB_XML
#include "LibXmlXmlParser.h"
#endif

#if defined( USE_S60_XML_PARSER )
#include "s60xmlparser.h"
#endif

IXmlParser::IXmlParser(void)
{
}

IXmlParser::~IXmlParser(void)
{
}

IXmlParser *IXmlParser::CreateNew()
{
#if defined( USE_MS_XML_PARSER )
    return new CMsXmlParser();
#elif defined ( USE_LIB_XML )
    return new CLibXmlXmlParser();
#elif defined ( USE_S60_XML_PARSER )
    return new CS60XMLParser();
#else
    return NULL;
#endif
}
