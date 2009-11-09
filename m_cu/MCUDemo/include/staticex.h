#pragma once
#include "afxwin.h"

/** ��չ��̬�ؼ��� 
*	͸������ʵ�ַ�ʽ��ץȡ������DCͼ��
*	��Ϊ�ڿؼ����ڴ�������û�����ص�ʱ�򣬸�����DC�ϻ�ͼʱ�ؼ�����λ���ϻ���ֿհס�
*	�����ڿؼ���ʾ��ʱ���Ƚ������أ�֮��ץȡ������DC�ϵ�ͼ����Ϊ������
*/

class CStaticEx :
	public CStatic
{
public:
	CStaticEx(void);
	~CStaticEx(void);

public:
	/** ���ñ���͸���� */
	BOOL SetBkTransparent( BOOL bTrans = TRUE, BOOL bRepaint = TRUE );

	/** �����ı���ɫ�� */
	BOOL SetTextColor( COLORREF corNormal, COLORREF corDisable, BOOL bRepaint = TRUE );

	/** ��ȡ�ı���ɫ. */
	void GetTextColor( COLORREF& corNormal, COLORREF& corDisable ) const;

	/** ���ñ�����ɫ�� */
	BOOL SetBkColor( COLORREF corBk );

	/** ��ȡ������ɫ. */
	void GetBkColor( COLORREF& corBk ) const;

	/** ����͸����ʱ�򣬸��¿ؼ������� */
	BOOL UpdateBk();

private:
	/** �Ƿ񱳾�͸���� */
	BOOL m_bBkTransparent;

	/** �������ı���ɫ�� */
	COLORREF m_normalTextColor;
	/** �����õ�ʱ�����ɫ��*/
	COLORREF m_disableTextColor;

	/** ������ɫ�� */
	COLORREF m_bkColor;

	/** ����DC */
	CDC m_bkDC;
	/** ץȡ���ı���DC��ͼƬ��*/
	CBitmap m_bkBmp;

	/** �Ƿ���Ҫ���������Ա�ץȡ������ */
	BOOL m_bNeedFechBkImage;

	/** ��ʱ��Id��*/
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
