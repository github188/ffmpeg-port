// UnitTestMainDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcu.h"
#include "UnitTestMainDialog.h"
#include "DecodeTestDialog.h"
#include "UDPTestDialog.h"
#include "RTSPTestDialog.h"
#include "UITestDialog.h"
#include "PlayerDlgTestDialog.h"
#include "HtmlTestDialog.h"
#include "ImageShowTest.h"
// CUnitTestMainDialog �Ի���

IMPLEMENT_DYNAMIC(CUnitTestMainDialog, CDialog)

CUnitTestMainDialog::CUnitTestMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUnitTestMainDialog::IDD, pParent)
{

}

CUnitTestMainDialog::~CUnitTestMainDialog()
{
}

void CUnitTestMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnitTestMainDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DECODE_TEST, &CUnitTestMainDialog::OnBnClickedButtonDecodeTest)
	ON_BN_CLICKED(IDC_BUTTON_UDP_TEST, &CUnitTestMainDialog::OnBnClickedButtonUdpTest)
	ON_BN_CLICKED(IDC_BUTTON_RTSP_TEST, &CUnitTestMainDialog::OnBnClickedButtonRtspTest)
	ON_BN_CLICKED(IDC_BUTTON_UI_TEST, &CUnitTestMainDialog::OnBnClickedButtonUiTest)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER_DIALOG_TEST, &CUnitTestMainDialog::OnBnClickedButtonPlayerDialogTest)
	ON_BN_CLICKED(IDC_BUTTON_HTML_TEST, &CUnitTestMainDialog::OnBnClickedButtonHtmlTest)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_TEST, &CUnitTestMainDialog::OnBnClickedButtonImageTest)
END_MESSAGE_MAP()


// CUnitTestMainDialog ��Ϣ�������

void CUnitTestMainDialog::OnBnClickedButtonDecodeTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDecodeTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonUdpTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUDPTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonRtspTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRTSPTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonUiTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUITestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonPlayerDialogTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPlayerDlgTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonHtmlTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHtmlTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonImageTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CImageShowTest dlg;
	dlg.DoModal();
}
