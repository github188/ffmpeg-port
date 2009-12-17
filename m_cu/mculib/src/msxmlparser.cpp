#include "..\include\msxmlparser.h"
#include "log.h"

CMsXmlParser::CMsXmlParser(void)
{
}

CMsXmlParser::~CMsXmlParser(void)
{
}

BOOL CMsXmlParser::LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName )
{
    this->ReleaseXML();

    this->m_strXmlFileName = strXmlFile;

    BOOL bSuccess = FALSE;

    COleVariant vXmlFile( this->m_strXmlFileName.c_str() );
    VARIANT_BOOL vSuccess;

    HRESULT hr = this->m_pXMLDoc.CoCreateInstance(__uuidof (DOMDocument));
    m_pXMLDoc->put_validateOnParse(VARIANT_FALSE);
    m_pXMLDoc->put_resolveExternals(VARIANT_FALSE);
    m_pXMLDoc->put_preserveWhiteSpace(VARIANT_FALSE);
    hr = m_pXMLDoc->load(vXmlFile,&vSuccess);

    // 载入失败时可能是文件不存在。保存一个出来。
    bSuccess = ( S_OK == hr );

    bSuccess &= this->SetRootElement( strRootEleName );
    bSuccess &= this->SaveXML();

    return bSuccess;
}

tstring CMsXmlParser::GetXMLPath() const
{
    return this->m_strXmlFileName;
}

BOOL CMsXmlParser::SetRootElement( LPCTSTR strEleName )
{
    this->m_strRootElementName = strEleName;
    BOOL bSuccess = FALSE;


    // 获取config节点。
    if ( m_pXMLDoc )
    {
        IXMLDOMNodePtr pCfgNode;
        IXMLDOMNodeListPtr pNodeList;
        HRESULT hr = m_pXMLDoc->getElementsByTagName( XML_TEXT( strEleName ), &pNodeList );
        long lCount = 0;
        if ( pNodeList )
        {
            pNodeList->get_length( &lCount );
        }
        if ( lCount > 0 )
        {			
            _ASSERT( lCount == 1 );
            pNodeList->get_item( 0, &pCfgNode );
        }
        else
        {
            // 没有这个节点，创建新的。
            IXMLDOMElementPtr pNewEle;
            hr = m_pXMLDoc->createElement( XML_TEXT( strEleName ), &pNewEle );
            bSuccess = ( S_OK == hr );
            if ( bSuccess )
            {
                hr = m_pXMLDoc->appendChild( pNewEle, &pCfgNode );
                bSuccess = ( S_OK == hr );
                this->SaveXML();
            }
        }

        m_pXMLRootNode = pCfgNode;

        bSuccess = NULL != m_pXMLRootNode;
    }


    return bSuccess;
}

BOOL CMsXmlParser::SaveXML()
{


    COleVariant vXmlFile( this->m_strXmlFileName.c_str() );
    HRESULT hr = this->m_pXMLDoc->save( vXmlFile );
    return ( S_OK == hr );

}

BOOL CMsXmlParser::ReleaseXML()
{
    BOOL bSuccess = TRUE;

    if ( this->m_pXMLRootNode )
    {
        this->m_pXMLRootNode.Release();
    }
    if ( this->m_pXMLDoc )
    {
        this->m_pXMLDoc.Release();
    }

    return bSuccess;
}

tstring CMsXmlParser::GetElementValue(LPCTSTR strElementName)
{
    tstring strResult;
    if ( m_pXMLRootNode )
    {
        IXMLDOMNodeListPtr pNodeList;
        IXMLDOMNodePtr pNode;



        HRESULT hr = this->m_pXMLRootNode->getElementsByTagName( XML_TEXT( strElementName ), &pNodeList );
        long lCount = 0;
        if ( pNodeList )
        {
            pNodeList->get_length( &lCount );
        }
        if ( lCount > 0 )
        {
            // 找到了。
            pNodeList->get_item( 0, &pNode );
        }

        if ( pNode )
        {
            BSTR strValue;
            pNode->get_text( &strValue );
            strResult = strValue;
        }
        else
        {
            strResult = _T("");
        }
    }	


    return strResult;
}

BOOL CMsXmlParser::SetElementValue( LPCTSTR strElementName, LPCTSTR strValue )
{
    BOOL bResult = FALSE;
    IXMLDOMNodeListPtr pNodeList;
    IXMLDOMNodePtr pNode;

    HRESULT hr = this->m_pXMLRootNode->getElementsByTagName( XML_TEXT( strElementName ), &pNodeList );
    long lCount = 0;
    if ( pNodeList )
    {
        pNodeList->get_length( &lCount );
    }
    if ( lCount > 0 )
    {
        // 找到了。
        pNodeList->get_item( 0, &pNode );
    }
    else
    {
        IXMLDOMElementPtr pEle;
        hr = this->m_pXMLDoc->createElement( XML_TEXT( strElementName ), &pEle );
        if ( S_OK == hr )
        {
            this->m_pXMLRootNode->appendChild( pEle, &pNode );
        }
    }

    if ( pNode )
    {
        hr = pNode->put_text( XML_TEXT( strValue ) );
    }



    bResult = ( S_OK == hr );

    // 写完保存。
    bResult &= this->SaveXML();
    return bResult;
}

