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
	/** �Ƿ񱳾�͸���� */
	BOOL m_bBkTransparent;

	/** ����DC */
	CDC m_bkDC;
	/** ץȡ���ı���DC��ͼƬ��*/
	CBitmap m_bkBmp;

	/** �Ƿ���Ҫ���������Ա�ץȡ������ */
	BOOL m_bNeedFechBkImage;

	/** ��ʱ��Id��*/
	UINT m_uTimerShowWindow;

	/** ���ؼ��Ļ��ʡ� */
	CPen m_penMain;
	/** ��ѡ��ʱ�İ�Χ����Ļ��ʡ� */
	CPen m_penFrame;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};
