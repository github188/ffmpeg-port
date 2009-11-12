#pragma once

#include "videosession.h"
/** �����*/

class CVirtualWebpageCtrl
{
public:
	enum EUrlType
	{
		UrlNormal = 0,
		UrlRtsp,
		UrlHttpWithParam,	// Я���в�����http���ӡ�
	};
public:
	CVirtualWebpageCtrl(void);
	virtual ~CVirtualWebpageCtrl(void);

	/** ����ҳ�棬��Ҫ����ʵ�֡�*/
	virtual BOOL OpenUrl( LPCTSTR strUrl ) = 0;
protected:
	/** �򿪲������� */
	virtual BOOL OpenPlayer( CVideoSession *pVideoSession ) = 0;

	/** ��ʷ�� */
	virtual BOOL HistoryBack( int nStep ) = 0;
protected:
	/** ����ҳ�洦�� 
	*	��Ҫ��������á�
	*	����ֵ�� TRUE FALSE
	*/
	EUrlType OnLoadPage( LPCTSTR strUrl );

	/** ��ȡҳ��Ŵ����� */
	float GetZoomLevel() const;
};
