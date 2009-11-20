#pragma once

#include "mculib.h"

class CImageShow;
// CUIButton

class CUIButton : public CButton
{
	DECLARE_DYNAMIC(CUIButton)

public:
	CUIButton();
	virtual ~CUIButton();

	/** 设置要显示的图片. */
	void SetImage( LPCTSTR strImageNormal, 
		LPCTSTR strImgFocus, 
		LPCTSTR strImgDisable,
		BOOL bRePaint = TRUE );

	/** 适合图片大小. */
	BOOL FitToPicture();

	/** 设置透明色，是否支持图片透明色。 */
	BOOL SetTransparentMask( BOOL bTransparent, COLORREF maskColor = NULL, BOOL bRePaint = TRUE );

	/** 设置背景透明。 */
	BOOL SetBkTransparent( BOOL bTransparent, BOOL bRePaint = TRUE );

	/** 获取图片大小。 */
	BOOL GetImageSize( LPRECT prcImg );
private:
	/** 获取当前显示的图片。 */
	tstring GetCurImage() const;
private:
	/** 按钮正常状态时显示的图片路径. */
	tstring m_strNormalPicPath;
	/** 按钮拥有焦点时的图片。 */
	tstring m_strPicPathFocus;

	/** 按钮禁用时的图片。 */
	tstring m_strPicPathDisable;

	/** 透明色。 */
	COLORREF m_colorMask;
	/** 是否图片透明。 */
	BOOL m_bImageTransparent;

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

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


