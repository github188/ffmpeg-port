// PlayerDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "PlayerDialog.h"
#include <afxwin.h>

#ifdef USE_FFMPEG
#include "ffplay.h"
#endif

#include <winbase.h>
#include <stdlib.h>

#include "MediaNet.h"
#include "mcucommon.h"
#include "MCUSession.h"
#include "config.h"
#include "log.h"

// CPlayerDialog �Ի���
#define MAX_VALUE_NAME 1024
#define  APP_RUNNING     2
#define  APP_NORMALEXIT  1
#define  APP_FIRSTRUN    0

static BOOL g_bIsFirst = TRUE;

IMPLEMENT_DYNAMIC(CPlayerDialog, CUIDialog)

CPlayerDialog::CPlayerDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CPlayerDialog::IDD, pParent)
{
	m_bFullScreenMode = FALSE;
//	m_hSoftKeyBar = NULL;
	m_nOrigRotation = 0;
	m_pSubPtzMenu = NULL;
	m_bIsEnding = FALSE;
}

CPlayerDialog::~CPlayerDialog()
{
}

void CPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BOTTOM_BG, m_stBottomBg);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_btnRecord);
	DDX_Control(pDX, IDC_BUTTON_CAPTURE, m_btnCapture);
	DDX_Control(pDX, IDC_BUTTON_PTZ, m_btnPtz);
	DDX_Control(pDX, IDC_BUTTON_FULLSCREEN, m_btnFullscreen);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnReturn);
}


BEGIN_MESSAGE_MAP(CPlayerDialog, CUIDialog)
//	ON_BN_CLICKED(IDOK, &CPlayerDialog::OnBnClickedOk)
//ON_BN_CLICKED(ID_BUTTON_PLAY, &CPlayerDialog::OnBnClickedButtonPlay)
ON_WM_SIZE()
//ON_COMMAND(ID_MENU_FULL_SCREEN, &CPlayerDialog::OnMenuFullScreen)
//ON_COMMAND(ID_MENU_CLOSE, &CPlayerDialog::OnMenuClose)
ON_WM_LBUTTONDOWN()
ON_COMMAND(ID_MENU_RESTORE, &CPlayerDialog::OnMenuRestore)
ON_MESSAGE( WM_VIDEO_OPEN_FAIL, &CPlayerDialog::OnVideoFail )
ON_MESSAGE( WM_RECORD_FAIL, &CPlayerDialog::OnRecordFail )
//ON_COMMAND(ID_MENU_CAPTURE, &CPlayerDialog::OnMenuCapture)
ON_WM_LBUTTONDBLCLK()
ON_UPDATE_COMMAND_UI(ID_MENU_PTZ, &CPlayerDialog::OnUpdateMenuPtz)
ON_UPDATE_COMMAND_UI(ID_MENU_DIGITAL_PTZ, &CPlayerDialog::OnUpdateMenuDigitalPtz)
ON_COMMAND(ID_MENU_PTZ, &CPlayerDialog::OnMenuPtz)
ON_COMMAND(ID_MENU_DIGITAL_PTZ, &CPlayerDialog::OnMenuDigitalPtz)
//ON_COMMAND(ID_MENU_RECODER, &CPlayerDialog::OnMenuRecoder)
//ON_COMMAND(ID_MENU_PAUSE, &CPlayerDialog::OnMenuPause)
ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CPlayerDialog::OnBnClickedButtonPause)
ON_BN_CLICKED(IDC_BUTTON_RECORD, &CPlayerDialog::OnBnClickedButtonRecord)
ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CPlayerDialog::OnBnClickedButtonCapture)
ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CPlayerDialog::OnBnClickedButtonFullscreen)
ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPlayerDialog::OnBnClickedButtonClose)
ON_BN_CLICKED(IDC_BUTTON_PTZ, &CPlayerDialog::OnBnClickedButtonPtz)
ON_WM_ACTIVATE()
END_MESSAGE_MAP()



// CPlayerDialog ��Ϣ�������

BOOL CPlayerDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	tstring strTitle;
	CConfig::Instance()->GetAppTitle( strTitle );
	this->SetWindowText( strTitle.c_str() );

	CRect rcClient;
	GetClientRect( rcClient );
	/** ������Ƶ���Ŵ���.*/
	m_cVideoWnd.Create( CVideoWndDialog::IDD, this );


	/** ����ptz����. */
	m_cPtzDlg.Create( CPTZDialog::IDD, this );

	/** �������ڲ���. */
	this->UpdateLayout();

	m_cVideoWnd.ShowWindow( SW_SHOW );
	m_cPtzDlg.ShowWindow( SW_SHOW );

	// logo
	m_logoDlg.Create( CLogoDialog::IDD, this );
	m_logoDlg.ShowWindow( SW_SHOW );

	// ptz �˵���
	BOOL bResult = m_menuPtz.LoadMenu( IDR_MENU_PTZ );
	_ASSERT( bResult );
	m_pSubPtzMenu = m_menuPtz.GetSubMenu( 0 );
	_ASSERT( m_pSubPtzMenu );

	BOOL bHasPtz = this->HasPtz();
	this->m_btnPtz.EnableWindow( bHasPtz );

	// �ײ��˵�������ť��
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnPause.SetBkTransparent( FALSE, FALSE );
	this->m_btnPause.SetImage( _T( "btn_player_pause_normal.jpg" ), _T( "btn_player_pause_focus.jpg" ), _T( "btn_player_pause_disable.jpg" ), FALSE );

	this->m_btnRecord.SetBkTransparent( FALSE, FALSE );
	this->m_btnRecord.SetImage( _T( "btn_player_record_normal.jpg" ), _T( "btn_player_record_focus.jpg" ), _T( "btn_player_record_disable.jpg" ), FALSE );

	this->m_btnCapture.SetBkTransparent( FALSE, FALSE );
	this->m_btnCapture.SetImage( _T( "btn_player_capture_normal.jpg" ), _T( "btn_player_capture_focus.jpg" ), _T( "btn_player_capture_disable.jpg" ), FALSE );

	this->m_btnPtz.SetBkTransparent( FALSE, FALSE );
	this->m_btnPtz.SetImage( _T( "btn_player_ptz_normal.jpg" ), _T( "btn_player_ptz_focus.jpg" ), _T( "btn_player_ptz_disable.jpg" ), FALSE );

	this->m_btnFullscreen.SetBkTransparent( FALSE, FALSE );
	this->m_btnFullscreen.SetImage( _T( "btn_player_fullscreen_normal.jpg" ), _T( "btn_player_fullscreen_focus.jpg" ), _T( "btn_player_fullscreen_disable.jpg" ), FALSE );

	this->m_btnReturn.SetBkTransparent( FALSE, FALSE );
	this->m_btnReturn.SetImage( _T( "btn_player_return_normal.jpg" ), _T( "btn_player_return_focus.jpg" ), _T( "btn_player_return_disable.jpg" ), FALSE );


	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	this->UpdateLayout();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlayerDialog::SetRtspUrl( LPCTSTR strRtspUrl )
{
	this->m_cVideoWnd.SetRstpUrl( strRtspUrl );
}

void CPlayerDialog::PlayFullScreen( BOOL bFullScreen )
{
	m_bFullScreenMode = bFullScreen;

	// ����/��ʾ�ؼ���
	m_stBottomBg.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_logoDlg.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnPause.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnRecord.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnCapture.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnFullscreen.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnReturn.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnPtz.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );

	BOOL bShowPtzDlg = this->HasPtz() && !m_bFullScreenMode;
	this->m_cPtzDlg.ShowWindow( bShowPtzDlg ? SW_SHOW : SW_HIDE );
	
	
	m_cVideoWnd.SetFullScreen( bFullScreen );

	this->FullScreen( bFullScreen ? FS_HideTaskBar | FS_HorizontalScreen : FS_ShowTaskBar | FS_ResumeScreen );

}

BOOL CPlayerDialog::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ���Ȼظ�����.
	this->OnMenuRestore();

	// �����Ӵ���.
	if ( m_cVideoWnd.GetSafeHwnd() && ::IsWindow( m_cVideoWnd.GetSafeHwnd() ) )
	{
		m_cVideoWnd.DestroyWindow();
	}

	if ( m_cPtzDlg.GetSafeHwnd() && ::IsWindow( m_cPtzDlg.GetSafeHwnd() ) )
	{
		m_cPtzDlg.DestroyWindow();
	}

	return CUIDialog::DestroyWindow();
}

BOOL CPlayerDialog::HasPtz()const
{
	return CMCUSession::Instance()->CurVideoSession()->PtzControl();
}

void CPlayerDialog::UpdateLayout( LPRECT lprcClient /* =/* = NULL */ )
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
	

	// ��Ļ��С��
	int nScreenLong = GetScreenLong();
	int nScreenWidth = ::GetScreenWidth();

	BOOL bShowCtrl = !m_bFullScreenMode;

	// ʣ��Ŀռ䡣
	CRect rcLeftSpace = rcClient;

	// logo 	
	if( m_logoDlg.GetSafeHwnd() && bShowCtrl )
	{
		int nLogoHeight = m_logoDlg.GetHeight();

		m_logoDlg.MoveWindow( 0,0, rcClient.Width(), nLogoHeight );

		// ��ʣ��ռ���ȥ��logoռ�õĿռ䡣
		rcLeftSpace.top += nLogoHeight;
	}

	// �±߲˵�����
	// �����˵�����λ�á�
	std::vector< CUIButton * > tBtnList;
	tBtnList.push_back( &m_btnPause );
	tBtnList.push_back( &m_btnRecord );
	tBtnList.push_back( &m_btnCapture );
	tBtnList.push_back( &m_btnPtz );
	tBtnList.push_back( &m_btnFullscreen );
	tBtnList.push_back( &m_btnReturn );

	if ( m_stBottomBg.GetSafeHwnd() /*&& m_btnDelete.GetSafeHwnd() && m_btnReturn.GetSafeHwnd()*/ && bShowCtrl )
	{
		// �ײ���ť���������ǰ�Ĳ˵����ĸ߶�	
		const float conFMenubarHeight = 0.082;
		int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

		CRect rcBottomBg = rcClient;
		rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
		//m_stBottomBg.MoveWindow( rcBottomBg );
		this->m_stBottomBg.SetWindowPos( &CWnd::wndTop, rcBottomBg.left, rcBottomBg.top, rcBottomBg.Width(), rcBottomBg.Height(), NULL );

		// ��ȡ��ť�Ŀ�ȡ���ť��ȶ���һ���ģ����յ�һ����ť�Ŀ�ȼ��㡣
		CRect rcFirstBtn;
		BOOL bResult = tBtnList[0]->GetImageSize( rcFirstBtn );
		if ( bResult )
		{
			int nBtnWidth = (int)( ( float( nMenubarHeight * rcFirstBtn.Width() ) / float( rcFirstBtn.Height() ) ) + 0.5 );	// �������롣

			// ���㰴ť���϶�Ŀ�ȵĺ͡�
			int nTotalBank = nScreenWidth - ( nBtnWidth * tBtnList.size() );

			// ���е���λ�á�
			int nLayoutPos = 0;

			// ���а�ť��
			// ���㰴ť��Ŀհ������ʱ�򣬳����������Ϊ��ֹ�����������һ���հ��������Բ�ȡÿ�ζ���һ�εķ�����
			int nButtonCount = 6;
			int nBankCount = nButtonCount - 1;
			for ( size_t i=0; i<tBtnList.size(); ++i )
			{	
				int nBankWidth = 0;
				if ( i != 0 )
				{
					nBankWidth = nTotalBank / nBankCount;
					nTotalBank -= nBankWidth;
					nBankCount --;
				}

				// ��ť��
				CRect rcBtn = rcBottomBg;
				rcBtn.left = nLayoutPos + nBankWidth;
				rcBtn.right = rcBtn.left + nBtnWidth;

				if ( tBtnList[i]->GetSafeHwnd() )
				{
					tBtnList[i]->SetWindowPos( &CWnd::wndTop, rcBtn.left, rcBtn.top, rcBtn.Width(), rcBtn.Height(), NULL );
				}

				nLayoutPos = rcBtn.right;			
			}
		}
		// ��ʣ��ռ���ȥ���˵���ռ�õĿռ䡣
		rcLeftSpace.bottom -= nMenubarHeight;
	}


	BOOL bShowPtzDlg = TRUE;

	// ȫ��ʱ����ptz�Ի���.��ȫ��ʱ��ʾ.
	bShowPtzDlg &= (!m_bFullScreenMode);


	// ��ptz����Ȩ�޵�ʱ������ptz���档
	bShowPtzDlg &= this->HasPtz();

	
	if( m_cPtzDlg.GetSafeHwnd() && bShowPtzDlg )
	{
		const float conFPtzDlgHeight = 0.082;

		int nPtzBtnHeight = int( nScreenLong * conFPtzDlgHeight );
		CRect rcPtzWnd;
		this->m_cPtzDlg.GetWindowRect( rcPtzWnd );
		int nPtzWidth = rcPtzWnd.Width();

		// ptz���������·�����.
		CRect rcPtz = rcLeftSpace;
		rcPtz.top = rcPtz.bottom - nPtzBtnHeight;
		rcPtz.left = ( nScreenWidth - nPtzWidth ) / 2;
		rcPtz.right = rcPtz.left + nPtzWidth;

		m_cPtzDlg.MoveWindow( rcPtz );

		// ��ʣ��ռ���ȥ��ptz������ռ�õĿռ䡣
		rcLeftSpace.bottom -= nPtzBtnHeight;
	}

	if ( m_cVideoWnd.GetSafeHwnd() )
	{
		CRect rcVideoWnd = rcLeftSpace;
		m_cVideoWnd.MoveWindow( rcVideoWnd );
	}
	
}
void CPlayerDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	//tstringstream ssMsg;
	//ssMsg << _T( "(" ) << cx << _T( "," ) << cy << _T( ")" ) << endl;
	//AfxMessageBox( ssMsg.str().c_str() );

	//mcu::tlog << _T( "cy: " ) << cy << endl;

	if ( GetSafeHwnd() )
	{
		this->UpdateLayout( CRect( 0,0, cx, cy ) );
	}
	//
	// TODO: �ڴ˴������Ϣ����������
}

//void CPlayerDialog::OnMenuFullScreen()
//{
//	// TODO: �ڴ���������������
//	this->PlayFullScreen( TRUE );
//}

//void CPlayerDialog::OnMenuClose()
//{
//	// TODO: �ڴ���������������
//	// ���ȫ��,���Ƴ�ȫ��.
//	this->OnMenuRestore();
//
//
//	this->EndDialog( 0 );
//}

void CPlayerDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CUIDialog::OnLButtonDown(nFlags, point);
}

void CPlayerDialog::OnMenuRestore()
{
	// TODO: �ڴ���������������
	if( m_bFullScreenMode )
	{
		this->PlayFullScreen( FALSE );
	}
}

LRESULT CPlayerDialog::OnVideoFail( WPARAM eErrorCode, LPARAM )
{
	if ( MCU_Error_Rtsp_Server_Full == eErrorCode )
	{
		MessageBox( _T( "��������ת���������㣬��ʱ�޷��ṩ�������Ժ����ԣ�" ) );
		//this->EndDialog( E_DlgEndNull );
		OnRtspClose(E_DlgEndNull);
	}
	else if( IDYES == MessageBox( _T( "������Ƶ������ʧ�ܣ��Ƿ����ԣ�" ), _T( "" ), MB_YESNO ) )
	{
		//this->EndDialog( E_DlgEndRetry );
		OnRtspClose(E_DlgEndRetry);
	}
	else
	{
		//this->EndDialog( E_DlgEndNull );
		OnRtspClose(E_DlgEndNull);
	}
	
	return S_OK;
}

LRESULT CPlayerDialog::OnRecordFail( WPARAM errorCode, LPARAM )
{
	EMCU_ErrorCode eErrorCode = EMCU_ErrorCode( errorCode );

	this->UpdateRecordButton();

	MessageBox( _T( "�洢�ռ䲻�㣬¼��ֹͣ��" ) );
	return S_OK;
}

BOOL CPlayerDialog::WindowsEnumCBForFulscreen( HWND hWnd, LPARAM param )
{
	CPlayerDialog *pThis = ( CPlayerDialog * )param;
	// �ж��Ǳ����ڵ��Ӵ���.
	if ( pThis->GetSafeHwnd() == ::GetParent( hWnd ) )
	{
		// �ж��ǲ���menu_worker
		CString strClassName;
		::GetClassName( hWnd, strClassName.GetBuffer( 100 ), 100 );
		strClassName.ReleaseBuffer();
		if ( strClassName == _T("menu_worker") )
		{
			if( pThis->m_bFullScreenMode )
			{
				::ShowWindow( hWnd, SW_HIDE );
			}
		}
	}

	return TRUE;
}
void CPlayerDialog::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CUIDialog::PostNcDestroy();

//	delete this;
}

//void CPlayerDialog::OnMenuCapture()
//{
//	// TODO: �ڴ���������������
//
//
//}

void CPlayerDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->PlayFullScreen( !this->m_bFullScreenMode );


	CUIDialog::OnLButtonDblClk(nFlags, point);
}

void CPlayerDialog::OnUpdateMenuPtz(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	BOOL bHasPtz = CMCUSession::Instance()->CurVideoSession()->PtzControl();
	pCmdUI->Enable( bHasPtz );
	pCmdUI->SetCheck( !this->m_cPtzDlg.IsDigitalPtz() );

}

void CPlayerDialog::OnUpdateMenuDigitalPtz(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	BOOL bHasPtz = CMCUSession::Instance()->CurVideoSession()->PtzControl();
	pCmdUI->Enable( bHasPtz );
	pCmdUI->SetCheck( this->m_cPtzDlg.IsDigitalPtz() );
}

void CPlayerDialog::OnMenuPtz()
{
	// TODO: �ڴ���������������
	this->m_cPtzDlg.SetDigitPtz( FALSE );

	this->UpdateMenu();
}

void CPlayerDialog::OnMenuDigitalPtz()
{
	// TODO: �ڴ���������������
	this->m_cPtzDlg.SetDigitPtz( TRUE );

	this->UpdateMenu();	
}

void CPlayerDialog::UpdateMenu()
{
	if ( m_pSubPtzMenu )
	{
		BOOL bDigitalPtz = m_cPtzDlg.IsDigitalPtz();
		m_pSubPtzMenu->CheckMenuItem( ID_MENU_PTZ, MF_BYCOMMAND | ( bDigitalPtz ? MF_UNCHECKED : MF_CHECKED ) );
		m_pSubPtzMenu->CheckMenuItem( ID_MENU_DIGITAL_PTZ, MF_BYCOMMAND | ( bDigitalPtz ? MF_CHECKED : MF_UNCHECKED ) );
	}	
}


void CPlayerDialog::OnBnClickedButtonPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bIsPause = m_cVideoWnd.IsPause();

	if ( bIsPause )
	{
		BOOL bResult = m_cVideoWnd.Resume();
		mcu::tlog << _T( "Resume result " ) << bResult << endl;
	}
	else
	{
		BOOL bResult = m_cVideoWnd.Pause();
		mcu::tlog << _T( "Pause result " ) << bResult << endl;
	}

	// �л���ť״̬��
	bIsPause = m_cVideoWnd.IsPause();
	if ( bIsPause )
	{
		this->m_btnPause.SetImage( _T( "btn_player_play_normal.jpg" ), _T( "btn_player_play_focus.jpg" ), _T( "btn_player_play_disable.jpg" ), TRUE );
	}
	else
	{
		this->m_btnPause.SetImage( _T( "btn_player_pause_normal.jpg" ), _T( "btn_player_pause_focus.jpg" ), _T( "btn_player_pause_disable.jpg" ), TRUE );
	}
}

void CPlayerDialog::OnBnClickedButtonRecord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bRecording = this->m_cVideoWnd.IsRecoding();
	if ( bRecording )
	{
		BOOL bResult = m_cVideoWnd.StopRecord();
		mcu::tlog << _T( "stop record result: " ) << bResult << endl;
	}
	else
	{
		tstring strPicDir;
		CConfig::Instance()->GetCaptureDir( strPicDir );

		// �����ļ��С�
		BOOL bResult = ::CreateDirectory( strPicDir.c_str(), NULL );

		// �����̿ռ䡣
		ULARGE_INTEGER nFreeSpaceToCaller, nTotalSpace, nFreeSpace;
		bResult = ::GetDiskFreeSpaceEx( strPicDir.c_str(), &nFreeSpaceToCaller, &nTotalSpace, &nFreeSpace );
		int nMinSpace = MIN_STORAGE_SPACE;
		CConfig::Instance()->GetMinStorageSpace( nMinSpace ) ;
		if ( nFreeSpaceToCaller.QuadPart < nMinSpace )
		{
			MessageBox( _T( "�洢�ռ䲻�㣬�޷�����¼��" ) );
			return;
		}

		tstring strPuName = CMCUSession::Instance()->CurVideoSession()->PuName();

		tstring strRec = this->GetFileName( strPicDir.c_str(), strPuName.c_str(), _T( "3gp" ) );

		bResult = this->m_cVideoWnd.StartRecord( strRec.c_str() );
		if(bResult)
		{
			;//MessageBox(L"¼��ɹ���\n");
		}
		else
		{
			MessageBox(L"¼��ʧ�� ����ͼƬ��¼�񱣴�·����\n");
		}

		mcu::tlog << _T( "Start record " ) << strRec << _T( " result " ) << bResult << endl;
	}

	// ���ݵ�ǰ״̬����ͼ�ꡣ
	this->UpdateRecordButton();
}

void CPlayerDialog::OnBnClickedButtonCapture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	tstring strPicDir;
	CConfig::Instance()->GetCaptureDir( strPicDir );

	// �����ļ��С�
	BOOL bResult = ::CreateDirectory( strPicDir.c_str(), NULL );

	// �����̿ռ䡣
	ULARGE_INTEGER nFreeSpaceToCaller, nTotalSpace, nFreeSpace;
	bResult = ::GetDiskFreeSpaceEx( strPicDir.c_str(), &nFreeSpaceToCaller, &nTotalSpace, &nFreeSpace );
	int nMinSpace = MIN_STORAGE_SPACE;
	CConfig::Instance()->GetMinStorageSpace( nMinSpace ) ;
	if ( nFreeSpaceToCaller.QuadPart < nMinSpace )
	{
		MessageBox( _T( "�洢�ռ䲻�㣬�޷�����ͼ��" ) );
		return;
	}

	tstring strPuName = CMCUSession::Instance()->CurVideoSession()->PuName();

	tstring strPic = this->GetFileName( strPicDir.c_str(), strPuName.c_str(), _T( "jpg" ) ); 
	bResult = this->m_cVideoWnd.PicCapture( strPic.c_str() );

	if (!bResult)
	{
		MessageBox(_T("ץ��ʧ��"), _T("ͼ��ץ��"), MB_OK);
	}

	if ( bResult )
	{
		tstring strModulePath = ::GetModulePath();
		tstring strDir = ::ParsePath( strModulePath.c_str() ).m_strDirectory;
		tstring strSoundFile = strDir + UI_PIC_DIR + _T( "capture.wav" );
		BOOL bSound = ::PlaySound( strSoundFile.c_str(), NULL, SND_ASYNC | SND_FILENAME );
		_ASSERT( bSound );
	}
	else
	{

	}
	//ȡ����ǰ����,��ť����
	this->SetFocus();

	mcu::tlog << _T( "capture pic :" ) << strPic << _T( " result: " ) << bResult << endl;
}

void CPlayerDialog::OnBnClickedButtonFullscreen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->PlayFullScreen( TRUE );
}

void CPlayerDialog::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnUserClose();	
}

void CPlayerDialog::OnBnClickedButtonPtz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( m_pSubPtzMenu )
	{
		CRect rcBtnPtz;
		this->m_btnPtz.GetWindowRect( rcBtnPtz );
		CPoint ptCenterPtn = rcBtnPtz.CenterPoint();
		m_pSubPtzMenu->TrackPopupMenu( TPM_CENTERALIGN, ptCenterPtn.x, ptCenterPtn.y, this );
	}

}

void CPlayerDialog::UpdateRecordButton()
{
	BOOL bRecording = this->m_cVideoWnd.IsRecoding();
	if ( bRecording )
	{
		this->m_btnRecord.SetImage( _T( "btn_player_recording_normal.jpg" ), _T( "btn_player_recording_focus.jpg" ), _T( "btn_player_recording_disable.jpg" ), TRUE );
	}
	else
	{
		this->m_btnRecord.SetImage( _T( "btn_player_record_normal.jpg" ), _T( "btn_player_record_focus.jpg" ), _T( "btn_player_record_disable.jpg" ), TRUE );
	}
}

tstring CPlayerDialog::GetFileName( LPCTSTR strDir, LPCTSTR lpPuName, LPCTSTR strFileExt )
{
	tstringstream ssFileName;
	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );
	CTime sysNow( sysTime );

	CString strTime;
	strTime = ::TimeToStr( sysNow.GetTime() ).c_str();

    tstring strPuName = lpPuName;
    strPuName = strPuName.substr( 0, 5 );

	// �ļ������Ҫ��3λ�������
	int nRadom = GetTickCount();
	CString strRadom;
	strRadom.Format( _T( "%03d" ), nRadom ) ;
	strRadom = strRadom.Right( 3 );

	//strPicDir + strPuName + _T( "_" ) + (LPCTSTR)strTime + strRadom + _T( ".jpg" );
	ssFileName << strDir << strPuName << _T( "_" ) << (LPCTSTR)strTime << (LPCTSTR)strRadom << _T( "." ) << strFileExt;
	return ssFileName.str();
}
void CPlayerDialog::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->OnUserClose();
}

void CPlayerDialog::OnUserClose()
{
	if( IDYES == MessageBox( _T( "�Ƿ�Ҫ�˳���" ), _T( "�˳�ȷ��" ), MB_YESNO ) )
	{
		//�������¼�� ֹͣ��ǰ¼��¼��
		BOOL bRecording = this->m_cVideoWnd.IsRecoding();
		m_bIsEnding = TRUE;
		if ( bRecording )
		{
			BOOL bResult = m_cVideoWnd.StopRecord();
			mcu::tlog << _T( "stop record result: " ) << bResult << endl;
		}
		this->OnMenuRestore();

		this->EndDialog( 0 );
	}	
}


void CPlayerDialog::OnRtspClose(int nErrorCode)
{
	//�������¼�� ֹͣ��ǰ¼��¼��
	BOOL bRecording = this->m_cVideoWnd.IsRecoding();
	if ( bRecording )
	{
		BOOL bResult = m_cVideoWnd.StopRecord();
		mcu::tlog << _T( "stop record result: " ) << bResult << endl;
	}

	this->OnMenuRestore();

	this->EndDialog( nErrorCode );
}

void CPlayerDialog::BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
{
	HKEY hkey;
	if (RegOpenKeyEx(root, szSubKey, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		//AfxMessageBox( _T( " BackupRtspPlayerRegKey ok!"), MB_TOPMOST );
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
		if( retCode != ERROR_SUCCESS )
		{
			mcu::tlog << _T( "RegQueryInfoKey ʧ�ܣ�" ) << endl;
		}


		BYTE* bData = (BYTE *) LocalAlloc(LMEM_FIXED, dwcMaxValueData);
		dwSize = dwcMaxValueData;

		//��ȡ
		if( RegQueryValueEx(hkey, szValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//����
			if( RegSetValueEx(hkey, szBakValueName, NULL, dwType, bData, dwSize) == ERROR_SUCCESS )
			{
				//����Ϊ��
				WCHAR* szTmp = L"";
				RegSetValueEx(hkey, szValueName, NULL, dwType, (BYTE*)szTmp, 2);
			}
		}
		else
		{
			mcu::tlog << _T( "RegQueryValueEx ʧ�ܣ�" ) << endl;
		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		mcu::tlog << _T( "BackupRegValue ��ע���ʧ�ܣ�" ) << endl;
	}
}

void CPlayerDialog::RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
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
		mcu::tlog << _T( "RestoreRegValue ��ע���ʧ�ܣ�" ) << endl;
	}
}


void CPlayerDialog::BackupRtspPlayerRegKey()
{
	BackupRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");		
	BackupRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	BackupRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");	
}

void CPlayerDialog::RestoreRtspPlayerRegKey()
{
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");
}

BOOL CPlayerDialog::IsAppNormalExit()
{
	CWinApp* pApp = AfxGetApp();
	UINT dwStatus = pApp->GetProfileInt(L"AppStatus",L"Status",APP_FIRSTRUN);
	if(dwStatus != APP_NORMALEXIT)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void CPlayerDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CUIDialog::OnActivate(nState, pWndOther, bMinimized);

	CWinApp* pApp = AfxGetApp();
	//����
	if (nState == 1)
	{
		if (g_bIsFirst)
  		{
			g_bIsFirst = False;
 		}else
		{
			BackupRtspPlayerRegKey();
		}
	}else//ʧȥ����
	{
		//�ָ�
		if (m_bIsEnding == TRUE)
		{
			return;
		}
		RestoreRtspPlayerRegKey();
	}

	// TODO: �ڴ˴������Ϣ����������
}
