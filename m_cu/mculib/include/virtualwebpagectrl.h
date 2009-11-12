#pragma once

#include "videosession.h"
/** 抽象的*/

class CVirtualWebpageCtrl
{
public:
	enum EUrlType
	{
		UrlNormal = 0,
		UrlRtsp,
		UrlHttpWithParam,	// 携带有参数的http链接。
	};
public:
	CVirtualWebpageCtrl(void);
	virtual ~CVirtualWebpageCtrl(void);

	/** 载入页面，需要子类实现。*/
	virtual BOOL OpenUrl( LPCTSTR strUrl ) = 0;
protected:
	/** 打开播放器。 */
	virtual BOOL OpenPlayer( CVideoSession *pVideoSession ) = 0;

	/** 历史。 */
	virtual BOOL HistoryBack( int nStep ) = 0;
protected:
	/** 载入页面处理。 
	*	需要派生类调用。
	*	返回值： TRUE FALSE
	*/
	EUrlType OnLoadPage( LPCTSTR strUrl );

	/** 获取页面放大倍数。 */
	float GetZoomLevel() const;
};
