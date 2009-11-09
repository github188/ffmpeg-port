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
	/** ������ɫ�� */
	BOOL SetColor( COLORREF colorFront, COLORREF colorBack, BOOL bRePaint = TRUE );
	/** �Ƿ�͸���� */
	BOOL SetBkTransparent( BOOL bTrans, BOOL bRepaint = TRUE );

	/** ����ͼƬ�� */
	BOOL SetImage( LPCTSTR strNormalImage,
		LPCTSTR strDisableImage,
		LPCTSTR strFocusedImage,
		LPCTSTR strNormalSelectedImage,
		LPCTSTR strDisableSelectedImage,
		LPCTSTR strFocusedSelectedImage,
		BOOL bRePaint = TRUE );

private:
	/** �Ƿ񱳾�͸���� */
	BOOL m_bBkTransparent;

	/** �Ƿ�ѡ�С� */
	UINT m_uCheckState;

	/** ǰ��ɫ����������ѡ�б�־�����֡� */
	COLORREF m_colorFront;
	/** ����ɫ��δѡ��ʱ�����е���ɫ�� */
	COLORREF m_colorBack;

	/** ץȡ������ͼƬ��DC�� */
	CDC m_bkDC;
	/** ץȡ���ı���DC��ͼƬ��*/
	CBitmap m_bkBmp;

	/** �Ƿ���Ҫ���������Ա�ץȡ������ */
	BOOL m_bNeedFechBkImage;

	/** ��ʱ��Id��*/
	UINT m_uTimerShowWindow;


	/** ����״̬��ͼƬ�� */
	tstring m_strNormalImage;

	/** ����״̬ͼƬ�� */
	tstring m_strDisableImage;

	/** ��ý���ʱ��ͼƬ�� */
	tstring m_strFocusedImage;

	/** ������ѡ��״̬ʱ���ͼƬ�� */
	tstring m_strNormalCheckedImage;

	/** ���õ�ѡ��״̬ʱ��ͼƬ�� */
	tstring m_strDisableCheckedImage;

	/** ��ý����ʱ���ѡ��״̬ʱ��ͼƬ�� */
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
