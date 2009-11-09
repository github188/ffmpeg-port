// UITestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "UITestDialog.h"

#include "PlayerDialog.h"
#include "PPCLoginDialog.h"
#include "MCUDemoDlg.h"

#include "MainDialog.h"
#include "controltestdialog.h"
#include "picmanagedialog.h"
#include "MCUSession.h"
#include "configdialog.h"
#include "dirselectdialog.h"

// CUITestDialog 对话框

IMPLEMENT_DYNAMIC(CUITestDialog, CUIDialog)

CUITestDialog::CUITestDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CUITestDialog::IDD, pParent)
{

}

CUITestDialog::~CUITestDialog()
{
}

void CUITestDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, ID_BUTTON_IMAGE, m_btnImage);
}


BEGIN_MESSAGE_MAP(CUITestDialog, CUIDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER_DLG_TEST, &CUITestDialog::OnBnClickedButtonPlayerDlgTest)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_DIALOG_TEST, &CUITestDialog::OnBnClickedButtonLoginDialogTest)
	ON_BN_CLICKED(IDC_BUTTON_HTML_DIALOG_TEST, &CUITestDialog::OnBnClickedButtonHtmlDialogTest)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG_DIALOG_TEST, &CUITestDialog::OnBnClickedButtonConfigDialogTest)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_DIALOG_TEST, &CUITestDialog::OnBnClickedButtonMainDialogTest)
	ON_BN_CLICKED(IDOK, &CUITestDialog::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_IMAGE, &CUITestDialog::OnBnClickedButtonImage)
	ON_BN_CLICKED(IDC_BUTTON_CONTROL_TEST, &CUITestDialog::OnBnClickedButtonControlTest)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER_DLG_NO_PTZ, &CUITestDialog::OnBnClickedButtonPlayerDlgNoPtz)
	ON_BN_CLICKED(IDC_BUTTON_DIR_SEL_TEST, &CUITestDialog::OnBnClickedButtonDirSelTest)
END_MESSAGE_MAP()


// CUITestDialog 消息处理程序

void CUITestDialog::OnBnClickedButtonPlayerDlgTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CPlayerDialog dlg;
	CMCUSession::Instance()->CurVideoSession()->PtzControl( TRUE );
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonLoginDialogTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CPPCLoginDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonHtmlDialogTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CMCUDemoDlg dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonConfigDialogTest()
{
	CConfigDialog dlg;
	dlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
	//CSysCfgPage sysPage;
	//CConfigPropertySheet sheet( _T( "unit test" ) );
	//sheet.AddPage( &sysPage );
	//sheet.DoModal();
}

void CUITestDialog::OnBnClickedButtonMainDialogTest()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bResult = ::DeleteFile( _T( "\\Storage Card\\Images\\20090904164351.jpg.3gp" ) );
	if ( !bResult )
	{
		int sdf = ::GetLastError();
		sdf;
	}

	//bResult = ::DeleteFile( _T( "\\1.jpeg" ) );
	//if ( !bResult )
	//{
	//	int sdf = ::GetLastError();
	//	sdf;
	//}

	CMainDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CUITestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	this->m_btnOk.SetIma
// c_l_cancel.JPG
//	this->m_btnImage.SetImage( _T( "\\Storage Card\\picres\\c_o_back.JPG" ) );

	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUITestDialog::OnBnClickedButtonImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CPicManageDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonControlTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CControlTestDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonPlayerDlgNoPtz()
{
	// TODO: 在此添加控件通知处理程序代码
	CPlayerDialog dlg;
	CMCUSession::Instance()->CurVideoSession()->PtzControl( FALSE );
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonDirSelTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CDirSelectDialog dlg;
	int nResult = dlg.DoModal();
	CString strDir = dlg.GetSelectedDir().c_str();

	if( IDOK == nResult )
	{
		MessageBox( strDir + _T( "\n" ) + _T( "用户点击确定！" ) );
	}
	else if( IDCANCEL == nResult )
	{
		MessageBox(  strDir + _T( "\n" ) + _T("用户点击取消！" ) );
	}
}
