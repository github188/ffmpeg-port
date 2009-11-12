#pragma once

#include "mcudefine.h"

class CVirtualConfigWnd
{
public:
	struct TConfigInfo
	{
		tstring m_strServerUrl;	// ������Url��
		tstring m_strRegion;	// �ͻ�������
		tstring m_strSavePath;	// ����·����
		BOOL m_bCustomSavePath;	// �ǲ����û��Զ���ı���·����
		int m_nPtzSpeed;	// ��̨�ٶȡ�
		int m_nLenSpeed;	// ��ͷ�ٶȡ�

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
	/** ��ȡ�� */
	BOOL ReadConfig( TConfigInfo& tConfInfo );
	/** д�롣 */
	BOOL SaveConfig( const TConfigInfo& tConfInfo );

};
