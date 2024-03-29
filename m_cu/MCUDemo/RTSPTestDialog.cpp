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
//    CWindowFactory::Instance()->ShowWindow( WndPlayer, this->GetWindowId() );
	CDialog *pDlg = CWindowFactory::Instance()->NewDlg( WndPlayer );

    CPlayerDialog *pPlayer = dynamic_cast< CPlayerDialog* >( pDlg );
    if ( pPlayer )
    {
        BOOL bResult = pPlayer->Play( CMCUSession::Instance()->CurVideoSession() );
        Log() << _T( "Play rtsp: " ) << CMCUSession::Instance()->CurVideoSession()->RtspUrl() 
            << _T( " ret " ) << bResult << endl;
    }
    else
    {
        Log() << _T( "Can't get the Player Window!" ) << endl;
    }

	pPlayer->DoModal();
}

BOOL CRTSPTestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_cmbRtspUrl.AddString( _T( "rtsp://218.107.241.238:5540/service?PuId-ChannelNo=55000000000000000011200002000000-1&PlayMethod=0&StreamingType=1" ) );

	this->m_cmbRtspUrl.AddString( _T( "rtsp://202.111.157.17:1554/PuId-ChannelNo=170100112000441000-1&PlayMethod=0&StreamingType=1&UserId=170100400143001000&PuProperty=1&VauPtzAdd=202.111.157.17&VauPtzPort=5060&VauRtspAdd=202.111.157.17&VauRtspPort=554&PuName=%E4%BA%BA%E6%B0%91%E8%B7%AF%E5%A4%A7%E9%85%92%E5%BA%97-00" ) );

	this->m_cmbRtspUrl.AddString( _T( "rtsp://202.111.157.17:554/PuId-ChannelNo=170100112000441000-1&PlayMethod=0&StreamingType=1&UserId=170100400143001000&PuProperty=1&VauPtzAdd=202.111.157.17&VauPtzPort=5060&VauRtspAdd=202.111.157.17&VauRtspPort=554&PuName=%E4%BA%BA%E6%B0%91%E8%B7%AF%E5%A4%A7%E9%85%92%E5%BA%97-00" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://117.35.58.70:554/service?PuId-ChannelNo=27000000000000000011200014200000-1&PlayMethod=0&StreamingType=1" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://218.61.2.152:554/service?PuId-ChannelNo=11000000000000000011200001100000-1&PlayMethod=0&StreamingType=1" ) );

	this->m_cmbRtspUrl.AddString( _T( "rtsp://10.150.254.248:554/PuId-ChannelNo=55000000000000000011200001200000-1&PlayMethod=0&StreamingType=1" ) );
	this->m_cmbRtspUrl.AddString( _T( "rtsp://172.16.65.67:554/service?PUID-ChannelNo=110000112000014000-1&PlayMethod=0&StreamingType=3" ) );

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

	this->FullScreen( FS_ShowSipButton | FS_ShowTaskBar | FS_ShowMenuBar );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
