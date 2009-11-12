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

	/** ��¼��Ϣ�������� */
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
	/** ��ҳ�档 */
	virtual BOOL OpenWebpage( LPCTSTR strLoginUrl ) = 0;

	/** ������ʾ���� */
	virtual void AssertError( ELoginInfoErrorCode ) = 0;

	
protected:
	/** ��¼�� 
	*	������á�
	*/
	BOOL Login( const TLoginInfo& tLoginInfo, ELoginType eLoginType );

	BOOL ReadConfig( TLoginInfo& tLoginInfo );
	BOOL SaveConfig( const TLoginInfo& tLoginInfo );

private:
	ELoginInfoErrorCode CheckInvalid( const TLoginInfo& tLoginInfo, ELoginType eLoginType );
};

