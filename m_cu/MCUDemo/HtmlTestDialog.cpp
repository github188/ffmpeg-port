// HtmlTestDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcu.h"
#include "HtmlTestDialog.h"


// CHtmlTestDialog �Ի���

IMPLEMENT_DYNAMIC(CHtmlTestDialog, CUIDialog)

CHtmlTestDialog::CHtmlTestDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CHtmlTestDialog::IDD, pParent)
{

}

CHtmlTestDialog::~CHtmlTestDialog()
{
}

void CHtmlTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_URL, m_cmbUrlList);
}


BEGIN_MESSAGE_MAP(CHtmlTestDialog, CUIDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_URL, &CHtmlTestDialog::OnBnClickedButtonOpenUrl)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CHtmlTestDialog::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CHtmlTestDialog ��Ϣ�������

void CHtmlTestDialog::OnBnClickedButtonOpenUrl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strUrl;
    this->m_cmbUrlList.GetWindowText( strUrl );


    CWindowFactory::Instance()->ShowWindow( WndWebpage, this->GetWindowId() );
    CWebpageDlg *pWebDlg = dynamic_cast< CWebpageDlg * >( CWindowFactory::Instance()->GetWnd( WndWebpage ) );
    if ( pWebDlg )
    {
        pWebDlg->OpenUrl( strUrl );
    }



    //if( ! m_webpageDlg.GetSafeHwnd() )
    //{
    //    m_webpageDlg.Create( CWebpageDlg::IDD, this );
    //}    
    //m_webpageDlg.ShowWindow( SW_SHOW );

    //m_webpageDlg.OpenUrl( strUrl );

}

BOOL CHtmlTestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
//	m_cHtmlWnd.Create( CHtmlWnd::IDD, this );
//	m_cHtmlWnd.ShowWindow( SW_SHOW );

    this->m_cmbUrlList.AddString( _T( "http://172.16.161.125/msp/index.php" ) );
    this->m_cmbUrlList.AddString( _T( "file://\\Program Files\\M_CU\\htmldoc\\mcufail.htm" ) );
    this->m_cmbUrlList.AddString( _T( "file://\\Program%20Files\\M_CU\\htmldoc\\mcufail.htm" ) );
	this->m_cmbUrlList.AddString( _T( "http://www.google.com" ) );
	this->m_cmbUrlList.AddString( _T( "http://www.taobao.com" ) );
	this->m_cmbUrlList.AddString( _T( "http://218.242.128.205/msp/login.html" ) );
	this->m_cmbUrlList.AddString( _T( "http://218.242.128.205:8081/login.aspx" ) );
	this->m_cmbUrlList.AddString( _T( "http://verycd.com" ) );
	this->m_cmbUrlList.AddString( _T( "file://\\Storage%20Card\\htmlfail\\fail.htm" ) );

	this->m_cmbUrlList.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHtmlTestDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	//if ( m_cHtmlWnd.GetSafeHwnd() )
	//{
	//	CRect rcCmb;
	//	m_cmbUrlList.GetWindowRect( rcCmb );

	//	CRect rcHtml( 0, rcCmb.Height() + 3, cx, cy );
	//	m_cHtmlWnd.MoveWindow( rcHtml );
	//}

	// TODO: �ڴ˴������Ϣ����������
}

void CHtmlTestDialog::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	::SendMessage( m_hHtmlCtrlWnd, DTM_ADDTEXT, TRUE, (LPARAM)"Fail!!" );
//	m_cHtmlWnd.m_hHtmlCtrlWnd
}
