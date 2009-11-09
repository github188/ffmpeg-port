#pragma once

#include <imaging.h>

struct CImageInfo
{
public:
	int Width;
	int Height;

	CImageInfo()
	{
		Width = 0;
		Height = 0;
	}
};

class CImageShow
{
public:
	//enum EImageDrawFlag
	//{
	//	IDF_Orientation_0 = 0,						// 图片旋转,不旋转.
	//	IDF_Orientation_90,							// 图片顺时针旋转90度.
	//	IDF_Orientation_180,						// 图片旋转180度.
	//	IDF_Orientation_270,						// 图片顺时针旋转270度.
	//	IDF_Orientation_Default = IDF_Orientation_0,// 默认情况下图片不旋转.

	//};
public:
	CImageShow(void);
	virtual ~CImageShow(void);

	BOOL LoadImage( LPCTSTR strImage );
	BOOL Draw( HDC hdc, LPRECT pRect = NULL/*, EImageDrawFlag eIDF = IDF_Orientation_Default*/ );

	BOOL IsOk() const;

	/** 获取图片信息. */
	BOOL GetImageInfo( CImageInfo& imgInfo );

	/** 设置透明色。 */
	BOOL SetTransparentMask( BOOL bTransparent, COLORREF maskColor = NULL );


private:
	IImage *m_pImage;
	IImagingFactory *m_pImgFactory;

	/** 透明色。 */
	COLORREF m_colorMask;
	/** 是否透明。 */
	BOOL m_bTransparent;

	///** 合成后的透明化处理图。 */
	//CBitmap m_bmpFinal;
	///** 合成前的图片。 */
	//CBitmap m_bmpImage;

	///** 合成用的DC。 */
	//CDC m_dcCombine;

	///** 最近一次使用的目标绘图DC。 */
	//HDC m_hLastDC;

};
