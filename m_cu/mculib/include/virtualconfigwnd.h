#pragma once

#include "mcudefine.h"

class CVirtualConfigWnd
{
public:
	struct TConfigInfo
	{
		tstring m_strServerUrl;	// 服务器Url。
		tstring m_strRegion;	// 客户域名。
		tstring m_strSavePath;	// 保存路径。
		BOOL m_bCustomSavePath;	// 是不是用户自定义的保存路径。
		int m_nPtzSpeed;	// 云台速度。
		int m_nLenSpeed;	// 镜头速度。

		TConfigInfo()
		{
			m_bCustomSavePath = FALSE;
			m_nLenSpeed = 5;
			m_nPtzSpeed = 5;
		}
	};
public:
	CVirtualConfigWnd(void);
	virtual ~CVirtualConfigWnd(void);

protected:
	/** 读取。 */
	BOOL ReadConfig( TConfigInfo& tConfInfo );
	/** 写入。 */
	BOOL SaveConfig( const TConfigInfo& tConfInfo );

};
