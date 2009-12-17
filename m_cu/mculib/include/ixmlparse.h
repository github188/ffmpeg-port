#pragma once

#include "portabledefine.h"

class IXmlParser
{
public:
    static IXmlParser *CreateNew();
public:
    IXmlParser(void);
    virtual ~IXmlParser(void);

    /** ����XML�ļ��� 
    */
    virtual BOOL LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName ) = 0;

    /** ��ȡxml�ļ�·����*/
    virtual tstring GetXMLPath() const  = 0;



    /** ���档 */
    virtual BOOL SaveXML() = 0;

    /** �ͷţ����١� */
    virtual BOOL ReleaseXML() = 0;

    /** get name of root element */
//    virtual tstring GetRootName() = 0;

    /** ����һ���ڵ㡣
    *	ֻ�����ҵ��ĵ�һ����
    *	���û�ҵ������ؿ��ַ�����
    */
    virtual tstring GetElementValue( LPCTSTR strElementName ) = 0;

    /** дһ���ڵ㡣 
    *	ֻд���ҵ��ĵ�һ�������С�
    */
    virtual BOOL SetElementValue( LPCTSTR strElementName, LPCTSTR strValue ) = 0;
};
