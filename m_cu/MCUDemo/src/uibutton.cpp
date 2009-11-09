// ImageButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "uibutton.h"

#include "ImageShow.h"
#include "ImageFactory.h"
// CUIButton

IMPLEMENT_DYNAMIC(CUIButton, CButton)

CUIButton::CUIButton()
{
	m_bImageTransparent = FALSE;
	m_bBkTransparent = TRUE;

	m_bNeedFechBkImage = m_bBkTransparent;
	m_colorMask = RGB( 255,255,255 );
	m_uTimerShowWindow = NULL;
}

CUIButton::~CUIButton()
{
	this->m_bkBmp.DeleteObject();
	this->m_bkDC.DeleteDC();
}


BEGIN_MESSAGE_MAP(CUIButton, CButton)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CUIButton::OnNMCustomdraw)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CUIButton ��Ϣ�������


void CUIButton::OnPaint()
{
	
	// �����Ҫץȡ�����ڵ�DC��Ϊ��������ô��Ҫ�����������ڲ��ܵõ������ڵı�����
	if ( m_bNeedFechBkImage )
	{
		this->ShowWindow( SW_HIDE );
		m_bNeedFechBkImage = FALSE;
		m_uTimerShowWindow = this->SetTimer( 1, 1, NULL );
		return;
	}
	else
	{
		// ȡ������
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

		return __super::OnPaint();

		//tstring strShowPicPath = m_strNormalPicPath;
		//UINT uSt = this->GetState();

		//// ��ť�Ƿ�ӵ�н��㡣
		//if ( uSt & BST_FOCUS )
		//{
		//	strShowPicPath = m_strPicPathFocus;
		//}

		//// �Ƿ���á�
		//if ( !this->IsWindowEnabled() )
		//{
		//	strShowPicPath = m_strPicPathDisable;
		//}

		//CImageShow *pImg = CImageFactory::Instance()->GetImage( strShowPicPath.c_str() );
		//if( pImg && pImg->IsOk() )
		//{
		//	CRect rcClient;
		//	this->GetClientRect( rcClient );

		//	CPaintDC dc(this); // device context for painting

		//	// ��������
		//	dc.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &m_bkDC, 0, 0, SRCCOPY );

		//	
		//	pImg->SetTransparentMask( m_bImageTransparent, m_colorMask );
		//	pImg->Draw( dc, &rcClient );
		//}
		//else
		//{
		//	CRect rcClient;
		//	this->GetClientRect( rcClient );

		//	CPaintDC dc(this); // device context for painting

		//	// ����.
		//	dc.FillSolidRect( rcClient, RGB( 255,255,255 ) );
		//	CBrush brFrame( RGB( 0,0,0 ) );
		//	dc.FrameRect( rcClient, &brFrame );
		//	

		//	// ��������.
		//	dc.SetBkMode( TRANSPARENT );

		//	// �������塣
		//	CFont *pFont = this->GetFont();
		//	CFont *pOldFont = dc.SelectObject( pFont );

		//	BOOL bEnable = this->IsWindowEnabled();
		//	dc.SetTextColor( bEnable ? RGB( 0,0,0 ) : RGB( 128,128,128 ) );

		//	CString strText;
		//	this->GetWindowText( strText );	

		//	DWORD dwStyle = this->GetStyle();
		//	DWORD dwDrawTextStyle = DT_CENTER | DT_VCENTER;			

		//	// д�֡� 
		//	int nHeight = dc.DrawText( strText, rcClient, dwDrawTextStyle );

		//	if ( pOldFont )
		//	{
		//		dc.SelectObject( pOldFont );
		//	}
		//	
		//}
	}
	

}

void CUIButton::SetImage( LPCTSTR strNormalImg, LPCTSTR strFocusImg, 
							LPCTSTR strDisableImg, BOOL bRePaint /* = TRUE */ )
{
	this->m_strNormalPicPath = strNormalImg;
	this->m_strPicPathFocus = strFocusImg;
	this->m_strPicPathDisable = strDisableImg;
	if ( bRePaint && this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
}

BOOL CUIButton::FitToPicture()
{
	CImageInfo info;
	CImageShow * pImgShow = CImageFactory::Instance()->GetImage( m_strNormalPicPath.c_str() );
	if ( pImgShow && pImgShow->GetImageInfo( info ) )
	{
		CRect rcWnd;
		rcWnd.left = rcWnd.top = 0;
		rcWnd.right = info.Width;
		rcWnd.bottom = info.Height;
		this->SetWindowPos( NULL, 0, 0, info.Width, info.Height, SWP_NOZORDER | SWP_NOMOVE );
		CImageFactory::Instance()->ReleaseImage( m_strNormalPicPath.c_str() );
		return TRUE;
	}
	else
	{
		CImageFactory::Instance()->ReleaseImage( m_strNormalPicPath.c_str() );
		return FALSE;
	}
}

BOOL CUIButton::SetTransparentMask( BOOL bTransparent, COLORREF maskColor /* = NULL */, BOOL bRePaint /* = TRUE */ )
{
	this->m_bImageTransparent = bTransparent;
	this->m_colorMask = maskColor;
	if ( bRePaint && this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}


LRESULT CUIButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	// ����Ҫ�ֶ���ˢ�½��棬����Ļ������ػ治��Ч��������������
	if ( WM_ENABLE == message || WM_SETFOCUS == message )
	{
		this->Invalidate();
	}

	LRESULT lR = CButton::WindowProc(message, wParam, lParam);

	return lR;
}

void CUIButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( m_uTimerShowWindow == nIDEvent )
	{
		this->ShowWindow( SW_SHOW );

		this->KillTimer( m_uTimerShowWindow );
	}

	CButton::OnTimer(nIDEvent);
}

BOOL CUIButton::SetBkTransparent( BOOL bTransparent, BOOL bRePaint /* = TRUE */ )
{
	m_bBkTransparent = bTransparent;
	m_bNeedFechBkImage = bTransparent;
	if ( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}
void CUIButton::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	DWORD dwResult = CDRF_SKIPDEFAULT;

	if ( pNMCD )
	{
		mcu::tlog << _T( "custom draw: stage: " ) << (void*)pNMCD->dwDrawStage << _T( " spec: " ) << (void*)pNMCD->dwItemSpec << 
			_T( " state: " ) << (void*)pNMCD->uItemState << _T( " param: " ) << pNMCD->lItemlParam << endl;
		switch( pNMCD->dwDrawStage )
		{
		case CDDS_PREPAINT:
			// ��������
			if ( m_bkDC.GetSafeHdc() )
			{
				CRect rcClient;
				this->GetClientRect( rcClient );
				::BitBlt( pNMCD->hdc, 0, 0, rcClient.Width(), rcClient.Height(), m_bkDC, 0, 0, SRCCOPY );
			}
			else
			{
				mcu::tlog << _T( "����DCû�г�ʼ��������" ) << endl;
			}

			dwResult = CDRF_NOTIFYITEMDRAW;
			break;

		case CDDS_ITEMPREPAINT:
			{
				switch( pNMCD->dwItemSpec )
				{
				case TBCD_CHANNEL:
					/*					mcu::tlog << _T( "Channel: l: " ) << pNMCD->rc.left << _T( " t: " ) << pNMCD->rc.top << 
					_T( " r" ) << pNMCD->rc.right << _T( " b: " ) << pNMCD->rc.bottom << endl;		*/			
					{
						//LPCTSTR PIC_SLIDER_CHANNEL = _T( "slider_chanel.jpg" );

						//CImageShow *pImgShow = CImageFactory::Instance()->GetImage( PIC_SLIDER_CHANNEL );
						//if ( pImgShow )
						//{
						//}
						//else
						{
							// ��������	
							CRect rcChannel = pNMCD->rc;
							CBrush brushCh( RGB( 231,164,33 ) );
							::FillRect( pNMCD->hdc, &rcChannel, brushCh );	
						}									

					}
					dwResult = CDRF_SKIPDEFAULT;
					break;
				case TBCD_TICS:		
					//mcu::tlog << _T( "tics: l: " ) << pNMCD->rc.left << _T( " t: " ) << pNMCD->rc.top << 
					//	_T( " r" ) << pNMCD->rc.right << _T( " b: " ) << pNMCD->rc.bottom << endl;
					// �����̶ȡ�
					dwResult = CDRF_SKIPDEFAULT ;
					break;
				case TBCD_THUMB:
					//mcu::tlog << _T( "Thumb: l: " ) << pNMCD->rc.left << _T( " t: " ) << pNMCD->rc.top << 
					//	_T( " r" ) << pNMCD->rc.right << _T( " b: " ) << pNMCD->rc.bottom << endl;

					{
						CRect rcChannel = pNMCD->rc;
						CBrush brushCh( RGB( 191,219,253 ) );


						::FillRect( pNMCD->hdc, &rcChannel, brushCh );

					}

					dwResult = CDRF_SKIPDEFAULT ;
					break;
				default:
					break;
				}
			}

			break;
		default:
			break;
		}
	}

	*pResult = dwResult;
}

void CUIButton::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	CButton::PreSubclassWindow();

	// ����ť��Ϊ�Ի淽ʽ��
	ModifyStyle(0, BS_OWNERDRAW);
}

void CUIButton::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	// TODO:  ������Ĵ����Ի���ָ����

	if ( m_bNeedFechBkImage )
	{
		this->ShowWindow( SW_HIDE );
		m_bNeedFechBkImage = FALSE;
		m_uTimerShowWindow = this->SetTimer( 1, 1, NULL );
		return;
	}
	else
	{
		// ȡ������
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

		//		return CButton::OnPaint();

		tstring strShowPicPath = this->GetCurImage();
		

		CImageShow *pImg = CImageFactory::Instance()->GetImage( strShowPicPath.c_str() );
		if( pImg && pImg->IsOk() )
		{
			CRect rcClient;
			this->GetClientRect( rcClient );

			CPaintDC dc(this); // device context for painting

			// ��������
			dc.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &m_bkDC, 0, 0, SRCCOPY );


			pImg->SetTransparentMask( m_bImageTransparent, m_colorMask );
			pImg->Draw( dc, &rcClient );
		}
		else
		{
			CRect rcClient;
			this->GetClientRect( rcClient );

			CPaintDC dc(this); // device context for painting

			// ����.
			dc.FillSolidRect( rcClient, RGB( 255,255,255 ) );
			CBrush brFrame( RGB( 0,0,0 ) );
			dc.FrameRect( rcClient, &brFrame );


			// ��������.
			dc.SetBkMode( TRANSPARENT );

			// �������塣
			CFont *pFont = this->GetFont();
			CFont *pOldFont = dc.SelectObject( pFont );

			BOOL bEnable = this->IsWindowEnabled();
			dc.SetTextColor( bEnable ? RGB( 0,0,0 ) : RGB( 128,128,128 ) );

			CString strText;
			this->GetWindowText( strText );	

			DWORD dwStyle = this->GetStyle();
			DWORD dwDrawTextStyle = DT_CENTER | DT_VCENTER;			

			// д�֡� 
			int nHeight = dc.DrawText( strText, rcClient, dwDrawTextStyle );

			if ( pOldFont )
			{
				dc.SelectObject( pOldFont );
			}

		}
		CImageFactory::Instance()->ReleaseImage( strShowPicPath.c_str() );
	}

}

BOOL CUIButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

BOOL CUIButton::GetImageSize( LPRECT prcImg )
{	
	if ( !prcImg )
	{
		return FALSE;
	}

	tstring strImagePath = this->GetCurImage();
	CImageInfo info;
	CImageShow * pImgShow = CImageFactory::Instance()->GetImage( strImagePath.c_str() );
	if ( pImgShow && pImgShow->GetImageInfo( info ) )
	{
		CRect rcWnd;
		rcWnd.left = rcWnd.top = 0;
		rcWnd.right = info.Width;
		rcWnd.bottom = info.Height;

		*prcImg = rcWnd;
		CImageFactory::Instance()->ReleaseImage( strImagePath.c_str() );
		return TRUE;
	}
	else
	{
		CImageFactory::Instance()->ReleaseImage( strImagePath.c_str() );
		return FALSE;
	}
}

tstring CUIButton::GetCurImage() const
{
	tstring strShowPicPath = this->m_strNormalPicPath;
	UINT uSt = this->GetState();

	// ��ť�Ƿ�ӵ�н��㡣
	if ( uSt & BST_FOCUS )
	{
		strShowPicPath = m_strPicPathFocus;
	}

	// �Ƿ���á�
	if ( !this->IsWindowEnabled() )
	{
		strShowPicPath = m_strPicPathDisable;
	}

	return strShowPicPath;
}