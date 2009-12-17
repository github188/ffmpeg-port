#include "LibXmlXmlParser.h"

CLibXmlXmlParser::CLibXmlXmlParser(void)
{
    this->m_pXMLDoc = NULL;
    this->m_pXMLRootNode = NULL;
}

CLibXmlXmlParser::~CLibXmlXmlParser(void)
{
    
}

BOOL CLibXmlXmlParser::LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName )
{
    this->ReleaseXML();

    this->m_strXmlFileName = strXmlFile;

    BOOL bSuccess = FALSE;

    // xmlNodePtr    proot_node = NULL ,pnode = NULL , pnode1 = NULL;

    m_pXMLDoc = xmlReadFile( strXmlFile ,"UTF-8",XML_PARSE_RECOVER);
    if(NULL == this->m_pXMLDoc )
    {
        this->m_pXMLDoc = xmlNewDoc( BAD_CAST "1.0");		
    }



    bSuccess &= this->SetRootElement( strRootEleName );
    bSuccess &= this->SaveXML();

    return bSuccess;
}

tstring CLibXmlXmlParser::GetXMLPath() const
{
    return this->m_strXmlFileName;
}

BOOL CLibXmlXmlParser::SetRootElement( LPCTSTR strEleName )
{
    this->m_strRootElementName = strEleName;
    BOOL bSuccess = FALSE;

    if ( m_pXMLDoc )
    {
        this->m_pXMLRootNode = xmlDocGetRootElement( m_pXMLDoc );
        if( !m_pXMLRootNode )
        {
            // Create root node.
            this->m_pXMLRootNode = xmlNewNode(NULL , BAD_CAST ( strEleName ));
            if( this->m_pXMLRootNode )
            {
                xmlDocSetRootElement( this->m_pXMLDoc, m_pXMLRootNode );
            }
        }
    }
    bSuccess &= ( NULL != this->m_pXMLDoc );
    bSuccess &= ( NULL != this->m_pXMLRootNode );


    return bSuccess;
}

BOOL CLibXmlXmlParser::SaveXML()
{


    if( m_pXMLDoc )
    {
        //保存文档
        int nResult = xmlSaveFormatFileEnc( this->m_strXmlFileName.c_str(), this->m_pXMLDoc, "UTF-8", 1);
        return nResult > 0;
    }

    return FALSE;


}

BOOL CLibXmlXmlParser::ReleaseXML()
{
    BOOL bSuccess = TRUE;



    //释放文档指针
    if( m_pXMLDoc )
    {
        xmlFreeDoc( m_pXMLDoc );
    }
    xmlCleanupParser();
    xmlMemoryDump();	  //debug memory for regression tests
    bSuccess = TRUE;

    return bSuccess;
}

tstring CLibXmlXmlParser::GetElementValue(LPCTSTR strElementName)
{
    tstring strResult;



    if( this->m_pXMLRootNode )
    {
        xmlNodePtr pChild = this->m_pXMLRootNode->children;
        while( pChild )
        {
            if( xmlStrcmp( pChild->name , BAD_CAST(strElementName) ) == 0 )
            {
                break;
            }
            pChild = pChild->next;
        }

        if( pChild )
        {
            // !Content of the node can't be read directly!
            strResult = (LPCTSTR)xmlNodeGetContent( pChild );//(LPCTSTR)pChild->content;
            //			mcu::tlog << "read config: " << strResult << endl;
        }
    }


    return strResult;
}

BOOL CLibXmlXmlParser::SetElementValue( LPCTSTR strElementName, LPCTSTR strValue )
{
    BOOL bResult = FALSE;


    if( this->m_pXMLRootNode )
    {
        xmlNodePtr pChild = this->m_pXMLRootNode->children;
        while( pChild )
        {
            if( xmlStrcmp( pChild->name , BAD_CAST(strElementName) ) == 0 )
            {
                break;
            }
            pChild = pChild->next;
        }
        if( !pChild )
        {
            //			pChild = xmlNewNode(NULL , BAD_CAST ( strEleName ));
            pChild = xmlNewChild( this->m_pXMLRootNode, NULL, BAD_CAST(strElementName), BAD_CAST(strValue) );
        }
        if( pChild )
        {
            xmlNodeSetContent( pChild,  BAD_CAST( strValue ) );
            //pChild->content = BAD_CAST( strValue );
        }
        bResult = ( NULL != pChild );
    }

    // 写完保存。
    bResult &= this->SaveXML();
    return bResult;
}

