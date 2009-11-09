// ImageShowTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "ImageShowTest.h"
#include "ImageShow.h"

// CImageShowTest �Ի���

IMPLEMENT_DYNAMIC(CImageShowTest, CDialog)

CImageShowTest::CImageShowTest(CWnd* pParent /*=NULL*/)
	: CDialog(CImageShowTest::IDD, pParent)
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
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IMAGE_PATH, m_cmbImagePath);
	DDX_CBString(pDX, IDC_COMBO_IMAGE_PATH, m_strImagePath);
}


BEGIN_MESSAGE_MAP(CImageShowTest, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CImageShowTest::OnBnClickedButtonOpen)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CImageShowTest ��Ϣ�������

void CImageShowTest::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		CPaintDC dc(this); // device context for painting
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
		CRect rcClient;
		GetClientRect( rcClient );
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
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->m_cmbImagePath.AddString( _T( "\\Storage Card\\1.jpg" ) );

	m_cmbImagePath.SetCurSel( 0 );

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
