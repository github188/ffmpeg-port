#pragma once
#include "ixmlparse.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

class CLibXmlXmlParser :
    public IXmlParser
{
public:
    CLibXmlXmlParser(void);
    virtual ~CLibXmlXmlParser(void);

    /** ����XML�ļ��� 
    */
    virtual BOOL LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName );

    /** Create xml */
    //	BOOL CreateXML( LPCTSTR strXmlFile, LPCTSTR strDocRootName );

    /** ��ȡxml�ļ�·����*/
    virtual tstring GetXMLPath() const;



    /** ���档 */
    virtual BOOL SaveXML();

    /** �ͷţ����١� */
    virtual BOOL ReleaseXML();

    /** get name of root element */
//    virtual tstring GetRootName();

    /** ����һ���ڵ㡣
    *	ֻ�����ҵ��ĵ�һ����
    *	���û�ҵ������ؿ��ַ�����
    */
    virtual tstring GetElementValue( LPCTSTR strElementName );

    /** дһ���ڵ㡣 
    *	ֻд���ҵ��ĵ�һ�������С�
    */
    virtual BOOL SetElementValue( LPCTSTR strElementName, LPCTSTR strValue );
private:
    /** ���õ�ǰ��Ϊ�������ӽڵ㡣 
    *	���ҽڵ��ʱ��ֻ���ڵ�ǰ�����ڵ������һ���ӽڵ���������
    */
    BOOL SetRootElement( LPCTSTR strEleName );

private:

    /** �ļ����� */
    tstring m_strXmlFileName;

    /** root element */
    tstring m_strRootElementName;

    xmlDocPtr	m_pXMLDoc;
    xmlNodePtr	m_pXMLRootNode;
};
