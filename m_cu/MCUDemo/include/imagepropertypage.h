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
	/** 设置图片。*/
	BOOL SetImage( LPCTSTR strImagePath, BOOL bRePaint = TRUE );

	/** 设置底色。 */
	BOOL SetColor( COLORREF bgColor );

private:
	/** 图片路径。 */
	tstring m_strImagePath;
	/** 背景颜色。 */
	COLORREF m_corBg;
	/** 背景颜色是否设置过。 */
	BOOL m_bCorSet;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
