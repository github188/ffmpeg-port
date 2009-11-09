#pragma once

#include "mcudef.h"


class CUrlParse
{
public:
	CUrlParse(void);
	virtual ~CUrlParse(void);

	/** 解析url. */
	BOOL Parse( LPCTSTR strUrl );
	
	/** 获取协议. */
	tstring Protocol() const;

	/** 获取服务器域名或ip*/
	tstring Server() const;

	/** 基本url*/
	tstring BasicUrl() const;

	/** 获取参数列表. */
	typedef std::vector<tstring> TParamNameList;
	TParamNameList ParamList() const;

	/** 获取参数的值. */
	BOOL ParamValue( LPCTSTR strParamName, tstring& strParamValue ) const;

	BOOL ParamValue( LPCTSTR strParamName, int & iParamValue  ) const;

private:
	/** 原始url. */
	tstring m_strUrl;
	
	/** 协议. */
	tstring m_strProtocol;

	/** 服务器域名或ip*/
	tstring m_strServerName;

	/** 参数和值的列表.*/
	typedef std::map< tstring, tstring > TParamTable;
	TParamTable m_tParamTable;
};
