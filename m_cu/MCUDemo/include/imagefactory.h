#pragma once

#include "mculib.h"

class CImageShow;

class CImageFactory
{
public:
	static CImageFactory *Instance();
	static void Release();

	/** 获取图片类.
	*	增加相对名称的支持。如果传入图片名中没有“\”，就到设置的默认目录下去寻找该图片。
	*/
	CImageShow *GetImage( LPCTSTR strImagePath );

	/** 销毁图片类。 */
	BOOL ReleaseImage( LPCTSTR strImagePath );

	/** 设置默认的图片搜索路径。 */
	void SetDefaultPicDir( LPCTSTR strImageDir );

private:
	static CImageFactory *s_instance;

	/** 图片映射表. */
	typedef map< tstring, CImageShow * > TImageTable;
	TImageTable m_tImageTable;
	
	/** 默认的图片目录. */
	tstring m_strDefaultPicDirectory;

protected:
	CImageFactory(void);
	~CImageFactory(void);
};
