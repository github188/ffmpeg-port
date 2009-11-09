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
		case '?':	// �����б�ʼ.
			ssParamName.str(_T(""));
			break;
		case ':':	// Э�����.
			m_strProtocol = ssProtocol.str();
			break;
		case '=':
			strParamName = ssParamName.str();	// ȡ������.
			ssParamValue.str(_T(""));				// ׼�����ղ���ֵ.
		    break;
		case '&':
		case 0:
			strParamValue = ssParamValue.str();	// ȡ����ֵ.

			m_tParamTable[ strParamName ] = strParamValue;	// ��ȡȫ�Ĳ����������.

			ssParamName.str(_T(""));				// ׼�������¸�������.
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
		// ���һ�Բ�������û�н�β.
		strParamValue = ssParamValue.str();	// ȡ����ֵ.

		m_tParamTable[ strParamName ] = strParamValue;	// ��ȡȫ�Ĳ����������.
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