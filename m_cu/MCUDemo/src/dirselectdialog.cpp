// .\src\dirselectdialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dirselectdialog.h"
#include "mcucommon.h"

// CDirSelectDialog �Ի���

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

	// �������뷨��ť��
	this->FullScreen( FS_HideSipButton );

	CRect rcList;
	this->m_listctrlDir.GetClientRect( rcList );

	this->m_listctrlDir.InsertColumn( 0, _T( "�ļ���" ), 0, rcList.Width() );

	LPCTSTR strInitDir = _T( "\\" );

	this->UpdateDirShow( strInitDir );
//	this->m_editDir.SetWindowText( strInitDir );

	SHMENUBARINFO shmbi;

	// Create our softkey bar
	// ���������󴴽����ܾ���ʾ��������.
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


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

	// TODO: �ڴ˴������Ϣ����������
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

	// ʣ��Ŀռ䡣
	CRect rcLeftSpace = rcClient;

	// ��Ļ���ȣ���͸��и������Ǹ�����Ϊ�ؼ������Ļ�����
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

	// ��վ��б�
	this->m_listctrlDir.DeleteAllItems();

	// ��������ӡ�\��
	CString strTarget = strRootPath;
	if( strTarget.Right(1) != _T( "\\" ) )
	{
		strTarget += _T( "\\" );
	}
	this->m_editDir.SetWindowText( strTarget );

	m_strCurDir = (LPCTSTR)strTarget;

	// ��ʼ�����ļ��С�
	strTarget += _T( "*" );
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile( strTarget, &fd );
	BOOL bResult = ( hFind != INVALID_HANDLE_VALUE  );
	while ( bResult )
	{
		CString strDirName = fd.cFileName;

		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )	// ֻ��ʾ�ļ��С�
		{
			this->m_listctrlDir.InsertItem( 0, strDirName );
		}

		bResult = ::FindNextFile( hFind, &fd );
	}

	if ( INVALID_HANDLE_VALUE != hFind )
	{
		::FindClose( hFind );
	}

	// �������ϵͳ��Ŀ¼���������һ����һ����
	if ( !m_strCurDir.empty() && m_strCurDir != _T( "\\" ) )
	{
		this->m_listctrlDir.InsertItem( 0, _T( ".." ) );
	}
}
void CDirSelectDialog::OnLvnItemchangedListDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDirSelectDialog::OnHdnItemclickListDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

void CDirSelectDialog::OnNMClickListDir(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	LPNMLISTVIEW pNMItemActivate = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
			// ����һ��Ŀ¼��������
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
				mcu::tlog << _T( "��һ��Ŀ¼ʧ�ܣ����� " ) << strCurPath << endl;
				strNewPath = strCurPath;
			}
		}
		else
		{
			// ���ӡ�
			strNewPath = strCurPath + strDir;
		}
		this->UpdateDirShow( strNewPath );
	}

	*pResult = 0;
}

void CDirSelectDialog::OnMenuOk()
{
	// TODO: �ڴ���������������
	this->EndDialog( IDOK );
}

void CDirSelectDialog::OnMenuCancel()
{
	// TODO: �ڴ���������������
	this->EndDialog( IDCANCEL );
}

tstring CDirSelectDialog::GetSelectedDir() const
{
	return this->m_strCurDir;
}