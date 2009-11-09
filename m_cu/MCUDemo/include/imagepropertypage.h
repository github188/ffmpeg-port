#pragma once
#include "afxdlgs.h"
#include "mcucommon.h"

class CImagePropertyPage :
	public CPropertyPage
{
	DECLARE_DYNAMIC(CImagePropertyPage)
public:
	CImagePropertyPage(void);
	~CImagePropertyPage(void);

	explicit CImagePropertyPage(UINT nIDTemplate, UINT nIDCaption = 0, DWORD dwSize = sizeof(PROPSHEETPAGE));

public:
	/** ����ͼƬ��*/
	BOOL SetImage( LPCTSTR strImagePath, BOOL bRePaint = TRUE );

	/** ���õ�ɫ�� */
	BOOL SetColor( COLORREF bgColor );

private:
	/** ͼƬ·���� */
	tstring m_strImagePath;
	/** ������ɫ�� */
	COLORREF m_corBg;
	/** ������ɫ�Ƿ����ù��� */
	BOOL m_bCorSet;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
