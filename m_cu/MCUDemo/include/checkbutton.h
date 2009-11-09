#pragma once
#include "afxwin.h"
#include "mcucommon.h"

class CCheckButton :
	public CButton
{
public:
	CCheckButton(void);
	~CCheckButton(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	/** 设置颜色。 */
	BOOL SetColor( COLORREF colorFront, COLORREF colorBack, BOOL bRePaint = TRUE );
	/** 是否透明。 */
	BOOL SetBkTransparent( BOOL bTrans, BOOL bRepaint = TRUE );

	/** 设置图片。 */
	BOOL SetImage( LPCTSTR strNormalImage,
		LPCTSTR strDisableImage,
		LPCTSTR strFocusedImage,
		LPCTSTR strNormalSelectedImage,
		LPCTSTR strDisableSelectedImage,
		LPCTSTR strFocusedSelectedImage,
		BOOL bRePaint = TRUE );

private:
	/** 是否背景透明。 */
	BOOL m_bBkTransparent;

	/** 是否选中。 */
	UINT m_uCheckState;

	/** 前景色。包括方框，选中标志，文字。 */
	COLORREF m_colorFront;
	/** 背景色。未选中时方框中的填色。 */
	COLORREF m_colorBack;

	/** 抓取父窗口图片的DC。 */
	CDC m_bkDC;
	/** 抓取到的背景DC的图片。*/
	CBitmap m_bkBmp;

	/** 是否需要隐藏自身以便抓取背景。 */
	BOOL m_bNeedFechBkImage;

	/** 定时器Id。*/
	UINT m_uTimerShowWindow;


	/** 正常状态的图片。 */
	tstring m_strNormalImage;

	/** 禁用状态图片。 */
	tstring m_strDisableImage;

	/** 获得焦点时的图片。 */
	tstring m_strFocusedImage;

	/** 正常的选中状态时候的图片。 */
	tstring m_strNormalCheckedImage;

	/** 禁用的选中状态时的图片。 */
	tstring m_strDisableCheckedImage;

	/** 获得焦点的时候的选中状态时的图片。 */
	tstring m_strFocusedCheckedImage;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnBnClicked();
	afx_msg LRESULT OnSetCheck( WPARAM fCheck, LPARAM );
	afx_msg LRESULT OnGetCheck( WPARAM, LPARAM );

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
