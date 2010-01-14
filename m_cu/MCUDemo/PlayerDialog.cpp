// PlayerDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
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

// CPlayerDialog 对话框
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
//	m_pSubPtzMenu = NULL;
	m_bIsEnding = FALSE;
	m_timeLastDbClick = CTime::GetCurrentTime();

	m_pVideoSessionInit = NULL;
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
ON_WM_DESTROY()
ON_MESSAGE( WM_VIDEO_PLAY_STATUS, &CPlayerDialog::OnRtspStatusNotify )
END_MESSAGE_MAP()



// CPlayerDialog 消息处理程序

BOOL CPlayerDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	tstring strTitle;
	CConfig::Instance()->GetAppTitle( strTitle );
	this->SetWindowText( strTitle.c_str() );

	CRect rcClient;
	GetClientRect( rcClient );
	/** 创建视频播放窗口.*/
	m_cVideoWnd.Create( CVideoWndDialog::IDD, this );


	/** 创建ptz窗口. */
	m_cPtzDlg.Create( CPTZDialog::IDD, this );

    m_cPtzDlg.SetPtzSender( &m_cVideoWnd );

	/** 调整窗口布局. */
//	this->UpdateLayout();

	m_cVideoWnd.ShowWindow( SW_SHOW );
	m_cPtzDlg.ShowWindow( SW_SHOW );

	// logo
	m_logoDlg.Create( CLogoDialog::IDD, this );
	m_logoDlg.ShowWindow( SW_SHOW );

	// ptz 菜单。
	BOOL bResult = m_menuPtz.LoadMenu( IDR_MENU_PTZ );
	_ASSERT( bResult );
	//m_pSubPtzMenu = m_menuPtz.GetSubMenu( 0 );
	//_ASSERT( m_pSubPtzMenu );
    if ( NULL == m_menuPtz.m_hMenu )
    {
        Log() << _T( "Ptz Menu Load Fail!" ) << endl;
    }

	BOOL bHasPtz = this->HasPtz();
	

	// 底部菜单条及按钮。
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

	this->m_btnCapture.EnableWindow( FALSE );
	this->m_btnRecord.EnableWindow( FALSE );

	this->UpdateLayout();

	if ( m_pVideoSessionInit )
	{
		this->Play( m_pVideoSessionInit );
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// void CPlayerDialog::SetRtspUrl( LPCTSTR strRtspUrl )
// {
// 	this->m_cVideoWnd.SetRstpUrl( strRtspUrl );
// }

void CPlayerDialog::PlayFullScreen( BOOL bFullScreen )
{
	m_bFullScreenMode = bFullScreen;

	// 隐藏/显示控件。
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

BOOL CPlayerDialog::Play( CVideoSession *pVs )
{
	if ( !GetSafeHwnd() )
	{
		this->m_pVideoSessionInit = pVs;
		return TRUE;
	}
	else if( pVs )
    {
		// 清屏。
		this->m_cVideoWnd.ClearScreen();

        EMCU_ErrorCode er;
        BOOL bResult = this->m_cVideoWnd.StartPlay( pVs, er );

        // 将数字ptz归位。
        this->m_cVideoWnd.SetDigitalPtz( FALSE );

        this->UpdateCtrlStatus();


        return bResult;
    }
    else
    {
        Log() << _T( "CPlayerDialog::Play video session is NULL" ) << endl;
        return FALSE;
    }
}



BOOL CPlayerDialog::HasPtz()const
{
	return CMCUSession::Instance()->CurVideoSession()->PtzControl();
}

void CPlayerDialog::UpdateCtrlStatus()
{
    this->UpdateRecordButton();

    // ptz 按钮。
    if ( this->m_btnPtz.GetSafeHwnd() )
    {
        this->m_btnPtz.EnableWindow( this->HasPtz() );
    }

    if( m_btnPause.GetSafeHwnd() )
    {
        BOOL bIsPause = m_cVideoWnd.IsPause();
        if ( bIsPause )
        {
            this->m_btnPause.SetImage( _T( "btn_player_play_normal.jpg" ), _T( "btn_player_play_focus.jpg" ), _T( "btn_player_play_disable.jpg" ), TRUE );
        }
        else
        {
            this->m_btnPause.SetImage( _T( "btn_player_pause_normal.jpg" ), _T( "btn_player_pause_focus.jpg" ), _T( "btn_player_pause_disable.jpg" ), TRUE );
        }
    }   

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
	

	// 屏幕大小。
	int nScreenLong = GetScreenLong();
	int nScreenWidth = ::GetScreenWidth();

	BOOL bShowCtrl = !m_bFullScreenMode;

	// 剩余的空间。
	CRect rcLeftSpace = rcClient;

	// logo 	
	if( m_logoDlg.GetSafeHwnd() && bShowCtrl )
	{
		int nLogoHeight = m_logoDlg.GetHeight();

		m_logoDlg.MoveWindow( 0,0, rcClient.Width(), nLogoHeight );

		// 在剩余空间中去除logo占用的空间。
		rcLeftSpace.top += nLogoHeight;
	}

    this->UpdateCtrlStatus();


	// 下边菜单条。
	// 调整菜单条的位置。
	std::vector< CUIButton * > tBtnList;
	tBtnList.push_back( &m_btnPause );
	tBtnList.push_back( &m_btnRecord );
	tBtnList.push_back( &m_btnCapture );
	tBtnList.push_back( &m_btnPtz );
	tBtnList.push_back( &m_btnFullscreen );
	tBtnList.push_back( &m_btnReturn );

	if ( m_stBottomBg.GetSafeHwnd() /*&& m_btnDelete.GetSafeHwnd() && m_btnReturn.GetSafeHwnd()*/ && bShowCtrl )
	{
		// 底部按钮条（替代以前的菜单）的高度	
		const float conFMenubarHeight = 0.082;
		int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

		CRect rcBottomBg = rcClient;
		rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
		//m_stBottomBg.MoveWindow( rcBottomBg );
		this->m_stBottomBg.SetWindowPos( &CWnd::wndTop, rcBottomBg.left, rcBottomBg.top, rcBottomBg.Width(), rcBottomBg.Height(), NULL );

		// 获取按钮的宽度。按钮宽度都是一样的，按照第一个按钮的宽度计算。
		CRect rcFirstBtn;
		BOOL bResult = tBtnList[0]->GetImageSize( rcFirstBtn );
		if ( bResult )
		{
			int nBtnWidth = (int)( ( float( nMenubarHeight * rcFirstBtn.Width() ) / float( rcFirstBtn.Height() ) ) + 0.5 );	// 四舍五入。

			// 计算按钮间空隙的宽度的和。
			int nTotalBank = nScreenWidth - ( nBtnWidth * tBtnList.size() );

			// 排列到的位置。
			int nLayoutPos = 0;

			// 排列按钮。
			// 计算按钮间的空白区域的时候，除法会产生误差，为防止误差积累在最后一个空白区域，所以采取每次都除一次的方法。
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

				// 按钮。
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
		// 在剩余空间中去除菜单条占用的空间。
		rcLeftSpace.bottom -= nMenubarHeight;
	}


	BOOL bShowPtzDlg = TRUE;

	// 全屏时隐藏ptz对话框.非全屏时显示.
	bShowPtzDlg &= (!m_bFullScreenMode);


	// 无ptz控制权限的时候隐藏ptz界面。
	bShowPtzDlg &= this->HasPtz();

    if ( m_cPtzDlg.GetSafeHwnd() )
    {
        m_cPtzDlg.ShowWindow( bShowPtzDlg ? SW_SHOW : SW_HIDE );
    }
	
  
	if( m_cPtzDlg.GetSafeHwnd() && bShowPtzDlg )
	{
		const float conFPtzDlgHeight = 0.082;

		int nPtzBtnHeight = int( nScreenLong * conFPtzDlgHeight );
		CRect rcPtzWnd;
		this->m_cPtzDlg.GetWindowRect( rcPtzWnd );
		int nPtzWidth = rcPtzWnd.Width();

		// ptz控制贴在下发居中.
		CRect rcPtz = rcLeftSpace;
		rcPtz.top = rcPtz.bottom - nPtzBtnHeight;
		rcPtz.left = ( nScreenWidth - nPtzWidth ) / 2;
		rcPtz.right = rcPtz.left + nPtzWidth;

		m_cPtzDlg.MoveWindow( rcPtz );

		// 在剩余空间中去除ptz控制条占用的空间。
		rcLeftSpace.bottom -= nPtzBtnHeight;
	}

	if ( m_cVideoWnd.GetSafeHwnd() )
	{
		CRect rcVideoWnd = rcLeftSpace;
		CRect rcCur;
		m_cVideoWnd.GetWindowRect( rcCur );
		if ( rcVideoWnd != rcCur )
		{
			m_cVideoWnd.MoveWindow( rcVideoWnd );
		}		
	}
	
}
void CPlayerDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	//tstringstream ssMsg;
	//ssMsg << _T( "(" ) << cx << _T( "," ) << cy << _T( ")" ) << endl;
	//AfxMessageBox( ssMsg.str().c_str() );

	//Log() << _T( "cy: " ) << cy << endl;

	if ( GetSafeHwnd() )
	{
		this->UpdateLayout( CRect( 0,0, cx, cy ) );
	}
	//
	// TODO: 在此处添加消息处理程序代码
}

//void CPlayerDialog::OnMenuFullScreen()
//{
//	// TODO: 在此添加命令处理程序代码
//	this->PlayFullScreen( TRUE );
//}

//void CPlayerDialog::OnMenuClose()
//{
//	// TODO: 在此添加命令处理程序代码
//	// 如果全屏,先推出全屏.
//	this->OnMenuRestore();
//
//
//	this->EndDialog( 0 );
//}

void CPlayerDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CUIDialog::OnLButtonDown(nFlags, point);
}

void CPlayerDialog::OnMenuRestore()
{
	// TODO: 在此添加命令处理程序代码
	if( m_bFullScreenMode )
	{
		this->PlayFullScreen( FALSE );
	}
}

LRESULT CPlayerDialog::OnVideoFail( WPARAM eErrorCode, LPARAM )
{
	if ( MCU_Error_Rtsp_Server_Full == eErrorCode )
	{
		MessageBox( _T( "服务器的转码能力不足，暂时无法提供服务，请稍候再试！" ) );
		//this->EndDialog( E_DlgEndNull );
		OnRtspClose(E_DlgEndNull);
	}
	else if( IDYES == MessageBox( _T( "连接视频服务器失败！是否重试？" ), _T( "" ), MB_YESNO ) )
	{
		//this->EndDialog( E_DlgEndRetry );
        Log() << _T( "OnRtspClose E_DlgEndRetry call! " ) << endl;
		OnRtspClose(E_DlgEndRetry);
	}
	else
	{
		//this->EndDialog( E_DlgEndNull );
		OnRtspClose(E_DlgEndNull);
	}
	
	return S_OK;
}

LRESULT CPlayerDialog::OnRecordFail( WPARAM wpErrorCode, LPARAM )
{
	EMCU_ErrorCode eErrorCode = EMCU_ErrorCode( wpErrorCode );

	this->UpdateRecordButton();

    switch( eErrorCode )
    {
    case MCU_Error_Storage_Full:
        MessageBox( _T( "存储空间不足，录像停止！" ) );
    	break;
    case MCU_Error_PlayStop:
    default:
        Log() << _T( "Player wnd receive Reord error MCU_Error_PlayStop message." ) << endl;
        break;
    }

	
	return S_OK;
}

BOOL CPlayerDialog::WindowsEnumCBForFulscreen( HWND hWnd, LPARAM param )
{
	CPlayerDialog *pThis = ( CPlayerDialog * )param;
	// 判断是本窗口的子窗口.
	if ( pThis->GetSafeHwnd() == ::GetParent( hWnd ) )
	{
		// 判断是不是menu_worker
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
	// TODO: 在此添加专用代码和/或调用基类

	CUIDialog::PostNcDestroy();

//	delete this;
}

//void CPlayerDialog::OnMenuCapture()
//{
//	// TODO: 在此添加命令处理程序代码
//
//
//}

void CPlayerDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 双击消息可能很频繁，超出视频窗口的处理速度，造成问题。
	// 这里进行限速，过于频繁的点击将被忽略。
	CTime tNow = CTime::GetCurrentTime();
	CTimeSpan ts = tNow - m_timeLastDbClick;	
	if ( ts.GetSeconds() < 1 )
	{
		return;
	}
	else
	{
		this->m_timeLastDbClick = tNow;
	}



	this->PlayFullScreen( !this->m_bFullScreenMode );


	CUIDialog::OnLButtonDblClk(nFlags, point);
}

void CPlayerDialog::OnUpdateMenuPtz(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bHasPtz = CMCUSession::Instance()->CurVideoSession()->PtzControl();
	pCmdUI->Enable( bHasPtz );
	pCmdUI->SetCheck( !this->m_cVideoWnd.IsDigitalPtz() );

}

void CPlayerDialog::OnUpdateMenuDigitalPtz(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL bHasPtz = CMCUSession::Instance()->CurVideoSession()->PtzControl();
	pCmdUI->Enable( bHasPtz );
	pCmdUI->SetCheck( this->m_cVideoWnd.IsDigitalPtz() );
}

void CPlayerDialog::OnMenuPtz()
{
	// TODO: 在此添加命令处理程序代码
	this->m_cVideoWnd.SetDigitalPtz( FALSE );

//	this->UpdateMenu();
}

void CPlayerDialog::OnMenuDigitalPtz()
{
	// TODO: 在此添加命令处理程序代码
	this->m_cVideoWnd.SetDigitalPtz( TRUE );

//	this->UpdateMenu();	
}

//void CPlayerDialog::UpdateMenu()
//{
//    CMenu *pSubM = m_menuPtz.GetSubMenu( 0 );
//
//	if ( pSubM && pSubM->m_hMenu )
//	{
//		BOOL bDigitalPtz = m_cVideoWnd.IsDigitalPtz();
//		pSubM->CheckMenuItem( ID_MENU_PTZ, MF_BYCOMMAND | ( bDigitalPtz ? MF_UNCHECKED : MF_CHECKED ) );
//		pSubM->CheckMenuItem( ID_MENU_DIGITAL_PTZ, MF_BYCOMMAND | ( bDigitalPtz ? MF_CHECKED : MF_UNCHECKED ) );
//	}	
//}


void CPlayerDialog::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bIsPause = m_cVideoWnd.IsPause();

// 	if ( bIsPause )
// 	{
// 		BOOL bResult = m_cVideoWnd.Resume();
// 		Log() << _T( "Resume result " ) << bResult << endl;
// 	}
// 	else
// 	{
// 		BOOL bResult = m_cVideoWnd.Pause();
// 		Log() << _T( "Pause result " ) << bResult << endl;
// 	}

    BOOL bResult =this->m_cVideoWnd.PausePlay( !bIsPause );
    Log() << _T( "pause cmd: ") << !bIsPause << _T(" result " ) << bResult << endl;

	// 切换按钮状态。
    this->UpdateCtrlStatus();
}

void CPlayerDialog::OnBnClickedButtonRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bRecording = this->m_cVideoWnd.IsRecording();
	if ( bRecording )
	{
        EMCU_ErrorCode er;
		BOOL bResult = m_cVideoWnd.StopRecord( er );
        Log() << _T( "stop record result: " ) << bResult << _T( " er: " ) << er << endl;
	}
	else
	{
		tstring strPicDir;
		CConfig::Instance()->GetCaptureDir( strPicDir );

		// 创建文件夹。
		BOOL bResult = ::CreateDirectory( strPicDir.c_str(), NULL );

		// 检测磁盘空间。
		ULARGE_INTEGER nFreeSpaceToCaller, nTotalSpace, nFreeSpace;
		bResult = ::GetDiskFreeSpaceEx( strPicDir.c_str(), &nFreeSpaceToCaller, &nTotalSpace, &nFreeSpace );
		int nMinSpace = MIN_STORAGE_SPACE;
		CConfig::Instance()->GetMinStorageSpace( nMinSpace ) ;
		if ( nFreeSpaceToCaller.QuadPart < nMinSpace )
		{
			MessageBox( _T( "存储空间不足，无法保存录像！" ) );
			return;
		}

        tstring strRecPath;
        EMCU_ErrorCode er;
        bResult = this->m_cVideoWnd.StartRecord( strRecPath, er );

		//tstring strPuName = CMCUSession::Instance()->CurVideoSession()->PuName();

		//tstring strRec = this->GetFileName( strPicDir.c_str(), strPuName.c_str(), _T( "3gp" ) );

		//bResult = this->m_cVideoWnd.StartRecord( strRec.c_str() );
		if(bResult)
		{
			;//MessageBox(L"录像成功！\n");
		}
		else
		{
			MessageBox(L"录像失败 请检查图片及录像保存路径！\n");
		}

		Log() << _T( "Start record " ) << strRecPath << _T( " result " ) << bResult << endl;
	}

	// 根据当前状态设置图标。
	this->UpdateRecordButton();
}

void CPlayerDialog::OnBnClickedButtonCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	tstring strPicDir;
	CConfig::Instance()->GetCaptureDir( strPicDir );

    

	// 创建文件夹。
	BOOL bResult = ::CreateDirectory( strPicDir.c_str(), NULL );

	// 检测磁盘空间。
	ULARGE_INTEGER nFreeSpaceToCaller, nTotalSpace, nFreeSpace;
	bResult = ::GetDiskFreeSpaceEx( strPicDir.c_str(), &nFreeSpaceToCaller, &nTotalSpace, &nFreeSpace );
	int nMinSpace = MIN_STORAGE_SPACE;
	CConfig::Instance()->GetMinStorageSpace( nMinSpace ) ;
	if ( nFreeSpaceToCaller.QuadPart < nMinSpace )
	{
		MessageBox( _T( "存储空间不足，无法保存图像！" ) );
		return;
	}

    EMCU_ErrorCode er;
    tstring strPicPath;
    bResult = this->m_cVideoWnd.Capture( strPicPath, er );

	//tstring strPuName = CMCUSession::Instance()->CurVideoSession()->PuName();

	//tstring strPic = this->GetFileName( strPicDir.c_str(), strPuName.c_str(), _T( "jpg" ) ); 
	//bResult = this->m_cVideoWnd.PicCapture( strPic.c_str() );

	//if (!bResult)
	//{
	//	MessageBox(_T("抓拍失败"), _T("图像抓拍"), MB_OK);
	//}

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

	//取消当前焦点,按钮弹起
	this->SetFocus();

	Log() << _T( "capture pic :" ) << strPicPath << _T( " result: " ) << bResult << endl;
}

void CPlayerDialog::OnBnClickedButtonFullscreen()
{
	// TODO: 在此添加控件通知处理程序代码
	this->PlayFullScreen( TRUE );
}

void CPlayerDialog::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnUserClose();	
}

void CPlayerDialog::OnBnClickedButtonPtz()
{
	// TODO: 在此添加控件通知处理程序代码
    CMenu *pSubM = this->m_menuPtz.GetSubMenu( 0 );



	if ( pSubM && pSubM->m_hMenu )
	{
		CRect rcBtnPtz;
		this->m_btnPtz.GetWindowRect( rcBtnPtz );
		CPoint ptCenterPtn = rcBtnPtz.CenterPoint();

		BOOL bDigitalPtz = m_cVideoWnd.IsDigitalPtz();
		pSubM->CheckMenuItem( ID_MENU_PTZ, MF_BYCOMMAND | ( bDigitalPtz ? MF_UNCHECKED : MF_CHECKED ) );
		pSubM->CheckMenuItem( ID_MENU_DIGITAL_PTZ, MF_BYCOMMAND | ( bDigitalPtz ? MF_CHECKED : MF_UNCHECKED ) );

		pSubM->TrackPopupMenu( TPM_CENTERALIGN, ptCenterPtn.x, ptCenterPtn.y, this );
	}
    else
    {
        Log() << _T( "Ptz Menu is NULL!!" ) << endl;
    }

}

void CPlayerDialog::UpdateRecordButton()
{
    if ( this->m_btnRecord.GetSafeHwnd() )
    {
        BOOL bRecording = this->m_cVideoWnd.IsRecording();
        if ( bRecording )
        {
            this->m_btnRecord.SetImage( _T( "btn_player_recording_normal.jpg" ), _T( "btn_player_recording_focus.jpg" ), _T( "btn_player_recording_disable.jpg" ), TRUE );
        }
        else
        {
            this->m_btnRecord.SetImage( _T( "btn_player_record_normal.jpg" ), _T( "btn_player_record_focus.jpg" ), _T( "btn_player_record_disable.jpg" ), TRUE );
        }
    }	
}

//tstring CPlayerDialog::GetFileName( LPCTSTR strDir, LPCTSTR lpPuName, LPCTSTR strFileExt )
//{
//	tstringstream ssFileName;
//
//    while( ssFileName.str().empty() || IsFileExist( ssFileName.str().c_str() ) )
//    {
//        tstring strTime;
//        strTime = ::TimeToStr( ::GetCurTime() ).c_str();
//
//        // 时间只保留月日小时分钟。
//        strTime = strTime.substr( 4, 8 );
//
//        // 前端名只保留5位，限制长度。
//        tstring strPuName = lpPuName;
//        strPuName = strPuName.substr( 0, 5 );
//
//        // 文件名最后要加3位随机数。
//        int nRadom = GetTickCount();
//        CString strRadom;
//        strRadom.Format( _T( "%03d" ), nRadom ) ;
//        strRadom = strRadom.Right( 3 );
//
//        //strPicDir + strPuName + _T( "_" ) + (LPCTSTR)strTime + strRadom + _T( ".jpg" );
//        ssFileName << strDir << strPuName << _T( "_" ) << strTime << (LPCTSTR)strRadom << _T( "." ) << strFileExt;
//    }
//
//	
//	return ssFileName.str();
//}
void CPlayerDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->OnUserClose();
}

void CPlayerDialog::OnUserClose()
{
	if( IDYES == MessageBox( _T( "是否要退出？" ), _T( "退出确定" ), MB_YESNO ) )
	{
		//如果正在录像 停止当前录像录像
		BOOL bRecording = this->m_cVideoWnd.IsRecording();
		m_bIsEnding = TRUE;
		if ( bRecording )
		{
            EMCU_ErrorCode er;
			BOOL bResult = m_cVideoWnd.StopRecord( er );
			Log() << _T( "stop record result: " ) << bResult << endl;
		}

        BOOL bStopRet = this->m_cVideoWnd.StopPlay();
        Log() << _T( "stop video play ret: " ) << bStopRet << endl;

		this->OnMenuRestore();

//		this->EndDialog( 0 );
        // 清理屏幕。
        this->m_cVideoWnd.ClearScreen();

        __super::OnOK();
	}	
}


void CPlayerDialog::OnRtspClose(int nErrorCode)
{
	//如果正在录像 停止当前录像录像
//    Log() << _T( "CPlayerDialog::OnRtspClose begin! " ) << endl;
	BOOL bRecording = this->m_cVideoWnd.IsRecording();

    Log() << _T( "CPlayerDialog::OnRtspClose is rec:" ) << bRecording << endl;
	if ( bRecording )
	{
        EMCU_ErrorCode er;
		BOOL bResult = m_cVideoWnd.StopRecord( er );
		Log() << _T( "stop record result: " ) << bResult << endl;
	}

//     Log() << _T( "before OnMenuRestore! " ) << endl;
	this->OnMenuRestore();

    // 清理屏幕。
//    Log() << _T( "before clear screen! " ) << endl;
    this->m_cVideoWnd.ClearScreen();

    if ( nErrorCode == E_DlgEndRetry )
    {
        // this->m_cVideoWnd.SetRstpUrl( m)
//        __super::OnOK();
        Log() << _T( "befre Restart video play. ret: " ) << endl;
        BOOL bResult = this->m_cVideoWnd.RestartPlay();
        Log() << _T( "Restart video play. ret: " ) << bResult << endl;
    }
    else
    {
        __super::OnOK();
    }

	
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
			Log() << _T( "RegQueryInfoKey 失败！" ) << endl;
		}


		BYTE* bData = (BYTE *) LocalAlloc(LMEM_FIXED, dwcMaxValueData);
		dwSize = dwcMaxValueData;

		//读取
		if( RegQueryValueEx(hkey, szValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//备份
			if( RegSetValueEx(hkey, szBakValueName, NULL, dwType, bData, dwSize) == ERROR_SUCCESS )
			{
				//设置为空
				WCHAR* szTmp = L"";
				RegSetValueEx(hkey, szValueName, NULL, dwType, (BYTE*)szTmp, 2);
			}
		}
		else
		{
			Log() << _T( "RegQueryValueEx 失败！" ) << endl;
		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		Log() << _T( "BackupRegValue 打开注册表失败！" ) << endl;
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

		//读取
		if( RegQueryValueEx(hkey, szBakValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//还原
			RegSetValueEx(hkey, szValueName, NULL, dwType, bData, dwSize);

		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		Log() << _T( "RestoreRegValue 打开注册表失败！" ) << endl;
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
	//激活
	if (nState == 1)
	{
		if (g_bIsFirst)
  		{
			g_bIsFirst = FALSE;
 		}else
		{
			BackupRtspPlayerRegKey();
		}
	}else//失去焦点
	{
		//恢复
		if (m_bIsEnding == TRUE)
		{
			return;
		}
		RestoreRtspPlayerRegKey();
	}

	// TODO: 在此处添加消息处理程序代码
}

void CPlayerDialog::OnDestroy()
{
    // 首先回复窗口.
    this->OnMenuRestore();

    // 销毁子窗口.
    if ( m_cVideoWnd.GetSafeHwnd() && ::IsWindow( m_cVideoWnd.GetSafeHwnd() ) )
    {
        m_cVideoWnd.DestroyWindow();
    }

    if ( m_cPtzDlg.GetSafeHwnd() && ::IsWindow( m_cPtzDlg.GetSafeHwnd() ) )
    {
        m_cPtzDlg.DestroyWindow();
    }

    __super::OnDestroy();
}

void CPlayerDialog::OnShowWindowCmd( int nSWCmd )
{
    if ( SW_SHOW == nSWCmd )
    {
        this->UpdateLayout();
    }
}

LRESULT CPlayerDialog::OnRtspStatusNotify( WPARAM wParam, LPARAM lParam )
{
	BOOL bRecEnable = FALSE;
	BOOL bCapEnable = FALSE;
	ERTSPStatus eRtspStatus = (ERTSPStatus)lParam;
	switch( eRtspStatus )
	{
	case RTSPStatus_Idle:
	case RTSPStatus_Init:
	case RTSPStatus_Opition:
	case RTSPStatus_Description:
	case RTSPStatus_Setup:
	case RTSPStatus_Play:
	case RTSPStatus_Error_Server_Full:
	case RTSPStatus_Error_Unknown:    
	case RTSPStatus_Error_Init_Fail:
	case RTSPStatus_Error_Opition:
	case RTSPStatus_Error_Description:
	case RTSPStatus_Error_Setup:
	case RTSPStatus_Error_Play:
	case RTSPStatus_Error_SDP:
	case RTSPStatus_Error_Create_Rcv:
	case RTSPStatus_Error_WaitPacket:
	case RTSPStatus_Error_Decoder_Fail:
		// 不能抓拍，不能录像。
		bRecEnable = FALSE;
		bCapEnable = FALSE;
		break;

	case RTSPStatus_Running:
	case RTSPStatus_WaitingPacket:
		// 可以抓拍，可以录像。
		bRecEnable = TRUE;
		bCapEnable = TRUE;
		break;
	default:
		break;
	}

	if ( m_btnCapture.GetSafeHwnd() )
	{
		m_btnCapture.EnableWindow( bCapEnable );
	}
	if ( m_btnRecord.GetSafeHwnd() )
	{
		m_btnRecord.EnableWindow( bRecEnable );
	}
	return TRUE;
}