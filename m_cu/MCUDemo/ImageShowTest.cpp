// ImageShowTest.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "ImageShowTest.h"
#include "ImageShow.h"

// CImageShowTest 对话框

IMPLEMENT_DYNAMIC(CImageShowTest, CUIDialog)

CImageShowTest::CImageShowTest(CWnd* pParent /*=NULL*/)
	: CUIDialog(CImageShowTest::IDD, pParent)
	, m_strImagePath(_T(""))
{
	m_pImgShow = new CImageShow();
}

CImageShowTest::~CImageShowTest()
{
	if ( m_pImgShow )
	{
		delete m_pImgShow;
	}
}

void CImageShowTest::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IMAGE_PATH, m_cmbImagePath);
	DDX_CBString(pDX, IDC_COMBO_IMAGE_PATH, m_strImagePath);
}


BEGIN_MESSAGE_MAP(CImageShowTest, CUIDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CImageShowTest::OnBnClickedButtonOpen)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CImageShowTest 消息处理程序

void CImageShowTest::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( );

	BOOL bResult = m_pImgShow->LoadImage( m_strImagePath );
	if ( bResult )
	{
		this->Invalidate();
	}
}

void CImageShowTest::OnPaint()
{
	if ( m_pImgShow && m_pImgShow->IsOk() )
	{
//        __super::OnPaint();

		CPaintDC dc(this); // device context for painting

        

		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CUIDialog::OnPaint()
		CRect rcClient;
		GetClientRect( rcClient );

        dc.FillSolidRect( rcClient, RGB( 0, 128, 100 ) );

		rcClient.top = 40;

		m_pImgShow->Draw( dc, rcClient/*, CImageShow::IDF_Orientation_90*/ );
	}
	else
	{
		__super::OnPaint();
	}
}

BOOL CImageShowTest::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_cmbImagePath.AddString( _T( "\\1.jpg" ) );
    this->m_cmbImagePath.AddString( _T( "\\mcu-check.png" ) );
    this->m_cmbImagePath.AddString( _T( "\\mcu-check.gif" ) );

	m_cmbImagePath.SetCurSel( 0 );

    this->SetImage( _T( "main_bg.JPG" ), FALSE );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
