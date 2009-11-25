// UnitTestMainDialog.cpp : 实现文件
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
// CUnitTestMainDialog 对话框

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


// CUnitTestMainDialog 消息处理程序

void CUnitTestMainDialog::OnBnClickedButtonDecodeTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CDecodeTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonUdpTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CUDPTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonRtspTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CRTSPTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonUiTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CUITestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonPlayerDialogTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CPlayerDlgTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonHtmlTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CHtmlTestDialog dlg;
	dlg.DoModal();
}

void CUnitTestMainDialog::OnBnClickedButtonImageTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CImageShowTest dlg;
	dlg.DoModal();
}
