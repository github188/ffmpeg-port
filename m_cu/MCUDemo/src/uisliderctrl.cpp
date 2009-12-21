#include "StdAfx.h"
#include "uisliderctrl.h"
#include "mcucommon.h"
#include "ImageFactory.h"

CUISliderCtrl::CUISliderCtrl(void)
{
	m_bBkTransparent = TRUE;

	m_bNeedFechBkImage = m_bBkTransparent;
	m_uTimerShowWindow = NULL;
}

CUISliderCtrl::~CUISliderCtrl(void)
{
	this->m_bkBmp.DeleteObject();
	this->m_bkDC.DeleteDC();

	this->m_penMain.DeleteObject();
	this->m_penFrame.DeleteObject();

}
BEGIN_MESSAGE_MAP(CUISliderCtrl, CSliderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
//	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CUISliderCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

void CUISliderCtrl::OnPaint()
{
//	return CSliderCtrl::OnPaint();

	if ( m_bNeedFechBkImage )
	{
		this->ShowWindow( SW_HIDE );
		m_bNeedFechBkImage = FALSE;
		m_uTimerShowWindow = this->SetTimer( 1, 1, NULL );
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

		CRect rcClient;
		this->GetClientRect( rcClient );

		CPaintDC dc(this); // device context for painting

		// 画背景。
		if ( m_bkDC.GetSafeHdc() )
		{
			CRect rcClient;
			this->GetClientRect( rcClient );
			::BitBlt( dc, 0, 0, rcClient.Width(), rcClient.Height(), m_bkDC, 0, 0, SRCCOPY );
		}
		else
		{
			Log() << _T( "背景DC没有初始化！！！" ) << endl;
		}

		{
			CRect rcChannel;
			this->GetChannelRect( rcChannel );

			CBrush brushCh( RGB( 231,164,33 ) );
			::FillRect( dc, &rcChannel, brushCh );	

			CRect rcThumb;
			this->GetThumbRect( rcThumb );

			CBrush brushThumb( RGB( 191,219,253 ) );
			::FillRect( dc, &rcThumb, brushThumb );				
		}

		return;

	}



}

BOOL CUISliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CSliderCtrl::OnEraseBkgnd(pDC);
}

void CUISliderCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_uTimerShowWindow == nIDEvent )
	{
		this->ShowWindow( SW_SHOW );

		this->KillTimer( m_uTimerShowWindow );
	}

	CSliderCtrl::OnTimer(nIDEvent);
}

//void CUISliderCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//
//	DWORD dwResult = CDRF_SKIPDEFAULT;
//
//	if ( pNMCD )
//	{
//		//Log() << _T( "custom draw: stage: " ) << (void*)pNMCD->dwDrawStage << _T( " spec: " ) << (void*)pNMCD->dwItemSpec << 
//		//	_T( " state: " ) << (void*)pNMCD->uItemState << _T( " param: " ) << pNMCD->lItemlParam << endl;
//		switch( pNMCD->dwDrawStage )
//		{
//		case CDDS_PREPAINT:
//			// 画背景。
//			if ( m_bkDC.GetSafeHdc() )
//			{
//				CRect rcClient;
//				this->GetClientRect( rcClient );
//				::BitBlt( pNMCD->hdc, 0, 0, rcClient.Width(), rcClient.Height(), m_bkDC, 0, 0, SRCCOPY );
//			}
//			else
//			{
//				Log() << _T( "背景DC没有初始化！！！" ) << endl;
//			}
//
//			{
//				CRect rcChannel;
//				this->GetChannelRect( rcChannel );
//
//				CBrush brushCh( RGB( 231,164,33 ) );
//				::FillRect( pNMCD->hdc, &rcChannel, brushCh );	
//
//				CRect rcThumb;
//				this->GetThumbRect( rcThumb );
//
//				CBrush brushThumb( RGB( 191,219,253 ) );
//				::FillRect( pNMCD->hdc, &rcThumb, brushThumb );				
//			}
//			dwResult = CDRF_NOTIFYITEMDRAW;
//			dwResult = CDRF_NOTIFYSUBITEMDRAW  ;
//			dwResult = CDRF_SKIPDEFAULT;
//			
//			break;
//
//		case CDDS_ITEMPREPAINT:
//			{
//				switch( pNMCD->dwItemSpec )
//				{
//				case TBCD_CHANNEL:
///*					Log() << _T( "Channel: l: " ) << pNMCD->rc.left << _T( " t: " ) << pNMCD->rc.top << 
//						_T( " r" ) << pNMCD->rc.right << _T( " b: " ) << pNMCD->rc.bottom << endl;		*/			
//					{
//						//LPCTSTR PIC_SLIDER_CHANNEL = _T( "slider_chanel.jpg" );
//
//						//CImageShow *pImgShow = CImageFactory::Instance()->GetImage( PIC_SLIDER_CHANNEL );
//						//if ( pImgShow )
//						//{
//						//}
//						//else
//						{
//							// 绘制条。	
//							CRect rcChannel = pNMCD->rc;
//							CBrush brushCh( RGB( 231,164,33 ) );
//							::FillRect( pNMCD->hdc, &rcChannel, brushCh );	
//						}									
//
//					}
//					dwResult = CDRF_SKIPDEFAULT;
//					break;
//				case TBCD_TICS:		
//					//Log() << _T( "tics: l: " ) << pNMCD->rc.left << _T( " t: " ) << pNMCD->rc.top << 
//					//	_T( " r" ) << pNMCD->rc.right << _T( " b: " ) << pNMCD->rc.bottom << endl;
//					// 不画刻度。
//					dwResult = CDRF_SKIPDEFAULT ;
//					break;
//				case TBCD_THUMB:
//					//Log() << _T( "Thumb: l: " ) << pNMCD->rc.left << _T( " t: " ) << pNMCD->rc.top << 
//					//	_T( " r" ) << pNMCD->rc.right << _T( " b: " ) << pNMCD->rc.bottom << endl;
//
//					{
//						CRect rcChannel = pNMCD->rc;
//						CBrush brushCh( RGB( 191,219,253 ) );
//						
//						
//						::FillRect( pNMCD->hdc, &rcChannel, brushCh );
//
//					}
//					
//					dwResult = CDRF_SKIPDEFAULT ;
//					break;
//				default:
//				    break;
//				}
//			}
//		
//			break;
//		default:
//		    break;
//		}
//	}
//
//	*pResult = dwResult;
//}
