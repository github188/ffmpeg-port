// src\picviewdialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "picviewdialog.h"
#include "mcucommon.h"


// CPicViewDialog 对话框

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


// CPicViewDialog 消息处理程序

BOOL CPicViewDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 黑色背景。
	this->SetColor( RGB( 0,0,0 ) );

	// 创建logo对话框。
	this->m_logoDlg.Create( CLogoDialog::IDD, this );
	this->m_logoDlg.ShowWindow( SW_SHOW );

	// 创建图片控件。
	CRect rcClient;
	this->GetClientRect( rcClient );

	this->m_stPic.Create( _T(""), WS_CHILD, rcClient, this );
	this->m_stPic.FitToPicture();
	this->m_stPic.ShowWindow( SW_SHOW );

	// 按钮贴图。
	// 按钮贴图。
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

	// 文件名。
	this->m_stPicInfo.SetBkColor( RGB( 0,0,0 ) );
	this->m_stPicInfo.SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );
	this->m_stPicInfo.SetWindowText(  ParsePath( this->m_stPic.GetPicPath().c_str() ).m_strBaseName.c_str() );
	this->m_stPicInfo.SetBkTransparent( FALSE, FALSE );

	// 底部菜单条及按钮。
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnDelete.SetBkTransparent( FALSE, FALSE );
	this->m_btnDelete.SetImage( _T( "btn_delete_normal.jpg" ), _T( "btn_delete_focus.jpg" ), _T( "btn_delete_disable.jpg" ), FALSE );

	this->m_btnReturn.SetBkTransparent( FALSE, FALSE );
	this->m_btnReturn.SetImage( _T( "btn_return_normal.jpg" ), _T( "btn_return_focus.jpg" ), _T( "btn_return_normal.jpg" ), FALSE );

	

	this->UpdateLayout();


	//// 创建菜单。
	//SHMENUBARINFO shmbi;
	//// Create our softkey bar
	//// 如果不是最后创建可能就显示不出来了.
	//ZeroMemory(&shmbi, sizeof(shmbi));
	//shmbi.cbSize = sizeof(shmbi);
	//shmbi.hwndParent = GetSafeHwnd();
	//shmbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;

	//shmbi.nToolBarId = IDR_MENU_PIC_VIEW;
	//shmbi.hInstRes = AfxGetInstanceHandle();
	//if (!SHCreateMenuBar(&shmbi))
	//{
	//	// Failed!!
	//	MessageBox( TEXT( "创建菜单失败！！" ) );
	//	return FALSE;
	//}

//	this->m_hMenuBarWnd = shmbi.hwndMB;

	// 隐藏下面的菜单栏。隐藏输入法按钮.
	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPicViewDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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

	// 剩余的空间。
	CRect rcLeftSpace = rcClient;

	// 是否显示除图片外的其它控件。
	BOOL bShowCtrls = !m_bFullScreen;

	// logo.
	if ( m_logoDlg.GetSafeHwnd() && bShowCtrls )
	{
		int nLogoHeight = m_logoDlg.GetHeight();
		CRect rcLogoDlg = rcClient;
		rcLogoDlg.bottom = nLogoHeight;
		this->m_logoDlg.MoveWindow( rcLogoDlg );

		// 在剩余空间中去除logo占用的空间。
		rcLeftSpace.top += nLogoHeight;
	}

	// 屏幕长度（宽和高中更长的那个）作为控件比例的基数。
	int nScreenLong = GetScreenLong();

	int nScreenWidth = ::GetScreenWidth();

	// 调整菜单条的位置。
	if ( m_stBottomBg.GetSafeHwnd() && m_btnDelete.GetSafeHwnd() && m_btnReturn.GetSafeHwnd() && bShowCtrls )
	{
		// 底部按钮条（替代以前的菜单）的高度	
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

		

		// 在剩余空间中去除菜单条占用的空间。
		rcLeftSpace.bottom -= nMenubarHeight;
	}

	// 图片控件。

	// 三个按钮。
	// 放大按钮，图片信息，缩小按钮，三个控件贴近程序客户区的下边显示。
	// 全部都与屏幕大小成正比，按钮固定图片的长宽比例进行伸缩变化。
	// 应该在事先已经将按钮控件大小调节为图片的长宽比。

	// 对话框下部控制按钮高度。
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

	// 在剩余空间中去除信息显示。
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

		// 保持图片长宽比例，将图片放入。			
		CRect rcNewPic( 0,0,0,0 );
		if ( rcPicOld.Width() * nPicSpaceHeight > rcPicOld.Height() * nPicSpaceWidth )
		{
			// 图片的宽：高大与图片显示空间的宽高比。图片的横向将占满，纵向不满。
			rcNewPic.right = nPicSpaceWidth;
			rcNewPic.bottom = rcPicOld.Height() * nPicSpaceWidth /  rcPicOld.Width();  
		}
		else
		{
			// 与之相反。
			rcNewPic.right = rcPicOld.Width() * nPicSpaceHeight / rcPicOld.Height();
			rcNewPic.bottom = nPicSpaceHeight;
		}

		// 将图片rect移至图片空间中心。
		CPoint offset = rcMaxPicSpace.CenterPoint() - rcNewPic.CenterPoint();
		rcNewPic.OffsetRect( offset );

		this->m_stPic.MoveWindow( rcNewPic );
	}
	
}
void CPicViewDialog::OnMenuPicViewReturn()
{
	this->OnReturn();
		
	// TODO: 在此添加命令处理程序代码
}

void CPicViewDialog::OnMenuDeletePicFile()
{
	this->OnDeletePic();
	
	// TODO: 在此添加命令处理程序代码
}



void CPicViewDialog::OnBnClickedButtonZoomIn()
{
	// TODO: 在此添加控件通知处理程序代码
	// 注意浮点数的比较！
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
	// TODO: 在此添加控件通知处理程序代码
	// 注意浮点数的比较！
	if ( m_fZoomRadio - 0.8 > 0.01  )
	{
		m_fZoomRadio -= 0.2;
	}
	this->UpdateLayout();
}

void CPicViewDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
		// 全屏.
		DWORD dwFSFlag = FS_FullScreen;

		// 先确定手机是否需要旋转.
		// 就是比较图片的长宽和手机屏幕的长宽.如果长宽一致,则不需要旋转,否则旋转90度后效果更好.
		// 算法: 屏幕宽高差与图片的宽高差同号(同为正或负),则不需要旋转.否则手机需要旋转.
		int nScreenWidth = ::GetScreenWidth();
		int nScreenHeight = ::GetScreenHeight();
		CRect rcImg;
		this->m_stPic.GetImageSize( rcImg );
		if( ( nScreenWidth - nScreenHeight ) * ( rcImg.Width() - rcImg.Height() ) < 0 )
		{
			// 需要旋转.
			dwFSFlag |= FS_HorizontalScreen;
		}

		this->FullScreen( dwFSFlag );
	}
	else
	{
		// 恢复.
		DWORD dwFSFlag = FS_ShowTaskBar | FS_ResumeScreen;

		this->FullScreen( dwFSFlag );
	}

	// 隐藏/显示控件.
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
	// 删除图片。
	if( IDYES == MessageBox( _T( "确定删除图片？" ), _T( "删除图片" ), MB_YESNO ) )
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
	// TODO: 在此添加控件通知处理程序代码
	this->OnDeletePic();
}

void CPicViewDialog::OnBnClickedButtonReturn()
{
	// TODO: 在此添加控件通知处理程序代码
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