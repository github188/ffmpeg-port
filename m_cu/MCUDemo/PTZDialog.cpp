// PTZDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "PTZDialog.h"
#include "MCUSession.h"
#include "Config.h"
#include "mcucommon.h"

// CPTZDialog 对话框

IMPLEMENT_DYNAMIC(CPTZDialog, CUIDialog)

CPTZDialog::CPTZDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CPTZDialog::IDD, pParent)
{

}

CPTZDialog::~CPTZDialog()
{
}

void CPTZDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PTZ_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BUTTON_PTZ_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_BUTTON_PTZ_LEFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_BUTTON_PTZ_RIGHT, m_btnMoveRight);
	DDX_Control(pDX, IDC_BUTTON_PTZ_BRIGHT, m_btnLight);
	DDX_Control(pDX, IDC_BUTTON_PTZ_DARK, m_btnDark);
	DDX_Control(pDX, IDC_BUTTON_PTZ_ZOOM_IN, m_btnZoomIn);
	DDX_Control(pDX, IDC_BUTTON_PTZ_ZOOM_OUT, m_btnZoomOut);
	DDX_Control(pDX, IDC_BUTTON_PTZ_NEAR, m_btnNear);
	DDX_Control(pDX, IDC_BUTTON_PTZ_FAR, m_btnFar);
}


BEGIN_MESSAGE_MAP(CPTZDialog, CUIDialog)
//	ON_BN_CLICKED(IDC_BUTTON_PTZ_BRIGHT, &CPTZDialog::OnBnClickedButtonPtzBright)
ON_WM_LBUTTONDOWN()
ON_WM_SIZE()
ON_WM_LBUTTONUP()
ON_MESSAGE( WM_PTZ_BUTTON_DOWN, OnPtzButtonDown )
ON_MESSAGE( WM_PTZ_BUTTON_UP, OnPtzButtonUp )
ON_BN_CLICKED(IDC_BUTTON_PTZ_DOWN, &CPTZDialog::OnBnClickedButtonPtzDown)
//ON_BN_CLICKED(IDC_CHECK_DIGIT_PTZ, &CPTZDialog::OnBnClickedCheckDigitPtz)
END_MESSAGE_MAP()


// CPTZDialog 消息处理程序

//void CPTZDialog::OnBnClickedButtonPtzBright()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

void CPTZDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CUIDialog::OnLButtonDown(nFlags, point);
}

void CPTZDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	this->UpdateLayout( &CRect( 0,0,cx,cy ) );
}

void CPTZDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CUIDialog::OnLButtonUp(nFlags, point);
}


LRESULT CPTZDialog::OnPtzButtonDown( WPARAM btnId, LPARAM )
{
	UpdateData();

	EPTZCmdId ptzOp;
	// 是云台还是镜头.
	BOOL bPtz = TRUE;	
	if( btnId == m_btnMoveLeft.GetDlgCtrlID() )
	{
		ptzOp = PTZ_MOVELEFT;
		bPtz = TRUE;
	}
	else if( btnId == m_btnMoveRight.GetDlgCtrlID() )
	{
		ptzOp = PTZ_MOVERIGHT;
		bPtz = TRUE;
	}
	else if( btnId == m_btnMoveUp.GetDlgCtrlID() )
	{
		ptzOp = PTZ_MOVEUP;
		bPtz = TRUE;
	}
	else if( btnId == m_btnMoveDown.GetDlgCtrlID() )
	{
		ptzOp = PTZ_MOVEDOWN;
		bPtz = TRUE;
	}
	else if( btnId == m_btnLight.GetDlgCtrlID() )
	{
		ptzOp = PTZ_BRIGHTUP;
		bPtz = FALSE;
	}
	else if( btnId == m_btnDark.GetDlgCtrlID() )
	{
		ptzOp = PTZ_BRIGHTDOWN;
		bPtz = FALSE;
	}
	else if( btnId == m_btnZoomIn.GetDlgCtrlID() )
	{
		ptzOp = PTZ_ZOOMTELE;
		bPtz = FALSE;
	}
	else if( btnId == m_btnZoomOut.GetDlgCtrlID() )
	{
		ptzOp = PTZ_ZOOMWIDE;
		bPtz = FALSE;
	}
	else if( btnId == m_btnNear.GetDlgCtrlID() )
	{
		ptzOp = PTZ_FOCUSNEAR;
		bPtz = FALSE;
	}
	else if( btnId == m_btnFar.GetDlgCtrlID() )
	{
		ptzOp = PTZ_FOCUSFAR;
		bPtz = FALSE;
	}
	else
	{
		_ASSERT( FALSE );
		return S_FALSE;
	}

	
	sockaddr_in ptzServerAddr;
	memset( &ptzServerAddr, 0, sizeof( ptzServerAddr ) );


	int nAddrLen = sizeof( ptzServerAddr );
	int nResult = WSAStringToAddress( const_cast<LPWSTR>( CMCUSession::Instance()->CurVideoSession()->PtzIP().c_str() ), 
		AF_INET, NULL, (sockaddr*)&ptzServerAddr, &nAddrLen );
	if( SOCKET_ERROR == nResult )
	{
		int nErrorCode = WSAGetLastError();
		_ASSERT( FALSE );
	}

	//serverAddr.sin_family = AF_INET;
	////	serverAddr.sin_addr.s_addr = inet_addr( strIp );
	//serverAddr.sin_port = htons( _ttoi( strPort ) );
	SetPTZAddr( ntohl( ptzServerAddr.sin_addr.s_addr ), 
		CMCUSession::Instance()->CurVideoSession()->PtzPort() );
    this->SetPtzCmdId( ptzOp );

	int nSpeed;
	if ( bPtz )
	{
		CConfig::Instance()->GetPtzSpeed( nSpeed );
	}
	else
	{
		CConfig::Instance()->GetLenSpeed( nSpeed );
	}

	nSpeed = (nSpeed<3) ? 3:nSpeed;
	SetParam( nSpeed, CMCUSession::Instance()->CurVideoSession()->StreamType() );
	SetPuId( CMCUSession::Instance()->CurVideoSession()->PuId() );
	SetUserId( CMCUSession::Instance()->UserId() );

	SendPTZCmd();

	return S_OK;
}

LRESULT CPTZDialog::OnPtzButtonUp( WPARAM btnId, LPARAM )
{
	// 发送停止命令.
	// 不是数字云台,发送停止命令.
	if ( !this->IsDigitalPtz() )
	{
        this->SetPtzCmdId( PTZ_STOP );
		this->SendPTZCmd();
	}

	return S_OK;
}
void CPTZDialog::OnBnClickedButtonPtzDown()
{
	// TODO: 在此添加控件通知处理程序代码
}

//void CPTZDialog::OnBnClickedCheckDigitPtz()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData();
//	if ( !m_bDigitPtz )
//	{
//		SetDigitPtz( true );
//		SetOpId( PTZ_STOP );
//
//		this->SendPTZCmd();
//	}
//}

BOOL CPTZDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	this->m_btnMoveLeft.SetImage( _T( "\\Storage Card\\picres\\c_o_back.JPG" ) );
	this->SetColor( RGB( 4, 10, 44 ) );
	

	// 为按钮贴图。
	tstring strPtzLeftNormalImg =  _T( "btn_ptz_left_normal.jpg" );
	tstring strPtzLeftFocusImg = _T( "btn_ptz_left_focus.jpg" );
	this->m_btnMoveLeft.SetImage( strPtzLeftNormalImg.c_str(), strPtzLeftFocusImg.c_str(), strPtzLeftFocusImg.c_str(), FALSE );
	m_btnMoveLeft.SetBkTransparent( FALSE, FALSE );
	this->m_btnMoveLeft.FitToPicture();

	tstring strPtzRightNormalImg = _T( "btn_ptz_right_normal.jpg" );
	tstring strPtzRightFocusImg = _T( "btn_ptz_right_focus.jpg" );
	this->m_btnMoveRight.SetImage( strPtzRightNormalImg.c_str(), strPtzRightFocusImg.c_str(), strPtzRightFocusImg.c_str(), FALSE );
	m_btnMoveRight.SetBkTransparent( FALSE, FALSE );
	this->m_btnMoveRight.FitToPicture();

	tstring strPtzUpNormalImg = _T( "btn_ptz_up_normal.jpg" );
	tstring strPtzUpFocusImg =  _T( "btn_ptz_up_focus.jpg" );
	this->m_btnMoveUp.SetImage( strPtzUpNormalImg.c_str(), strPtzUpFocusImg.c_str(), strPtzUpFocusImg.c_str(), FALSE );
	m_btnMoveUp.SetBkTransparent( FALSE, FALSE );
	this->m_btnMoveUp.FitToPicture();

	tstring strPtzDownNormalImg = _T( "btn_ptz_down_normal.jpg" );
	tstring strPtzDownFocusImg = _T( "btn_ptz_down_focus.jpg" );
	this->m_btnMoveDown.SetImage( strPtzDownNormalImg.c_str(), strPtzDownFocusImg.c_str(), strPtzDownFocusImg.c_str(), FALSE );
	m_btnMoveDown.SetBkTransparent( FALSE, FALSE );
	this->m_btnMoveDown.FitToPicture();

	tstring strPtzZoominNormalImg = _T( "btn_ptz_zoomin_normal.jpg" );
	tstring strPtzZoominFocusImg = _T( "btn_ptz_zoomin_focus.jpg" );
	this->m_btnZoomIn.SetImage( strPtzZoominNormalImg.c_str(), strPtzZoominFocusImg.c_str(), strPtzZoominFocusImg.c_str(), FALSE );
	m_btnZoomIn.SetBkTransparent( FALSE, FALSE );
	this->m_btnZoomIn.FitToPicture();

	tstring strPtzZoomoutNormalImg = _T( "btn_ptz_zoomout_normal.jpg" );
	tstring strPtzZoomoutFocusImg = _T( "btn_ptz_zoomout_focus.jpg" );
	this->m_btnZoomOut.SetImage( strPtzZoomoutNormalImg.c_str(), strPtzZoomoutFocusImg.c_str(), strPtzZoomoutFocusImg.c_str(), FALSE );
	m_btnZoomOut.SetBkTransparent( FALSE, FALSE );
	this->m_btnZoomOut.FitToPicture();

	tstring strPtzLightNormalImg = _T( "btn_ptz_light_normal.jpg" );
	tstring strPtzLightFocusImg = _T( "btn_ptz_light_focus.jpg" );
	this->m_btnLight.SetImage( strPtzLightNormalImg.c_str(), strPtzLightFocusImg.c_str(), strPtzLightFocusImg.c_str(), FALSE );
	m_btnLight.SetBkTransparent( FALSE, FALSE );
	this->m_btnLight.FitToPicture();

	tstring strPtzDarkNormalImg = _T( "btn_ptz_dark_normal.jpg" );
	tstring strPtzDarkFocusImg = _T( "btn_ptz_dark_focus.jpg" );
	this->m_btnDark.SetImage( strPtzDarkNormalImg.c_str(), strPtzDarkFocusImg.c_str(), strPtzDarkFocusImg.c_str(), FALSE );
	m_btnDark.SetBkTransparent( FALSE, FALSE );
	this->m_btnDark.FitToPicture();


	// 不用的按钮隐藏。
	this->m_btnFar.ShowWindow( SW_HIDE );
	this->m_btnNear.ShowWindow( SW_HIDE );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPTZDialog::UpdateLayout( LPCRECT prcClient /* = NULL */ )
{
	if ( !GetSafeHwnd() )
	{
		return;
	}

	CRect rcClient;
	if ( prcClient )
	{
		rcClient = *prcClient;
	}
	else
	{
		this->GetClientRect( rcClient );
	}
	
	int nScreenLong = GetScreenLong();

	// 排列按钮。

	// 将需要排列的按钮按顺序放入列表。
	typedef std::vector< CButton * > TBtnList;
	TBtnList listButton;	
	listButton.push_back( &m_btnMoveLeft );
	listButton.push_back( &m_btnMoveRight );
	listButton.push_back( &m_btnMoveUp );
	listButton.push_back( &m_btnMoveDown );
	listButton.push_back( &m_btnZoomIn );
	listButton.push_back( &m_btnZoomOut );
	listButton.push_back( &m_btnLight );
	listButton.push_back( &m_btnDark );

	// 得到按钮的高度。
	const float conFPtzBtnHeight = 0.085;
	int nBtnHeight = int( nScreenLong * conFPtzBtnHeight );

	// 得到按钮们的相对位置。
	std::vector< CRect > listRcPtzBtn;
	CRect rcLastBtnPos( 0, 0, 0, 0 );
	for ( size_t i=0; i<listButton.size(); ++i )
	{
		if ( !listButton[i]->GetSafeHwnd() )
		{
			return;	// 窗口还没有创建，无法处理相对位置。
		}
		else
		{
			CRect rcCurBtnWnd;
			listButton[i]->GetWindowRect( rcCurBtnWnd );

			CRect rcBtnPos;
			rcBtnPos.top = 0;
			rcBtnPos.bottom = nBtnHeight;			
			rcBtnPos.left = rcLastBtnPos.right;
			rcBtnPos.right = rcBtnPos.left + rcCurBtnWnd.Width() * nBtnHeight / rcCurBtnWnd.Height();
			listRcPtzBtn.push_back( rcBtnPos );
			rcLastBtnPos = rcBtnPos;
		}
	}

	_ASSERT( listButton.size() == listRcPtzBtn.size() );

	// 得到绝对位置。
	// 按钮水平，垂直都居中。
	// 所有按钮的相对位置到绝对位置的偏移量都是一样的，根据第一个按钮计算偏移量。
	CRect rcFirstBtnAbsolutely;
	CRect rcFirstBtnRelative = listRcPtzBtn[0];
	CRect rcLastBtnRelative = listRcPtzBtn[ listRcPtzBtn.size() - 1 ];
	int nBtnsWidth = rcLastBtnRelative.right;
	rcFirstBtnAbsolutely.top = ( rcClient.Height() - nBtnHeight ) / 2;
	rcFirstBtnAbsolutely.bottom = rcFirstBtnAbsolutely.top + nBtnHeight;
	rcFirstBtnAbsolutely.left = ( rcClient.Width() - nBtnsWidth ) / 2;
	rcFirstBtnAbsolutely.right = rcFirstBtnAbsolutely.left + rcFirstBtnRelative.Width();

	CPoint offset = rcFirstBtnAbsolutely.TopLeft() - rcFirstBtnRelative.TopLeft();

	// 根据位置移动按钮。
	for ( size_t i=0; i<listButton.size(); ++i )
	{
		CRect rcBtnAbs = listRcPtzBtn[i];
		rcBtnAbs.OffsetRect( offset );

		listButton[i]->MoveWindow( rcBtnAbs );
	}


}

