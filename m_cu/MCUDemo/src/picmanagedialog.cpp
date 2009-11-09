// src\picmanagedialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "picmanagedialog.h"
#include "config.h"
#include "imagefactory.h"
#include "picviewdialog.h"
#include "mcucommon.h"
//#include <shellapi.h>

// CPicManageDialog 对话框

IMPLEMENT_DYNAMIC(CPicManageDialog, CBasePicManageDialog)

CPicManageDialog::CPicManageDialog(CWnd* pParent /*=NULL*/)
	: CBasePicManageDialog(CPicManageDialog::IDD, pParent)
	, m_strPageInfo(_T(""))
{
	this->m_nCurSelFile = 0;
	this->m_nCurPage = 0;
}

CPicManageDialog::~CPicManageDialog()
{
}

void CPicManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CBasePicManageDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PRE_PAGE, m_btnPrePage);
	DDX_Control(pDX, IDC_BUTTON_NEXT_PAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_STATIC_PAGE_INFO, m_stPageInfo);
	DDX_Text(pDX, IDC_STATIC_PAGE_INFO, m_strPageInfo);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_RETURN, m_btnReturn);
	DDX_Control(pDX, IDC_STATIC_BOTTOM_BAR, m_stBottomBg);
}


BEGIN_MESSAGE_MAP(CPicManageDialog, CBasePicManageDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRE_PAGE, &CPicManageDialog::OnBnClickedButtonPrePage)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_PAGE, &CPicManageDialog::OnBnClickedButtonNextPage)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_DELETE_PIC_FILE, &CPicManageDialog::OnMenuDeletePicFile)
	ON_COMMAND(ID_MENU_RETURN, &CPicManageDialog::OnMenuReturn)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CPicManageDialog::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_RETURN, &CPicManageDialog::OnBnClickedButtonReturn)
END_MESSAGE_MAP()


// CPicManageDialog 消息处理程序

void CPicManageDialog::OnBnClickedButtonPrePage()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_nCurPage > 0 )
	{
		--m_nCurPage;
		this->UpdatePicShow(FALSE);
		this->UpdateLayout();
	}
}

void CPicManageDialog::OnBnClickedButtonNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	int nFileCount = m_tFileList.size();
	int nTotalPage = ( nFileCount + E_PicNumPerPage - 1 ) / E_PicNumPerPage;
	if( m_nCurPage <= nTotalPage )
	{
		m_nCurPage ++;
		this->UpdatePicShow(FALSE);
		this->UpdateLayout();
	}
}

void CPicManageDialog::OnPaint()
{
	// CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CImageDialog::OnPaint()
	CBasePicManageDialog::OnPaint();
}

BOOL CPicManageDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置搜索目录。
	tstring strPicDir;
	CConfig::Instance()->GetCaptureDir( strPicDir );
	this->SetPicDir( strPicDir.c_str() );

	// 显示背景颜色为黑色。
	this->SetColor( RGB( 0,0,0 ), FALSE );

	this->m_logoDlg.Create( CLogoDialog::IDD, this );
	this->m_logoDlg.ShowWindow( SW_SHOW );

	// 按钮贴图。
	tstring strBtnPrePageNormal = _T( "btn_pic_pre_page_normal.jpg" );
	tstring strBtnPrePageDisable = _T( "btn_pic_pre_page_disable.jpg" );
	this->m_btnPrePage.SetImage( strBtnPrePageNormal.c_str(), 
		strBtnPrePageNormal.c_str(), 
		strBtnPrePageDisable.c_str(), FALSE ) ;
	this->m_btnPrePage.SetBkTransparent( FALSE, FALSE );
	this->m_btnPrePage.FitToPicture();

	tstring strBtnNextPageNormal = _T( "btn_pic_next_page_normal.jpg" );
	tstring strBtnNextPageDisable = _T( "btn_pic_next_page_disable.jpg" );
	this->m_btnNextPage.SetImage( strBtnNextPageNormal.c_str(), strBtnNextPageNormal.c_str(),
		strBtnNextPageDisable.c_str(), FALSE );
	this->m_btnNextPage.SetBkTransparent( FALSE, FALSE );
	this->m_btnNextPage.FitToPicture();

	// 页码信息。
	this->m_stPageInfo.SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );
	this->m_stPageInfo.SetBkColor( RGB( 0,0,0 ) );
	this->m_stPageInfo.SetBkTransparent( FALSE, FALSE );

	// 创建图片显示控件。
	for ( int i=0; i < E_PicNumPerPage; ++i )
	{
		CRect rcStPic = this->GetPicCtrlPos( i );
		m_arPicSt[i].SetBkTransparent( FALSE, FALSE );
		m_arPicSt[i].Create( NULL, WS_CHILD, rcStPic, this );
		m_arPicSt[i].ShowWindow( SW_HIDE );
	}

	// 创建录像显示控件。
	for ( int i=0; i < E_PicNumPerPage; ++i )
	{
		CRect rcStPic = this->GetPicCtrlPos( i );
		m_arRecSt[i].SetBkTransparent( FALSE, FALSE );
		m_arRecSt[i].SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );
		m_arRecSt[i].SetBkColor( RGB( 0,0,0 ) );
		m_arRecSt[i].Create( NULL, WS_CHILD, rcStPic, this );
		m_arRecSt[i].ShowWindow( SW_HIDE );
	}

	// 底部菜单条及按钮。
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnDelete.SetBkTransparent( FALSE, FALSE );
	this->m_btnDelete.SetImage( _T( "btn_delete_normal.jpg" ), _T( "btn_delete_focus.jpg" ), _T( "btn_delete_disable.jpg" ), FALSE );

	this->m_btnReturn.SetBkTransparent( FALSE, FALSE );
	this->m_btnReturn.SetImage( _T( "btn_return_normal.jpg" ), _T( "btn_return_focus.jpg" ), _T( "btn_return_normal.jpg" ), FALSE );

	

	// 调整布局。
	this->UpdateLayout();

	// 显示图片。 
	this->UpdatePicShow( TRUE );

	SetForegroundWindow();

	// 隐藏下面的菜单栏。隐藏输入法按钮.
	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	// 菜单。

	//SHMENUBARINFO shmbi;
	//// Create our softkey bar
	//// 如果不是最后创建可能就显示不出来了.
	//ZeroMemory(&shmbi, sizeof(shmbi));
	//shmbi.cbSize = sizeof(shmbi);
	//shmbi.hwndParent = GetSafeHwnd();
	//shmbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;

	//shmbi.nToolBarId = IDR_MENU_PIC_MANAGE;
	//shmbi.hInstRes = AfxGetInstanceHandle();
	//if (!SHCreateMenuBar(&shmbi))
	//{
	//	// Failed!!
	//	MessageBox( TEXT( "创建菜单失败！！" ) );
	//	return FALSE;
	//}
//	m_hSoftKeyBar = shmbi.hwndMB;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPicManageDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	this->UpdateLayout( CRect( 0,0, cx, cy ) );
	// TODO: 在此处添加消息处理程序代码
}

void CPicManageDialog::UpdateLayout( LPRECT lpRcClient /* = NULL */ )
{
	if ( !GetSafeHwnd() )
	{
		return;
	}

	CRect rcClient;
	if ( lpRcClient )
	{
		rcClient = *lpRcClient;
	}
	else
	{
		this->GetClientRect( rcClient );
	}
	
	// 剩余的空间。
	CRect rcLeftSpace = rcClient;

	// 屏幕长度（宽和高中更长的那个）作为控件比例的基数。
	int nScreenLong = GetScreenLong();
	int nScreenWidth = ::GetScreenWidth();

	// logo.
	if ( m_logoDlg.GetSafeHwnd() )
	{
		int nLogoHeight = m_logoDlg.GetHeight();
		CRect rcLogoDlg = rcClient;;
		rcLogoDlg.bottom = nLogoHeight;
		this->m_logoDlg.MoveWindow( rcLogoDlg );

		// 在剩余空间中去除logo占用的空间。
		rcLeftSpace.top += nLogoHeight;
	}

	// 下边菜单条。
	// 调整菜单条的位置。
	if ( m_stBottomBg.GetSafeHwnd() && m_btnDelete.GetSafeHwnd() && m_btnReturn.GetSafeHwnd() )
	{
		// 底部按钮条（替代以前的菜单）的高度	
		const float conFMenubarHeight = 0.082;
		int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

		CRect rcBottomBg = rcClient;
		rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
		//m_stBottomBg.MoveWindow( rcBottomBg );
		this->m_stBottomBg.SetWindowPos( &CWnd::wndTop, rcBottomBg.left, rcBottomBg.top, rcBottomBg.Width(), rcBottomBg.Height(), NULL );

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

	for ( int i=0; i < E_PicNumPerPage; ++i )
	{
		if ( m_arPicSt[i].GetSafeHwnd() )
		{
			CRect rcStPic = this->GetPicCtrlPos( i );
			m_arPicSt[i].MoveWindow( rcStPic );
		}	

		if ( m_arRecSt[i].GetSafeHwnd() )
		{
			CRect rcStPic = this->GetPicCtrlPos( i );
			m_arRecSt[i].MoveWindow( rcStPic );
		}
	}

	// 翻页按钮，页码信息，三个控件贴近程序客户区的下边显示。
	// 全部都与屏幕大小成正比，按钮固定图片的长宽比例进行伸缩变化。
	// 应该在事先已经将按钮控件大小调节为图片的长宽比。

	// 对话框下部控制按钮高度。
	const float conFControlHeight = 0.097;
	const float conFControlWidth = 0.116;

	const float conFPageInfoStWidth = 0.625;

	int nNewHeight = int( nScreenLong * conFControlHeight );
	int nNewWidth = int( nScreenLong * conFControlWidth );

	int nNewPageInfoStWidth = int( nScreenWidth * conFPageInfoStWidth );

	int nNewTop = rcLeftSpace.bottom - nNewHeight;
	int nNewBottom = rcLeftSpace.bottom;

	CRect rcNewPageInfo( 0, nNewTop, nNewPageInfoStWidth, nNewBottom );
	rcNewPageInfo.OffsetRect( ( nScreenWidth - nNewPageInfoStWidth ) / 2, 0 );

	CRect rcNewPrePageBtn( 0, nNewTop, nNewWidth, nNewBottom );
	rcNewPrePageBtn.OffsetRect( rcNewPageInfo.left - rcNewPrePageBtn.right, 0 );

	CRect rcNewNextPageBtn( 0, nNewTop, nNewWidth, nNewBottom );
	rcNewNextPageBtn.OffsetRect( rcNewPageInfo.right - rcNewNextPageBtn.left, 0 );
	
	if ( m_stPageInfo.GetSafeHwnd() )
	{
		m_stPageInfo.MoveWindow( rcNewPageInfo );
	}
	if ( m_btnPrePage.GetSafeHwnd() )
	{
		m_btnPrePage.MoveWindow( rcNewPrePageBtn );
	}
	if( m_btnNextPage.GetSafeHwnd() )
	{
		m_btnNextPage.MoveWindow( rcNewNextPageBtn );
	}
	
}

CRect CPicManageDialog::GetPicCtrlPos( int nIndex ) const
{
	int nRowIndex = nIndex / E_ColumnNum;
	int nColIndex = nIndex % E_RowNum;

	//CRect rcClient;
	//this->GetClientRect( rcClient );

	// 所有图片的显示区域。
	CRect rcPicViewSpace = this->GetPicSpace();

	// 图片显示区域上边是logo对话框。
//	rcPicViewSpace.top += m_logoDlg.GetHeight();

	// 图片区域大小。
	const float conFPicRadio = 0.3;
	// 图片与边框之间的空白。
	const float conFBordRadio = 0.03;
	// 图片之间的空白。
	const float conFPicIntervel = 0.02;

	

	// 对话框下部控制按钮高度。
//	const float conFControlHeight = 0.09;

	// 图片显示区域下面是控制按钮。
//	rcPicViewSpace.bottom -= int( GetScreenLong() * conFControlHeight );

	// 图片区域。
	CRect rcStdPic;
	rcStdPic.left = int( rcPicViewSpace.Width() * ( conFBordRadio + nColIndex * ( conFPicRadio + conFPicIntervel ) ) );
	rcStdPic.right = int( rcStdPic.left + rcPicViewSpace.Width() * conFPicRadio );
	rcStdPic.top = int( rcPicViewSpace.top + rcPicViewSpace.Height() * ( conFBordRadio + nRowIndex * ( conFPicRadio + conFPicIntervel ) ) );
	rcStdPic.bottom = int( rcStdPic.top + rcPicViewSpace.Height() * conFPicRadio );

	return rcStdPic;	
}

CRect CPicManageDialog::GetPicSpace() const
{
	CRect rcClient;
	this->GetClientRect( rcClient );

	// - 上面的logo高度。
	if ( m_logoDlg.GetSafeHwnd() )
	{
		rcClient.top += m_logoDlg.GetHeight();
	}

	// - 下面的菜单条高度。
	if ( m_stBottomBg.GetSafeHwnd() )
	{
		CRect rcMenuBar;
		m_stBottomBg.GetWindowRect( rcMenuBar );
		rcClient.bottom -= rcMenuBar.Height();
	}

	// 对话框下部控制按钮高度。
	const float conFControlHeight = 0.09;

	// 图片显示区域下面是控制按钮。
	rcClient.bottom -= int( GetScreenLong() * conFControlHeight );

	return rcClient;
}

void CPicManageDialog::UpdatePicShow( BOOL bReSearchFile )
{
	if ( bReSearchFile )
	{
		TFilePathList tPicList = this->FindFile( m_strPicDir.c_str(), _T( "jpg" ) );
		TFilePathList tRecList = this->FindFile( m_strPicDir.c_str(), _T( "3gp" ) );

		m_tFileList = tPicList;

		for ( size_t i=0; i<tRecList.size(); ++i )
		{
			m_tFileList.push_back( tRecList[i] );
		}
	}

	size_t nTotalFileCount = m_tFileList.size();

	// 总页数
	int nTotalPage = ( nTotalFileCount + E_PicNumPerPage - 1 ) / E_PicNumPerPage;

	if ( m_btnNextPage > 0 && m_nCurPage > nTotalPage - 1 )
	{
		m_nCurPage = max( 0, nTotalPage - 1 );
	}

	this->m_strPageInfo.Format( _T( "%d/%d页" ), m_nCurPage + 1, nTotalPage == 0 ? 1 : nTotalPage  );

	this->UpdateData( FALSE );

	// 前一页按钮使能。
	if ( m_btnPrePage.GetSafeHwnd() )
	{
		m_btnPrePage.EnableWindow( m_nCurPage > 0 );
	}
	// 下一页按钮使能。
	if ( m_btnNextPage.GetSafeHwnd() )
	{		
		m_btnNextPage.EnableWindow( m_nCurPage < nTotalPage - 1 );
	}
	
	int nBeginIndex = this->m_nCurPage * E_PicNumPerPage;

	for ( int i=0; i<E_PicNumPerPage; ++i )
	{
		size_t nPicIndex = i + nBeginIndex;
		if (  nPicIndex < nTotalFileCount )
		{
			tstring strPicPath = this->m_tFileList[ nPicIndex ];
			EFileType eFileType = this->GetFileType( strPicPath.c_str() );

			m_arPicSt[i].ShowWindow( ( FT_Pic == eFileType ) ? SW_SHOW : SW_HIDE );
			m_arRecSt[i].ShowWindow( ( FT_Rec == eFileType ) ? SW_SHOW : SW_HIDE );

			m_arPicSt[i].SetSelected( m_nCurSelFile == E_PicNumPerPage * m_nCurPage + i , FALSE );
			m_arRecSt[i].SetSelected( m_nCurSelFile == E_PicNumPerPage * m_nCurPage + i );

			if ( FT_Pic == eFileType )
			{
				m_arPicSt[i].SetPicPath( strPicPath.c_str() );		
			}
			else
			{
				m_arRecSt[i].SetRecordPath( strPicPath.c_str() );
			}
				
		}
		else
		{
			m_arPicSt[i].SetSelected( FALSE, FALSE );
			m_arPicSt[i].ShowWindow( SW_HIDE );

			m_arRecSt[i].SetSelected( FALSE );
			m_arRecSt[i].ShowWindow( SW_HIDE );
		}
	}


}

CPicManageDialog::TFilePathList CPicManageDialog::FindFile( LPCTSTR strDir, LPCTSTR strExtName )
{
	TFilePathList tPics;

	CString strTarget = strDir;
	if( strTarget.Right(1) != _T( "\\" ) )
	{
		strTarget += _T( "\\" );
	}

	tstring strDirBk = strTarget;

	strTarget += _T( "*." );
	strTarget += strExtName;

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile( strTarget, &fd );
	BOOL bResult = ( hFind != INVALID_HANDLE_VALUE  );
	while ( bResult )
	{
		CString strLibName = fd.cFileName;

		tPics.push_back( strDirBk + fd.cFileName );

		bResult = ::FindNextFile( hFind, &fd );
	}

	if ( INVALID_HANDLE_VALUE != hFind )
	{
		::FindClose( hFind );
	}

	return tPics;
}

void CPicManageDialog::SetPicDir( LPCTSTR strPicDir )
{
	m_strPicDir = strPicDir;
}
void CPicManageDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	// 得到用户点击的图片序号。
	int nSelStatic = -1;
	for ( int i=0; i<E_PicNumPerPage; ++i )
	{
		CRect rcPic = this->GetPicCtrlPos( i );

		if ( rcPic.PtInRect( point ) )
		{
			nSelStatic = i;
			break;
		}
	}

	if ( -1 != nSelStatic )
	{
		// 得到图片的真实序号。
		int nFileIndex = E_PicNumPerPage * this->m_nCurPage + nSelStatic;

		if ( nFileIndex >= int( m_tFileList.size() ) )
		{
			return;
		}

		if ( !m_arPicSt[nSelStatic].GetPicPath().empty() && m_arPicSt[nSelStatic].GetSafeHwnd() && m_arPicSt[nSelStatic].IsWindowVisible() )
		{
			// 图片.
			// 用户点击了图片。		
			if ( nFileIndex == m_nCurSelFile )
			{
				// 打开图片。
				CPicViewDialog dlg;
				dlg.SetPic( m_arPicSt[nSelStatic].GetPicPath().c_str() );
				if( CPicViewDialog::E_PicViewDelete == dlg.DoModal() )
				{
					if( this->DelFile( nFileIndex ) )
					{
						//
					}
					else
					{
						MessageBox( _T( "图片正在使用，无法删除！" ), _T( "删除图片" ) );
					}
				}
			}
			else
			{
				// 更新选中的图片。
				int nOldSel = m_nCurSelFile % E_PicNumPerPage;
				this->m_nCurSelFile = nFileIndex;
				// 更新显示。
				this->m_arPicSt[ nOldSel ].SetSelected( FALSE );
				this->m_arRecSt[ nOldSel ].SetSelected( FALSE );
				this->m_arPicSt[ nSelStatic ].SetSelected( TRUE );

			}

		}
		else if( !m_arRecSt[nSelStatic].GetRecordPath().empty() && m_arRecSt[nSelStatic].GetSafeHwnd() && m_arRecSt[nSelStatic].IsWindowVisible() )
		{
			// 录像.
			if ( nFileIndex == m_nCurSelFile )
			{
				// 打开录像。
	//			MessageBox( _T( "open record!" ) );

				tstring strFilePath = m_arRecSt[nSelStatic].GetRecordPath();

				SHELLEXECUTEINFO seInfo;
				memset( &seInfo, 0, sizeof( seInfo ) );
				seInfo.cbSize = sizeof( seInfo );
				seInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				seInfo.hwnd = this->GetSafeHwnd();
				seInfo.lpVerb = _T( "Open" );
				seInfo.lpFile = strFilePath.c_str();


				BOOL bResult = ::ShellExecuteEx( &seInfo );

				mcu::tlog << _T( "shell execute result: " ) << bResult << _T( " instapp: " ) << seInfo.hInstApp 
					<< _T( " process: " ) << seInfo.hProcess << endl;
				
			}
			else
			{
				// 更新选中的图片。
				int nOldSel = m_nCurSelFile % E_PicNumPerPage;
				this->m_nCurSelFile = nFileIndex;
				// 更新显示。
				this->m_arPicSt[ nOldSel ].SetSelected( FALSE );
				this->m_arRecSt[ nOldSel ].SetSelected( FALSE );
				this->m_arRecSt[ nSelStatic ].SetSelected( TRUE );

			}
		}

		
	}
	

	CUIDialog::OnLButtonDown(nFlags, point);
}

void CPicManageDialog::OnMenuDeletePicFile()
{
	// TODO: 在此添加命令处理程序代码
	this->OnDelete();	
}

void CPicManageDialog::OnMenuReturn()
{
	// TODO: 在此添加命令处理程序代码
	this->OnReturn();	
}

BOOL CPicManageDialog::DelFile( int nFileIndex )
{
	if ( nFileIndex >= 0 && nFileIndex < int( m_tFileList.size() ) )
	{
		tstring strPicPath = m_tFileList[ nFileIndex ];
		
		int nPicShowIndex = nFileIndex % E_PicNumPerPage;

		m_arPicSt[ nPicShowIndex ].ShowWindow( SW_HIDE );
		m_arPicSt[ nPicShowIndex ].SetPicPath( _T( "" ), FALSE );

		m_arRecSt[ nPicShowIndex ].ShowWindow( SW_HIDE );
		m_arRecSt[ nPicShowIndex ].SetRecordPath( _T( "" ) );

		BOOL bResult = CImageFactory::Instance()->ReleaseImage( strPicPath.c_str() );

		bResult = ::DeleteFile( strPicPath.c_str() );

		if ( bResult )
		{
			m_tFileList.erase( m_tFileList.begin() + nFileIndex );			
		}
		else
		{
			int nErrCode = GetLastError();
			mcu::tlog << _T( "Del file: " ) << strPicPath << _T( " fail! error code: " ) << nErrCode << endl;
		}
		//else
		//{
		//	m_arPicSt[ nPicShowIndex ].SetPicPath( strPicPath.c_str(), FALSE );
		//	m_arPicSt[ nPicShowIndex ].ShowWindow( SW_SHOW );			
		//}

		this->UpdatePicShow(FALSE);

		return bResult;

	}
	return FALSE;
}

CPicManageDialog::EFileType CPicManageDialog::GetFileType( LPCTSTR lpstrFileName )
{
	CString strExt = ::ParsePath( lpstrFileName ).m_strExtName.c_str();

	strExt.MakeLower();
	
	if ( strExt == _T( "jpg" ) )
	{
		return FT_Pic;
	}
	else if( strExt == _T( "3gp" ) )
	{
		return FT_Rec;
	}
	else
	{
		return FT_Unknow;
	}

}
void CPicManageDialog::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnDelete();
}

void CPicManageDialog::OnBnClickedButtonReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnReturn();
}

void CPicManageDialog::OnDelete()
{
	// 获取当前选中的图片。
	int nTotalCount = m_tFileList.size();
	if ( m_nCurSelFile >= 0 && m_nCurSelFile < nTotalCount )
	{
		tstring strPicPath = m_tFileList[ m_nCurSelFile ];
		int nPicShowIndex = m_nCurSelFile % E_PicNumPerPage;
		if ( m_arPicSt[nPicShowIndex].GetSafeHwnd() && m_arPicSt[nPicShowIndex].IsWindowVisible() && ( FT_Pic == this->GetFileType( strPicPath.c_str() ) )  )
		{
			// 删除图片。
			if( IDYES == MessageBox( _T( "确定删除图片？" ), _T( "删除图片" ), MB_YESNO ) )
			{
				if( this->DelFile( m_nCurSelFile ) )
				{
					//
				}
				else
				{
					MessageBox( _T( "图片正在使用，无法删除！" ), _T( "删除图片" ) );
				}
			}
		}
		else if( m_arRecSt[nPicShowIndex].GetSafeHwnd() && m_arRecSt[nPicShowIndex].IsWindowVisible() && ( FT_Rec == this->GetFileType( strPicPath.c_str() ) ) )
		{
			// 删除图片。
			if( IDYES == MessageBox( _T( "确定删除录像？" ), _T( "删除录像" ), MB_YESNO ) )
			{				
				if( this->DelFile( m_nCurSelFile ) )
				{
					//
				}
				else
				{
					MessageBox( _T( "录像正在使用，无法删除！" ), _T( "删除录像" ) );
				}
			}
		}
		else
		{
			MessageBox( _T( "请选择要删除的文件！" ), _T( "删除文件" ) );
		}
	}
	else
	{
		MessageBox( _T( "请选择要删除的文件！" ), _T( "删除文件" ) );
	}
}

void CPicManageDialog::OnReturn()
{
	this->EndDialog( 0 );
}

void CPicManageDialog::OnClickLeftFunKey()
{
	this->OnBnClickedButtonDelete();
}

void CPicManageDialog::OnClickRightFunKey()
{
	this->OnBnClickedButtonReturn();
}