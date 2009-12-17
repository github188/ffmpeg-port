#pragma once
#include "ixmlparse.h"

#include "stdafx.h"
#include <msxml.h>
#include <atlbase.h>
#include <afxdisp.h>

/** 将普通字符串转换为XML字符串。 */
#define XML_TEXT(x) COleVariant(x).bstrVal

class CMsXmlParser :
    public IXmlParser
{
public:
    CMsXmlParser(void);
    virtual ~CMsXmlParser(void);

    /** 载入XML文件。 
    */
    virtual BOOL LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName );

    /** Create xml */
    //	BOOL CreateXML( LPCTSTR strXmlFile, LPCTSTR strDocRootName );

    /** 获取xml文件路径。*/
    virtual tstring GetXMLPath() const;



    /** 保存。 */
    virtual BOOL SaveXML();

    /** 释放，销毁。 */
    virtual BOOL ReleaseXML();

    /** get name of root element */
 //   virtual tstring GetRootName();

    /** 查找一个节点。
    *	只返回找到的第一个。
    *	如果没找到，返回空字符串。
    */
    virtual tstring GetElementValue( LPCTSTR strElementName );

    /** 写一个节点。 
    *	只写到找到的第一个对象中。
    */
    virtual BOOL SetElementValue( LPCTSTR strElementName, LPCTSTR strValue );
private:
    /** 设置当前作为树根的子节点。 
    *	查找节点的时候都只是在当前树根节点下面的一级子节点中搜索。
    */
    BOOL SetRootElement( LPCTSTR strEleName );

private:

    /** 文件名。 */
    tstring m_strXmlFileName;

    /** root element */
    tstring m_strRootElementName;


    /** 解析xml。 */
    typedef CComPtr<IXMLDOMDocument> IXMLDOMDocumentPtr;
    typedef CComPtr<IXMLDOMNodeList> IXMLDOMNodeListPtr;
    typedef CComPtr<IXMLDOMNode> IXMLDOMNodePtr;
    typedef CComPtr<IXMLDOMElement> IXMLDOMElementPtr;

    IXMLDOMDocumentPtr m_pXMLDoc;
    IXMLDOMElementPtr  m_pXMLRootNode;


};
