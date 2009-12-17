// 
// #pragma once
// 
// /** 简单的xml解析封装。
// *	现只用于配置文件的读取和写入。
// */
// #ifdef _WIN32_WCE
// /** wince下使用微软的com的xml解析器。*/
// #define USE_MS_XML_PARSER
// #else
// /** linux 下使用libxml解析。 */
// #define USE_LIB_XML
// #endif
// 
// 
// #ifdef USE_MS_XML_PARSER
// #include "stdafx.h"
// #include <msxml.h>
// #include <atlbase.h>
// #include <afxdisp.h>
// 
// /** 将普通字符串转换为XML字符串。 */
// #define XML_TEXT(x) COleVariant(x).bstrVal
// 
// #endif
// 
// 
// #ifdef USE_LIB_XML
// #include <libxml/parser.h>
// #include <libxml/tree.h>
// #endif
// 
// #include "mcudefine.h"
// 
// class CXmlParse
// {
// public:
// 	CXmlParse();
// 	~CXmlParse();
// 
// 	/** 载入XML文件。 
// 	*/
// 	BOOL LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName );
// 
// 	/** Create xml */
// //	BOOL CreateXML( LPCTSTR strXmlFile, LPCTSTR strDocRootName );
// 
// 	/** 获取xml文件路径。*/
// 	tstring GetXMLPath() const;
// 
// 
// 
// 	/** 保存。 */
// 	BOOL SaveXML();
// 
// 	/** 释放，销毁。 */
// 	BOOL ReleaseXML();
// 
// 	/** get name of root element */
// 	tstring GetRootName();
// 
// 	/** 查找一个节点。
// 	*	只返回找到的第一个。
// 	*	如果没找到，返回空字符串。
// 	*/
// 	tstring GetElementValue( LPCTSTR strElementName );
// 
// 	/** 写一个节点。 
// 	*	只写到找到的第一个对象中。
// 	*/
// 	BOOL SetElementValue( LPCTSTR strElementName, LPCTSTR strValue );
// private:
// 	/** 设置当前作为树根的子节点。 
// 	*	查找节点的时候都只是在当前树根节点下面的一级子节点中搜索。
// 	*/
// 	BOOL SetRootElement( LPCTSTR strEleName );
// 
// private:
// 
// 	/** 文件名。 */
// 	tstring m_strXmlFileName;
// 
// 	/** root element */
// 	tstring m_strRootElementName;
// 
// #ifdef USE_MS_XML_PARSER
// 
// 	/** 解析xml。 */
// 	typedef CComPtr<IXMLDOMDocument> IXMLDOMDocumentPtr;
// 	typedef CComPtr<IXMLDOMNodeList> IXMLDOMNodeListPtr;
// 	typedef CComPtr<IXMLDOMNode> IXMLDOMNodePtr;
// 	typedef CComPtr<IXMLDOMElement> IXMLDOMElementPtr;
// 
// 	IXMLDOMDocumentPtr m_pXMLDoc;
// 	IXMLDOMElementPtr  m_pXMLRootNode;
// #endif
// 
// #ifdef USE_LIB_XML
// 	xmlDocPtr	m_pXMLDoc;
// 	xmlNodePtr	m_pXMLRootNode;
// #endif
// };
// 
// 
// 
