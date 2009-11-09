#include "StdAfx.h"
#include "UrlParse.h"

CUrlParse::CUrlParse(void)
{
}

CUrlParse::~CUrlParse(void)
{
}

BOOL CUrlParse::Parse( LPCTSTR strUrl )
{
	m_strUrl = strUrl;

	tstring::const_iterator iter = m_strUrl.begin();

	tstringstream ssProtocol, ssParamName, ssParamValue;
	tstring strParamName, strParamValue;
	while ( iter != m_strUrl.end() )
	{
		switch( *iter )
		{
		case '?':	// 参数列表开始.
			ssParamName.str(_T(""));
			break;
		case ':':	// 协议结束.
			m_strProtocol = ssProtocol.str();
			break;
		case '=':
			strParamName = ssParamName.str();	// 取参数名.
			ssParamValue.str(_T(""));				// 准备接收参数值.
		    break;
		case '&':
		case 0:
			strParamValue = ssParamValue.str();	// 取参数值.

			m_tParamTable[ strParamName ] = strParamValue;	// 将取全的参数放入表中.

			ssParamName.str(_T(""));				// 准备接收下个参数名.
			break;

		
			break;
	
		default:
			ssProtocol << *iter;
			ssParamName << *iter;
			ssParamValue << *iter;
		    break;
		}
		
		++iter;
	}

	if ( !strParamName.empty() || !ssParamValue.str().empty() )
	{
		// 最后一对参数可能没有结尾.
		strParamValue = ssParamValue.str();	// 取参数值.

		m_tParamTable[ strParamName ] = strParamValue;	// 将取全的参数放入表中.
	}


	return TRUE;
}

CUrlParse::TParamNameList CUrlParse::ParamList() const
{
	TParamNameList tAllParamName;
	TParamTable::const_iterator iter = m_tParamTable.begin();
	for ( ; iter != m_tParamTable.end(); ++iter )
	{
		tAllParamName.push_back( iter->first );
	}
	return tAllParamName;
}

BOOL CUrlParse::ParamValue( LPCTSTR strParamName, tstring& strParamValue ) const
{
	TParamTable::const_iterator iter = m_tParamTable.find( strParamName );
	if ( iter != m_tParamTable.end() )
	{
		strParamValue = iter->second;
		return TRUE;
	}

	return FALSE;
}

BOOL CUrlParse::ParamValue( LPCTSTR strParamName, int & iParamValue ) const
{
	tstring strValue;
	BOOL bResult = ParamValue( strParamName, strValue );
	if ( bResult )
	{
		iParamValue = _ttoi( strValue.c_str() );
	}
	return bResult;
}