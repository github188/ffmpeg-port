// src\logodialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcu.h"
#include "logodialog.h"
#include "mcucommon.h"

// CLogoDialog �Ի���

IMPLEMENT_DYNAMIC(CLogoDialog, CBaseLogoDialog)

CLogoDialog::CLogoDialog(CWnd* pParent /*=NULL*/)
	: CBaseLogoDialog(CLogoDialog::IDD, pParent)
{

}

CLogoDialog::~CLogoDialog()
{
}

void CLogoDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseLogoDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_picLogo);
}


BEGIN_MESSAGE_MAP(CLogoDialog, CBaseLogoDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLogoDialog ��Ϣ�������

void CLogoDialog::OnSize(UINT nType, int cx, int cy)
{
	CBaseLogoDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rcClient( 0,0, cx, cy );
	this->UpdateLayout( &rcClient );
}

BOOL CLogoDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	tstring strLogo = _T( "titlelogo.jpg" );
	this->m_picLogo.SetPicPath( strLogo.c_str() );
	this->m_picLogo.SetBkTransparent( FALSE, FALSE );
	this->m_picLogo.FitToPicture();

	tstring strLogoBg = _T( "titlebg.jpg" );
	this->SetImage( strLogoBg.c_str() );

	this->UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLogoDialog::UpdateLayout( CRect *prcClient /* = NULL */ )
{
	CRect rcClient;
	if ( GetSafeHwnd() )
	{
		if ( prcClient )
		{
			rcClient = *prcClient;
		}
		else
		{
			this->GetClientRect( rcClient );
		}
	}
	else
	{
		return;
	}

	// logo,���쵽�Ի���߶ȣ����ָ߿�Ȳ��䡣
	if ( m_picLogo.GetSafeHwnd() )
	{
		CRect rcImg;
		BOOL bGetSize = m_picLogo.GetImageSize( rcImg );

		CRect rcLogoWnd;
		m_picLogo.GetWindowRect( rcLogoWnd );
		
		// ����ͼƬ�ı�������logo�ؼ���
		if ( bGetSize && rcLogoWnd.Height() != rcClient.Height() )
		{
			CRect rcNewLogoPos(0,0,0,0);
			rcNewLogoPos.bottom = rcClient.Height();
			rcNewLogoPos.right = ( rcImg.Width() * rcClient.Height() )/rcImg.Height() + 1;
			m_picLogo.MoveWindow( rcNewLogoPos );
		}
	}
}

int CLogoDialog::GetHeight() const
{
	const float conFLogoHeight = 0.095;
	
	// ��Ļ��С��
	int nScreenLong = GetScreenLong();

	return int( nScreenLong * conFLogoHeight );

}