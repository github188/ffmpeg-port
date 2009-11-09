// src\picmanagedialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "picmanagedialog.h"
#include "config.h"
#include "imagefactory.h"
#include "picviewdialog.h"
#include "mcucommon.h"
//#include <shellapi.h>

// CPicManageDialog �Ի���

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


// CPicManageDialog ��Ϣ�������

void CPicManageDialog::OnBnClickedButtonPrePage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( m_nCurPage > 0 )
	{
		--m_nCurPage;
		this->UpdatePicShow(FALSE);
		this->UpdateLayout();
	}
}

void CPicManageDialog::OnBnClickedButtonNextPage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CImageDialog::OnPaint()
	CBasePicManageDialog::OnPaint();
}

BOOL CPicManageDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��������Ŀ¼��
	tstring strPicDir;
	CConfig::Instance()->GetCaptureDir( strPicDir );
	this->SetPicDir( strPicDir.c_str() );

	// ��ʾ������ɫΪ��ɫ��
	this->SetColor( RGB( 0,0,0 ), FALSE );

	this->m_logoDlg.Create( CLogoDialog::IDD, this );
	this->m_logoDlg.ShowWindow( SW_SHOW );

	// ��ť��ͼ��
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

	// ҳ����Ϣ��
	this->m_stPageInfo.SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );
	this->m_stPageInfo.SetBkColor( RGB( 0,0,0 ) );
	this->m_stPageInfo.SetBkTransparent( FALSE, FALSE );

	// ����ͼƬ��ʾ�ؼ���
	for ( int i=0; i < E_PicNumPerPage; ++i )
	{
		CRect rcStPic = this->GetPicCtrlPos( i );
		m_arPicSt[i].SetBkTransparent( FALSE, FALSE );
		m_arPicSt[i].Create( NULL, WS_CHILD, rcStPic, this );
		m_arPicSt[i].ShowWindow( SW_HIDE );
	}

	// ����¼����ʾ�ؼ���
	for ( int i=0; i < E_PicNumPerPage; ++i )
	{
		CRect rcStPic = this->GetPicCtrlPos( i );
		m_arRecSt[i].SetBkTransparent( FALSE, FALSE );
		m_arRecSt[i].SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );
		m_arRecSt[i].SetBkColor( RGB( 0,0,0 ) );
		m_arRecSt[i].Create( NULL, WS_CHILD, rcStPic, this );
		m_arRecSt[i].ShowWindow( SW_HIDE );
	}

	// �ײ��˵�������ť��
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnDelete.SetBkTransparent( FALSE, FALSE );
	this->m_btnDelete.SetImage( _T( "btn_delete_normal.jpg" ), _T( "btn_delete_focus.jpg" ), _T( "btn_delete_disable.jpg" ), FALSE );

	this->m_btnReturn.SetBkTransparent( FALSE, FALSE );
	this->m_btnReturn.SetImage( _T( "btn_return_normal.jpg" ), _T( "btn_return_focus.jpg" ), _T( "btn_return_normal.jpg" ), FALSE );

	

	// �������֡�
	this->UpdateLayout();

	// ��ʾͼƬ�� 
	this->UpdatePicShow( TRUE );

	SetForegroundWindow();

	// ��������Ĳ˵������������뷨��ť.
	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	// �˵���

	//SHMENUBARINFO shmbi;
	//// Create our softkey bar
	//// ���������󴴽����ܾ���ʾ��������.
	//ZeroMemory(&shmbi, sizeof(shmbi));
	//shmbi.cbSize = sizeof(shmbi);
	//shmbi.hwndParent = GetSafeHwnd();
	//shmbi.dwFlags = SHCMBF_HMENU | SHCMBF_HIDESIPBUTTON;

	//shmbi.nToolBarId = IDR_MENU_PIC_MANAGE;
	//shmbi.hInstRes = AfxGetInstanceHandle();
	//if (!SHCreateMenuBar(&shmbi))
	//{
	//	// Failed!!
	//	MessageBox( TEXT( "�����˵�ʧ�ܣ���" ) );
	//	return FALSE;
	//}
//	m_hSoftKeyBar = shmbi.hwndMB;


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPicManageDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	this->UpdateLayout( CRect( 0,0, cx, cy ) );
	// TODO: �ڴ˴������Ϣ����������
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
	
	// ʣ��Ŀռ䡣
	CRect rcLeftSpace = rcClient;

	// ��Ļ���ȣ���͸��и������Ǹ�����Ϊ�ؼ������Ļ�����
	int nScreenLong = GetScreenLong();
	int nScreenWidth = ::GetScreenWidth();

	// logo.
	if ( m_logoDlg.GetSafeHwnd() )
	{
		int nLogoHeight = m_logoDlg.GetHeight();
		CRect rcLogoDlg = rcClient;;
		rcLogoDlg.bottom = nLogoHeight;
		this->m_logoDlg.MoveWindow( rcLogoDlg );

		// ��ʣ��ռ���ȥ��logoռ�õĿռ䡣
		rcLeftSpace.top += nLogoHeight;
	}

	// �±߲˵�����
	// �����˵�����λ�á�
	if ( m_stBottomBg.GetSafeHwnd() && m_btnDelete.GetSafeHwnd() && m_btnReturn.GetSafeHwnd() )
	{
		// �ײ���ť���������ǰ�Ĳ˵����ĸ߶�	
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

		// ��ʣ��ռ���ȥ���˵���ռ�õĿռ䡣
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

	// ��ҳ��ť��ҳ����Ϣ�������ؼ���������ͻ������±���ʾ��
	// ȫ��������Ļ��С�����ȣ���ť�̶�ͼƬ�ĳ���������������仯��
	// Ӧ���������Ѿ�����ť�ؼ���С����ΪͼƬ�ĳ���ȡ�

	// �Ի����²����ư�ť�߶ȡ�
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

	// ����ͼƬ����ʾ����
	CRect rcPicViewSpace = this->GetPicSpace();

	// ͼƬ��ʾ�����ϱ���logo�Ի���
//	rcPicViewSpace.top += m_logoDlg.GetHeight();

	// ͼƬ�����С��
	const float conFPicRadio = 0.3;
	// ͼƬ��߿�֮��Ŀհס�
	const float conFBordRadio = 0.03;
	// ͼƬ֮��Ŀհס�
	const float conFPicIntervel = 0.02;

	

	// �Ի����²����ư�ť�߶ȡ�
//	const float conFControlHeight = 0.09;

	// ͼƬ��ʾ���������ǿ��ư�ť��
//	rcPicViewSpace.bottom -= int( GetScreenLong() * conFControlHeight );

	// ͼƬ����
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

	// - �����logo�߶ȡ�
	if ( m_logoDlg.GetSafeHwnd() )
	{
		rcClient.top += m_logoDlg.GetHeight();
	}

	// - ����Ĳ˵����߶ȡ�
	if ( m_stBottomBg.GetSafeHwnd() )
	{
		CRect rcMenuBar;
		m_stBottomBg.GetWindowRect( rcMenuBar );
		rcClient.bottom -= rcMenuBar.Height();
	}

	// �Ի����²����ư�ť�߶ȡ�
	const float conFControlHeight = 0.09;

	// ͼƬ��ʾ���������ǿ��ư�ť��
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

	// ��ҳ��
	int nTotalPage = ( nTotalFileCount + E_PicNumPerPage - 1 ) / E_PicNumPerPage;

	if ( m_btnNextPage > 0 && m_nCurPage > nTotalPage - 1 )
	{
		m_nCurPage = max( 0, nTotalPage - 1 );
	}

	this->m_strPageInfo.Format( _T( "%d/%dҳ" ), m_nCurPage + 1, nTotalPage == 0 ? 1 : nTotalPage  );

	this->UpdateData( FALSE );

	// ǰһҳ��ťʹ�ܡ�
	if ( m_btnPrePage.GetSafeHwnd() )
	{
		m_btnPrePage.EnableWindow( m_nCurPage > 0 );
	}
	// ��һҳ��ťʹ�ܡ�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	// �õ��û������ͼƬ��š�
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
		// �õ�ͼƬ����ʵ��š�
		int nFileIndex = E_PicNumPerPage * this->m_nCurPage + nSelStatic;

		if ( nFileIndex >= int( m_tFileList.size() ) )
		{
			return;
		}

		if ( !m_arPicSt[nSelStatic].GetPicPath().empty() && m_arPicSt[nSelStatic].GetSafeHwnd() && m_arPicSt[nSelStatic].IsWindowVisible() )
		{
			// ͼƬ.
			// �û������ͼƬ��		
			if ( nFileIndex == m_nCurSelFile )
			{
				// ��ͼƬ��
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
						MessageBox( _T( "ͼƬ����ʹ�ã��޷�ɾ����" ), _T( "ɾ��ͼƬ" ) );
					}
				}
			}
			else
			{
				// ����ѡ�е�ͼƬ��
				int nOldSel = m_nCurSelFile % E_PicNumPerPage;
				this->m_nCurSelFile = nFileIndex;
				// ������ʾ��
				this->m_arPicSt[ nOldSel ].SetSelected( FALSE );
				this->m_arRecSt[ nOldSel ].SetSelected( FALSE );
				this->m_arPicSt[ nSelStatic ].SetSelected( TRUE );

			}

		}
		else if( !m_arRecSt[nSelStatic].GetRecordPath().empty() && m_arRecSt[nSelStatic].GetSafeHwnd() && m_arRecSt[nSelStatic].IsWindowVisible() )
		{
			// ¼��.
			if ( nFileIndex == m_nCurSelFile )
			{
				// ��¼��
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
				// ����ѡ�е�ͼƬ��
				int nOldSel = m_nCurSelFile % E_PicNumPerPage;
				this->m_nCurSelFile = nFileIndex;
				// ������ʾ��
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
	// TODO: �ڴ���������������
	this->OnDelete();	
}

void CPicManageDialog::OnMenuReturn()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnDelete();
}

void CPicManageDialog::OnBnClickedButtonReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnReturn();
}

void CPicManageDialog::OnDelete()
{
	// ��ȡ��ǰѡ�е�ͼƬ��
	int nTotalCount = m_tFileList.size();
	if ( m_nCurSelFile >= 0 && m_nCurSelFile < nTotalCount )
	{
		tstring strPicPath = m_tFileList[ m_nCurSelFile ];
		int nPicShowIndex = m_nCurSelFile % E_PicNumPerPage;
		if ( m_arPicSt[nPicShowIndex].GetSafeHwnd() && m_arPicSt[nPicShowIndex].IsWindowVisible() && ( FT_Pic == this->GetFileType( strPicPath.c_str() ) )  )
		{
			// ɾ��ͼƬ��
			if( IDYES == MessageBox( _T( "ȷ��ɾ��ͼƬ��" ), _T( "ɾ��ͼƬ" ), MB_YESNO ) )
			{
				if( this->DelFile( m_nCurSelFile ) )
				{
					//
				}
				else
				{
					MessageBox( _T( "ͼƬ����ʹ�ã��޷�ɾ����" ), _T( "ɾ��ͼƬ" ) );
				}
			}
		}
		else if( m_arRecSt[nPicShowIndex].GetSafeHwnd() && m_arRecSt[nPicShowIndex].IsWindowVisible() && ( FT_Rec == this->GetFileType( strPicPath.c_str() ) ) )
		{
			// ɾ��ͼƬ��
			if( IDYES == MessageBox( _T( "ȷ��ɾ��¼��" ), _T( "ɾ��¼��" ), MB_YESNO ) )
			{				
				if( this->DelFile( m_nCurSelFile ) )
				{
					//
				}
				else
				{
					MessageBox( _T( "¼������ʹ�ã��޷�ɾ����" ), _T( "ɾ��¼��" ) );
				}
			}
		}
		else
		{
			MessageBox( _T( "��ѡ��Ҫɾ�����ļ���" ), _T( "ɾ���ļ�" ) );
		}
	}
	else
	{
		MessageBox( _T( "��ѡ��Ҫɾ�����ļ���" ), _T( "ɾ���ļ�" ) );
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