#pragma once

#include "mcudef.h"


class CUrlParse
{
public:
	CUrlParse(void);
	virtual ~CUrlParse(void);

	/** ����url. */
	BOOL Parse( LPCTSTR strUrl );
	
	/** ��ȡЭ��. */
	tstring Protocol() const;

	/** ��ȡ������������ip*/
	tstring Server() const;

	/** ����url*/
	tstring BasicUrl() const;

	/** ��ȡ�����б�. */
	typedef std::vector<tstring> TParamNameList;
	TParamNameList ParamList() const;

	/** ��ȡ������ֵ. */
	BOOL ParamValue( LPCTSTR strParamName, tstring& strParamValue ) const;

	BOOL ParamValue( LPCTSTR strParamName, int & iParamValue  ) const;

private:
	/** ԭʼurl. */
	tstring m_strUrl;
	
	/** Э��. */
	tstring m_strProtocol;

	/** ������������ip*/
	tstring m_strServerName;

	/** ������ֵ���б�.*/
	typedef std::map< tstring, tstring > TParamTable;
	TParamTable m_tParamTable;
};
