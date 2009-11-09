 #include "StdAfx.h"
#include "uidialog.h"
#include "ImageFactory.h"
#include "ImageShow.h"
#include "mcucommon.h"

IMPLEMENT_DYNAMIC(CUIDialog, CDialog)

CUIDialog::CUIDialog(void)
{
	this->Init();
}

CUIDialog::~CUIDialog(void)
{
}

CUIDialog::CUIDialog(UINT nIDTemplate, CWnd *pParentWnd/* = 0*/)
:CDialog( nIDTemplate, pParentWnd )
{
	this->Init();
}

void CUIDialog::Init()
{
	m_bCorSet = FALSE;
	m_corBg = 0;
	m_dwFullScreenCmd = NULL;
	m_bAutoResume = TRUE;
	m_dwOriginalScreenMode = -1;
	m_bChangeScreenMode = FALSE;
}


BEGIN_MESSAGE_MAP(CUIDialog, CDialog)
	ON_WM_PAINT()
	ON_MESSAGE( WM_FULLSCREEN, &CUIDialog::OnFullScreenCmd )
	ON_MESSAGE( WM_SCREEN_DISPLAY_MODE, &CUIDialog::OnScreenOrientation )
	ON_MESSAGE( WM_ENUM_WINDOWS_NOTIFY, &CUIDialog::OnWindowsEnumNotify )
	ON_MESSAGE( WM_FS_SET_WINDOW_POS, &CUIDialog::OnSetWindowPosCmd )
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

void CUIDialog::OnPaint()
{
	CImageShow *pImgShow = CImageFactory::Instance()->GetImage( m_strImagePath.c_str() );
	if ( pImgShow )
	{
		CPaintDC dc(this); // device context for painting
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
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
		CDialog::OnPaint();
		return;
	}	
}

BOOL CUIDialog::SetImage( LPCTSTR strImagePath, BOOL bRePaint /* = TRUE */ )
{
	m_strImagePath = strImagePath;
	if ( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}

BOOL CUIDialog::SetColor( COLORREF bgColor, BOOL bRePaint /* = TRUE */ )
{
	m_corBg = bgColor;
	m_bCorSet = TRUE;
	if ( bRePaint && GetSafeHwnd() )
	{
		this->Invalidate();
	}
	return TRUE;
}


BOOL CUIDialog::FullScreen( DWORD dwFullScreenCmd )
{
	this->m_dwFullScreenCmd = dwFullScreenCmd;

	// �������ڱ任֮ǰ,���ȼ�¼��ԭʼ�Ĵ���λ��.
	if( this->HasCmd( FS_HideMenuBar ) || this->HasCmd( FS_HideTaskBar ) )
	{
		this->GetWindowRect( m_rcOldWndRect );
	}

	
	this->PostMessage( WM_SCREEN_DISPLAY_MODE );
	this->PostMessage( WM_FULLSCREEN );


	return TRUE;
}

BOOL CUIDialog::HasCmd( DWORD eCmd )
{
	return ( eCmd == ( this->m_dwFullScreenCmd & eCmd ) );
}

void CUIDialog::SetAutoResume( BOOL bAutoResume )
{
	m_bAutoResume = bAutoResume;
}

LRESULT CUIDialog::OnFullScreenCmd( WPARAM, LPARAM )
{
	DWORD dwFlag = 0;
	if ( this->HasCmd( FS_HideSipButton ) )
	{
		dwFlag |= SHFS_HIDESIPBUTTON;

		// �������뷨��ť��Ҫ��ʱ���á�
		this->CreateDelayedEvent( DelayEvent_HideSipButton, 10 );

	}
	else if( this->HasCmd( FS_ShowSipButton ) )
	{
		dwFlag |= SHFS_SHOWSIPBUTTON;
	}

	if ( this->HasCmd( FS_HideTaskBar ) )
	{
		dwFlag |= SHFS_HIDETASKBAR | SHFS_HIDESTARTICON;		
	}
	else if( this->HasCmd( FS_ShowTaskBar ) )
	{
		dwFlag |= SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON;
	}
	if ( dwFlag )
	{
		BOOL bResult = SHFullScreen( this->GetSafeHwnd(), dwFlag );   
		if ( !bResult )
		{
			mcu::tlog << _T( "SHFullScreen Fail!" ) << endl; 
		}
		//_ASSERT( bResult );
	}

	// ��������,�ɵ�����һ��.
	::EnumWindows( WindowsEnumCBForFulscreenS, (LPARAM)GetSafeHwnd() );
	
	this->PostMessage( WM_FS_SET_WINDOW_POS );


	return S_OK;
}

LRESULT CUIDialog::OnSetWindowPosCmd( WPARAM, LPARAM )
{
	// �������ڴ�С.
	CRect rcOldWnd, rcNewWnd;
	this->GetWindowRect( rcOldWnd );
	rcNewWnd = rcOldWnd;
	// ��Ļ��С.
	int nScreenWidth = ::GetScreenWidth();
	int nScreenHeight = ::GetScreenHeight();
	if( this->HasCmd( FS_FullScreen ) )
	{
		// ����Ϊ��Ļ��С.
		rcNewWnd.left = 0;
		rcNewWnd.right = nScreenWidth;
		rcNewWnd.top = 0;
		rcNewWnd.bottom = nScreenHeight;
	}
	else if( this->HasCmd( FS_HideMenuBar ) )
	{
		rcNewWnd.bottom = nScreenHeight;
	}
	else if( this->HasCmd( FS_HideTaskBar ) )
	{
		rcNewWnd.top = 0;
	}
	if ( rcOldWnd != rcNewWnd )
	{
		::SetWindowPos( GetSafeHwnd(), NULL, rcNewWnd.left, rcNewWnd.top, rcNewWnd.Width(), rcNewWnd.Height(), SWP_NOZORDER );
	}

	if( this->HasCmd( FS_ShowTaskBar ) )
	{
		// ����Ϊԭʼ��С.
		::SetWindowPos( GetSafeHwnd(), 0, m_rcOldWndRect.left, m_rcOldWndRect.top, m_rcOldWndRect.Width(), m_rcOldWndRect.Height(), SWP_NOZORDER );
	}
	return S_OK;
}

LRESULT CUIDialog::OnScreenOrientation( WPARAM, LPARAM )
{
	DEVMODE settings;
	LONG lResult;
	memset( &settings, 0, sizeof( settings ) );
	settings.dmSize = sizeof( settings );

	// ask current mode
	settings.dmFields = DM_DISPLAYORIENTATION;
	lResult = ChangeDisplaySettingsEx(NULL, &settings, NULL, CDS_TEST, NULL);
	if ( DISP_CHANGE_SUCCESSFUL != lResult )
	{
		mcu::tlog << _T( "Change Display setting fail 1!" ) << endl;
	}
	_ASSERT( DISP_CHANGE_SUCCESSFUL == lResult );	

	BOOL bNeedChange = FALSE;
	if ( this->HasCmd( FS_HorizontalScreen ) )
	{
		this->m_dwOriginalScreenMode = settings.dmDisplayOrientation;
		// ˳ʱ��ת90��.
		switch( m_dwOriginalScreenMode )
		{
		case DMDO_0:
			settings.dmDisplayOrientation = DMDO_90;
			break;
		case DMDO_90:
			settings.dmDisplayOrientation = DMDO_180;
			break;
		case DMDO_180:
			settings.dmDisplayOrientation = DMDO_270;
			break;
		case DMDO_270:
			settings.dmDisplayOrientation = DMDO_0;
			break;
		default:
			_ASSERT( FALSE );
			break;
		}
		// ��Ҫ����ʱ�Զ��ָ�.
		this->m_bChangeScreenMode = TRUE;

		// ���ñ�־,���淭ת.
		bNeedChange = TRUE;		
	}
	else if( this->HasCmd( FS_ResumeScreen ) )
	{
		if ( -1 != this->m_dwOriginalScreenMode && this->m_dwOriginalScreenMode != settings.dmDisplayOrientation )
		{
			// �ָ�.
			settings.dmDisplayOrientation = m_dwOriginalScreenMode;
			// ���ñ�־,���淭ת.
			bNeedChange = TRUE;
		}
		// �Ѿ��ָ���,����Ҫ�ٻָ���.
		this->m_bChangeScreenMode = FALSE;		
	}

	if ( bNeedChange )
	{
		DEVMODE testSetting = settings;
		lResult = ChangeDisplaySettingsEx(NULL,&testSetting,NULL, CDS_TEST,NULL);
		if ( DISP_CHANGE_SUCCESSFUL != lResult )
		{
			mcu::tlog << _T( "Change Display setting fail 2!" ) << endl;
		}
		if ( DISP_CHANGE_SUCCESSFUL == lResult )
		{
			lResult = ChangeDisplaySettingsEx( NULL, &settings, NULL, CDS_RESET, NULL );
			if ( DISP_CHANGE_SUCCESSFUL != lResult )
			{
				mcu::tlog << _T( "Change Display setting fail 3!" ) << endl;
			}
			_ASSERT( DISP_CHANGE_SUCCESSFUL == lResult );
		}
		else
		{
			mcu::tlog << _T( "The Machine do not surpport Trun around the screen!" ) << endl;
		}
		_ASSERT( DISP_CHANGE_SUCCESSFUL == lResult );
	}
	return S_OK;
}

BOOL CUIDialog::WindowsEnumCBForFulscreenS( HWND hWnd, LPARAM param )
{
	HWND hNotifyWnd = HWND( param );
	BOOL bResult = ::SendMessage( hNotifyWnd, WM_ENUM_WINDOWS_NOTIFY, (WPARAM)hWnd, NULL );
	return bResult;
}

LRESULT CUIDialog::OnWindowsEnumNotify( WPARAM wparam, LPARAM )
{
	HWND hWnd = HWND( wparam );
	// �ж��Ǳ����ڵ��Ӵ���.
	if ( GetSafeHwnd() == ::GetParent( hWnd ) )
	{
		// �ж��ǲ���menu_worker
		CString strClassName;
		::GetClassName( hWnd, strClassName.GetBuffer( 100 ), 100 );
		strClassName.ReleaseBuffer();
		if ( strClassName == _T("menu_worker") )
		{
			// ��ӵ�menu��¼��.
			this->m_tMenuWorkerWndList.push_back( hWnd );

			if ( this->HasCmd( FS_ShowMenuBar ) )
			{
				// ��ʾ�������ס�˵�,����������ʱ����ʾ.
	//			::ShowWindow( hWnd, SW_SHOW );
				
			}
			else if( this->HasCmd( FS_HideMenuBar ) )
			{
				::ShowWindow( hWnd, SW_HIDE );
			}
		}
	}
	return TRUE;
}
LRESULT CUIDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

//	mcu::tlog << _T( "uidialog message: 10: " ) << message << _T( " 16: " ) << (void*)message << endl;
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CUIDialog::PreTranslateMessage(MSG* pMsg)
{
	// handle tooltip messages (some messages cancel, some may cause it to popup)

	switch( pMsg->message )
	{
	case WM_KEYDOWN:
//		mcu::tlog << _T( "key down, wp " ) << pMsg->wParam << _T( " lp " ) << pMsg->lParam << endl;
		break;
	case WM_KEYUP:
//		mcu::tlog << _T( "key up, wp " ) << pMsg->wParam << _T( " lp " ) << pMsg->lParam << endl;
		if ( pMsg->wParam == VK_F1 )
		{
			this->OnClickLeftFunKey();
			return TRUE;
		}
		else if( pMsg->wParam == VK_F2 )
		{
			this->OnClickRightFunKey();
			return TRUE;
		}

		break;
	
	default:
	    break;
	}

	// no default processing
	return __super::PreTranslateMessage( pMsg );
}

void CUIDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	// ���ע���˴��¼���������ʱ�¼���
	if ( m_setEventId.find( nIDEvent ) != m_setEventId.end() )
	{
		this->OnDelayedEvent( nIDEvent );
		this->KillTimer( nIDEvent );
		this->m_setEventId.erase( nIDEvent );
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CUIDialog::CreateDelayedEvent( int nEventId, int nDelayTime )
{
	this->SetTimer( nEventId, nDelayTime, NULL );
	this->m_setEventId.insert( nEventId );
	return TRUE;
}

BOOL CUIDialog::CancelDelayedEvent( int nEventId )
{
	TEventIdSet::iterator iter = this->m_setEventId.find( nEventId );
	if ( iter != m_setEventId.end() )
	{
		this->KillTimer( nEventId );
		this->m_setEventId.erase( iter );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CUIDialog::OnDelayedEvent( int nEventId )
{
	if ( nEventId == DelayEvent_HideSipButton )
	{
		DWORD dwFlag = SHFS_HIDESIPBUTTON;
		BOOL bResult = SHFullScreen( this->GetSafeHwnd(), dwFlag );   
		if ( !bResult )
		{
			mcu::tlog << _T( "SHFullScreen Fail!" ) << endl;
		}
//		_ASSERT( bResult );
	}
}
void CUIDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CUIDialog::OnClickRightFunKey()
{
	
}

void CUIDialog::OnClickLeftFunKey()
{

}

//�����������

BOOL CUIDialog::LowerSip()
{
	BOOL fRes = FALSE;
	SIPINFO si;
	memset( &si, 0, sizeof( si ) );
	si.cbSize = sizeof( si );

	if( SHSipInfo( SPI_GETSIPINFO, 0, &si, 0 ) )
	{
		si.fdwFlags &= ~SIPF_ON;
		fRes = SHSipInfo( SPI_SETSIPINFO, 0, &si, 0 );
	}
	return fRes;
}

//�����������

BOOL CUIDialog::RaiseSip( void )
{
	BOOL fRes ;
	SIPINFO si;
	memset( &si, 0, sizeof( si ) );
	si.cbSize = sizeof( si );

	if( SHSipInfo( SPI_GETSIPINFO, 0, &si, 0 ) )
	{
		si.fdwFlags |= SIPF_ON;
		fRes = SHSipInfo( SPI_SETSIPINFO, 0, &si, 0 );
	}
	
	
	return fRes;

}

BOOL CUIDialog::IsSipRaise()
{
	SIPINFO tSipInfo;
	memset( &tSipInfo, 0, sizeof( tSipInfo ) );
	tSipInfo.cbSize = sizeof( tSipInfo );
	BOOL bGetSipResult = SipGetInfo( &tSipInfo );
	if ( !bGetSipResult )
	{
		mcu::tlog << _T( "BOOL bGetSipResult = SipGetInfo( &tSipInfo ); fail!!!" ) << endl;
	}

	return ( SIPF_ON & tSipInfo.fdwFlags );
}


void CUIDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	if (nState == 1 && IsWindow(m_hWnd))
	{
		this->FullScreen( FS_HideMenuBar | FS_HideSipButton );
	}
}
