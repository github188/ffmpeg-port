// MCUDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "MCUDemoDlg.h"
#include "UDPTestDialog.h"
#include "PlayerDialog.h"

//#include <afxhtml.h>
//#include <exdisp.idl>
#include "VideoWndDialog.h"

#ifdef USE_FFMPEG
#include "ffplay.h"
#endif

#include "Config.h"
#include "PPCLoginDialog.h"
#include "MCUSession.h"

#include "UrlParse.h"
#include "mcucommon.h"

#include "picmanagedialog.h"
#include "configdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMCUDemoDlg �Ի���
#define EVENT_CLOSE_PLAY 100
#define MAX_VALUE_NAME 1024

CMCUDemoDlg::CMCUDemoDlg(CWnd* pParent /*=NULL*/)
	: CUIDialog(CMCUDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

//	m_hSoftKeyBar = NULL;
 
	m_pPlayerDlg = NULL;
}

void CMCUDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_EXPLORER1, m_ctrlExplorer);
	DDX_Control(pDX, IDC_COMBO_URL, m_cmbUrl);
	DDX_Control(pDX, IDC_BUTTON_PIC, m_btnPic);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_STATIC_BOTTOM_BG, m_stBottomBg);
	DDX_Control(pDX, IDC_BUTTON_SIP_BUTTON, m_btnSip);
}

BEGIN_MESSAGE_MAP(CMCUDemoDlg, CUIDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMCUDemoDlg::OnBnClickedButtonTest)
	ON_COMMAND(ID_MENU_UDP_TEST, &CMCUDemoDlg::OnMenuUdpTest)
	ON_COMMAND(ID_MENU_VIDEO_PLAY, &CMCUDemoDlg::OnMenuVideoPlay)
	ON_COMMAND(ID_MENU_CONFIG, &CMCUDemoDlg::OnMenuConfig)
	ON_COMMAND(ID_MENU_LOGIN, &CMCUDemoDlg::OnMenuLogin)
	ON_COMMAND(ID_MENU_BACK, &CMCUDemoDlg::OnMenuBack)
	ON_COMMAND(ID_MENU_VIDEO_WND_TEST, &CMCUDemoDlg::OnMenuVideoWndTest)
	ON_COMMAND(ID_MENU_PTZ, &CMCUDemoDlg::OnMenuPtz)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE( WM_HTML_PRE_LOAD, &CMCUDemoDlg::OnHtmlPreLoad )
	ON_COMMAND(ID_MENU_FORWARD, &CMCUDemoDlg::OnMenuForward)
	ON_COMMAND(ID_MENU_PLAY_FILE, &CMCUDemoDlg::OnMenuPlayFile)
	ON_MESSAGE( WM_HTML_META, &CMCUDemoDlg::OnHtmlMeta )
	ON_COMMAND(ID_MENU_REFRESH, &CMCUDemoDlg::OnMenuRefresh)
//	ON_COMMAND(ID_MENU_PIC, &CMCUDemoDlg::OnMenuPic)
	ON_BN_CLICKED(IDC_BUTTON_PIC, &CMCUDemoDlg::OnBnClickedButtonPic)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CMCUDemoDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_SIP_BUTTON, &CMCUDemoDlg::OnBnClickedButtonSipButton)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()


// CMCUDemoDlg ��Ϣ�������

BOOL CMCUDemoDlg::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	tstring strTitle;
	CConfig::Instance()->GetAppTitle( strTitle );
	this->SetWindowText( strTitle.c_str() );

#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ��� CommandBar\n");
		return FALSE;      // δ�ܴ���
	}
#endif // WIN32_PLATFORM_WFSP
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	this->m_cmbUrl.AddString( TEXT( "http://218.242.128.205/vau/index2.html" ) );
	this->m_cmbUrl.AddString( TEXT( "http://218.242.128.205/vau/index.html" ) );
	this->m_cmbUrl.AddString( TEXT( "http://172.16.160.125/vau/index2.html" ) );
	this->m_cmbUrl.AddString( TEXT( "http://www.google.com" ) );

	this->m_cmbUrl.AddString( TEXT( "file://\\Storage Card\\�û���¼.htm" ) );

	this->m_cmbUrl.AddString( TEXT( "http://58.223.2.10/jsmsp/" ) );
	this->m_cmbUrl.AddString( TEXT( "http://218.242.128.205:8081/Login.aspx" ) );
	this->m_cmbUrl.AddString( TEXT( "file://\\Storage Card\\wml_example.htm" ) );


	// ����ҳ���������.
	m_htmlWnd.Create( CHtmlWnd::IDD, this );
	this->UpdateLayout();

	// logo
	m_logoDlg.Create( CLogoDialog::IDD, this );
	m_logoDlg.ShowWindow( SW_SHOW );

	// �ײ��˵�������ť��
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnPic.SetBkTransparent( FALSE, FALSE );
	this->m_btnPic.SetImage( _T( "btn_pic_normal.jpg" ), _T( "btn_pic_focus.jpg" ), _T( "btn_pic_disable.jpg.jpg" ), FALSE );

	this->m_btnConfig.SetBkTransparent( FALSE, FALSE );
	this->m_btnConfig.SetImage( _T( "btn_config_normal.jpg" ), _T( "btn_config_focus.jpg" ), _T( "btn_config_normal.jpg" ), FALSE );

	// ���뷨���ư�ť��
	this->m_btnSip.SetBkTransparent( FALSE, FALSE );
	this->m_btnSip.SetImage( _T( "btn_sip_normal.jpg" ), _T( "btn_sip_focus.jpg" ), _T( "btn_sip_disable.jpg" ), FALSE );

	this->Login();

	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );


//	SHMENUBARINFO shmbi;
//
//	// Create our softkey bar
//	// ���������󴴽����ܾ���ʾ��������.
//	ZeroMemory(&shmbi, sizeof(shmbi));
//	shmbi.cbSize = sizeof(shmbi);
//	shmbi.hwndParent = GetSafeHwnd();
//	shmbi.dwFlags = SHCMBF_HMENU;
//
//	shmbi.nToolBarId = IDR_MENU_MAIN;
//
//
////#ifdef _DEBUG
////	shmbi.nToolBarId = IDR_MENU_LEFT;
////#endif
//	shmbi.hInstRes = AfxGetInstanceHandle();
//	if (!SHCreateMenuBar(&shmbi))
//	{
//		// Failed!!
//		MessageBox( TEXT( "�����˵�ʧ�ܣ���" ) );
//		return FALSE;
//	}
//	m_hSoftKeyBar = shmbi.hwndMB;
	
	//m_htmlWnd.ShowWindow( SW_SHOW );
	

	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CMCUDemoDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_MCUDEMO_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_MCUDEMO_DIALOG));
	}

	this->UpdateLayout();
}
#endif


void CMCUDemoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CUIDialog::OnLButtonUp(nFlags, point);

	//CString strMsg;
	//strMsg.Format( _T( "���꣺ x:%d.y:%d.flag:%d." ), point.x, point.y, nFlags );
	//this->MessageBox( strMsg, _T( "���" ), MB_OK );
}


void CMCUDemoDlg::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strPage;
	this->m_cmbUrl.GetWindowText( strPage );
//	this->OpenUrl( strPage );



}

BOOL CMCUDemoDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	NM_HTMLVIEW* pnmHTMLView = (NM_HTMLVIEW*)lParam;

//	WCHAR wszURL[1000];
//
//	switch (pnmHTMLView->hdr.code)
//	{
//		// this code is processed when a user clicks on a hyperlink
//	case NM_HOTSPOT:
//		{
//
//			// read the target string
//			mbstowcs(wszURL, (const char *)pnmHTMLView->szTarget, 1000);
//			OutputDebugString( wszURL );
//			MessageBox( wszURL, TEXT( "������URL���ӣ�" ) );
//			// rtsp://172.16.160.32/service?PUID-ChannelNo=11000000000000000011100000300000-1&PlayMethod=0&StreamingType=1
//			
//			CString strUrl = wszURL;
//			//if ( strUrl.Left( 7 ) == _T( "rtsp://" ) )
//			//{
//			//	
//
//
//			//	CVideoWndDialog *pdlg = new CVideoWndDialog( strUrl );
//			//	pdlg->Create( CVideoWndDialog::IDD, this );
//			//	pdlg->ShowWindow( SW_SHOW );
//
//			//	beginrtsp( "", (int)pdlg->GetSafeHwnd() );
//
//			//}
//			//VideoRTSP.aspx?ID=1&UserId=270000400022001000&PUID-ChannelNo=27000000000000000011200001900000-1
//			// &PlayMethod=0&Offset=0&PuProperty=1&VAUADD=218.242.128.205:5060&PUName=������&StreamingType=1&
//			// VauPtzAdd=218.242.128.205&VauPtzPort=5060&VauRtspAdd=218.242.128.205&VauRtspPort=554
//			
//			// ��ʾ֮ǰ��ҳ�档
////			http://172.16.160.125/service.asp?PUID-ChannelNo=27000000000000000011200003300000-1&PlayMethod=0
//
//			// if the link is just "exit", then exit
//			//if(wcscmp(wszURL, L"exit") == 0)
//			//{
//			//	SendMessage(hWnd, WM_CLOSE, NULL, NULL);
//			//	// Returning TRUE indicates that we have handled the Msg
//			//	// and the control will not do its default processing.
//			//	return TRUE;
//			//}
//			break;
//
//		}
//	case NM_BEFORENAVIGATE:
//		{
//			mbstowcs(wszURL, (const char *)pnmHTMLView->szTarget, 1000);
//			OutputDebugString( wszURL );		
//
//			CString strUrl = wszURL;
//			// �����¼��
//			this->m_listUrlHistory.push( strUrl );
//
//			if ( strUrl.Left( 7 ) == _T( "rtsp://" ) )
//			{
//				MessageBox( wszURL, TEXT( "Ҫ�򿪵�rtsp rul��" ) );
//
//				// ��ʾ֮ǰ��ҳ�档
//
////				return TRUE;
//				//CVideoWndDialog *pdlg = new CVideoWndDialog( this );
//				//pdlg->Create( CVideoWndDialog::IDD, this );
//				//pdlg->ShowWindow( SW_SHOW );
//
//				//beginrtsp( "", (int)pdlg->GetSafeHwnd() );
//
//				return true;
//			}
//		}
//		break;
//	}			

	return CUIDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CMCUDemoDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CString strUrl;
	m_cmbUrl.GetWindowText( strUrl );

	// TODO: �ڴ����ר�ô����/����û���
	switch( wParam )
	{
	//case ID_MENU_BACK:
	//	MessageBox( TEXT( "����" ) );

	//	break;
	//case ID_MENU_FORWARD:
	//	MessageBox( TEXT( "ǰ��" ) );
	//	break;
	case ID_MENU_REFRESH:
//		MessageBox( TEXT( "ˢ��" ) );
//		::SendMessage( m_hHtmlCtrlWnd, DTM_NAVIGATE, NAVIGATEFLAG_REFRESH, (LPARAM)(LPCTSTR)strUrl );
		break;
	default:
	    break;
	}

	return CUIDialog::OnCommand(wParam, lParam);
}

//bool CMCUDemoDlg::CreateHtmlCtrl()
//{
//	BOOL bHtmlInit = ( this->m_hHtmlCtrlWnd && ::IsWindow( m_hHtmlCtrlWnd ) );
//	if( !bHtmlInit )
//	{
//		HINSTANCE hInst = AfxGetInstanceHandle();
//		BOOL bResult = ::InitHTMLControl( hInst );
//		if( bResult )
//		{
//			// create the window for the control
//			m_hHtmlCtrlWnd = CreateWindow(TEXT("DISPLAYCLASS"), NULL, WS_VISIBLE, 0, 0, 
//				GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) - 40, 
//				GetSafeHwnd(), NULL, hInst, NULL);
//			::ShowWindow(m_hHtmlCtrlWnd, SW_SHOW);
//			::UpdateWindow( m_hHtmlCtrlWnd );
//		}
//		else
//		{
//			MessageBox( TEXT( "��ʼ��Html�ؼ�ʧ�ܣ�" ) );
//		}
//	}
//
//	return m_hHtmlCtrlWnd != NULL;
//}
//void CMCUDemoDlg::OpenUrl( CString strUrl, BOOL bBackHistroy /* = FALSE */ )
//{
//	if( strUrl.IsEmpty() )
//	{
//		MessageBox( TEXT( "Ҫ�򿪵�URLΪ�գ�" ) );
//	}
//	else
//	{
//		if ( !bBackHistroy )
//		{
//			// ��¼�����ʷ��
//			this->m_listUrlHistory.push( strUrl );	
//		}			
//
//		// load the start page into the browser window
//		LRESULT lr = ::SendMessage( m_hHtmlCtrlWnd, DTM_NAVIGATE, 0, (LPARAM)(LPCTSTR)strUrl );
//	}
//
//}
void CMCUDemoDlg::OnMenuUdpTest()
{
	// TODO: �ڴ���������������
	
}

void CMCUDemoDlg::OnMenuVideoPlay()
{
	// TODO: �ڴ���������������
	tstring strRtsp = _T( "rtsp://172.16.160.125/PUID-ChannelNo=27000000000000000011200003300000-1&PlayMethod=0&StreamingType=2" );
	
//	strRtsp = _T( "rtsp://172.16.131.100/service?PUID-ChannelNo=12000000000000000011200001300000-1&PlayMethod=0&StreamingType=1" );
	
	CPlayerDialog dlg;
	dlg.SetRtspUrl( strRtsp.c_str() );

	CMCUSession::Instance()->CurVideoSession()->PuId( _T( "27000000000000000011200003300000-1" ) );
//	CMCUSession::Instance()->CurVideoSession()->PuId( _T( "12000000000000000011200001300000-1" ) );
	CMCUSession::Instance()->CurVideoSession()->PtzIP( _T( "172.16.160.125" ) );
//	CMCUSession::Instance()->CurVideoSession()->PtzAddr( _T( "172.16.131.100" ) );
	CMCUSession::Instance()->CurVideoSession()->PtzPort( 5060 );
	CMCUSession::Instance()->CurVideoSession()->StreamType( STREAM_1X );
	
	dlg.DoModal();
}

void CMCUDemoDlg::OnMenuConfig()
{
	// TODO: �ڴ���������������
	CConfigDialog dlg;
	dlg.DoModal();
	/*CSysCfgPage syscfg;

	CConfigPropertySheet sheet( _T( "����" ), this );
	sheet.AddPage( &syscfg );
	sheet.DoModal();*/

}

void CMCUDemoDlg::OnMenuLogin()
{
	// TODO: �ڴ���������������
	this->Login();
}

void FilterCharactor(char* pOut, const char c)
{
	static char achFilter[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@$^*()-=_[]{}\\|/?;':\",.<>";
	const char* const p = strchr(achFilter, c);
	if ( '#' == c )
	{
		strncat( pOut, "%23", 3);
	}
	else if ( '%' == c )
	{
		strncat( pOut, "%25", 3);
	}
	else if ( '&' == c)
	{
		strncat( pOut, "%26", 3);
	}
	else if ( '+' == c)
	{
		strncat( pOut, "%2b", 3);
	}
	else if ( ' ' == c )
	{
		strncat( pOut, "%20", 3);
	}
	else if ( '\n' == c )
	{
		strncat( pOut, "\0a", 3);
	}
	else if ( NULL != p )
	{
		strncat( pOut, &c, 1);
	}
	{
	}
}

bool UTF_8ToAsciiDisplay( char *pOut, int nOutLen, const char*pIn, int nInLen )
{
	if ( (nOutLen < (nInLen*3)) || (nInLen == 0) )
	{
		return false;
	}

	int nIndex = 0;
	char achTemp[10] = { 0 };
	for ( nIndex = 0; nIndex < nInLen; ++nIndex)
	{
		if ( pIn[nIndex] < 0 )
		{// �ַ�Ϊ����
			strncat( pOut, "%", 1);
			sprintf( achTemp,"%.2x", pIn[nIndex]);
			strncat( pOut, &achTemp[strlen(achTemp) - 2], 2);
		}
		else
		{// �ַ�ΪӢ��
			FilterCharactor(pOut, pIn[nIndex]);
		}
	}
	return true;
}

void CMCUDemoDlg::Login()
{
	tstringstream strServerFullUrl;	

	BOOL bHasInterface;
	CConfig::Instance()->GetLoginInterface( bHasInterface );
	if ( bHasInterface )
	{
		CPPCLoginDialog loginDlg;
		int nloginType = loginDlg.DoModal();
		if ( CPPCLoginDialog::LOGIN_CANCEL == nloginType )
		{
			// �˳�����
			EndDialog(0);
			return;
		}

		this->m_htmlWnd.ClearHistoryURL();

		TCHAR ServerFullUrl[1024];
		memset(ServerFullUrl, 0, sizeof(ServerFullUrl));
		if( CConfig::Instance()->GetServerFullUrl((LPTSTR)ServerFullUrl) )
		{
			mcu::log << L"ServerFullUrl:"<<ServerFullUrl << endl;
		}
		else
		{
			mcu::log << L"��������ַ���ǺϷ���HTTP��ַ:"<< endl;
			MessageBox(L"��������ַ���ǺϷ���HTTP��ַ!", L"����", MB_OK);
			return;
		}

		HRESULT hr = IsValidURL( NULL, (LPCTSTR)ServerFullUrl, 0 );
		if( S_OK != hr )
		{
			MessageBox(L"��������ַ���ò��Ϸ�!", L"����", MB_OK);
			return; 
		}

		strServerFullUrl << ServerFullUrl;

		tstring strRegionId;
		CConfig::Instance()->GetRegionId( strRegionId );
		tstringstream ssAccount;
		ssAccount << (LPCTSTR)loginDlg.m_strUserId << _T( "@" ) << strRegionId;

		string strResult = UTF16toUTF8( ssAccount.str() );
		char* strAccount = (char *) LocalAlloc(LMEM_FIXED, 10240);
		memset(strAccount, 0, 10240);
		UTF_8ToAsciiDisplay( strAccount, 10240, strResult.c_str(), strlen(strResult.c_str()));

		strServerFullUrl << _T( "?" );
		//strServerFullUrl << _T( "Account=" ) << ssAccount.str() << _T( "&" );
		//strServerFullUrl << _T( "Account=" ) << L"admin@%E4%BB%96.wx.js@110000"<<_T( "&" );
		strServerFullUrl << _T( "Account=" ) << strAccount << _T( "&" );
		strServerFullUrl << _T( "Password=" ) << (LPCTSTR)loginDlg.m_strPassword << _T( "&" );
		strServerFullUrl << _T( "StreamingType=" ) << int( loginDlg.m_b3GStream ? STREAM_EVDO : STREAM_1X )  << _T( "&" );
		strServerFullUrl << _T( "LoginType=" ) << (int)loginDlg.m_eLoginType;
		LocalFree (strAccount);

		// ����.
		CMCUSession::Instance()->UserId( ssAccount.str() );
		CMCUSession::Instance()->Password( (LPCTSTR)loginDlg.m_strPassword );
		
	}
	else
	{
		tstring strServer;
		CConfig::Instance()->GetServer( strServer );
		strServerFullUrl << strServer;
	}

	this->m_htmlWnd.OpenUrl( strServerFullUrl.str().c_str() );

}
void CMCUDemoDlg::OnMenuBack()
{
	// TODO: �ڴ���������������

	this->m_htmlWnd.HistoryBack();
}

void CMCUDemoDlg::OnMenuVideoWndTest()
{
	// TODO: �ڴ���������������
	CVideoWndDialog dlg( this );
	dlg.DoModal();
}

void CMCUDemoDlg::OnMenuPtz()
{
	// TODO: �ڴ���������������
}

void CMCUDemoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CUIDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
}

void CMCUDemoDlg::UpdateLayout( LPRECT lprcClient /* = NULL */ )
{
	if ( !GetSafeHwnd() )
	{
		return;
	}

	CRect rcClient;
	if ( lprcClient )
	{
		rcClient = *lprcClient;
	}
	else
	{
		this->GetClientRect( rcClient );
	}	

	// ʣ��Ŀռ䡣
	CRect rcLeftSpace = rcClient;

	// ��Ļ��С��	
	int nScreenLong = ::GetScreenLong();

	// logo.
	if ( m_logoDlg.GetSafeHwnd() )
	{
		int nLogoHeight = m_logoDlg.GetHeight();
		CRect rcLogoDlg = rcClient;;
		rcLogoDlg.bottom = nLogoHeight;
		this->m_logoDlg.MoveWindow( rcLogoDlg );

		// ��ʣ��ռ���ȥ��logoռ�õĿռ䡣
		rcLeftSpace.top += nLogoHeight;
	}

	// �±߲˵�����
	// �����˵�����λ�á�
	if ( m_stBottomBg.GetSafeHwnd() && m_btnPic.GetSafeHwnd() && m_btnConfig.GetSafeHwnd() )
	{
		// �ײ���ť���������ǰ�Ĳ˵����ĸ߶�	
		const float conFMenubarHeight = 0.082;
		int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

		CRect rcBottomBg = rcClient;
		rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
		//m_stBottomBg.MoveWindow( rcBottomBg );
		this->m_stBottomBg.SetWindowPos( &CWnd::wndTop, rcBottomBg.left, rcBottomBg.top, rcBottomBg.Width(), rcBottomBg.Height(), NULL );

		CRect rcBtnDelete;
		BOOL bResult = this->m_btnPic.GetImageSize( rcBtnDelete );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnDelete.Width() / rcBtnDelete.Height();
			rcBtnDelete = rcBottomBg;
			rcBtnDelete.right = rcBtnDelete.left + nNewWidth;
			this->m_btnPic.SetWindowPos( &CWnd::wndTop, rcBtnDelete.left, rcBtnDelete.top, rcBtnDelete.Width(), rcBtnDelete.Height(), NULL );
		}

		CRect rcBtnReturn;
		bResult = this->m_btnConfig.GetImageSize( rcBtnReturn );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnReturn.Width() / rcBtnReturn.Height();
			rcBtnReturn = rcBottomBg;
			rcBtnReturn.left = rcBtnReturn.right - nNewWidth;
			this->m_btnConfig.SetWindowPos( &CWnd::wndTop, rcBtnReturn.left, rcBtnReturn.top, rcBtnReturn.Width(), rcBtnReturn.Height(), NULL );
		}

		CRect rcBtnSip;
		bResult = this->m_btnSip.GetImageSize( rcBtnSip );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnSip.Width() / rcBtnSip.Height();
			rcBtnSip = rcBottomBg;
			rcBtnSip.left = rcBottomBg.left + ( rcBottomBg.Width() - nNewWidth ) / 2;
			rcBtnSip.right = rcBtnSip.left + nNewWidth;
			this->m_btnSip.SetWindowPos( &CWnd::wndTop, rcBtnSip.left, rcBtnSip.top, rcBtnSip.Width(), rcBtnSip.Height(), NULL );
		}

		// ��ʣ��ռ���ȥ���˵���ռ�õĿռ䡣
		rcLeftSpace.bottom -= nMenubarHeight;
	}


	if ( m_htmlWnd.GetSafeHwnd() )
	{
		CRect rcHtml = rcLeftSpace;
//		rcHtml.top += nLogoHeight;

		m_htmlWnd.MoveWindow( rcHtml );
	}	
}
void CMCUDemoDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if ( m_pPlayerDlg )
	{
		delete m_pPlayerDlg;
		m_pPlayerDlg = NULL;
	}

	CUIDialog::PostNcDestroy();
}

LRESULT CMCUDemoDlg::OnHtmlMeta( WPARAM wParam, LPARAM lParam )
{
	CString strHttpEquiv = ( LPCTSTR )wParam;
	CString strMetaData = ( LPCTSTR )lParam;

	if ( strHttpEquiv.CompareNoCase( _T( "refresh" ) ) == 0  )
	{
		int nIndex = strMetaData.Find( _T( "url=rtsp://" ) );
		if ( nIndex != -1 )
		{
//			this->m_htmlWnd.Stop();
//			this->m_htmlWnd.HistoryBack( 1 );

			nIndex = strMetaData.Find( _T( "url=" ) );
			CString strRtspUrl = strMetaData.Right( strMetaData.GetLength() - nIndex - _tcslen( _T( "url=" ) ) );
			
			if ( strRtspUrl.Left( 7 ) != _T( "rtsp://" ) )
			{
				mcu::log << _T( "rtsp url error!!!" ) << endl;
			}
			
			
			return TRUE;
			
		}
	}

	return TRUE;
}

LRESULT CMCUDemoDlg::OnHtmlPreLoad( WPARAM strUrlParam, LPARAM )
{
	CString strUrl = ( LPCTSTR )strUrlParam;

	CUrlParse urlParser;
	urlParser.Parse( strUrl );
	tstring strPtzAddr, strPtzPort, strStreamType, strPuId, strPuName;
	int bPtzControl;
	urlParser.ParamValue( _T( "VauPtzAdd" ), strPtzAddr );
	urlParser.ParamValue( _T( "VauPtzPort" ), strPtzPort );
	urlParser.ParamValue( _T( "StreamingType" ), strStreamType );
	urlParser.ParamValue( _T( "PuId-ChannelNo" ), strPuId );
	urlParser.ParamValue( _T( "PuProperty" ), bPtzControl );
	urlParser.ParamValue( _T( "PuName" ), strPuName );


	if ( !strPtzAddr.empty() && !strPtzPort.empty() && !strStreamType.empty() && !strPuId.empty() )
	{
		mcu::log << _T( "Is url with Param! " ) << strUrl << endl;
		CMCUSession::Instance()->CurVideoSession()->PtzIP( strPtzAddr );
		CMCUSession::Instance()->CurVideoSession()->PtzPort( _ttoi( strPtzPort.c_str() ) );
		CMCUSession::Instance()->CurVideoSession()->PuId( strPuId );
		CMCUSession::Instance()->CurVideoSession()->StreamType( (EStreamType)_ttoi( strStreamType.c_str() ) );
		CMCUSession::Instance()->CurVideoSession()->PuName( strPuName );

		CMCUSession::Instance()->CurVideoSession()->PtzControl( bPtzControl );
	}	
	else
	{
		mcu::log << _T( "Not right url! " ) << (LPCTSTR)strUrl << endl;
	}


	LPCTSTR lpRtspUrlToken = _T( "rtsp://" );
	if ( strUrl.Left( _tcslen( lpRtspUrlToken ) ).CompareNoCase( lpRtspUrlToken ) == 0 )
	{
		//_ASSERT( FALSE );
//		strUrl = _T( "rtsp://172.16.160.125/PUID-ChannelNo=27000000000000000011200003300000-1&PlayMethod=0&StreamingType=2" );
		if( this->m_htmlWnd.m_bWaittingNavigateComplete == TRUE)
		{
			this->m_htmlWnd.m_bWaittingNavigateComplete = FALSE;
		}
		this->m_htmlWnd.Stop();
		this->m_htmlWnd.HistoryBack( 2 );

		CString strRtspUrl = strUrl;
		if ( m_pPlayerDlg )
		{
			mcu::log << _T( "m_pPlayerDlg was not null!!!" ) << endl;
			_ASSERT( FALSE );
			return FALSE;
		}

		// �û�ѡ���Ƿ�����,�������,�����´򿪴˶Ի���.
		int nEndCode = CPlayerDialog::E_DlgEndRetry;
		while ( CPlayerDialog::E_DlgEndRetry == nEndCode )
		{
			CPlayerDialog playerDlg;
			m_pPlayerDlg = &playerDlg;
			playerDlg.SetRtspUrl( strRtspUrl );
			nEndCode = playerDlg.DoModal();
			m_pPlayerDlg = NULL;
		}
		

		CRect rchtml;
		m_htmlWnd.GetWindowRect( rchtml );
		BOOL bResult = m_htmlWnd.IsWindowVisible();

		m_htmlWnd.ShowWindow( SW_HIDE );
		m_htmlWnd.ShowWindow( SW_SHOW );

		CreateDelayedEvent(EVENT_CLOSE_PLAY, 1);
		//RestoreRtspPlayerRegKey();


			//if ( m_pPlayerDlg == NULL )
			//{
			//	m_pPlayerDlg = new CPlayerDialog( this );
			//}
			//else
			//{
			//	_ASSERT( FALSE );	
			//	return TRUE;

			//}

			//m_pPlayerDlg->SetRtspUrl( strUrl );
			//m_pPlayerDlg->DoModal();
			//delete m_pPlayerDlg;
			//m_pPlayerDlg = NULL;


		return TRUE;
	}

	return FALSE;
}

void CMCUDemoDlg::OnDelayedEvent( int nEventId )
{
	switch (nEventId)
	{
	case EVENT_CLOSE_PLAY:
		RestoreRtspPlayerRegKey();
		break;
	default:
		break;
	}
}


void CMCUDemoDlg::RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
{
	HKEY hkey;

	if (RegOpenKeyEx(root, szSubKey, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS) 
	{
		DWORD dwType = 0;
		DWORD dwSize = 0;
		DWORD		cbValueName = MAX_VALUE_NAME;
		DWORD		retCode;

		TCHAR		ClassName[MAX_PATH];
		DWORD		dwcClassLen = MAX_PATH;
		DWORD		dwcSubKeys;
		DWORD		dwcMaxSubKey;
		DWORD		dwcMaxClass;
		DWORD		dwcValues;
		DWORD		dwcMaxValueName;
		DWORD		dwcMaxValueData;
		DWORD		dwcSecDesc;
		FILETIME	ftLastWriteTime;

		retCode =
			RegQueryInfoKey (hkey,              // Key handle.
			ClassName,         // Buffer for class name.
			&dwcClassLen,      // Length of class string.
			NULL,              // Reserved.
			&dwcSubKeys,       // Number of sub keys.
			&dwcMaxSubKey,     // Longest sub key size.
			&dwcMaxClass,      // Longest class string.
			&dwcValues,        // Number of values for this key.
			&dwcMaxValueName,  // Longest Value name.
			&dwcMaxValueData,  // Longest Value data.
			&dwcSecDesc,       // Security descriptor.
			&ftLastWriteTime); // Last write time.			

		BYTE* bData = (BYTE *) LocalAlloc(LMEM_FIXED, dwcMaxValueData);
		dwSize = dwcMaxValueData;

		//��ȡ
		if( RegQueryValueEx(hkey, szBakValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//��ԭ
			RegSetValueEx(hkey, szValueName, NULL, dwType, bData, dwSize);

		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		mcu::log << _T( "RestoreRegValue ��ע���ʧ�ܣ�" ) << endl;
	}
}

void CMCUDemoDlg::RestoreRtspPlayerRegKey()
{
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");
}

void CMCUDemoDlg::OnMenuForward()
{
	// TODO: �ڴ���������������
	m_htmlWnd.HistoryForward();
}

void CMCUDemoDlg::OnMenuPlayFile()
{
	// TODO: �ڴ���������������
//	begin( "1.asf", 0 );
	//begin( "1.jpg", 0 );
}

void CMCUDemoDlg::OnMenuRefresh()
{
	// TODO: �ڴ���������������
	this->m_htmlWnd.Refresh();
}

//void CMCUDemoDlg::OnMenuPic()
//{
//	// TODO: �ڴ���������������
//	CPicManageDialog dlg;
//
//	dlg.DoModal();
//}

void CMCUDemoDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	if( IDOK ==  MessageBox( _T( "���Ƿ��˳���¼��" ), _T( "��ʾ" ), MB_OKCANCEL ) )
	{
		this->OnMenuLogin();
	}
	else
	{
		return;
	}

//	CUIDialog::OnOK();
}

void CMCUDemoDlg::OnBnClickedButtonPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPicManageDialog dlg;
	dlg.DoModal();
}

void CMCUDemoDlg::OnBnClickedButtonConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CConfigDialog dlg;
	dlg.DoModal();
}

void CMCUDemoDlg::OnBnClickedButtonSipButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( this->IsSipRaise() )
	{
		this->LowerSip();
	}
	else
	{
		this->RaiseSip();
	}
}

void CMCUDemoDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	// �����뷨����ʱ���������Ի����С��
	if (SPI_SETSIPINFO == uFlags){
		SHACTIVATEINFO sai;
		memset(&sai, 0, sizeof(SHACTIVATEINFO));
		SHHandleWMSettingChange( GetSafeHwnd(), uFlags, (LPARAM)lpszSection, &sai);

		return;
	}

	CUIDialog::OnSettingChange(uFlags, lpszSection);

	// TODO: �ڴ˴������Ϣ����������
}
