// .\src\dirselectdialog.cpp : 实现文件
//

#include "stdafx.h"
#include "dirselectdialog.h"
#include "mcucommon.h"

// CDirSelectDialog 对话框

IMPLEMENT_DYNCREATE(CDirSelectDialog, CUIDialog)

CDirSelectDialog::CDirSelectDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CDirSelectDialog::IDD, pParent)
{

}

CDirSelectDialog::~CDirSelectDialog()
{
}

void CDirSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DIR, m_editDir);
	DDX_Control(pDX, IDC_LIST_DIR, m_listctrlDir);
}

BOOL CDirSelectDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	DWORD exStyle = this->m_listctrlDir.GetExtendedStyle();
	this->m_listctrlDir.SetExtendedStyle( exStyle | LVS_EX_FULLROWSELECT );

	this->UpdateLayout();

	// 隐藏输入法按钮。
	this->FullScreen( FS_HideSipButton );

	CRect rcList;
	this->m_listctrlDir.GetClientRect( rcList );

	this->m_listctrlDir.InsertColumn( 0, _T( "文件夹" ), 0, rcList.Width() );

	LPCTSTR strInitDir = _T( "\\" );

	this->UpdateDirShow( strInitDir );
//	this->m_editDir.SetWindowText( strInitDir );

	SHMENUBARINFO shmbi;

	// Create our softkey bar
	// 如果不是最后创建可能就显示不出来了.
	ZeroMemory(&shmbi, sizeof(shmbi));
	shmbi.cbSize = sizeof(shmbi);
	shmbi.hwndParent = GetSafeHwnd();
	shmbi.dwFlags = SHCMBF_HMENU;

	shmbi.nToolBarId = IDR_MENU_OKCANCEL;
	shmbi.hInstRes = AfxGetInstanceHandle();
	if (!SHCreateMenuBar(&shmbi))
	{
		// Failed!!
		mcu::tlog << _T( "dirselectdialog create menu fail!" ) << endl;
		return FALSE;
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CDirSelectDialog, CUIDialog)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DIR, &CDirSelectDialog::OnLvnItemchangedListDir)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDirSelectDialog::OnHdnItemclickListDir)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIR, &CDirSelectDialog::OnNMClickListDir)
	ON_COMMAND(ID_MENU_OK, &CDirSelectDialog::OnMenuOk)
	ON_COMMAND(ID_MENU_CANCEL, &CDirSelectDialog::OnMenuCancel)
END_MESSAGE_MAP()




void CDirSelectDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	this->UpdateLayout( CRect( 0,0,cx,cy ) );
}

void CDirSelectDialog::UpdateLayout( LPRECT lprcClient /* = NULL */ )
{
	if ( !GetSafeHwnd() )
	{
		return;
	}

	CRect rcClient;
	if ( lprcClient )
	{
		rcClient = *lprcClient;
	}
	else
	{
		this->GetClientRect( rcClient );
	}

	// 剩余的空间。
	CRect rcLeftSpace = rcClient;

	// 屏幕长度（宽和高中更长的那个）作为控件比例的基数。
	int nScreenLong = GetScreenLong();
//	int nScreenWidth = ::GetScreenWidth();

	if ( this->m_editDir.GetSafeHwnd() )
	{
		const float fEditDir = 0.044;
		CRect rcEditDir = rcLeftSpace;
		rcEditDir.bottom = rcEditDir.top + int( fEditDir * nScreenLong );
		this->m_editDir.MoveWindow( rcEditDir );

		rcLeftSpace.top = rcEditDir.bottom;
	}

	if ( this->m_listctrlDir.GetSafeHwnd() )
	{
		CRect rcListDir;
		rcListDir = rcLeftSpace;

		this->m_listctrlDir.MoveWindow( rcListDir );

		rcLeftSpace.top = rcListDir.bottom;
	}
	


}

void CDirSelectDialog::UpdateDirShow( LPCTSTR strRootPath )
{

	// 清空旧列表。
	this->m_listctrlDir.DeleteAllItems();

	// 在最后增加“\”
	CString strTarget = strRootPath;
	if( strTarget.Right(1) != _T( "\\" ) )
	{
		strTarget += _T( "\\" );
	}
	this->m_editDir.SetWindowText( strTarget );

	m_strCurDir = (LPCTSTR)strTarget;

	// 开始查找文件夹。
	strTarget += _T( "*" );
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile( strTarget, &fd );
	BOOL bResult = ( hFind != INVALID_HANDLE_VALUE  );
	while ( bResult )
	{
		CString strDirName = fd.cFileName;

		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )	// 只显示文件夹。
		{
			this->m_listctrlDir.InsertItem( 0, strDirName );
		}

		bResult = ::FindNextFile( hFind, &fd );
	}

	if ( INVALID_HANDLE_VALUE != hFind )
	{
		::FindClose( hFind );
	}

	// 如果不是系统根目录，最后增加一个上一级。
	if ( !m_strCurDir.empty() && m_strCurDir != _T( "\\" ) )
	{
		this->m_listctrlDir.InsertItem( 0, _T( ".." ) );
	}
}
void CDirSelectDialog::OnLvnItemchangedListDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDirSelectDialog::OnHdnItemclickListDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}

void CDirSelectDialog::OnNMClickListDir(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	LPNMLISTVIEW pNMItemActivate = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

//	POSITION psSel = this->m_listctrlDir.GetFirstSelectedItemPosition();
	if ( pNMItemActivate->iItem != -1 )
	{
		int nSel = pNMItemActivate->iItem;
		CString strDir = m_listctrlDir.GetItemText( nSel, 0 );

		CString strCurPath;
		this->m_editDir.GetWindowText( strCurPath );

		CString strNewPath;
		if ( strDir == _T( ".." ) )
		{
			// 对上一级目录单独处理。
			if ( strCurPath.GetLength() > 0 && strCurPath.GetAt( strCurPath.GetLength() - 1 ) == '\\' )
			{
				strCurPath = strCurPath.Left( strCurPath.GetLength() - 1 );
			}
			int nPos = strCurPath.ReverseFind( '\\' );
			if ( nPos != -1 )
			{
				strNewPath = strCurPath.Left( nPos );
			}
			else
			{
				mcu::tlog << _T( "上一级目录失败！！！ " ) << strCurPath << endl;
				strNewPath = strCurPath;
			}
		}
		else
		{
			// 附加。
			strNewPath = strCurPath + strDir;
		}
		this->UpdateDirShow( strNewPath );
	}

	*pResult = 0;
}

void CDirSelectDialog::OnMenuOk()
{
	// TODO: 在此添加命令处理程序代码
	this->EndDialog( IDOK );
}

void CDirSelectDialog::OnMenuCancel()
{
	// TODO: 在此添加命令处理程序代码
	this->EndDialog( IDCANCEL );
}

tstring CDirSelectDialog::GetSelectedDir() const
{
	return this->m_strCurDir;
}