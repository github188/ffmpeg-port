#include "StdAfx.h"
#include "checkbutton.h"
#include "mcucommon.h"
#include "imagefactory.h"
#include "ImageShow.h"

CCheckButton::CCheckButton(void)
{
	m_bBkTransparent = TRUE;
	m_uCheckState = BST_UNCHECKED;

	this->m_colorFront = RGB( 0,0,0 );
	this->m_colorBack = RGB( 255,255,255 );

	this->m_uTimerShowWindow = NULL;
	this->m_bNeedFechBkImage = m_bBkTransparent;
	
}

CCheckButton::~CCheckButton(void)
{
	this->m_bkBmp.DeleteObject();
	this->m_bkDC.DeleteDC();
}
BEGIN_MESSAGE_MAP(CCheckButton, CButton)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, &CCheckButton::OnBnClicked)
	ON_MESSAGE( BM_SETCHECK, &CCheckButton::OnSetCheck )
	ON_MESSAGE( BM_GETCHECK, &CCheckButton::OnGetCheck )
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CCheckButton::OnPaint()
{
	DrawItem( NULL );
	return;
	__super::OnPaint();
	return;

	if( m_bBkTransparent )
	{
		CPaintDC dc(this); // device context for painting

		CRect rcClient;
		this->GetClientRect( rcClient );

		// ������
		dc.FillSolidRect( &rcClient, RGB( 10, 100, 100 ) );
		// ���֡�

		
	}
	else
	{
		__super::OnPaint();
	}
	
}

LRESULT CCheckButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
//	mcu::tlog << _T( "message: " ) << (void*)message << _T( " w: " ) << wParam << _T( " l: " ) << lParam << endl;

	return CButton::WindowProc(message, wParam, lParam);
}

void CCheckButton::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���


	CButton::PreSubclassWindow();

	// ����ť��Ϊ�Ի淽ʽ��
	ModifyStyle(0, BS_OWNERDRAW);
}

void CCheckButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
	}

	BOOL bNeedRelease = FALSE;
	PAINTSTRUCT pst;
	if ( NULL == lpDrawItemStruct )
	{
		bNeedRelease = TRUE;
		lpDrawItemStruct = new DRAWITEMSTRUCT;
		memset( lpDrawItemStruct, 0, sizeof( DRAWITEMSTRUCT ) );
		
		lpDrawItemStruct->CtlID = this->GetDlgCtrlID();
		lpDrawItemStruct->hDC = this->GetDC()->GetSafeHdc();

		::BeginPaint( this->GetSafeHwnd(), &pst );

		lpDrawItemStruct->rcItem = pst.rcPaint;
		lpDrawItemStruct->itemState = this->GetState();
		// ...δ�������
	}
	

	CDC *pDc = CDC::FromHandle( lpDrawItemStruct->hDC );

	pDc->SetBkMode( TRANSPARENT );

	CRect rcClient;//( lpDrawItemStruct->rcItem );
	this->GetClientRect( rcClient );

	// ��������
	pDc->BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &m_bkDC, 0, 0, SRCCOPY );
	
	if ( this->GetButtonStyle() & BS_PUSHLIKE )
	{
		// �Ǹ���ť����ֻ֧����ͼ�İ�ť��
		LPCTSTR strPic = NULL;
		UINT uState = this->GetState();
		UINT uCheck = this->GetCheck();
		BOOL bDisable = !this->IsWindowEnabled();
		BOOL bFocuse = ( uState & BST_FOCUS );
		BOOL bChecked = ( uCheck & BST_CHECKED );
		
		if( !bDisable && !bFocuse && !bChecked )
		{
			strPic = m_strNormalImage.c_str();
		}
		else if( bDisable && !bFocuse && !bChecked )
		{
			strPic = m_strDisableImage.c_str();
		}
		else
		{
			mcu::tlog << _T( "not supported check button state��" ) << endl;
			_ASSERT( FALSE );
		}

		CImageShow *pShow = CImageFactory::Instance()->GetImage( strPic );
		if ( pShow )
		{
			pShow->Draw( pDc->GetSafeHdc(), rcClient );
		}
		else
		{
			mcu::tlog << _T( "Can't get image show!" ) << endl;
			_ASSERT( FALSE );
		}
		CImageFactory::Instance()->ReleaseImage( strPic );

	}
	else
	{

//		const int conLineWidth = 2;

		const int conMaxBoxLen = rcClient.Height();

		COLORREF colorFont = m_colorFront;
		COLORREF colorBack = m_colorBack;

		// д��
//		const int conTextOffset = 8;
		CRect rcText = rcClient;
		rcText.left += ( conMaxBoxLen  );

		CString strText;
		this->GetWindowText( strText );

		CFont *pFont = this->GetFont();

		pDc->SetTextColor( colorFont );
		pDc->SelectObject( pFont );
		int nDTResult = pDc->DrawText( strText, rcText, DT_LEFT | DT_VCENTER );

		// �����ֵĸ߶�ȷ������ĸ߶ȡ�
		int nTextHeight = nDTResult * 2 - rcText.Height();
		const int conBoxLen = int( nTextHeight * 0.9 );


		// ������

		CRect rcCheckBox = rcClient;
		rcCheckBox.DeflateRect( 1, 1, 1, 1 );
		rcCheckBox.top = ( ( rcClient.Height() - conBoxLen ) / 2 ) ;
		rcCheckBox.right = rcCheckBox.left + conBoxLen;
		rcCheckBox.bottom = rcCheckBox.top + conBoxLen;



		CBrush brushCheckBox( colorBack );
		pDc->FrameRect( rcCheckBox, &brushCheckBox );
		brushCheckBox.DeleteObject();

		// checkbox�ڲ���䱳��ɫ��
		CRect rcCheckBoxInner = rcCheckBox;
		rcCheckBoxInner.DeflateRect( 1, 1, 1, 1 );
		pDc->FillSolidRect( rcCheckBoxInner, colorFont );

		// ѡ��״̬��
		UINT uState = this->GetState();
		UINT uCheck = this->GetCheck();
		if ( uCheck == BST_CHECKED )
		{
			CRect rcChecked = rcCheckBoxInner;
			int nBlankWidth = int( conBoxLen * 0.1 );
			rcChecked.DeflateRect( nBlankWidth,nBlankWidth,nBlankWidth,nBlankWidth );
			pDc->FillSolidRect( rcChecked, colorBack );
		}


	}

	

	// 

	if ( bNeedRelease )
	{
		::EndPaint( this->GetSafeHwnd(), &pst );
		::ReleaseDC( this->GetSafeHwnd(), lpDrawItemStruct->hDC );

		delete lpDrawItemStruct;
		lpDrawItemStruct = NULL;
	}

	// TODO:  ������Ĵ����Ի���ָ����
}

void CCheckButton::OnBnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT uNewCheckState = ( m_uCheckState == BST_CHECKED ) ? BST_UNCHECKED : BST_CHECKED;
	this->SetCheck( uNewCheckState );
	// �ػ档
	this->Invalidate();
}

LRESULT CCheckButton::OnSetCheck( WPARAM fCheck, LPARAM )
{
	this->m_uCheckState = fCheck;
	return S_OK;
}

LRESULT CCheckButton::OnGetCheck( WPARAM, LPARAM )
{
	return m_uCheckState;
}

BOOL CCheckButton::SetColor( COLORREF colorFront, COLORREF colorBack, BOOL bRePaint /* = TRUE */ )
{
	m_colorFront = colorFront;
	m_colorBack = colorBack;
	if ( bRePaint && this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}

BOOL CCheckButton::SetBkTransparent( BOOL bTrans, BOOL bRepaint /* = TRUE */ )
{
	m_bBkTransparent = bTrans;
	if ( bRepaint && this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}

BOOL CCheckButton::SetImage( LPCTSTR strNormalImage, LPCTSTR strDisableImage, LPCTSTR strFocusedImage, LPCTSTR strNormalSelectedImage, LPCTSTR strDisableSelectedImage, LPCTSTR strFocusedSelectedImage, BOOL bRePaint /* = TRUE */ )
{
	this->m_strNormalImage = strNormalImage;
	this->m_strDisableImage = strDisableImage;
	this->m_strFocusedImage = strFocusedImage;
	this->m_strNormalCheckedImage = strNormalSelectedImage;
	this->m_strDisableCheckedImage = strDisableSelectedImage;
	this->m_strFocusedCheckedImage = strFocusedSelectedImage;

	if( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}
void CCheckButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( m_uTimerShowWindow == nIDEvent )
	{
		this->ShowWindow( SW_SHOW );

		this->KillTimer( m_uTimerShowWindow );
	}

	CButton::OnTimer(nIDEvent);
}
