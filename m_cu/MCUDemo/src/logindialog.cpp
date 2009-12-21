// PPCLoginDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "LoginDialog.h"
#include "mculib.h"

#include "picmanagedialog.h"
#include "configdialog.h"
#include "webpagedlg.h"


//#define MCU_VERSION KDM_VER(MCU, 0101)
#define MCU_VERSION L"MCU V1.0 Beta1"

// CLoginDialog 对话框

IMPLEMENT_DYNAMIC(CLoginDialog, CBasePPCLoginDialog)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
: CBasePPCLoginDialog(CLoginDialog::IDD, pParent)
, m_strUserId(_T(""))
, m_strPassword(_T(""))
, m_bSaveLoginInfo(FALSE)
, m_b3GStream(FALSE)
{
}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
    CBasePPCLoginDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_UERID, m_strUserId);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
    DDX_Check(pDX, IDC_CHECK_SAVE_LOGIN_INFO, m_bSaveLoginInfo);
    DDX_Check(pDX, IDC_CHECK_3G_STREAM, m_b3GStream);
    //	DDX_Control(pDX, IDC_STATIC_LOGO, m_cLogo);
    //DDX_Control(pDX, IDC_STATIC_USER_ID_TITLE, m_stUserIdTitle);
    //DDX_Control(pDX, IDC_STATIC_PASSWORD_TITLE, m_stPasswordTitle);
    DDX_Control(pDX, IDC_STATIC_USER_ID_TITLE, m_stUserIdTitle);
    DDX_Control(pDX, IDC_STATIC_PASSWORD_TITLE, m_stPasswordTitle);
    DDX_Control(pDX, IDC_STATIC_MCUVER, m_stMCUVer);
    DDX_Control(pDX, IDC_CHECK_SAVE_LOGIN_INFO, m_ckSaveLoginInfo);
    DDX_Control(pDX, IDC_CHECK_3G_STREAM, m_ckStreamType);
    DDX_Control(pDX, IDC_BUTTON_LOGIN, m_btnLogin);
    DDX_Control(pDX, IDC_BUTTON_LOGIN_THIS_PHONE, m_btnLoginThisPhone);
    DDX_Control(pDX, IDC_STATIC_USERID_LOGO, m_stUserIdLogo);
    DDX_Control(pDX, IDC_STATIC_PASSWORD_LOGO, m_stPasswordLogo);
    DDX_Control(pDX, IDC_EDIT_UERID, m_edUserId);
    DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edPassword);
    DDX_Control(pDX, IDC_BUTTON_PIC_REC_MANAGE, m_btnPicRecManage);
    DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnConfig);
    DDX_Control(pDX, IDC_STATIC_BOTTOM_BG, m_stBottomBg);
    DDX_Control(pDX, IDC_BUTTON_SIP_BUTTON, m_btnSip);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CBasePPCLoginDialog)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDialog::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN_THIS_PHONE, &CLoginDialog::OnBnClickedButtonLoginThisPhone)
    ON_COMMAND(ID_MENU_LOGIN, &CLoginDialog::OnMenuLogin)
    ON_COMMAND(ID_MENU_CONFIG, &CLoginDialog::OnMenuConfig)
    ON_WM_SIZE()
    ON_COMMAND(ID_MENU_PIC, &CLoginDialog::OnMenuPic)
    ON_BN_CLICKED(IDC_BUTTON_PIC_REC_MANAGE, &CLoginDialog::OnBnClickedButtonPicRecManage)
    ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CLoginDialog::OnBnClickedButtonConfig)
    ON_BN_CLICKED(IDC_BUTTON_SIP_BUTTON, &CLoginDialog::OnBnClickedButtonSipButton)
    ON_WM_SETTINGCHANGE()
    ON_EN_SETFOCUS(IDC_EDIT_UERID, &CLoginDialog::OnEnSetfocusEditUerid)
    ON_EN_SETFOCUS(IDC_EDIT_PASSWORD, &CLoginDialog::OnEnSetfocusEditPassword)
    ON_EN_KILLFOCUS(IDC_EDIT_UERID, &CLoginDialog::OnEnKillfocusEditUerid)
    ON_EN_KILLFOCUS(IDC_EDIT_PASSWORD, &CLoginDialog::OnEnKillfocusEditPassword)
END_MESSAGE_MAP()


// CLoginDialog 消息处理程序

BOOL CLoginDialog::LoginIn( ELoginType eType )
{
    TLoginInfo tLoginInfo;
    tLoginInfo.m_strUserId = this->m_strUserId;
    tLoginInfo.m_strPassword = this->m_strPassword;
    tLoginInfo.m_bSaveLoginInfo = this->m_bSaveLoginInfo;
    tLoginInfo.m_eStreamType = this->m_b3GStream ? STREAM_EVDO : STREAM_1X;


    return this->Login( tLoginInfo, LOGIN_NORMAL );
}

void CLoginDialog::OnBnClickedButtonLogin()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData();
    SaveConfig();

    this->m_edUserId.SetFocus();

    this->LoginIn( LOGIN_NORMAL );

    // 清除焦点。
//    this->m_edUserId.SetFocus();

 /*   if ( this->CheckAndAlert( LOGIN_NORMAL ) )
    {
        m_eLoginType = LOGIN_NORMAL;
        EndDialog( LOGIN_NORMAL );
    }	*/
}

void CLoginDialog::OnBnClickedButtonLoginThisPhone()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData();
    SaveConfig();

    this->LoginIn( LOGIN_LOCAL_PHONE );

    //if( this->CheckAndAlert( LOGIN_LOCAL_PHONE ) )
    //{
    //    m_eLoginType = LOGIN_LOCAL_PHONE;
    //    EndDialog( LOGIN_LOCAL_PHONE );
    //}	
}

void CLoginDialog::OnOK()
{
//    m_eLoginType = LOGIN_CANCEL;
 //   EndDialog( LOGIN_CANCEL );

    __super::OnOK();
}

BOOL CLoginDialog::OnInitDialog()
{
    CBasePPCLoginDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    ReadConfig();

    // 设置背景图片。
    tstring strMainBkImg = _T( "main_bg.JPG" );
    this->SetImage( strMainBkImg.c_str(), FALSE );

    // logo
    m_logoDlg.Create( CLogoDialog::IDD, this );

    // 用户名，密码标题颜色。
    const COLORREF conTextColor = RGB( 255,255,255 );
    this->m_stUserIdTitle.SetTextColor( conTextColor, conTextColor );
    this->m_stPasswordTitle.SetTextColor( conTextColor, conTextColor );
    this->m_stMCUVer.SetTextColor( conTextColor, conTextColor );


    //TCHAR MCUVER[128];
    //mbtowc( MCUVER, MCU_VERSION, 128 );
    //MultiByteToWideChar( CP_ACP, 0, MCU_VERSION,
    //	strlen(MCU_VERSION)+1, MCUVER,   
    //	sizeof(MCUVER)/sizeof(MCUVER[0]) );


    this->m_stMCUVer.SetWindowText(MCU_VERSION);


    // 设置复选框颜色
    this->m_ckSaveLoginInfo.SetColor( RGB( 255,255,255 ), RGB( 0,0,0 ), FALSE );
    this->m_ckStreamType.SetColor( RGB( 255,255,255 ), RGB( 0,0,0 ), FALSE );

    // 为登录按钮设置图片。
    this->m_btnLogin.SetImage( _T( "btn_login_normal.jpg" ),  _T( "btn_login_focus.jpg" ),  _T( "btn_login_disable.jpg" ), FALSE );
    this->m_btnLogin.SetTransparentMask( TRUE, RGB( 255,0,255), FALSE );
    this->m_btnLogin.FitToPicture();

    // 一键登录。
    this->m_btnLoginThisPhone.SetImage( _T( "btn_login_direct_normal.jpg" ), _T( "btn_login_direct_focus.jpg" ), _T( "btn_login_direct_disable.jpg" ), FALSE );
    this->m_btnLoginThisPhone.SetTransparentMask( TRUE, RGB( 255,0,255 ), FALSE );
    this->m_btnLoginThisPhone.FitToPicture();

    // 用户Id的logo图片。
    tstring strStUseIdLogoImg = _T( "st_user_logo.jpg" );
    this->m_stUserIdLogo.SetPicPath( strStUseIdLogoImg.c_str(), FALSE );
    this->m_stUserIdLogo.SetTransparentMask( TRUE, RGB( 255,0,255 ), FALSE );
    this->m_stUserIdLogo.FitToPicture();

    // 密码的logo。
    tstring strStPasswordLogoImg = _T( "st_password_logo.jpg" );
    this->m_stPasswordLogo.SetPicPath( strStPasswordLogoImg.c_str(), FALSE );
    this->m_stPasswordLogo.SetTransparentMask( TRUE, RGB( 255,0,255 ), FALSE );
    this->m_stPasswordLogo.FitToPicture();

    // 底部菜单条及按钮。
    this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
    this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );
    this->m_btnPicRecManage.SetBkTransparent( FALSE, FALSE );
    this->m_btnPicRecManage.SetImage( _T( "btn_pic_normal.jpg" ), _T( "btn_pic_focus.jpg" ), _T( "btn_pic_disable.jpg" ), FALSE );
    this->m_btnConfig.SetBkTransparent( FALSE, FALSE );
    this->m_btnConfig.SetImage( _T( "btn_config_normal.jpg" ), _T( "btn_config_focus.jpg" ), _T( "btn_config_normal.jpg" ), FALSE );

    // 输入法控制按钮。
    this->m_btnSip.SetBkTransparent( FALSE, FALSE );
    this->m_btnSip.SetImage( _T( "btn_sip_normal.jpg" ), _T( "btn_sip_focus.jpg" ), _T( "btn_sip_disable.jpg" ), FALSE );


    // 设置标题。
    tstring strTitle;
    BOOL bResult = CConfig::Instance()->GetAppTitle( strTitle );
    if ( !bResult )
    {
        Log() << _T( "Get App title fail!" ) << endl;
        strTitle = _T( "MCU" ) ;
    }
    this->SetWindowText( strTitle.c_str() );

    this->FullScreen( FS_HideMenuBar | FS_HideSipButton );


    this->UpdateLayout();

    m_logoDlg.ShowWindow( SW_SHOW );

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CLoginDialog::ReadConfig()
{
    // 读取储存的登录信息。
    BOOL bSaveLoginInfo;
    CConfig::Instance()->GetIsSave( bSaveLoginInfo );
    m_bSaveLoginInfo = bSaveLoginInfo;

    if ( m_bSaveLoginInfo )
    {
        tstring strUser, strPw;
        EStreamType eStreamType;
        CConfig::Instance()->GetLoginInfo( strUser, strPw, eStreamType );

        this->m_strUserId = strUser.c_str();
        this->m_strPassword = strPw.c_str();
        this->m_b3GStream = !( STREAM_1X == eStreamType );
    }	

    UpdateData( FALSE );
}

void CLoginDialog::SaveConfig()
{
    UpdateData();

    CConfig::Instance()->SetIsSave( TRUE == m_bSaveLoginInfo );
    CConfig::Instance()->SetLoginInfo( (LPCTSTR)m_strUserId, (LPCTSTR)m_strPassword, m_b3GStream ? STREAM_EVDO : STREAM_1X );


}
void CLoginDialog::OnMenuLogin()
{
    // TODO: 在此添加命令处理程序代码
    this->OnBnClickedButtonLogin();
}

void CLoginDialog::OnMenuConfig()
{
    // TODO: 在此添加命令处理程序代码
    this->OnConfig();
}

void CLoginDialog::OnSize(UINT nType, int cx, int cy)
{
    CBasePPCLoginDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    this->UpdateLayout( &CRect(0,0,cx,cy) );
}

void CLoginDialog::UpdateLayout( LPCRECT prcClient /* = NULL */ )
{
    if ( !GetSafeHwnd() )
    {
        return;
    }

    CRect rcClient;
    if( prcClient )
    {
        rcClient = *prcClient;
    }
    else
    {
        this->GetClientRect( rcClient );
    }

    // 屏幕大小。
    int nScreenLong = GetScreenLong();

    // 剩余的空间。
    CRect rcLeftSpace = rcClient;

    // logo 
    if( m_logoDlg.GetSafeHwnd() )
    {
        int nLogoHeight = m_logoDlg.GetHeight();
        m_logoDlg.MoveWindow( 0,0, rcClient.Width(), nLogoHeight );

        // 在剩余空间中去除logo占用的空间。
        rcLeftSpace.top += nLogoHeight;
    }

    // 调整菜单条的位置。
    if ( m_stBottomBg.GetSafeHwnd() && m_btnPicRecManage.GetSafeHwnd() && m_btnConfig.GetSafeHwnd() && m_btnSip.GetSafeHwnd() )
    {
        // 底部按钮条（替代以前的菜单）的高度	
        const float conFMenubarHeight = 0.082;
        BOOL bResult = FALSE;
        int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

        CRect rcBottomBg = rcClient;
        rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
        m_stBottomBg.MoveWindow( rcBottomBg );


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

        CRect rcBtnDelete;
        bResult = this->m_btnPicRecManage.GetImageSize( rcBtnDelete );
        if ( bResult )
        {
            int nNewWidth = nMenubarHeight * rcBtnDelete.Width() / rcBtnDelete.Height();
            rcBtnDelete = rcBottomBg;
            rcBtnDelete.right = rcBtnDelete.left + nNewWidth;
            this->m_btnPicRecManage.SetWindowPos( &CWnd::wndTop, rcBtnDelete.left, rcBtnDelete.top, rcBtnDelete.Width(), rcBtnDelete.Height(), NULL );
        }


        // 在剩余空间中去除菜单条占用的空间。
        rcLeftSpace.bottom -= nMenubarHeight;
    }

    // 暂时不考虑登录对话框对横屏模式的适应。
    // 登录对话框中所有控件的大小和屏幕分辨率成正比。
    // 剩余的控件相对位置固定，相对距离同屏幕分辨率成正比。
    // 所有控件组合后在屏幕上居中。

    // 所有控件所在集合（层）与屏幕长度的比率。
    const float conFDivHeightRadio = 0.5;
    const float conFDivWidthRadio = 0.6;

    // 整个层的客户区（相对大小）。
    CRect rcDivClient(0, 0, int( nScreenLong * conFDivWidthRadio ), int( nScreenLong * conFDivHeightRadio ) );

    // 得到整个层在登录窗口上的相对位置。

    // 首先得到这个层相对应的空间。这个层在这块空间上居中。
    CRect rcDivSpace = rcLeftSpace;

    // 移动。
    CPoint offset = rcDivSpace.CenterPoint() - rcDivClient.CenterPoint();	
    CRect rcDivWnd = rcDivClient;
    rcDivWnd.OffsetRect( offset );

    // 在刚刚得到的层中排列图标。


    // 每行的高度与屏幕长度比。
    const float conTRHeightRadio = 0.072;
    // 空白行高度。
    const float conFBlankHeight = 0.036;


    // 第一行。 用户名logo，用户名标题，用户名输入。

    CRect rcUserLine = rcDivWnd;
    rcUserLine.bottom = rcUserLine.top + int( nScreenLong * conTRHeightRadio );

    const float conFUseridLogoWidthRadio = 0.041;
    const float conFUseridLogoHeightRadio = 0.041;
    int nUseridLogoHeight = int( nScreenLong * conFUseridLogoHeightRadio );
    CRect rcUseridLogo = rcUserLine;
    rcUseridLogo.top = ( rcUserLine.bottom + rcUserLine.top - nUseridLogoHeight ) / 2;
    rcUseridLogo.right = rcUseridLogo.left + int( nScreenLong * conFUseridLogoWidthRadio );
    rcUseridLogo.bottom = rcUseridLogo.top + nUseridLogoHeight;
    if ( m_stUserIdLogo.GetSafeHwnd() )
    {
        m_stUserIdLogo.MoveWindow( rcUseridLogo );
    }

    const float conFUseridInputWidthRadio = 0.372;
    CRect rcUseridInput = rcUserLine;
    rcUseridInput.left = rcUseridInput.right - int( nScreenLong * conFUseridInputWidthRadio );
    if ( this->m_edUserId.GetSafeHwnd() )
    {		
        m_edUserId.MoveWindow( rcUseridInput );
    }

    if ( this->m_stUserIdTitle.GetSafeHwnd() )
    {
        CRect rcUserIdTitle = rcUserLine;
        rcUserIdTitle.left = rcUseridLogo.right;
        rcUserIdTitle.right = rcUseridInput.left;
        this->m_stUserIdTitle.MoveWindow( rcUserIdTitle );
    }

    // 第二行密码。

    CRect rcPasswordLine = rcDivWnd;
    rcPasswordLine.top = rcUserLine.bottom + int( nScreenLong * conFBlankHeight );
    rcPasswordLine.bottom = rcPasswordLine.top + int ( nScreenLong * conTRHeightRadio );

    const float conFPasswordLogoWidthRadio = 0.041;
    const float conFPasswordLogoHeightRadio = 0.041;
    int nPasswordLogoHeight = int( nScreenLong * conFPasswordLogoHeightRadio );
    CRect rcPasswordLogo = rcPasswordLine;
    rcPasswordLogo.top = ( rcPasswordLine.bottom + rcPasswordLine.top - nPasswordLogoHeight ) / 2;
    rcPasswordLogo.right = rcPasswordLogo.left + int( nScreenLong * conFPasswordLogoWidthRadio );
    rcPasswordLogo.bottom = rcPasswordLogo.top + nPasswordLogoHeight;
    if ( m_stPasswordLogo.GetSafeHwnd() )
    {
        m_stPasswordLogo.MoveWindow( rcPasswordLogo );
    }

    const float conFPasswordInputWidthRadio = 0.372;
    CRect rcPasswordInput = rcPasswordLine;
    rcPasswordInput.left = rcPasswordInput.right - int( nScreenLong * conFPasswordInputWidthRadio );
    if ( this->m_edPassword.GetSafeHwnd() )
    {		
        m_edPassword.MoveWindow( rcPasswordInput );
    }

    if ( this->m_stPasswordTitle.GetSafeHwnd() )
    {
        CRect rcPasswordTitle = rcPasswordLine;
        rcPasswordTitle.left = rcPasswordLogo.right;
        rcPasswordTitle.right = rcUseridInput.left;
        this->m_stPasswordTitle.MoveWindow( rcPasswordTitle );
    }

    // 第三行。复选框，左边位置固定，右边不限，宽度不变。
    const float conFLeftBord = 0.11;
    int nLeftPos = rcDivWnd.left + int( conFLeftBord * nScreenLong );
    CRect rcCkSaveLine = rcDivWnd;
    rcCkSaveLine.top = rcPasswordLine.bottom + int( nScreenLong * conFBlankHeight );
    rcCkSaveLine.bottom = rcCkSaveLine.top + int ( nScreenLong * conTRHeightRadio );
    if( m_ckSaveLoginInfo.GetSafeHwnd() )
    {
        CRect rcCkSave = rcCkSaveLine;
        rcCkSave.left = nLeftPos;
        m_ckSaveLoginInfo.MoveWindow( rcCkSave );
    }

    // 第四行。
    CRect rcCkStreamTypeLine = rcDivWnd;
    rcCkStreamTypeLine.top = rcCkSaveLine.bottom;
    rcCkStreamTypeLine.bottom = rcCkStreamTypeLine.top + int( nScreenLong * conTRHeightRadio );
    if ( m_ckStreamType.GetSafeHwnd() )
    {
        CRect rcCkStreamType = rcCkStreamTypeLine;
        rcCkStreamType.left = nLeftPos;
        m_ckStreamType.MoveWindow( rcCkStreamType );
    }

    // 第五行。
    CRect rcCkMCUVerLine = rcDivWnd;
    rcCkMCUVerLine.top = rcCkStreamTypeLine.bottom;
    rcCkMCUVerLine.bottom = rcCkMCUVerLine.top + int( nScreenLong * conTRHeightRadio );
    if ( this->m_stMCUVer.GetSafeHwnd() )
    {
        CRect rcCkStreamType = rcCkMCUVerLine;
        rcCkStreamType.left = 70;
        this->m_stMCUVer.MoveWindow( rcCkStreamType );
    }

    // 第六行，按钮。
    // 按钮贴下面,两侧分开对齐，按钮按照图片高宽比伸缩。
    const float conFBtnHeight = 0.076;
    int nBtnHeight = int( nScreenLong * conFBtnHeight );

    if ( m_btnLogin.GetSafeHwnd() )
    {
        CRect rcLogin;
        this->m_btnLogin.GetWindowRect( rcLogin );
        CRect rcNewLogin = rcDivWnd;
        rcNewLogin.top = rcNewLogin.bottom - nBtnHeight;
        rcNewLogin.right = rcNewLogin.left + rcLogin.Width() * nBtnHeight / rcLogin.Height() ;

        m_btnLogin.MoveWindow( rcNewLogin );
    }

    if ( m_btnLoginThisPhone.GetSafeHwnd() )
    {
        CRect rcLoginDirectly;
        this->m_btnLoginThisPhone.GetWindowRect( rcLoginDirectly );
        CRect rcNewLoginDir = rcDivWnd;
        rcNewLoginDir.top = rcNewLoginDir.bottom - nBtnHeight;
        rcNewLoginDir.left = rcNewLoginDir.right - rcLoginDirectly.Width() * nBtnHeight / rcLoginDirectly.Height();

        m_btnLoginThisPhone.MoveWindow( rcNewLoginDir );
    }

    if ( this->IsWindowVisible() )
    {
//        this->m_edUserId.SetFocus();
    }
}
void CLoginDialog::OnMenuPic()
{
    // TODO: 在此添加命令处理程序代码
    this->OnPicManage();	
}

void CLoginDialog::OnPicManage()
{
    //CPicManageDialog dlg;
    //dlg.DoModal();
    CWindowFactory::Instance()->ShowWindow( WndPicManage, this->GetWindowId() );
}

void CLoginDialog::OnConfig()
{
    CWindowFactory::Instance()->ShowWindow( WndConfig, this->GetWindowId() );
    //CConfigDialog dlg;
    //dlg.DoModal();
    //CSysCfgPage syscfg;

    //CConfigPropertySheet sheet( _T( "设置" ), this );
    //sheet.AddPage( &syscfg );
    //sheet.DoModal();
}
void CLoginDialog::OnBnClickedButtonPicRecManage()
{
    // TODO: 在此添加控件通知处理程序代码
    this->OnPicManage();
}



void CLoginDialog::OnBnClickedButtonConfig()
{
    // TODO: 在此添加控件通知处理程序代码
    this->OnConfig();
}

void CLoginDialog::OnBnClickedButtonSipButton()
{
    // TODO: 在此添加控件通知处理程序代码	
    if ( this->IsSipRaise() )
    {
        LowerSip();
    }
    else
    {
        RaiseSip();
    }
}

void CLoginDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    // 当输入法弹出时，不调整对话框大小。
    if (SPI_SETSIPINFO == uFlags){
        SHACTIVATEINFO sai;
        memset(&sai, 0, sizeof(SHACTIVATEINFO));
        SHHandleWMSettingChange( GetSafeHwnd(), uFlags, (LPARAM)lpszSection, &sai);

        return;
    }

    CUIDialog::OnSettingChange(uFlags, lpszSection);

    // TODO: 在此处添加消息处理程序代码
}

void CLoginDialog::OnClickLeftFunKey()
{
    this->OnPicManage();
}

void CLoginDialog::OnClickRightFunKey()
{
    this->OnConfig();
}

BOOL CLoginDialog::CheckAndAlert( ELoginType eLoginType )
{
    if ( this->m_strUserId.IsEmpty() && eLoginType != LOGIN_LOCAL_PHONE )
    {
        MessageBox( _T( "帐号不能为空！" ) );
        return FALSE;
    }

    if( this->m_strPassword.IsEmpty() && eLoginType != LOGIN_LOCAL_PHONE )
    {
        MessageBox( _T( "密码不能为空！" ) );
        return FALSE;
    }



    tstring strServer;
    CConfig::Instance()->GetServer( strServer );
    // msp的地址不能为空。
    if ( strServer.empty() )
    {
        MessageBox( _T( "服务器地址没有设置！" ) );
        this->OnBnClickedButtonConfig();
        return FALSE;
    }

    tstring strRegion;
    CConfig::Instance()->GetRegionId( strRegion );
    if ( strRegion.empty() && eLoginType != LOGIN_LOCAL_PHONE )
    {
        MessageBox( _T( "客户域名没有设置！" ) );
        this->OnBnClickedButtonConfig();
        return FALSE;
    }


    return TRUE;
}

void CLoginDialog::OnEnSetfocusEditUerid()
{
    // TODO: 在此添加控件通知处理程序代码

    BOOL bRaise = this->IsSipRaise();
    if ( !bRaise )
    {
        this->RaiseSip();
    }
}

void CLoginDialog::OnEnSetfocusEditPassword()
{
    // TODO: 在此添加控件通知处理程序代码

    BOOL bRaise = this->IsSipRaise();
    if ( !bRaise )
    {
        this->RaiseSip();
    }
}

void CLoginDialog::OnEnKillfocusEditUerid()
{
    // TODO: 在此添加控件通知处理程序代码
    BOOL bRaise = this->IsSipRaise();
    if ( bRaise )
    {
        this->LowerSip();
    }
}

void CLoginDialog::OnEnKillfocusEditPassword()
{
    // TODO: 在此添加控件通知处理程序代码
    BOOL bRaise = this->IsSipRaise();
    if ( bRaise )
    {
        this->LowerSip();
    }
}

BOOL CLoginDialog::OpenWebpage( LPCTSTR strLoginUrl )
{
    BOOL bResult = TRUE;

    bResult &= CWindowFactory::Instance()->ShowWindow( WndWebpage, this->GetWindowId() );

    CWebpageDlg *pWebpageDlg = dynamic_cast< CWebpageDlg * >( CWindowFactory::Instance()->GetWnd( WndWebpage ) );
    if ( pWebpageDlg )
    {
        bResult &= pWebpageDlg->OpenUrl( strLoginUrl );
    }
    else
    {
        bResult = FALSE;
    }

    return bResult;
}

void CLoginDialog::AssertError( ELoginInfoErrorCode eErrCode )
{
    switch( eErrCode )
    {
    case ErrorUserIdEmpty:
        MessageBox( _T( "帐号不能为空！" ) );
    	break;
    case ErrorPasswordEmpty:
        MessageBox( _T( "密码不能为空！" ) );
        break;
    case ErrorServerUrlEmpty:
        MessageBox( _T( "服务器地址没有设置！" ) );
        this->OnBnClickedButtonConfig();
        break;
    case ErrorRegionEmpty:
        MessageBox( _T( "客户域名没有设置！" ) );
        this->OnBnClickedButtonConfig();
        break;
    case ErrorServerUrlInvalid:
        MessageBox( _T( "服务器地址不是合法的HTTP地址！" ) );
        this->OnBnClickedButtonConfig();
        break;
    default:
        break;
    }

}

