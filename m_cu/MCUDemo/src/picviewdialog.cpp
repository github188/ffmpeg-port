// src\picviewdialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "picviewdialog.h"
#include "mcucommon.h"


// CPicViewDialog �Ի���

IMPLEMENT_DYNAMIC(CPicViewDialog, CBasePicViewDialog)

CPicViewDialog::CPicViewDialog(CWnd* pParent /*=NULL*/)
	: CBasePicViewDialog(CPicViewDialog::IDD, pParent)
{
	m_fZoomRadio = 0.8;
	m_bFullScreen = FALSE;
//	m_hMenuBarWnd = NULL;
}

CPicViewDialog::~CPicViewDialog()
{
}

void CPicViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CBasePicViewDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC_INFO, m_stPicInfo);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_btnZoomIn);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_btnZoomOut);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_RETURN, m_btnReturn);
	DDX_Control(pDX, IDC_STATIC_BOTTOM_BG, m_stBottomBg);
}


BEGIN_MESSAGE_MAP(CPicViewDialog, CBasePicViewDialog)
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_PIC_VIEW_RETURN, &CPicViewDialog::OnMenuPicViewReturn)
	ON_COMMAND(ID_MENU_DELETE_PIC_FILE, &CPicViewDialog::OnMenuDeletePicFile)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CPicViewDialog::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CPicViewDialog::OnBnClickedButtonZoomOut)
	ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_BUTTON_DELETE, &CPicViewDialog::OnBnClickedButtonDelete)
ON_BN_CLICKED(IDC_BUTTON_RETURN, &CPicViewDialog::OnBnClickedButtonReturn)
END_MESSAGE_MAP()


// CPicViewDialog ��Ϣ�������

BOOL CPicViewDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ɫ������
	this->SetColor( RGB( 0,0,0 ) );

	// ����logo�Ի���
	this->m_logoDlg.Create( CLogoDialog::IDD, this );
	this->m_logoDlg.ShowWindow( SW_SHOW );

	// ����ͼƬ�ؼ���
	CRect rcClient;
	this->GetClientRect( rcClient );

	this->m_stPic.Create( _T(""), WS_CHILD, rcClient, this );
	this->m_stPic.FitToPicture();
	this->m_stPic.ShowWindow( SW_SHOW );

	// ��ť��ͼ��
	// ��ť��ͼ��
	tstring strBtnZoomInNormal = _T( "btn_zoom_in_normal.jpg" );
	tstring strBtnZoomInFocus = _T( "btn_zoom_in_focus.jpg" );
	this->m_btnZoomIn.SetImage( strBtnZoomInNormal.c_str(), strBtnZoomInFocus.c_str(), strBtnZoomInFocus.c_str(), FALSE );
	this->m_btnZoomIn.SetBkTransparent( FALSE, FALSE );
	this->m_btnZoomIn.FitToPicture();

	tstring strBtnZoomOutNormal = _T( "btn_zoom_out_normal.jpg" );
	tstring strBtnZoomOutFocus = _T( "btn_zoom_out_focus.jpg" );
	this->m_btnZoomOut.SetImage( strBtnZoomOutNormal.c_str(), strBtnZoomOutFocus.c_str(), strBtnZoomOutFocus.c_str(), FALSE );
	this->m_btnZoomOut.SetBkTransparent( FALSE, FALSE );
	this->m_btnZoomOut.FitToPicture();

	// �ļ�����
	this->m_stPicInfo.SetBkColor( RGB( 0,0,0 ) );
	this->m_stPicInfo.SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );
	this->m_stPicInfo.SetWindowText(  ParsePath( this->m_stPic.GetPicPath().c_str() ).m_strBaseName.c_str() );
	this->m_stPicInfo.SetBkTransparent( FALSE, FALSE );

	// �ײ��˵�������ť��
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnDelete.SetBkTransparent( FALSE, FALSE );
	this->m_btnDelete.SetImage( _T( "btn_delete_normal.jpg" ), _T( "btn_delete_focus.jpg" ), _T( "btn_delete_disable.jpg" ), FALSE );

	this->m_btnReturn.SetBkTransparent( FALSE, FALSE );
	this->m_btnReturn.SetImage( _T( "btn_return_normal.jpg" ), _T( "btn_return_focus.jpg" ), _T( "btn_return_normal.jpg" ), FALSE );

	

	this->UpdateLayout();


	//// �����˵���
	//SHMENUBARINFO shmbi;
	//// Create our softkey bar
	//// ���������󴴽����ܾ���ʾ��������.
	//ZeroMemory(&shmbi, sizeof(shmbi));
	//shmbi.cbSize = sizeof(shmbi);
	//shmbi.hwndParent = GetSafeHwnd();
	//shmbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;

	//shmbi.nToolBarId = IDR_MENU_PIC_VIEW;
	//shmbi.hInstRes = AfxGetInstanceHandle();
	//if (!SHCreateMenuBar(&shmbi))
	//{
	//	// Failed!!
	//	MessageBox( TEXT( "�����˵�ʧ�ܣ���" ) );
	//	return FALSE;
	//}

//	this->m_hMenuBarWnd = shmbi.hwndMB;

	// ��������Ĳ˵������������뷨��ť.
	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPicViewDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	this->UpdateLayout( &CRect( 0,0, cx, cy ) );
}

void CPicViewDialog::SetPic( LPCTSTR strPicPath )
{
	this->m_stPic.SetPicPath( strPicPath );
}

void CPicViewDialog::UpdateLayout( LPCRECT prcClient /* = NULL */ )
{
	if( !this->GetSafeHwnd() )
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

	// ʣ��Ŀռ䡣
	CRect rcLeftSpace = rcClient;

	// �Ƿ���ʾ��ͼƬ��������ؼ���
	BOOL bShowCtrls = !m_bFullScreen;

	// logo.
	if ( m_logoDlg.GetSafeHwnd() && bShowCtrls )
	{
		int nLogoHeight = m_logoDlg.GetHeight();
		CRect rcLogoDlg = rcClient;
		rcLogoDlg.bottom = nLogoHeight;
		this->m_logoDlg.MoveWindow( rcLogoDlg );

		// ��ʣ��ռ���ȥ��logoռ�õĿռ䡣
		rcLeftSpace.top += nLogoHeight;
	}

	// ��Ļ���ȣ���͸��и������Ǹ�����Ϊ�ؼ������Ļ�����
	int nScreenLong = GetScreenLong();

	int nScreenWidth = ::GetScreenWidth();

	// �����˵�����λ�á�
	if ( m_stBottomBg.GetSafeHwnd() && m_btnDelete.GetSafeHwnd() && m_btnReturn.GetSafeHwnd() && bShowCtrls )
	{
		// �ײ���ť���������ǰ�Ĳ˵����ĸ߶�	
		const float conFMenubarHeight = 0.082;
		int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

		CRect rcBottomBg = rcClient;
		rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
		m_stBottomBg.MoveWindow( rcBottomBg );

		CRect rcBtnDelete;
		BOOL bResult = this->m_btnDelete.GetImageSize( rcBtnDelete );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnDelete.Width() / rcBtnDelete.Height();
			rcBtnDelete = rcBottomBg;
			rcBtnDelete.right = rcBtnDelete.left + nNewWidth;
			this->m_btnDelete.SetWindowPos( &CWnd::wndTop, rcBtnDelete.left, rcBtnDelete.top, rcBtnDelete.Width(), rcBtnDelete.Height(), NULL );
		}
		
		CRect rcBtnReturn;
		bResult = this->m_btnReturn.GetImageSize( rcBtnReturn );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnReturn.Width() / rcBtnReturn.Height();
			rcBtnReturn = rcBottomBg;
			rcBtnReturn.left = rcBtnReturn.right - nNewWidth;
			this->m_btnReturn.SetWindowPos( &CWnd::wndTop, rcBtnReturn.left, rcBtnReturn.top, rcBtnReturn.Width(), rcBtnReturn.Height(), NULL );
		}

		

		// ��ʣ��ռ���ȥ���˵���ռ�õĿռ䡣
		rcLeftSpace.bottom -= nMenubarHeight;
	}

	// ͼƬ�ؼ���

	// ������ť��
	// �Ŵ�ť��ͼƬ��Ϣ����С��ť�������ؼ���������ͻ������±���ʾ��
	// ȫ��������Ļ��С�����ȣ���ť�̶�ͼƬ�ĳ���������������仯��
	// Ӧ���������Ѿ�����ť�ؼ���С����ΪͼƬ�ĳ���ȡ�

	// �Ի����²����ư�ť�߶ȡ�
	const float conFControlHeight = 0.097;
	const float conFControlWidth = 0.116;
	const float conFPicInfoStWidth = 0.625;

	

	int nNewHeight = int( nScreenLong * conFControlHeight );
	int nNewBtnWidth = int( nScreenLong * conFControlWidth );
	

	CRect rcZoomIn, rcZoomOut, rcPicInfo;

	int nNewPicInfoWidth = int( nScreenWidth * conFPicInfoStWidth );

	int nNewTop = rcLeftSpace.bottom - nNewHeight;
	int nNewBottom = rcLeftSpace.bottom;

	CRect rcNewPicInfo( 0, nNewTop, nNewPicInfoWidth, nNewBottom );
	rcNewPicInfo.OffsetRect( ( nScreenWidth - nNewPicInfoWidth ) / 2, 0 );

	CRect rcNewZoomIn( 0, nNewTop, nNewBtnWidth, nNewBottom );
	rcNewZoomIn.OffsetRect( rcNewPicInfo.left - rcNewZoomIn.right, 0 );

	CRect rcNewZoomOut( 0, nNewTop, nNewBtnWidth, nNewBottom );
	rcNewZoomOut.OffsetRect( rcNewPicInfo.right - rcNewZoomOut.left, 0 );

	BOOL bStPicInfoShow = m_stPicInfo.GetSafeHwnd() && bShowCtrls;
	BOOL bBtnZoomInShow = m_btnZoomIn.GetSafeHwnd() && bShowCtrls;
	BOOL bBtnZoomOutShow = m_btnZoomOut.GetSafeHwnd() && bShowCtrls;
	if ( bStPicInfoShow )
	{
		m_stPicInfo.MoveWindow( rcNewPicInfo );
	}
	if ( bBtnZoomInShow )
	{
		m_btnZoomIn.MoveWindow( rcNewZoomIn );
	}
	if( bBtnZoomOutShow )
	{
		m_btnZoomOut.MoveWindow( rcNewZoomOut );
	}

	// ��ʣ��ռ���ȥ����Ϣ��ʾ��
	if ( bStPicInfoShow || bBtnZoomInShow || bBtnZoomOutShow )
	{
		rcLeftSpace.bottom -= nNewHeight;
	}

	if ( m_stPic.GetSafeHwnd() )
	{
		CRect rcPicOriginal;
		m_stPic.GetImageSize( rcPicOriginal );

		CRect rcPicOld = rcPicOriginal;

		CRect rcMaxPicSpace;
		int nPicSpaceHeight = 0;
		int nPicSpaceWidth = 0;

		if ( m_bFullScreen )
		{
			nPicSpaceWidth = rcClient.Width();
			nPicSpaceHeight = rcClient.Height();

			rcMaxPicSpace = rcClient;
		}
		else
		{
			//rcMaxPicSpace = rcClient;
			//rcMaxPicSpace.top = m_logoDlg.GetHeight();
			//rcMaxPicSpace.bottom -= nNewHeight;
			rcMaxPicSpace = rcLeftSpace;

			nPicSpaceHeight = int( rcMaxPicSpace.Height() * m_fZoomRadio );
			nPicSpaceWidth = int( rcMaxPicSpace.Width() * m_fZoomRadio );			
		}

		// ����ͼƬ�����������ͼƬ���롣			
		CRect rcNewPic( 0,0,0,0 );
		if ( rcPicOld.Width() * nPicSpaceHeight > rcPicOld.Height() * nPicSpaceWidth )
		{
			// ͼƬ�Ŀ��ߴ���ͼƬ��ʾ�ռ�Ŀ�߱ȡ�ͼƬ�ĺ���ռ������������
			rcNewPic.right = nPicSpaceWidth;
			rcNewPic.bottom = rcPicOld.Height() * nPicSpaceWidth /  rcPicOld.Width();  
		}
		else
		{
			// ��֮�෴��
			rcNewPic.right = rcPicOld.Width() * nPicSpaceHeight / rcPicOld.Height();
			rcNewPic.bottom = nPicSpaceHeight;
		}

		// ��ͼƬrect����ͼƬ�ռ����ġ�
		CPoint offset = rcMaxPicSpace.CenterPoint() - rcNewPic.CenterPoint();
		rcNewPic.OffsetRect( offset );

		this->m_stPic.MoveWindow( rcNewPic );
	}
	
}
void CPicViewDialog::OnMenuPicViewReturn()
{
	this->OnReturn();
		
	// TODO: �ڴ���������������
}

void CPicViewDialog::OnMenuDeletePicFile()
{
	this->OnDeletePic();
	
	// TODO: �ڴ���������������
}



void CPicViewDialog::OnBnClickedButtonZoomIn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ע�⸡�����ıȽϣ�
	if ( 1.0 - m_fZoomRadio > 0.01 )
	{
		m_fZoomRadio += 0.2;
	}
	else
	{
		this->SetFullScreen( TRUE );
	}
	this->UpdateLayout();
}

void CPicViewDialog::OnBnClickedButtonZoomOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ע�⸡�����ıȽϣ�
	if ( m_fZoomRadio - 0.8 > 0.01  )
	{
		m_fZoomRadio -= 0.2;
	}
	this->UpdateLayout();
}

void CPicViewDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if ( m_bFullScreen )
	{
		this->SetFullScreen( FALSE );
	}

	CUIDialog::OnLButtonDown(nFlags, point);
}

void CPicViewDialog::SetFullScreen( BOOL bFullScreen )
{
	m_bFullScreen = bFullScreen;



	if ( m_bFullScreen )
	{
		// ȫ��.
		DWORD dwFSFlag = FS_FullScreen;

		// ��ȷ���ֻ��Ƿ���Ҫ��ת.
		// ���ǱȽ�ͼƬ�ĳ�����ֻ���Ļ�ĳ���.�������һ��,����Ҫ��ת,������ת90�Ⱥ�Ч������.
		// �㷨: ��Ļ��߲���ͼƬ�Ŀ�߲�ͬ��(ͬΪ����),����Ҫ��ת.�����ֻ���Ҫ��ת.
		int nScreenWidth = ::GetScreenWidth();
		int nScreenHeight = ::GetScreenHeight();
		CRect rcImg;
		this->m_stPic.GetImageSize( rcImg );
		if( ( nScreenWidth - nScreenHeight ) * ( rcImg.Width() - rcImg.Height() ) < 0 )
		{
			// ��Ҫ��ת.
			dwFSFlag |= FS_HorizontalScreen;
		}

		this->FullScreen( dwFSFlag );
	}
	else
	{
		// �ָ�.
		DWORD dwFSFlag = FS_ShowTaskBar | FS_ResumeScreen;

		this->FullScreen( dwFSFlag );
	}

	// ����/��ʾ�ؼ�.
	this->m_btnZoomIn.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnZoomOut.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_stPicInfo.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
//	::ShowWindow( this->m_hMenuBarWnd, bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_logoDlg.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );

	this->m_stBottomBg.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnDelete.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );
	this->m_btnReturn.ShowWindow( bFullScreen ? SW_HIDE : SW_SHOW );


	this->UpdateLayout();
}

void CPicViewDialog::OnDeletePic()
{
	// ɾ��ͼƬ��
	if( IDYES == MessageBox( _T( "ȷ��ɾ��ͼƬ��" ), _T( "ɾ��ͼƬ" ), MB_YESNO ) )
	{
		this->EndDialog( E_PicViewDelete );
	}
}

void CPicViewDialog::OnReturn()
{
	this->EndDialog( E_PicViewClose );
}
void CPicViewDialog::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnDeletePic();
}

void CPicViewDialog::OnBnClickedButtonReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnReturn();
}

void CPicViewDialog::OnClickLeftFunKey()
{
	this->OnBnClickedButtonDelete();
}

void CPicViewDialog::OnClickRightFunKey()
{
	this->OnBnClickedButtonReturn();
}