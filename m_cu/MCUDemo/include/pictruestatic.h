#pragma once

#include "mculib.h"
// CPictureStatic

class CPictureStatic : public CStatic
{
	DECLARE_DYNAMIC(CPictureStatic)

public:
	//enum EPicDisplayMode
	//{
	//	PDM_Orientation_0 = 0,						// ͼƬ��ת,����ת.
	//	PDM_Orientation_90,							// ͼƬ˳ʱ����ת90��.
	//	PDM_Orientation_180,						// ͼƬ��ת180��.
	//	PDM_Orientation_270,						// ͼƬ˳ʱ����ת270��.
	//	PDM_Orientation_Default = PDM_Orientation_0,// Ĭ�������ͼƬ����ת.

	//};
public:
	CPictureStatic();
	virtual ~CPictureStatic();

	/** ����ͼƬ·��. */
	void SetPicPath( LPCTSTR strPicPath, BOOL bRePaint = TRUE );

	/** ��ȡ��ǰ��ͼƬ·����*/
	tstring GetPicPath() const;

	/** �ʺ�ͼƬ��С. */
	BOOL FitToPicture();

	/** ����͸��ɫ���Ƿ�֧��͸���� */
	BOOL SetTransparentMask( BOOL bTransparent, COLORREF maskColor = NULL, BOOL bRePaint = TRUE );

	/** ���ñ���͸���� */
	BOOL SetBkTransparent( BOOL bTrans, BOOL bRePaint = TRUE );

	/** �����Ƿ�ѡ�С� */
	void SetSelected( BOOL bSelected, BOOL bRePaint = TRUE );

	/** ������ɫ�� */
	void SetColor( COLORREF colSelectedFrame );

	/** ��ȡͼƬ��С�� */
	BOOL GetImageSize( LPRECT prcImg );

	/** ����ͼƬ��ʾģʽ. */
//	void SetPicDisplayMode( EPicDisplayMode eMode, BOOL bRePaint = TRUE );

	/** ��ȡ��ǰ��ͼƬ��ʾģʽ. */
//	EPicDisplayMode GetPicDisplayMode() const;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	tstring m_strPicPath;

	/** ͸��ɫ�� */
	COLORREF m_colorMask;

	/** �Ƿ�ͼƬ͸���� */
	BOOL m_bImageTrans;

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

	/** �Ƿ�ѡ�С� */
	BOOL m_bSelected;
	
	/** ѡ�п���ɫ�� */
	COLORREF m_colorSelectedFrame;

	/** ͼƬ��ת.*/
//	EPicDisplayMode m_ePicDisplayMode;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
protected:
	virtual void PreSubclassWindow();
public:
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
};


