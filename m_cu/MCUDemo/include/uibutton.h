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

	/** ����Ҫ��ʾ��ͼƬ. */
	void SetImage( LPCTSTR strImageNormal, 
		LPCTSTR strImgFocus, 
		LPCTSTR strImgDisable,
		BOOL bRePaint = TRUE );

	/** �ʺ�ͼƬ��С. */
	BOOL FitToPicture();

	/** ����͸��ɫ���Ƿ�֧��ͼƬ͸��ɫ�� */
	BOOL SetTransparentMask( BOOL bTransparent, COLORREF maskColor = NULL, BOOL bRePaint = TRUE );

	/** ���ñ���͸���� */
	BOOL SetBkTransparent( BOOL bTransparent, BOOL bRePaint = TRUE );

	/** ��ȡͼƬ��С�� */
	BOOL GetImageSize( LPRECT prcImg );
private:
	/** ��ȡ��ǰ��ʾ��ͼƬ�� */
	tstring GetCurImage() const;
private:
	/** ��ť����״̬ʱ��ʾ��ͼƬ·��. */
	tstring m_strNormalPicPath;
	/** ��ťӵ�н���ʱ��ͼƬ�� */
	tstring m_strPicPathFocus;

	/** ��ť����ʱ��ͼƬ�� */
	tstring m_strPicPathDisable;

	/** ͸��ɫ�� */
	COLORREF m_colorMask;
	/** �Ƿ�ͼƬ͸���� */
	BOOL m_bImageTransparent;

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


