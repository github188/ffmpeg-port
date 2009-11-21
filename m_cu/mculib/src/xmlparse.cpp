#include "xmlparse.h"
#include "log.h"

CXmlParse::CXmlParse()
{
#ifdef USE_MS_XML_PARSER
	
	
#endif

#ifdef USE_LIB_XML
	this->m_pXMLDoc = NULL;
	this->m_pXMLRootNode = NULL;
#endif

}

CXmlParse::~CXmlParse()
{

}

BOOL CXmlParse::LoadXML( LPCTSTR strXmlFile, LPCTSTR strRootEleName )
{
	this->ReleaseXML();

	this->m_strXmlFileName = strXmlFile;

	BOOL bSuccess = FALSE;

#ifdef USE_MS_XML_PARSER
	COleVariant vXmlFile( this->m_strXmlFileName.c_str() );
	VARIANT_BOOL vSuccess;

	HRESULT hr = this->m_pXMLDoc.CoCreateInstance(__uuidof (DOMDocument));
	m_pXMLDoc->put_validateOnParse(VARIANT_FALSE);
	m_pXMLDoc->put_resolveExternals(VARIANT_FALSE);
	m_pXMLDoc->put_preserveWhiteSpace(VARIANT_FALSE);
	hr = m_pXMLDoc->load(vXmlFile,&vSuccess);

	// 载入失败时可能是文件不存在。保存一个出来。
	bSuccess = ( S_OK == hr );

#endif

#ifdef USE_LIB_XML
	// xmlNodePtr    proot_node = NULL ,pnode = NULL , pnode1 = NULL;

	m_pXMLDoc = xmlReadFile( strXmlFile ,"UTF-8",XML_PARSE_RECOVER);
	if(NULL == this->m_pXMLDoc )
	{
		this->m_pXMLDoc = xmlNewDoc( BAD_CAST "1.0");		
	}

#endif

	bSuccess &= this->SetRootElement( strRootEleName );
	bSuccess &= this->SaveXML();

	return bSuccess;
}

tstring CXmlParse::GetXMLPath() const
{
	return this->m_strXmlFileName;
}

BOOL CXmlParse::SetRootElement( LPCTSTR strEleName )
{
	this->m_strRootElementName = strEleName;
	BOOL bSuccess = FALSE;
#ifdef USE_MS_XML_PARSER
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
#endif

#ifdef USE_LIB_XML
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

#endif

	return bSuccess;
}

BOOL CXmlParse::SaveXML()
{
#ifdef USE_MS_XML_PARSER

	COleVariant vXmlFile( this->m_strXmlFileName.c_str() );
	HRESULT hr = this->m_pXMLDoc->save( vXmlFile );
	return ( S_OK == hr );
#endif

#ifdef USE_LIB_XML
	if( m_pXMLDoc )
	{
		//保存文档
		int nResult = xmlSaveFormatFileEnc( this->m_strXmlFileName.c_str(), this->m_pXMLDoc, "UTF-8", 1);
		return nResult > 0;
	}
	
	return FALSE;
#endif

}

BOOL CXmlParse::ReleaseXML()
{
	BOOL bSuccess = TRUE;
#ifdef USE_MS_XML_PARSER
	if ( this->m_pXMLRootNode )
	{
		this->m_pXMLRootNode.Release();
	}
	if ( this->m_pXMLDoc )
	{
		this->m_pXMLDoc.Release();
	}
#endif

#ifdef USE_LIB_XML
	//释放文档指针
	if( m_pXMLDoc )
	{
		xmlFreeDoc( m_pXMLDoc );
	}
	xmlCleanupParser();
	xmlMemoryDump();	  //debug memory for regression tests
	bSuccess = TRUE;
#endif
	return bSuccess;
}

tstring CXmlParse::GetElementValue(LPCTSTR strElementName)
{
	tstring strResult;
#ifdef USE_MS_XML_PARSER
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
#endif

#ifdef USE_LIB_XML
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

#endif

	return strResult;
}

BOOL CXmlParse::SetElementValue( LPCTSTR strElementName, LPCTSTR strValue )
{
	BOOL bResult = FALSE;
#ifdef USE_MS_XML_PARSER
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
#endif

#ifdef USE_LIB_XML
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
#endif
	// 写完保存。
	bResult &= this->SaveXML();
	return bResult;
}

