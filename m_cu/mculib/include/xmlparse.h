// 
// #pragma once
// 
// /** �򵥵�xml������װ��
// *	��ֻ���������ļ��Ķ�ȡ��д�롣
// */
// #ifdef _WIN32_WCE
// /** wince��ʹ��΢���com��xml��������*/
// #define USE_MS_XML_PARSER
// #else
// /** linux ��ʹ��libxml������ */
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
// /** ����ͨ�ַ���ת��ΪXML�ַ����� */
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
// 	/** ����XML�ļ��� 
// 	*/
// 	BOOL LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName );
// 
// 	/** Create xml */
// //	BOOL CreateXML( LPCTSTR strXmlFile, LPCTSTR strDocRootName );
// 
// 	/** ��ȡxml�ļ�·����*/
// 	tstring GetXMLPath() const;
// 
// 
// 
// 	/** ���档 */
// 	BOOL SaveXML();
// 
// 	/** �ͷţ����١� */
// 	BOOL ReleaseXML();
// 
// 	/** get name of root element */
// 	tstring GetRootName();
// 
// 	/** ����һ���ڵ㡣
// 	*	ֻ�����ҵ��ĵ�һ����
// 	*	���û�ҵ������ؿ��ַ�����
// 	*/
// 	tstring GetElementValue( LPCTSTR strElementName );
// 
// 	/** дһ���ڵ㡣 
// 	*	ֻд���ҵ��ĵ�һ�������С�
// 	*/
// 	BOOL SetElementValue( LPCTSTR strElementName, LPCTSTR strValue );
// private:
// 	/** ���õ�ǰ��Ϊ�������ӽڵ㡣 
// 	*	���ҽڵ��ʱ��ֻ���ڵ�ǰ�����ڵ������һ���ӽڵ���������
// 	*/
// 	BOOL SetRootElement( LPCTSTR strEleName );
// 
// private:
// 
// 	/** �ļ����� */
// 	tstring m_strXmlFileName;
// 
// 	/** root element */
// 	tstring m_strRootElementName;
// 
// #ifdef USE_MS_XML_PARSER
// 
// 	/** ����xml�� */
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
