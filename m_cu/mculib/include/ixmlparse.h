#pragma once

#include "portabledefine.h"

class IXmlParser
{
public:
    static IXmlParser *CreateNew();
public:
    IXmlParser(void);
    virtual ~IXmlParser(void);

    /** 载入XML文件。 
    */
    virtual BOOL LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName ) = 0;

    /** 获取xml文件路径。*/
    virtual tstring GetXMLPath() const  = 0;



    /** 保存。 */
    virtual BOOL SaveXML() = 0;

    /** 释放，销毁。 */
    virtual BOOL ReleaseXML() = 0;

    /** get name of root element */
//    virtual tstring GetRootName() = 0;

    /** 查找一个节点。
    *	只返回找到的第一个。
    *	如果没找到，返回空字符串。
    */
    virtual tstring GetElementValue( LPCTSTR strElementName ) = 0;

    /** 写一个节点。 
    *	只写到找到的第一个对象中。
    */
    virtual BOOL SetElementValue( LPCTSTR strElementName, LPCTSTR strValue ) = 0;
};
