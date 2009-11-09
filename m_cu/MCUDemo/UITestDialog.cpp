// UITestDialog.cpp : ʵ���ļ�
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

// CUITestDialog �Ի���

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


// CUITestDialog ��Ϣ�������

void CUITestDialog::OnBnClickedButtonPlayerDlgTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPlayerDialog dlg;
	CMCUSession::Instance()->CurVideoSession()->PtzControl( TRUE );
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonLoginDialogTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPPCLoginDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonHtmlDialogTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMCUDemoDlg dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonConfigDialogTest()
{
	CConfigDialog dlg;
	dlg.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CSysCfgPage sysPage;
	//CConfigPropertySheet sheet( _T( "unit test" ) );
	//sheet.AddPage( &sysPage );
	//sheet.DoModal();
}

void CUITestDialog::OnBnClickedButtonMainDialogTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

BOOL CUITestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
//	this->m_btnOk.SetIma
// c_l_cancel.JPG
//	this->m_btnImage.SetImage( _T( "\\Storage Card\\picres\\c_o_back.JPG" ) );

	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUITestDialog::OnBnClickedButtonImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPicManageDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonControlTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CControlTestDialog dlg;
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonPlayerDlgNoPtz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPlayerDialog dlg;
	CMCUSession::Instance()->CurVideoSession()->PtzControl( FALSE );
	dlg.DoModal();
}

void CUITestDialog::OnBnClickedButtonDirSelTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDirSelectDialog dlg;
	int nResult = dlg.DoModal();
	CString strDir = dlg.GetSelectedDir().c_str();

	if( IDOK == nResult )
	{
		MessageBox( strDir + _T( "\n" ) + _T( "�û����ȷ����" ) );
	}
	else if( IDCANCEL == nResult )
	{
		MessageBox(  strDir + _T( "\n" ) + _T("�û����ȡ����" ) );
	}
}
