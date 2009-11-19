// HtmlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "HtmlWnd.h"
#include "Config.h"

#include "mcucommon.h"
// CHtmlWnd 对话框

IMPLEMENT_DYNAMIC(CHtmlWnd, CDialog)

//WNDPROC CHtmlWnd::s_lpOldHtmlProc = NULL;

CHtmlWnd::CHtmlWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CHtmlWnd::IDD, pParent)
{
	m_hHtmlCtrlWnd = NULL;
	m_nHistoryHeadIndex = INVALID_INDEX;
	m_nCurUrlHistoryIndex = INVALID_INDEX;
	m_nHistoryEndIndex = INVALID_INDEX;
	m_bWaittingNavigateComplete = FALSE;
	m_bIsFaild = FALSE;
}

CHtmlWnd::~CHtmlWnd()
{
}

void CHtmlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHtmlWnd, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHtmlWnd 消息处理程序

BOOL CHtmlWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HINSTANCE hInst = AfxGetInstanceHandle();
	BOOL bResult = ::InitHTMLControl( hInst );
	if( bResult )
	{
		// create the window for the control
		CRect rcClient;
		this->GetClientRect( rcClient );
		m_hHtmlCtrlWnd = CreateWindow( WC_HTML , NULL, WS_VISIBLE , 0, 0, 
			rcClient.Width(), rcClient.Height(), 
			GetSafeHwnd(), NULL, hInst, NULL );
		::ShowWindow(m_hHtmlCtrlWnd, SW_SHOW);
		::UpdateWindow( m_hHtmlCtrlWnd );
		::SendMessage(m_hHtmlCtrlWnd, DTM_ENABLECONTEXTMENU, 0, TRUE);

		// 劫持消息.
//		s_lpOldHtmlProc = (WNDPROC)::SetWindowLong( m_hHtmlCtrlWnd, DWL_DLGPROC, (LONG)HtmlProc );
	}
	else
	{
		MessageBox( TEXT( "初始化Html控件失败！" ) );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHtmlWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_hHtmlCtrlWnd )
	{
		::MoveWindow( m_hHtmlCtrlWnd, 0, 0, cx, cy, TRUE );
	}
	
	// TODO: 在此处添加消息处理程序代码
}

BOOL CHtmlWnd::OpenUrl( LPCTSTR strUrl )
{
	if( NULL == strUrl || *strUrl == 0 )
	{
		return FALSE;
	}
	else
	{
		// 先清空页面.
		this->Clear();

		// load the start page into the browser window
		//strUrl = L"file://\\Program Files\\M_CU\\failhtml\\fail.htm";
		//strUrl = L"file://\\Program Files\\MEGA EYES\\htmlfail\\fail.htm";
		//LRESULT lr = ::SendMessage( m_hHtmlCtrlWnd, DTM_NAVIGATE, NAVIGATEFLAG_REFRESH, (LPARAM)strUrl );
		LRESULT lr = ::SendMessage( m_hHtmlCtrlWnd, DTM_NAVIGATE, NAVIGATEFLAG_REFRESH, (LPARAM)strUrl );
		return lr;
	}
}
BOOL CHtmlWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	NM_HTMLVIEW* pnmHTMLView = (NM_HTMLVIEW*)lParam;


	switch (pnmHTMLView->hdr.code)
	{
		// this code is processed when a user clicks on a hyperlink
	case NM_HOTSPOT:
		{
			mcu::tlog << _T( "NM_HOTSPOT" ) << endl;
			// read the target string

			tstring strUrl = UTF8toUTF16( (const char *)pnmHTMLView->szTarget );

			mcu::tlog << strUrl << endl;

			if( m_bWaittingNavigateComplete )
			{
				//停止控件继续操作
				*pResult = 1;
				return TRUE;
			}

			LPCTSTR lpUrlToken = _T( "/download/downloadmcu.do?" );
			CString UrlString = ( LPCTSTR )strUrl.c_str();
			if ( UrlString.Left( _tcslen( lpUrlToken ) ).CompareNoCase( lpUrlToken ) == 0 )
			{
				//判断是下载MCU链接 不兼容其它版本MSP

				//需要拼接完整的URL
				TCHAR ServerFullUrl[1024];
				memset(ServerFullUrl, 0, sizeof(ServerFullUrl));
				if(CConfig::Instance()->GetServerFullSvrUrl((LPTSTR)ServerFullUrl))
				{
					
					TCHAR DownloadFullUrl[2048];
					memset(DownloadFullUrl, 0, sizeof(DownloadFullUrl));
					swprintf(DownloadFullUrl, L"%s%s", ServerFullUrl, strUrl.c_str());

					mcu::tlog << L"Download Url:"<<DownloadFullUrl << endl;

					//调用系统Shell下载
					{
						//添加是为了不显示IE的历史记录
						SHELLEXECUTEINFO seInfo;
						memset( &seInfo, 0, sizeof( seInfo ) );
						seInfo.cbSize = sizeof( seInfo );
						seInfo.fMask = 0;
						seInfo.hwnd = this->GetSafeHwnd();

						//seInfo.lpVerb = _T( "Open" );
						seInfo.lpVerb = _T("Open");
						//seInfo.lpVerb = _T( "iexplore.exe %1" );
						//seInfo.lpFile = strUrl.c_str();
						//seInfo.lpFile = DownloadFullUrl;
						seInfo.lpFile = _T("IEXPLORE.EXE");
						seInfo.lpParameters = L"about:blank";
						BOOL bResult = ::ShellExecuteEx( &seInfo );
					}

					Sleep(1000);

					SHELLEXECUTEINFO seInfo;
					memset( &seInfo, 0, sizeof( seInfo ) );
					seInfo.cbSize = sizeof( seInfo );
					seInfo.fMask = 0;
					seInfo.hwnd = this->GetSafeHwnd();

					//seInfo.lpVerb = _T( "Open" );
					seInfo.lpVerb = _T("Open");
					//seInfo.lpVerb = _T( "iexplore.exe %1" );
					//seInfo.lpFile = strUrl.c_str();
					//seInfo.lpFile = DownloadFullUrl;
					seInfo.lpFile = _T("IEXPLORE.EXE");
					seInfo.lpParameters = DownloadFullUrl;
					BOOL bResult = ::ShellExecuteEx( &seInfo );					

					mcu::tlog << _T( "shell execute result: " ) << bResult << _T( " instapp: " ) << seInfo.hInstApp 
						<< _T( " process: " ) << seInfo.hProcess << endl;
				}
				else
				{
					mcu::tlog << L"服务器地址不是合法的HTTP地址:"<< endl;
				}

				//停止控件继续操作
				*pResult = 1;
				return TRUE;				
			}
			

			break;

		}
	case NM_META:
		{
			mcu::tlog << _T( "NM_META" ) << endl;
			tstring strHttpEquiv = UTF8toUTF16( (const char *)pnmHTMLView->szTarget );
			tstring strData = UTF8toUTF16( ( const char * ) pnmHTMLView->szData );

			mcu::tlog << _T( "meta httpequiv: " ) << strHttpEquiv << endl;
			mcu::tlog << _T( "meta data: " ) << strData << endl;

			//BOOL bResult = this->GetParent()->SendMessage( WM_HTML_WND_META, (WPARAM)strHttpEquiv.c_str(), (LPARAM)strData.c_str() );
			//return bResult;
		}
		break;
	case NM_BEFORENAVIGATE:
		{
			// 等待Navigate 完成.
			m_bWaittingNavigateComplete = TRUE;

			tstring strUrl = UTF8toUTF16( (const char *)pnmHTMLView->szTarget );

			mcu::tlog << _T( "NM_BEFORENAVIGATE: " ) << strUrl << endl;

			
//			MessageBox( strUrl.c_str(), TEXT( "你打开的URL链接：" ) );

			/** 记录历史. */
			if( INVALID_INDEX == m_nCurUrlHistoryIndex || ( strUrl.compare( m_tHistoryUrl[m_nCurUrlHistoryIndex] ) != 0 ) )
			{
				m_nCurUrlHistoryIndex ++;
				m_nCurUrlHistoryIndex %= MAX_HISTORY;
				if ( int( m_tHistoryUrl.size() ) <= m_nCurUrlHistoryIndex )
				{
					m_tHistoryUrl.push_back( strUrl );				
				}
				else
				{
					m_tHistoryUrl[m_nCurUrlHistoryIndex] = strUrl;
				}
				m_nHistoryEndIndex = m_nCurUrlHistoryIndex;

				if ( m_nHistoryHeadIndex == INVALID_INDEX )
				{
					m_nHistoryHeadIndex = m_nCurUrlHistoryIndex;
				}
				else if( m_nHistoryHeadIndex == m_nCurUrlHistoryIndex )
				{
					m_nHistoryHeadIndex ++;
					m_nHistoryHeadIndex %= MAX_HISTORY;
				}
			}

			BOOL bResult = this->GetParent()->SendMessage( WM_HTML_WND_PRE_LOAD, (WPARAM)strUrl.c_str() );

			return bResult;

		}
		break;

	case NM_NAVIGATECOMPLETE:
		{
			// Navigate 完成.
			tstring strUrl = UTF8toUTF16( (const char *)pnmHTMLView->szTarget );
			m_bWaittingNavigateComplete = FALSE;
			mcu::tlog << _T( "NM_NAVIGATECOMPLETE url: " ) << strUrl << endl;

			// 更新历史记录。
			if ( m_nCurUrlHistoryIndex != INVALID_INDEX )
			{
				this->m_tHistoryUrl[ m_nCurUrlHistoryIndex ] = strUrl;
			}
			
		}
		break;

	case NM_DOCUMENTCOMPLETE:
		if ( m_bWaittingNavigateComplete && m_bIsFaild == FALSE)
		{
			mcu::tlog << _T( "NM_DOCUMENTCOMPLETE not do NM_NAVIGATECOMPLETE" ) << endl;
			//tstring strUrl = UTF8toUTF16( (const char *)pnmHTMLView->szTarget );
			//mcu::tlog << _T( "m_bWaittingNavigateComplete = false Url " )<< strUrl << endl;
			tstring strModulePath = GetModulePath();
			tstring strDir = ParsePath( strModulePath.c_str() ).m_strDirectory;
			tstring strFailHtml = _T( "file://" ) + strDir + _T( "failhtml\\fail.htm" );
			m_bWaittingNavigateComplete = FALSE;
			this->OpenUrl( strFailHtml.c_str() );
			m_bIsFaild = TRUE;
		}
		else
		{
			m_bWaittingNavigateComplete = FALSE;
			m_bIsFaild = FALSE;
		}
		mcu::tlog << _T( "NM_DOCUMENTCOMPLETE" ) << endl;
		break;
	case NM_TITLE:
		mcu::tlog << _T( "NM_TITLE" ) <<endl;
		break;
	case NM_TITLECHANGE:
		mcu::tlog << _T( "NM_TITLECHANGE" ) << endl;
		break;
	case NM_INLINE_IMAGE:
		mcu::tlog << _T( "NM_INLINE_IMAGE" ) << endl;
		break;
	case NM_CONTEXTMENU:
		{
			//MessageBox( TEXT( "NM_CONTEXTMENU！" ) );
			NM_HTMLCONTEXT * pnmHTMLContext = (NM_HTMLCONTEXT *)lParam;
			HandlePopupMenu(pnmHTMLContext->pt);
			*pResult = 1;
			return TRUE;
			//return FALSE;
		}
		break;
	default:
		mcu::tlog << _T( "html notify: pnmHTMLView->hdr.code = " ) << pnmHTMLView->hdr.code << endl;
		break;
	}		

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CHtmlWnd::HistoryBack( int nStep /* = 1 */ )
{
	if ( INVALID_INDEX == m_nCurUrlHistoryIndex || m_nCurUrlHistoryIndex == m_nHistoryHeadIndex )
	{
		mcu::tlog << _T( "CHtmlWnd::HistoryBack : no history record!" ) << endl;
		return FALSE;
	}

	// 检查是否可以移动这些布。
	for ( int i=0; i<nStep; ++i )
	{
		if ( ( ( m_nCurUrlHistoryIndex - i ) % MAX_HISTORY ) == m_nHistoryHeadIndex )
		{
			mcu::tlog << _T( "CHtmlWnd::HistoryBack: History is shorter than back step!" ) << endl;
			return FALSE;
		}
	}

	m_nCurUrlHistoryIndex -= nStep;
	m_nCurUrlHistoryIndex %= MAX_HISTORY;

	tstring strTargetUrl = m_tHistoryUrl[ m_nCurUrlHistoryIndex ];
	mcu::tlog << _T( "Html window history Back " ) << nStep << _T( " step To : " ) << strTargetUrl << endl;

	return this->OpenUrl( strTargetUrl.c_str() );
}

BOOL CHtmlWnd::HistoryForward( int nStep /* = 1 */ )
{
	if ( INVALID_INDEX == m_nCurUrlHistoryIndex || m_nCurUrlHistoryIndex == m_nHistoryEndIndex )
	{
		return FALSE;
	}

	for ( int i=0; i<nStep; ++i )
	{
		if ( ( ( m_nCurUrlHistoryIndex + i ) % MAX_HISTORY ) == m_nHistoryEndIndex )
		{
			return FALSE;
		}
	}

	m_nCurUrlHistoryIndex += nStep;
	m_nCurUrlHistoryIndex %= MAX_HISTORY;

	tstring strTargetUrl = m_tHistoryUrl[ m_nCurUrlHistoryIndex ];
	mcu::tlog << _T( "Html window history Forward" ) << nStep << _T( " step To : " ) << strTargetUrl << endl;

	return this->OpenUrl( strTargetUrl.c_str() );
}

BOOL CHtmlWnd::IsCanBack(int nStep)
{
	if ( INVALID_INDEX == m_nCurUrlHistoryIndex || m_nCurUrlHistoryIndex == m_nHistoryHeadIndex )
	{
		mcu::tlog << _T( "CHtmlWnd::HistoryBack : no history record!" ) << endl;
		return FALSE;
	}

	// 检查是否可以移动这些布。
	for ( int i=0; i<nStep; ++i )
	{
		if ( ( ( m_nCurUrlHistoryIndex - i ) % MAX_HISTORY ) == m_nHistoryHeadIndex )
		{
			mcu::tlog << _T( "CHtmlWnd::HistoryBack: History is shorter than back step!" ) << endl;
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CHtmlWnd::IsCanForward(int nStep)
{
	if ( INVALID_INDEX == m_nCurUrlHistoryIndex || m_nCurUrlHistoryIndex == m_nHistoryEndIndex )
	{
		return FALSE;
	}

	for ( int i=0; i<nStep; ++i )
	{
		if ( ( ( m_nCurUrlHistoryIndex + i ) % MAX_HISTORY ) == m_nHistoryEndIndex )
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CHtmlWnd::Refresh()
{
	this->Clear();

	if( INVALID_INDEX != m_nCurUrlHistoryIndex )
	{
		this->OpenUrl( m_tHistoryUrl[ m_nCurUrlHistoryIndex ].c_str() );
	}
	return TRUE;
}

BOOL CHtmlWnd::Stop()
{
	return ::SendMessage( m_hHtmlCtrlWnd, DTM_STOP, NULL, NULL );
}

BOOL CHtmlWnd::Clear()
{
	return ::SendMessage( m_hHtmlCtrlWnd, DTM_CLEAR, NULL, NULL );
}

//LRESULT CHtmlWnd::HtmlProc( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam )
//{
//	mcu::tlog << _T( "HtmlProc: wnd: " ) << hWnd << _T( " msg: " )<< wMsg << _T( " wp: " ) << wParam << _T( " lp: " ) << lParam << endl;
//	if ( s_lpOldHtmlProc )
//	{
//		return s_lpOldHtmlProc( hWnd, wMsg, wParam, lParam );
//	}
//	else
//	{
//		return FALSE;
//	}
//}

void CHtmlWnd::ClearHistoryURL()
{
	m_nHistoryHeadIndex = INVALID_INDEX;
	m_nCurUrlHistoryIndex = INVALID_INDEX;
	m_nHistoryEndIndex = INVALID_INDEX;
	m_tHistoryUrl.clear();
}

BOOL CHtmlWnd::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( m_hHtmlCtrlWnd && IsWindow( m_hHtmlCtrlWnd ) )
	{
		::DestroyWindow( m_hHtmlCtrlWnd );
	}

	return CDialog::DestroyWindow();
}

void CHtmlWnd::HandlePopupMenu ( POINT point )

{
	CMenu cHtmlPopMenu;
	BOOL bRet = cHtmlPopMenu.LoadMenu(IDR_HTMLPOPMENU);
	if(bRet == TRUE)
	{
		//ClientToScreen(&point);
		
		CMenu* file_menu = cHtmlPopMenu.GetSubMenu(0);    
		ASSERT(file_menu);

		file_menu->EnableMenuItem(ID_HTMLPOPMENU_REFRESH, MF_ENABLED);
		file_menu->EnableMenuItem(ID_HTMLPOPMENU_PREPAGE, IsCanBack() ? MF_ENABLED : MF_DISABLED);
		file_menu->EnableMenuItem(ID_HTMLPOPMENU_NEXTPAGE, IsCanForward() ? MF_ENABLED : MF_DISABLED);

		file_menu->TrackPopupMenu(TPM_LEFTALIGN , point.x, point.y, this);
	}
	else
	{
		MessageBox( TEXT( "LoadMenu Failed！" ) );
	}
	return; //for completeness
}

BOOL CHtmlWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(wParam == ID_HTMLPOPMENU_REFRESH)
	{
		Refresh();
		return TRUE;
	}
	else if(wParam == ID_HTMLPOPMENU_PREPAGE)
	{
		HistoryBack(1);
		return TRUE;
	}
	else if(wParam == ID_HTMLPOPMENU_NEXTPAGE)
	{		
		HistoryForward(1);
		return TRUE;
	}
	else
	{
		return CDialog::OnCommand(wParam, lParam);
	}
}
