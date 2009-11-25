// E:\works\mcu_v5r1b3\KDM50_VOB\A0-3G\m_cu\MCUDemo\src\webpagedlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcu.h"
#include "webpagedlg.h"


// CWebpageDlg �Ի���

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


// CWebpageDlg ��Ϣ�������

BOOL CWebpageDlg::OnInitDialog()
{
    CUIDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    BOOL bResult = CreateBrowserCtrl();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
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
    // TODO: �ڴ����ר�ô����/����û���

 //   __super::OnOK();

    this->ShowWindow( SW_HIDE );
}

void CWebpageDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    __super::OnClose();
}

void CWebpageDlg::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);

    CRect rcClient( 0,0,cx,cy );
    this->UpdateLayout( &rcClient );

    // TODO: �ڴ˴������Ϣ����������
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

