// src\controltestdialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "controltestdialog.h"
#include "mcucommon.h"
#include "pictruestatic.h"
#include "uibutton.h"
#include "checkbutton.h"
#include "uisliderctrl.h"

// CControlTestDialog 对话框

IMPLEMENT_DYNAMIC(CControlTestDialog, CUIDialog)

CControlTestDialog::CControlTestDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CControlTestDialog::IDD, pParent)
{
	this->m_pTestWnd = NULL;
}

CControlTestDialog::~CControlTestDialog()
{
}

void CControlTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlTestDialog, CUIDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_TEST_TRANSPARENT_MASK_IMAGE, &CControlTestDialog::OnBnClickedButtonTestTransparentMaskImage)
	ON_BN_CLICKED(IDC_BUTTON_TEST_STATIC_TEXT, &CControlTestDialog::OnBnClickedButtonTestStaticText)
	ON_BN_CLICKED(IDC_BUTTON_TEST_IMAGE_BUTTON, &CControlTestDialog::OnBnClickedButtonTestImageButton)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CHECK_BUTTON, &CControlTestDialog::OnBnClickedButtonTestCheckButton)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CHECK_PUSH_BUTTON, &CControlTestDialog::OnBnClickedButtonTestCheckPushButton)
	ON_BN_CLICKED(IDC_BUTTON_TEST_DISABLE_WND, &CControlTestDialog::OnBnClickedButtonTestDisableWnd)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SLIDER, &CControlTestDialog::OnBnClickedButtonTestSlider)
END_MESSAGE_MAP()


// CControlTestDialog 消息处理程序

BOOL CControlTestDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	this->SetColor( RGB( 33,65,181 ) );
	
	tstring strAppDir = ParsePath( GetModulePath().c_str() ).m_strDirectory;

	tstring  strLogoPath= strAppDir + _T( "res\\chs\\main_bg.JPG" );
	this->SetImage( strLogoPath.c_str() );

	OnBnClickedButtonTestTransparentMaskImage();
	//this->m_stGreenTextTransLeft.SetTextColor( RGB( 128, 10, 10 ), RGB( 128, 128, 128 ), TRUE );

	//CFont font;
	//BOOL bSuccess = font.CreatePointFont( 12, _T( "宋体" ) );
	//_ASSERT( bSuccess );
	//this->m_stGreenTextTransLeft.SetFont( &font );

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CControlTestDialog::OnPaint()
{
	CUIDialog::OnPaint();
	// CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CImageDialog::OnPaint()
}

void CControlTestDialog::OnBnClickedButtonTestTransparentMaskImage()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ResetTest();

	CPictureStatic *pPSt = new CPictureStatic();
	CRect rcClient;
	this->GetClientRect( rcClient );
	const CRect rcSt( 20,40, rcClient.Width()-20, 80 );
	pPSt->Create( _T( "" ), SS_LEFT | WS_CHILD | WS_VISIBLE, rcSt, this );

	tstring strAppDir = ParsePath( GetModulePath().c_str() ).m_strDirectory;

	tstring  strLogoPath= strAppDir + _T( "res\\chs\\btn_login_normal.jpg" );


	pPSt->SetPicPath( strLogoPath.c_str() );

	pPSt->SetTransparentMask( TRUE, RGB( 255,0,255) );

	m_pTestWnd = pPSt;

	
}

void CControlTestDialog::OnBnClickedButtonTestStaticText()
{
	this->ResetTest();

	CStaticEx *pSt = new CStaticEx();

	CRect rcClient;
	this->GetClientRect( rcClient );
	const CRect rcSt( 0,0, rcClient.Width(), 50 );
	BOOL bResult = pSt->Create( _T( "红色文字宋体12px透明背景左对齐" ), SS_LEFT | WS_VISIBLE | WS_CHILD, rcSt, this );
	_ASSERT( bResult );

	pSt->ShowWindow( SW_SHOW );
	
	m_pTestWnd = pSt;



	pSt->SetTextColor( RGB( 128, 10, 10 ), RGB( 128, 128, 128 ), TRUE );

	CFont font;
	BOOL bSuccess = font.CreatePointFont( 12, _T( "宋体" ) );
	_ASSERT( bSuccess );
	pSt->SetFont( &font );

	pSt->UpdateBk();

	
	// TODO: 在此添加控件通知处理程序代码
}

void CControlTestDialog::OnBnClickedButtonTestImageButton()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ResetTest();

	CUIButton *pBtn = new CUIButton();

	m_pTestWnd = pBtn;
	
	CRect rcClient;
	this->GetClientRect( rcClient );
	const CRect rcSt( 20,80, rcClient.Width()-20, 120 );

	pBtn->Create( _T( "" ), WS_CHILD | WS_VISIBLE, rcSt, this, 1000 );

	tstring strAppDir = ParsePath( GetModulePath().c_str() ).m_strDirectory;

	tstring  strLogoPath= strAppDir + _T( "res\\chs\\btn_login_normal.jpg" );
	tstring strDisPath = strAppDir + _T( "res\\chs\\btn_login_direct.jpg" );
	pBtn->SetImage( strLogoPath.c_str(), strLogoPath.c_str(), strDisPath.c_str() );

	pBtn->SetTransparentMask( TRUE, RGB( 255,0,255) );

	pBtn->ShowWindow( SW_SHOW );

}

void CControlTestDialog::OnBnClickedButtonTestCheckButton()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ResetTest();

	CCheckButton *pBtn = new CCheckButton();

	m_pTestWnd = pBtn;

	CRect rcClient;
	this->GetClientRect( rcClient );
	const CRect rcSt( 20,80, rcClient.Width()-20, 120 );

	pBtn->Create( _T( "复选按钮测试" ), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rcSt, this, 1000 );

	pBtn->ShowWindow( SW_SHOW );

	pBtn->SetColor( RGB( 120, 10, 10 ), RGB( 100, 100, 10 ) );

}

void CControlTestDialog::OnBnClickedButtonTestCheckPushButton()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ResetTest();

	CCheckButton *pBtn = new CCheckButton();

	m_pTestWnd = pBtn;

	CRect rcClient;
	this->GetClientRect( rcClient );
	const CRect rcSt( 20,120, rcClient.Width()-20, 160 );

	pBtn->Create( _T( "复选按钮测试" ), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE , rcSt, this, 1000 );

	//tstring strAppDir = ParsePath( GetModulePath().c_str() ).m_strDirectory;

	//tstring  strLogoPath= strAppDir + _T( "res\\chs\\btn_login.jpg" );
	//pBtn->SetImage( strLogoPath.c_str(), strLogoPath.c_str() );

	//pBtn->SetTransparentMask( TRUE, RGB( 255,0,255) );

	pBtn->ShowWindow( SW_SHOW );
}

void CControlTestDialog::OnBnClickedButtonTestDisableWnd()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_pTestWnd && m_pTestWnd->GetSafeHwnd() )
	{
		BOOL bEnable = m_pTestWnd->IsWindowEnabled();
		m_pTestWnd->EnableWindow( !bEnable );
	}
}

void CControlTestDialog::OnBnClickedButtonTestSlider()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ResetTest();

	CUISliderCtrl *pSlider = new CUISliderCtrl();
	this->m_pTestWnd = pSlider;

	CRect rcClient;
	this->GetClientRect( rcClient );
	const CRect rcSt( 20,120, rcClient.Width()-20, 160 );
	pSlider->Create( WS_CHILD, rcSt, this, 1000 );
	pSlider->ShowWindow( SW_SHOW );

}

void CControlTestDialog::ResetTest()
{
	if ( m_pTestWnd->GetSafeHwnd() )
	{
		m_pTestWnd->DestroyWindow();
	}

	if( m_pTestWnd )
	{		
		delete m_pTestWnd;
		m_pTestWnd = NULL;
	}
}