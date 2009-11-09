// MainDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "MainDialog.h"


#include "VideoWndDialog.h"
#include "mcucommon.h"

// CMainDialog 对话框

IMPLEMENT_DYNAMIC(CMainDialog, CUIDialog)

CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CBaseMainDialog(CMainDialog::IDD, pParent)
{

}

CMainDialog::~CMainDialog()
{
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseMainDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDialog, CBaseMainDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDialog 消息处理程序


BOOL CMainDialog::OnInitDialog()
{
	CBaseMainDialog::OnInitDialog();

	tstring strBgImgPath = GetModulePath();
	strBgImgPath = ParsePath( strBgImgPath.c_str() ).m_strDirectory + _T( "res\\chs\\main_bg.jpg" );
	this->SetImage( strBgImgPath.c_str() );

	m_logoDlg.Create( CLogoDialog::IDD, this );
	this->UpdateCtrlPos();

	m_logoDlg.ShowWindow( SW_SHOW );

	// TODO:  在此添加额外的初始化
//	CVideoWndDialog *dlg = new CVideoWndDialog();
//	dlg->Create( CVideoWndDialog::IDD, this );
//	dlg->ShowWindow( SW_HIDE );
//
//	//CWnd *pTopDlg = FindWindow( _T("menu_worker"),NULL);	//隐藏背景 
//	//if (pTopDlg != NULL) 
//	//{ 
//	//	pTopDlg->ShowWindow(SW_HIDE); 
//	//} 
//	
//	::EnumWindows( &CMainDialog::WindowsEnumCB, (LPARAM)this );
//
////	::EnumChildWindows( GetSafeHwnd(), WindowsEnumCB, (LPARAM) this );
//
//	DWORD dwFlag = 0;
////	if ( m_bFullScreenMode )
//	{
//		dwFlag = SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON;		
//	}
//	//else
//	//{
//	//	dwFlag = SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON;
//	//}
//	BOOL bResult = SHFullScreen( this->GetSafeHwnd(), dwFlag );   
//	_ASSERT( bResult );
//
//	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
//	int nScreenHeight = ::GetSystemMetrics( SM_CYSCREEN );
////	if ( nScreenWidth != rcClient.Width() || nScreenHeight != rcClient.Height() )
//	{
//		//			::MoveWindow( GetSafeHwnd(), 0, 0, nScreenWidth, nScreenHeight, TRUE );
//		//			this->MoveWindow( 0, 0, nScreenWidth, nScreenHeight );
//		this->SetWindowPos( &CWnd::wndTopMost, 0, 0, nScreenWidth, nScreenHeight, 0 );
//
//		//			this->MoveWindow( rcWnd );
//
//		//if ( m_cVideoWnd )
//		//{
//		//	m_cVideoWnd.MoveWindow( 0, 0, nScreenWidth, nScreenHeight );
//		//}
//	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CALLBACK CMainDialog::WindowsEnumCB( HWND hWnd, LPARAM param )
{
	CMainDialog *pThis = ( CMainDialog* )param;

	CString strClassName;
	::GetClassName( hWnd, strClassName.GetBuffer( 100 ), 100 );
	strClassName.ReleaseBuffer();

	if ( strClassName == _T("menu_worker") )
	{
		if( ::IsWindowVisible( hWnd ) )
		{
			mcu::tlog << _T("Find a visitable menu_worker!!!!") << hWnd <<endl;

			BOOL bEnable = ::IsWindowEnabled( hWnd );
			HWND hParent = ::GetParent( hWnd );
			if ( hParent == pThis->GetSafeHwnd() )
			{
				mcu::tlog << _T( "this window 's child window " ) << endl;

				::ShowWindow( hWnd, SW_HIDE );
			}
			

		}
		//else
		//{
		//	mcu::tlog << _T("Find a unvisitable menu_worker!!!!") <<endl;
		//}

		
		
	}

	return TRUE;
}
void CMainDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	UpdateCtrlPos();
}

void CMainDialog::UpdateCtrlPos()
{
	CRect rcClient;
	if ( GetSafeHwnd() )
	{
		GetClientRect( rcClient );
	}
	else
	{
		return;
	}

	if ( m_logoDlg.GetSafeHwnd() )
	{
		const int conLogoHeight = 30;
		m_logoDlg.MoveWindow( 0,0, rcClient.Width(), conLogoHeight );
	}
}
