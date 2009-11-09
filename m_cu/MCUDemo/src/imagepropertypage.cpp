#include "StdAfx.h"
#include "imagepropertypage.h"
#include "ImageShow.h"
#include "ImageFactory.h"


IMPLEMENT_DYNAMIC(CImagePropertyPage, CPropertyPage)

CImagePropertyPage::CImagePropertyPage(void)
{
	m_bCorSet = FALSE;
	m_corBg = RGB( 255,255,255 );
}

CImagePropertyPage::CImagePropertyPage(UINT nIDTemplate, UINT nIDCaption /* = 0 */, DWORD dwSize /* = sizeof */)
: CPropertyPage( nIDTemplate, nIDCaption, dwSize )
{
	m_bCorSet = FALSE;
	m_corBg = RGB( 255,255,255 );
}

CImagePropertyPage::~CImagePropertyPage(void)
{
}

BOOL CImagePropertyPage::SetImage( LPCTSTR strImagePath, BOOL bRePaint /* = TRUE */ )
{
	m_strImagePath = strImagePath;
	if ( bRePaint )
	{
		this->Invalidate();
	}
	return TRUE;
}

BOOL CImagePropertyPage::SetColor( COLORREF bgColor )
{
	m_corBg = bgColor;
	m_bCorSet = TRUE;
	return TRUE;
}BEGIN_MESSAGE_MAP(CImagePropertyPage, CPropertyPage)
ON_WM_PAINT()
END_MESSAGE_MAP()

void CImagePropertyPage::OnPaint()
{
	CImageShow *pImgShow = CImageFactory::Instance()->GetImage( m_strImagePath.c_str() );
	if ( pImgShow )
	{
		CPaintDC dc(this); // device context for painting
		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CDialog::OnPaint()
		CRect rcClient;
		this->GetClientRect( rcClient );
		pImgShow->Draw( dc, rcClient );
		CImageFactory::Instance()->ReleaseImage( m_strImagePath.c_str() );
	}
	else if( m_bCorSet )
	{
		CPaintDC dc(this); // device context for painting
		CRect rcClient;
		this->GetClientRect( rcClient );
		dc.FillSolidRect( rcClient, m_corBg );
	}
	else
	{
		CPropertyPage::OnPaint();
		return;
	}	
}
