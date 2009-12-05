// RTSPTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "RTSPTestDialog.h"

#include "PlayerDialog.h"
#include "MCUSession.h"

// CRTSPTestDialog 对话框

IMPLEMENT_DYNAMIC(CRTSPTestDialog, CUIDialog)

CRTSPTestDialog::CRTSPTestDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CRTSPTestDialog::IDD, pParent)
	, m_strRtspUrl(_T(""))
{

}

CRTSPTestDialog::~CRTSPTestDialog()
{
}

void CRTSPTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RTSP_URL, m_cmbRtspUrl);
	DDX_CBString(pDX, IDC_COMBO_RTSP_URL, m_strRtspUrl);
}


BEGIN_MESSAGE_MAP(CRTSPTestDialog, CUIDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPN, &CRTSPTestDialog::OnBnClickedButtonOpn)
END_MESSAGE_MAP()


// CRTSPTestDialog 消息处理程序

void CRTSPTestDialog::OnBnClickedButtonOpn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	int nSel = this->m_cmbRtspUrl.GetCurSel();
	tstringstream ssFakePuName ;
	ssFakePuName << _T( "Rtsp测试：" ) << nSel;
	
	CMCUSession::Instance()->CurVideoSession()->PuName( ssFakePuName.str().c_str() );

//	CPlayerDialog playerDlg;
//	playerDlg.SetRtspUrl( (LPCTSTR)m_strRtspUrl );
//	playerDlg.DoModal();
    CMCUSession::Instance()->CurVideoSession()->RtspUrl( (LPCTSTR)m_strRtspUrl );
    CWindowFactory::Instance()->ShowWindow( WndPlayer, this->GetWindowId() );
    CPlayerDialog *pPlayer = dynamic_cast< CPlayerDialog* >( CWindowFactory::Instance()->GetWnd( WndPlayer ) );
    if ( pPlayer )
    {
        BOOL bResult = pPlayer->Play( CMCUSession::Instance()->CurVideoSession() );
        mcu::log << _T( "Play rtsp: " ) << CMCUSession::Instance()->CurVideoSession()->RtspUrl() 
            << _T( " ret " ) << bResult << endl;
    }
    else
    {
        mcu::log << _T( "Can't get the Player Window!" ) << endl;
    }
}

BOOL CRTSPTestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_cmbRtspUrl.AddString( _T( "rtsp://221.224.163.200:554/service?PuId-ChannelNo=112200112000001000-1&PlayMethod=0&StreamingType=1" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://221.224.163.200:554/service?PuId-ChannelNo=112200112000001000-1&PlayMethod=0&StreamingType=2" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://218.4.130.248/service?PuId-ChannelNo=11040100000000000011100004400000-1&PlayMethod=0" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://invalidUrl" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://172.16.161.125:554/service?PuId-ChannelNo=110300112000001000-1&PlayMethod=0&StreamingType=2&UserID=110300400001001000" ) );

	this->m_cmbRtspUrl.AddString( _T( "rtsp://218.242.128.205:554/service?PuId-ChannelNo=110300112000001000-1&PlayMethod=0&StreamingType=1&UserID=110300400001001000" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://218.242.128.205:554/service?PuId-ChannelNo=110300112000001000-1&PlayMethod=0&StreamingType=2&UserID=110300400001001000" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://218.242.128.205:554/service?PuId-ChannelNo=110300112000001000-1&PlayMethod=0&StreamingType=3&UserID=110300400001001000" ) );

	this->m_cmbRtspUrl.AddString( _T( "rtsp://172.16.161.125:554/service?PuId-ChannelNo=110300112000001000-1&PlayMethod=0&StreamingType=1&UserID=110300400001001000" ) );
	m_cmbRtspUrl.SetCurSel( 0 );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
