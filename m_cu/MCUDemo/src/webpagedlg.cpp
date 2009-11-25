// E:\works\mcu_v5r1b3\KDM50_VOB\A0-3G\m_cu\MCUDemo\src\webpagedlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "webpagedlg.h"


// CWebpageDlg 对话框

IMPLEMENT_DYNAMIC(CWebpageDlg, CUIDialog)

CWebpageDlg::CWebpageDlg(CWnd* pParent /*=NULL*/)
	: CUIDialog(CWebpageDlg::IDD, pParent)
{

}

CWebpageDlg::~CWebpageDlg()
{
}

void CWebpageDlg::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWebpageDlg, CUIDialog)
    ON_WM_CLOSE()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CWebpageDlg 消息处理程序

BOOL CWebpageDlg::OnInitDialog()
{
    CUIDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    BOOL bResult = CreateBrowserCtrl();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

BOOL CWebpageDlg::CreateBrowserCtrl()
{
    HWND hWnd = m_browserCtrl.Create( this->GetSafeHwnd(), 
        CWindow::rcDefault, 
        _T( "MCU Browser" ) );   

    return ( NULL != hWnd );
}

BOOL CWebpageDlg::OpenUrl( LPCTSTR strUrl )
{

    return m_browserCtrl.OpenUrl( strUrl );
}

BOOL CWebpageDlg::HistoryBack( int nStep )
{
    return FALSE;
}

BOOL CWebpageDlg::OpenPlayer( CVideoSession *pVideoSession )
{
    return FALSE;
}

void CWebpageDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类

 //   __super::OnOK();

    this->ShowWindow( SW_HIDE );
}

void CWebpageDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    __super::OnClose();
}

void CWebpageDlg::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);

    CRect rcClient( 0,0,cx,cy );
    this->UpdateLayout( &rcClient );

    // TODO: 在此处添加消息处理程序代码
}

void CWebpageDlg::UpdateLayout( CRect *prcClient /* = NULL */ )
{
    if ( !GetSafeHwnd() )
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
        GetClientRect( rcClient ); 
    }

    if ( m_browserCtrl.IsWindow() )
    {
        m_browserCtrl.MoveWindow( rcClient );
    }
}

