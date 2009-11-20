#pragma once

#include "mculib.h"
// CPictureStatic

class CPictureStatic : public CStatic
{
	DECLARE_DYNAMIC(CPictureStatic)

public:
	//enum EPicDisplayMode
	//{
	//	PDM_Orientation_0 = 0,						// 图片旋转,不旋转.
	//	PDM_Orientation_90,							// 图片顺时针旋转90度.
	//	PDM_Orientation_180,						// 图片旋转180度.
	//	PDM_Orientation_270,						// 图片顺时针旋转270度.
	//	PDM_Orientation_Default = PDM_Orientation_0,// 默认情况下图片不旋转.

	//};
public:
	CPictureStatic();
	virtual ~CPictureStatic();

	/** 设置图片路径. */
	void SetPicPath( LPCTSTR strPicPath, BOOL bRePaint = TRUE );

	/** 获取当前的图片路径。*/
	tstring GetPicPath() const;

	/** 适合图片大小. */
	BOOL FitToPicture();

	/** 设置透明色，是否支持透明。 */
	BOOL SetTransparentMask( BOOL bTransparent, COLORREF maskColor = NULL, BOOL bRePaint = TRUE );

	/** 设置背景透明。 */
	BOOL SetBkTransparent( BOOL bTrans, BOOL bRePaint = TRUE );

	/** 设置是否选中。 */
	void SetSelected( BOOL bSelected, BOOL bRePaint = TRUE );

	/** 设置颜色。 */
	void SetColor( COLORREF colSelectedFrame );

	/** 获取图片大小。 */
	BOOL GetImageSize( LPRECT prcImg );

	/** 设置图片显示模式. */
//	void SetPicDisplayMode( EPicDisplayMode eMode, BOOL bRePaint = TRUE );

	/** 获取当前的图片显示模式. */
//	EPicDisplayMode GetPicDisplayMode() const;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	tstring m_strPicPath;

	/** 透明色。 */
	COLORREF m_colorMask;

	/** 是否图片透明。 */
	BOOL m_bImageTrans;

	/** 是否背景透明。 */
	BOOL m_bBkTransparent;

	/** 背景DC */
	CDC m_bkDC;
	/** 抓取到的背景DC的图片。*/
	CBitmap m_bkBmp;

	/** 是否需要隐藏自身以便抓取背景。 */
	BOOL m_bNeedFechBkImage;

	/** 定时器Id。*/
	UINT m_uTimerShowWindow;

	/** 是否选中。 */
	BOOL m_bSelected;
	
	/** 选中框颜色。 */
	COLORREF m_colorSelectedFrame;

	/** 图片旋转.*/
//	EPicDisplayMode m_ePicDisplayMode;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
protected:
	virtual void PreSubclassWindow();
public:
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
};


