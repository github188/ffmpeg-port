#pragma once
#include "afxwin.h"

/** 扩展静态控件。 
*	透明背景实现方式：抓取父窗口DC图像。
*	因为在控件窗口创建并且没有隐藏的时候，父窗口DC上绘图时控件所在位置上会出现空白。
*	所以在控件显示的时候先将其隐藏，之后抓取父窗口DC上的图像作为背景。
*/

class CStaticEx :
	public CStatic
{
public:
	CStaticEx(void);
	~CStaticEx(void);

public:
	/** 设置背景透明。 */
	BOOL SetBkTransparent( BOOL bTrans = TRUE, BOOL bRepaint = TRUE );

	/** 设置文本颜色。 */
	BOOL SetTextColor( COLORREF corNormal, COLORREF corDisable, BOOL bRepaint = TRUE );

	/** 获取文本颜色. */
	void GetTextColor( COLORREF& corNormal, COLORREF& corDisable ) const;

	/** 设置背景颜色。 */
	BOOL SetBkColor( COLORREF corBk );

	/** 获取背景颜色. */
	void GetBkColor( COLORREF& corBk ) const;

	/** 背景透明的时候，更新控件背景。 */
	BOOL UpdateBk();

private:
	/** 是否背景透明。 */
	BOOL m_bBkTransparent;

	/** 正常的文本颜色。 */
	COLORREF m_normalTextColor;
	/** 被禁用的时候的颜色。*/
	COLORREF m_disableTextColor;

	/** 背景颜色。 */
	COLORREF m_bkColor;

	/** 背景DC */
	CDC m_bkDC;
	/** 抓取到的背景DC的图片。*/
	CBitmap m_bkBmp;

	/** 是否需要隐藏自身以便抓取背景。 */
	BOOL m_bNeedFechBkImage;

	/** 定时器Id。*/
	UINT m_uTimerShowWindow;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnDestroy();
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
};
