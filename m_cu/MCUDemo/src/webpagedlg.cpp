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

    DDX_Control(pDX, IDC_BUTTON_PIC, m_btnPic);
    DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnConfig);
    DDX_Control(pDX, IDC_STATIC_BOTTOM_BG, m_stBottomBg);
    DDX_Control(pDX, IDC_BUTTON_SIP_BUTTON, m_btnSip);
}


BEGIN_MESSAGE_MAP(CWebpageDlg, CUIDialog)
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_PIC, &CWebpageDlg::OnBnClickedButtonPic)
    ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CWebpageDlg::OnBnClickedButtonConfig)
    ON_BN_CLICKED(IDC_BUTTON_SIP_BUTTON, &CWebpageDlg::OnBnClickedButtonSipButton)
    ON_MESSAGE( WM_HTML_CREATE_CMD, &CWebpageDlg::OnCreateBrowserCtrlCmd )
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CWebpageDlg ��Ϣ�������

BOOL CWebpageDlg::OnInitDialog()
{
    CUIDialog::OnInitDialog();

    
    BOOL bResult = SHDoneButton( FALSE );
    mcu::log << _T( "SHDoneButton result: " ) << bResult << endl;

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    bResult = CreateBrowserCtrl();

    // logo
    m_logoDlg.Create( CLogoDialog::IDD, this );
    m_logoDlg.ShowWindow( SW_SHOW );

    // �ײ��˵�������ť��
    this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
    this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

    this->m_btnPic.SetBkTransparent( FALSE, FALSE );
    this->m_btnPic.SetImage( _T( "btn_pic_normal.jpg" ), _T( "btn_pic_focus.jpg" ), _T( "btn_pic_disable.jpg.jpg" ), FALSE );

    this->m_btnConfig.SetBkTransparent( FALSE, FALSE );
    this->m_btnConfig.SetImage( _T( "btn_config_normal.jpg" ), _T( "btn_config_focus.jpg" ), _T( "btn_config_normal.jpg" ), FALSE );

    // ���뷨���ư�ť��
    this->m_btnSip.SetBkTransparent( FALSE, FALSE );
    this->m_btnSip.SetImage( _T( "btn_sip_normal.jpg" ), _T( "btn_sip_focus.jpg" ), _T( "btn_sip_disable.jpg" ), FALSE );


    this->UpdateLayout();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CWebpageDlg::CreateBrowserCtrl()
{
    return this->PostMessage( WM_HTML_CREATE_CMD );
}

LRESULT CWebpageDlg::OnCreateBrowserCtrlCmd( WPARAM, LPARAM )
{
    HWND hWnd = m_browserCtrl.Create( NULL, /*this->GetSafeHwnd(),*/ 
        CWindow::rcDefault, 
        _T( "MCU Browser" ) );   

    BOOL bResult = ( hWnd != NULL );
    if ( bResult )
    {
        m_browserCtrl.SetWebpageParentWnd( GetSafeHwnd() );
        m_browserCtrl.ShowWindow( SW_SHOW );
        this->UpdateLayout();
    }
    else
    {
        int err = ::GetLastError();
        mcu::log << _T( "Create web browser ctrl fail! errcode: " ) << err << endl;
    }
    

    return bResult;
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
    if( IDOK ==  MessageBox( _T( "���Ƿ��˳���¼��" ), _T( "��ʾ" ), MB_OKCANCEL ) )
    {
        //EWindowId eLoginAfter = WndInvalid;
        //CUIDialog *pLoginDlg = CWindowFactory::Instance()->GetWnd( WndLogin );
        //if ( pLoginDlg )
        //{
        //    eLoginAfter = pLoginDlg->GetWndAfterClose();
        //}
//        CWindowFactory::Instance()->ShowWindow( WndLogin, eLoginAfter ); 
         __super::OnOK();                
//        this->ShowWindow( SW_HIDE );
    }
    else
    {
        return;
    }   
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



    // ʣ��Ŀռ䡣
    CRect rcLeftSpace = rcClient;

    // ��Ļ��С��	
    int nScreenLong = ::GetScreenLong();

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
    if ( m_stBottomBg.GetSafeHwnd() && m_btnPic.GetSafeHwnd() && m_btnConfig.GetSafeHwnd() )
    {
        // �ײ���ť���������ǰ�Ĳ˵����ĸ߶�	
        const float conFMenubarHeight = 0.082;
        int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

        CRect rcBottomBg = rcClient;
        rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
        //m_stBottomBg.MoveWindow( rcBottomBg );
        this->m_stBottomBg.SetWindowPos( &CWnd::wndTop, rcBottomBg.left, rcBottomBg.top, rcBottomBg.Width(), rcBottomBg.Height(), NULL );

        CRect rcBtnDelete;
        BOOL bResult = this->m_btnPic.GetImageSize( rcBtnDelete );
        if ( bResult )
        {
            int nNewWidth = nMenubarHeight * rcBtnDelete.Width() / rcBtnDelete.Height();
            rcBtnDelete = rcBottomBg;
            rcBtnDelete.right = rcBtnDelete.left + nNewWidth;
            this->m_btnPic.SetWindowPos( &CWnd::wndTop, rcBtnDelete.left, rcBtnDelete.top, rcBtnDelete.Width(), rcBtnDelete.Height(), NULL );
        }

        CRect rcBtnReturn;
        bResult = this->m_btnConfig.GetImageSize( rcBtnReturn );
        if ( bResult )
        {
            int nNewWidth = nMenubarHeight * rcBtnReturn.Width() / rcBtnReturn.Height();
            rcBtnReturn = rcBottomBg;
            rcBtnReturn.left = rcBtnReturn.right - nNewWidth;
            this->m_btnConfig.SetWindowPos( &CWnd::wndTop, rcBtnReturn.left, rcBtnReturn.top, rcBtnReturn.Width(), rcBtnReturn.Height(), NULL );
        }

        CRect rcBtnSip;
        bResult = this->m_btnSip.GetImageSize( rcBtnSip );
        if ( bResult )
        {
            int nNewWidth = nMenubarHeight * rcBtnSip.Width() / rcBtnSip.Height();
            rcBtnSip = rcBottomBg;
            rcBtnSip.left = rcBottomBg.left + ( rcBottomBg.Width() - nNewWidth ) / 2;
            rcBtnSip.right = rcBtnSip.left + nNewWidth;
            this->m_btnSip.SetWindowPos( &CWnd::wndTop, rcBtnSip.left, rcBtnSip.top, rcBtnSip.Width(), rcBtnSip.Height(), NULL );
        }

        // ��ʣ��ռ���ȥ���˵���ռ�õĿռ䡣
        rcLeftSpace.bottom -= nMenubarHeight;
    }


    if ( m_browserCtrl.m_hWnd && m_browserCtrl.IsWindow() )
    {
        CRect rcHtml = rcLeftSpace;
        HWND hParent = ::GetParent( m_browserCtrl.m_hWnd );
        if ( hParent == this->GetSafeHwnd() )
        {
            m_browserCtrl.MoveWindow( rcHtml );
        }
        else
        {            
            this->ClientToScreen( rcHtml );
            CPoint ptHtmlLeftTop = rcHtml.TopLeft();
            if ( hParent )
            {
                ::ScreenToClient( hParent, &ptHtmlLeftTop );
            }
            m_browserCtrl.MoveWindow( ptHtmlLeftTop.x, ptHtmlLeftTop.y, rcHtml.Width(), rcHtml.Height() );
        }        
    }	
}

void CWebpageDlg::OnBnClickedButtonPic()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CWindowFactory::Instance()->ShowWindow( WndPicManage, this->GetWindowId() );
}

void CWebpageDlg::OnBnClickedButtonConfig()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CWindowFactory::Instance()->ShowWindow( WndConfig, this->GetWindowId() );
}

void CWebpageDlg::OnBnClickedButtonSipButton()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if ( this->IsSipRaise() )
    {
        this->LowerSip();
    }
    else
    {
        this->RaiseSip();
    }
}

void CWebpageDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    // �����뷨����ʱ���������Ի����С��
    if (SPI_SETSIPINFO == uFlags){
        SHACTIVATEINFO sai;
        memset(&sai, 0, sizeof(SHACTIVATEINFO));
        SHHandleWMSettingChange( GetSafeHwnd(), uFlags, (LPARAM)lpszSection, &sai);

        return;
    }

    CUIDialog::OnSettingChange(uFlags, lpszSection);

    // TODO: �ڴ˴������Ϣ����������
}

void CWebpageDlg::OnShowWindowCmd( int nSWCmd )
{
    if ( m_browserCtrl.m_hWnd  )
    {
        this->m_browserCtrl.ShowWindow( nSWCmd );
    }
    
    // ������Ī����������⡣    
    return;
    if ( SW_HIDE == nSWCmd )
    {
        LPCTSTR strWaitPage = _T( "htmldoc\\wait.htm" );//_T( "wait.htm" );
        tstring strModulePath = GetModulePath();
        tstring strDir = ParsePath( strModulePath.c_str() ).m_strDirectory;
        tstring strFailHtml = _T( "file://" ) + strDir + strWaitPage;
        this->OpenUrl( strFailHtml.c_str() );
    }
}




void CWebpageDlg::OnDestroy()
{
    this->m_browserCtrl.DestroyWindow();

    __super::OnDestroy();

    // TODO: �ڴ˴������Ϣ����������
}
