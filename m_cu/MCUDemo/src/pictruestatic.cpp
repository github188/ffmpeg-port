// src\pictruestatic.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "pictruestatic.h"
#include "ImageFactory.h"
#include "ImageShow.h"

// CPictureStatic

IMPLEMENT_DYNAMIC(CPictureStatic, CStatic)

CPictureStatic::CPictureStatic()
{
	m_colorMask = RGB( 255,255,255 );
	m_bImageTrans = FALSE;
	m_bBkTransparent = TRUE;

	m_bNeedFechBkImage = m_bBkTransparent;
	m_uTimerShowWindow = NULL;

	m_bSelected = FALSE;
	m_colorSelectedFrame = RGB( 239, 186, 0 );

//	this->m_ePicDisplayMode = PDM_Orientation_Default;

}

CPictureStatic::~CPictureStatic()
{
	this->m_bkBmp.DeleteObject();
	this->m_bkDC.DeleteDC();
}


BEGIN_MESSAGE_MAP(CPictureStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CPictureStatic 消息处理程序



void CPictureStatic::OnPaint()
{
	CImageShow * pImgShow = CImageFactory::Instance()->GetImage( m_strPicPath.c_str() );
	if ( pImgShow )
	{
		CRect rcClient;
		this->GetClientRect( rcClient );

		// 抓取背景。
		if ( m_bBkTransparent )
		{
			if ( m_bNeedFechBkImage )
			{
				this->ShowWindow( SW_HIDE );
				m_bNeedFechBkImage = FALSE;
				m_uTimerShowWindow = this->SetTimer( 1, 1, NULL );
				CImageFactory::Instance()->ReleaseImage( m_strPicPath.c_str() );
				return;
			}
			else
			{
				

				// 取背景。
				if ( m_bkDC.m_hDC == NULL )
				{
					CWnd *pParent = this->GetParent();
					CClientDC cParentDc( pParent );

					CRect rcWnd;
					this->GetWindowRect( rcWnd );
					pParent->ScreenToClient( rcWnd );


					m_bkDC.CreateCompatibleDC( &cParentDc );
					m_bkBmp.CreateCompatibleBitmap( &cParentDc, rcWnd.Width(), rcWnd.Height() );

					m_bkDC.SelectObject( &m_bkBmp );

					m_bkDC.BitBlt( 0, 0, rcWnd.Width(), rcWnd.Height(), &cParentDc, rcWnd.left, rcWnd.top, SRCCOPY );
				}				
			}
		}
		
		// 绘制。
		CPaintDC dc(this); // device context for painting

		if( m_bBkTransparent )
		{
			// 画背景。
			dc.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &m_bkDC, 0, 0, SRCCOPY );
		}

		// 绘制图片.
		if ( pImgShow && pImgShow->IsOk() )
		{
			pImgShow->SetTransparentMask( this->m_bImageTrans, m_colorMask );

			pImgShow->Draw( dc, rcClient/*, eIdf */);
		}		

		// 画选中方框。
		if ( m_bSelected )
		{
			CBrush brushSel( m_colorSelectedFrame );

			CRect rcSelectedFrame = rcClient;
			dc.FrameRect( rcSelectedFrame, &brushSel );

			rcSelectedFrame.DeflateRect( 1,1,1,1 );			
			dc.FrameRect( rcSelectedFrame, &brushSel );
		}
		
	}
	else
	{
		CStatic::OnPaint();

	}

	CImageFactory::Instance()->ReleaseImage( m_strPicPath.c_str() );	
}

void CPictureStatic::SetPicPath( LPCTSTR strPicPath, BOOL bRePaint /* = TRUE */ )
{
	m_strPicPath = strPicPath;
	if ( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
}

tstring CPictureStatic::GetPicPath() const
{
	return m_strPicPath;
}

BOOL CPictureStatic::FitToPicture()
{
	CRect rcImg;
	if ( GetImageSize( rcImg ) )
	{
		this->SetWindowPos( NULL, 0, 0, rcImg.Width(), rcImg.Height(), SWP_NOZORDER | SWP_NOMOVE );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//CImageInfo info;
	//CImageShow * pImgShow = CImageFactory::Instance()->GetImage( m_strPicPath.c_str() );
	//if ( pImgShow && pImgShow->GetImageInfo( info ) )
	//{
	//	CRect rcWnd;
	//	rcWnd.left = rcWnd.top = 0;
	//	rcWnd.right = info.Width;
	//	rcWnd.bottom = info.Height;
	//	this->SetWindowPos( NULL, 0, 0, info.Width, info.Height, SWP_NOZORDER | SWP_NOMOVE );
	//	return TRUE;
	//}
	//else
	//{
	//	return FALSE;
	//}
}

BOOL CPictureStatic::GetImageSize( LPRECT prcImg )
{
	if ( !prcImg )
	{
		return FALSE;
	}
	CImageInfo info;
	CImageShow * pImgShow = CImageFactory::Instance()->GetImage( m_strPicPath.c_str() );
	if ( pImgShow && pImgShow->GetImageInfo( info ) )
	{
		CRect rcWnd;
		rcWnd.left = rcWnd.top = 0;
		rcWnd.right = info.Width;
		rcWnd.bottom = info.Height;

		*prcImg = rcWnd;
		CImageFactory::Instance()->ReleaseImage( m_strPicPath.c_str() );
		return TRUE;
	}
	else
	{
		CImageFactory::Instance()->ReleaseImage( m_strPicPath.c_str() );
		return FALSE;
	}
	
}

BOOL CPictureStatic::SetTransparentMask( BOOL bTransparent, COLORREF maskColor /* = NULL */, BOOL bRePaint /* = TRUE */ )
{
	this->m_bImageTrans = bTransparent;
	m_colorMask = maskColor;
	if ( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}

BOOL CPictureStatic::SetBkTransparent( BOOL bTrans, BOOL bRePaint /* = TRUE */ )
{
	this->m_bBkTransparent = bTrans;
	if ( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}

void CPictureStatic::SetSelected( BOOL bSelected, BOOL bRePaint /* = TRUE */ )
{
	m_bSelected = bSelected;
	if ( bRePaint && this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
}

void CPictureStatic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_uTimerShowWindow == nIDEvent )
	{
		this->ShowWindow( SW_SHOW );
		this->KillTimer( m_uTimerShowWindow );
	}

	CStatic::OnTimer(nIDEvent);
}

BOOL CPictureStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	// 如果窗口创建时父窗口可见，那么不需要隐藏自身抓取背景。
	if ( pParentWnd->GetSafeHwnd() && pParentWnd->IsWindowVisible() )
	{
		this->m_bNeedFechBkImage = FALSE;
	}
	

	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

void CPictureStatic::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CStatic::PreSubclassWindow();
}

BOOL CPictureStatic::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	// 如果窗口创建时父窗口可见，那么不需要隐藏自身抓取背景。
	if ( pParentWnd->GetSafeHwnd() && pParentWnd->IsWindowVisible() )
	{
		this->m_bNeedFechBkImage = FALSE;
	}

	return CStatic::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}

//void CPictureStatic::SetPicDisplayMode( EPicDisplayMode eMode, BOOL bRePaint /* = TRUE */ )
//{
//	m_ePicDisplayMode = eMode;
//
//	if ( GetSafeHwnd() && bRePaint )
//	{
//		this->Invalidate();
//	}
//}
//
//CPictureStatic::EPicDisplayMode CPictureStatic::GetPicDisplayMode() const
//{
//	return m_ePicDisplayMode;
//}