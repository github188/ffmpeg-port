
#include "urlparse.h"

CUrlParse::CUrlParse(void)
{
}

CUrlParse::~CUrlParse(void)
{
}

BOOL CUrlParse::Parse( LPCTSTR strUrl )
{
	m_strUrl = strUrl;
    this->m_strProtocol.clear();
    this->m_strServerName.clear();
    this->m_tParamTable.clear();

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
			if( m_strProtocol.empty() )
			{
				m_strProtocol = ssProtocol.str();
			}	
            else
            {
                // Э���Ѿ����������ˣ�ʣ�µĲ���Э�飬���������ݡ�
                ssProtocol << *iter;
                ssParamName << *iter;
                ssParamValue << *iter;
            }
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
		tstringstream ssTmp;
		ssTmp << strValue;
		ssTmp >> iParamValue;
//		iParamValue = _ttoi( strValue.c_str() );
	}
	return bResult;
}

tstring CUrlParse::Protocol() const
{
    return m_strProtocol;
}

//tstring CUrlParse::Server() const
//{
//    return m_strServerName;
//}

