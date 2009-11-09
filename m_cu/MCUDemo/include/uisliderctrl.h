#pragma once
#include "afxcmn.h"

class CUISliderCtrl :
	public CSliderCtrl
{
public:
	CUISliderCtrl(void);
	~CUISliderCtrl(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
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

	/** 画控件的画笔。 */
	CPen m_penMain;
	/** 画选中时的包围方框的画笔。 */
	CPen m_penFrame;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};
