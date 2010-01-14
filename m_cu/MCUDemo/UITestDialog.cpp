// UITestDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcu.h"
#include "UITestDialog.h"
#include "MCUSession.h"
#include "windowfactory.h"
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
    ON_WM_SHOWWINDOW()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUITestDialog ��Ϣ�������

void CUITestDialog::OnBnClickedButtonPlayerDlgTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CPlayerDialog dlg;
	CMCUSession::Instance()->CurVideoSession()->PtzControl( TRUE );
//	dlg.DoModal();

//    CWindowFactory::Instance()->ShowWindow( WndPlayer, WndUnittestUI );
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndPlayer );
	pDlg->DoModal();

}

void CUITestDialog::OnBnClickedButtonLoginDialogTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//     CWindowFactory::Instance()->ShowWindow( WndLogin, this->GetWindowId() );
//     return ;

	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndLogin );
	pDlg->DoModal();

}

void CUITestDialog::OnBnClickedButtonHtmlDialogTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
 //   CWindowFactory::Instance()->ShowWindow( WndWebpage, this->GetWindowId() );
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndWebpage );
	pDlg->DoModal();
}

void CUITestDialog::OnBnClickedButtonConfigDialogTest()
{
 //   CWindowFactory::Instance()->ShowWindow( WndConfig, this->GetWindowId() );
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndConfig );
	pDlg->DoModal();
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

// 	CMainDialog dlg;
// 	dlg.DoModal();
}

void CUITestDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    this->OnOK();
}

BOOL CUITestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUITestDialog::OnBnClickedButtonImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
 //   CWindowFactory::Instance()->ShowWindow( WndPicManage, this->GetWindowId() );
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndPicManage );
	pDlg->DoModal();

}

void CUITestDialog::OnBnClickedButtonControlTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	CControlTestDialog dlg;
// 	dlg.DoModal();
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndUnittestCtrl );
	pDlg->DoModal();
}

void CUITestDialog::OnBnClickedButtonPlayerDlgNoPtz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CPlayerDialog dlg;
	CMCUSession::Instance()->CurVideoSession()->PtzControl( FALSE );
//	dlg.DoModal();

//    CWindowFactory::Instance()->ShowWindow( WndPlayer, this->GetWindowId() );
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndPlayer );
	pDlg->DoModal();
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




void CUITestDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
   
    CUIDialog::OnShowWindow(bShow, nStatus);

    // TODO: �ڴ˴������Ϣ����������
}

void CUITestDialog::OnShowWindowCmd( int nSWCmd )
{
    if ( SW_SHOW == nSWCmd )
    {
        this->FullScreen( FS_HideMenuBar | FS_HideSipButton );
    }
}


void CUITestDialog::OnDestroy()
{
    CUIDialog::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
}
