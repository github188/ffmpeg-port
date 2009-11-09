#include "StdAfx.h"
#include "staticex.h"

CStaticEx::CStaticEx(void)
:m_bBkTransparent( TRUE ),
m_normalTextColor( RGB( 0, 0, 0) ),
m_disableTextColor( RGB(128,128,128) ),
m_bkColor( RGB( 255, 255, 255 ) ),
m_bNeedFechBkImage( TRUE ),
m_uTimerShowWindow( 0 )
{
}

CStaticEx::~CStaticEx(void)
{
	this->m_bkBmp.DeleteObject();
	this->m_bkDC.DeleteDC();

}

BOOL CStaticEx::SetBkTransparent( BOOL bTrans /* = TRUE */, BOOL bRePaint  /*= TRUE*/ )
{
	m_bBkTransparent = bTrans;
	if ( bRePaint )
	{
		this->m_bNeedFechBkImage = m_bBkTransparent;

		this->m_bkDC.DeleteDC();
		this->m_bkBmp.DeleteObject();

		this->Invalidate( );
	}
	return TRUE;
}

BOOL CStaticEx::SetTextColor( COLORREF corNormal, COLORREF corDisable, BOOL bRepaint /* = TRUE */ )
{
	m_normalTextColor = corNormal;
	m_disableTextColor = corDisable;
	if ( bRepaint )
	{
		this->Invalidate();
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_WM_SHOWWINDOW()
ON_WM_SIZE()
ON_WM_TIMER()
ON_WM_DESTROY()
END_MESSAGE_MAP()

void CStaticEx::OnPaint()
{
	if( m_bBkTransparent )
	{
		if ( m_bNeedFechBkImage )
		{
			this->ShowWindow( SW_HIDE );
			m_bNeedFechBkImage = FALSE;
			m_uTimerShowWindow = this->SetTimer( 1, 1, NULL );
			return;
		}
		else
		{
			CRect rcClient;
			this->GetClientRect( rcClient );

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

			// 创建本窗口DC。
			CPaintDC dc(this); // device context for painting
			
			// 画背景。
			dc.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &m_bkDC, 0, 0, SRCCOPY );

			dc.SetBkMode( TRANSPARENT );

			// 设置字体。
			CFont *pFont = this->GetFont();
			CFont *pOldFont = dc.SelectObject( pFont );

			BOOL bEnable = this->IsWindowEnabled();
			dc.SetTextColor( bEnable ? m_normalTextColor : m_disableTextColor );

			CString strText;
			this->GetWindowText( strText );	

			DWORD dwStyle = this->GetStyle();
			DWORD dwDrawTextStyle = DT_LEFT | DT_VCENTER;
			if ( dwStyle & SS_RIGHT )
			{
				dwDrawTextStyle |= DT_RIGHT;
			}
			if( dwStyle & SS_CENTER )
			{
				dwDrawTextStyle |= DT_CENTER;
			}

			// 写字。 
			int nHeight = dc.DrawText( strText, rcClient, dwDrawTextStyle );

			if ( pOldFont )
			{
				dc.SelectObject( pOldFont );
			}
			return;
		}

		

	}
	else
	{
//		CStatic::OnPaint();
		CPaintDC dc( this );

		BOOL bEnable = this->IsWindowEnabled();
		dc.SetTextColor( bEnable ? m_normalTextColor : m_disableTextColor );
		dc.SetBkColor( m_bkColor );

		CString strText;
		this->GetWindowText( strText );

		CRect rcClient;
		this->GetClientRect( rcClient );

		// 填充背景色。
		dc.FillSolidRect( rcClient, m_bkColor );

		// 设置字体。
		CFont *pFont = this->GetFont();
		CFont *pOldFont = dc.SelectObject( pFont );

		DWORD dwStyle = this->GetStyle();
		UINT uDTStyle = DT_LEFT;
		if ( dwStyle & SS_CENTER )
		{
			uDTStyle |= DT_CENTER;
		}

		// 写字。 
		int nHeight = dc.DrawText( strText, rcClient, uDTStyle | DT_VCENTER );

		if ( pOldFont )
		{
			dc.SelectObject( pOldFont );
		}
		return;
	}
	
}

BOOL CStaticEx::SetBkColor( COLORREF corBk )
{
	m_bkColor = corBk;
	return TRUE;
}

BOOL CStaticEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}

void CStaticEx::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CStatic::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}

void CStaticEx::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CStaticEx::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CStatic::PreSubclassWindow();

}

BOOL CStaticEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CStatic::PreCreateWindow(cs);
}

LRESULT CStaticEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CStatic::WindowProc(message, wParam, lParam);
}

BOOL CStaticEx::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 如果窗口创建时父窗口可见，那么不需要隐藏自身抓取背景。
	if ( pParentWnd->GetSafeHwnd() && pParentWnd->IsWindowVisible() )
	{
		this->m_bNeedFechBkImage = FALSE;
	}

	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

void CStaticEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_uTimerShowWindow == nIDEvent )
	{
		this->ShowWindow( SW_SHOW );

		this->KillTimer( m_uTimerShowWindow );
	}

	CStatic::OnTimer(nIDEvent);
}

void CStaticEx::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	

	CStatic::PostNcDestroy();
}

void CStaticEx::OnDestroy()
{


	CStatic::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

BOOL CStaticEx::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 如果窗口创建时父窗口可见，那么不需要隐藏自身抓取背景。
	if ( pParentWnd->GetSafeHwnd() && pParentWnd->IsWindowVisible() )
	{
		this->m_bNeedFechBkImage = FALSE;
	}

	return CStatic::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}

BOOL CStaticEx::UpdateBk()
{
	if ( m_bBkTransparent )
	{
		m_bNeedFechBkImage = TRUE;
	}
	this->Invalidate();
	return TRUE;
}

void CStaticEx::GetTextColor( COLORREF& corNormal, COLORREF& corDisable ) const
{
	corNormal = this->m_normalTextColor;
	corDisable = this->m_disableTextColor;
}

void CStaticEx::GetBkColor( COLORREF& corBk ) const
{
	corBk = this->m_bkColor;
}