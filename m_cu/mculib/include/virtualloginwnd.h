#pragma once

#include "mcudefine.h"

class CVirtualLoginWnd
{
public:
	struct TLoginInfo
	{
		tstring m_strUserId;
		tstring m_strPassword;
		EStreamType m_eStreamType;
		BOOL m_bSaveLoginInfo;

		TLoginInfo()
		{
			m_eStreamType = STREAM_1X;
			m_bSaveLoginInfo = TRUE;
		}
	};

	/** 登录信息参数错误。 */
	enum ELoginInfoErrorCode
	{
		NoError,
		ErrorUserIdEmpty,
		ErrorPasswordEmpty,
		ErrorServerUrlEmpty,
		ErrorRegionEmpty,
	};

public:
	CVirtualLoginWnd(void);
	virtual ~CVirtualLoginWnd(void);

protected:
	/** 打开页面。 */
	virtual BOOL OpenWebpage( LPCTSTR strLoginUrl ) = 0;

	/** 界面提示错误。 */
	virtual void AssertError( ELoginInfoErrorCode ) = 0;

	
protected:
	/** 登录。 
	*	子类调用。
	*/
	BOOL Login( const TLoginInfo& tLoginInfo, ELoginType eLoginType );

	BOOL ReadConfig( TLoginInfo& tLoginInfo );
	BOOL SaveConfig( const TLoginInfo& tLoginInfo );

private:
	ELoginInfoErrorCode CheckInvalid( const TLoginInfo& tLoginInfo, ELoginType eLoginType );
};

