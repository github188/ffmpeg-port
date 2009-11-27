// src\configdialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "configdialog.h"
#include "config.h"
#include "mcucommon.h"
#include "dirselectdialog.h"

// CConfigDialog 对话框

IMPLEMENT_DYNAMIC(CConfigDialog, CUIDialog)

CConfigDialog::CConfigDialog(CWnd* pParent /*=NULL*/)
	: CUIDialog(CConfigDialog::IDD, pParent)
	, m_strServerUrl(_T(""))
	, m_bShowLoginDialog(FALSE)
	, m_nPtzSpeed(0)
	, m_nLenSpeed(0)
	, m_strRegionId(_T(""))
{
}

CConfigDialog::~CConfigDialog()
{
}

void CConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CUIDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServerUrl);
	DDX_Check(pDX, IDC_CHECK_SHOW_LOGIN_DIALOG, m_bShowLoginDialog);
	DDX_Control(pDX, IDC_SLIDER_PTZ_SPEED, m_sliderPtzSpeed);
	DDX_Control(pDX, IDC_SLIDER_LEN_SPEED, m_sliderLenSpeed);
	DDX_Text(pDX, IDC_STATIC_PTZ_SPEED, m_nPtzSpeed);
	DDV_MinMaxInt(pDX, m_nPtzSpeed, 0, 10);
	DDX_Text(pDX, IDC_STATIC_LEN_SPEED, m_nLenSpeed);
	DDV_MinMaxInt(pDX, m_nLenSpeed, 0, 10);
	DDX_Text(pDX, IDC_EDIT_USER_REGION, m_strRegionId);
	DDX_Control(pDX, IDC_STATIC_SERVER_ADDR_TITLE, m_stServerAddrTitle);
	DDX_Control(pDX, IDC_STATIC_CLIENT_ID_TITLE, m_stClientIdTitle);
	DDX_Control(pDX, IDC_STATIC_IMAGE_DIRECTRY_TITLE, m_stCaptureDirTitle);
	DDX_Control(pDX, IDC_STATIC_PTZ_SPEED_TITLE, m_stPTZSpeedTitle);
	DDX_Control(pDX, IDC_STATIC_LEN_SPEED_TITLE, m_stLenSpeedTitle);
	DDX_Control(pDX, IDC_STATIC_PTZ_SPEED, m_stPTZSpeedValue);
	DDX_Control(pDX, IDC_STATIC_LEN_SPEED, m_stLenSpeedValue);
	DDX_Control(pDX, IDC_COMBO_PIC_SAVE_DIR, m_cmbPicSaveDir);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_RETURN, m_btnReturn);
	DDX_Control(pDX, IDC_STATIC_BOTOOM_BG, m_stBottomBg);
	DDX_Control(pDX, IDC_BUTTON_SIP_BUTTON, m_btnSip);
}


BEGIN_MESSAGE_MAP(CConfigDialog, CUIDialog)
	ON_COMMAND(ID_OK, &CConfigDialog::OnOk)
	ON_COMMAND(ID_CANCEL, &CConfigDialog::OnCancel)
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CConfigDialog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_RETURN, &CConfigDialog::OnBnClickedButtonReturn)
	ON_WM_SETTINGCHANGE()
	ON_BN_CLICKED(IDC_CHECK_SHOW_LOGIN_DIALOG, &CConfigDialog::OnBnClickedCheckShowLoginDialog)
	ON_BN_CLICKED(IDC_BUTTON_SIP_BUTTON, &CConfigDialog::OnBnClickedButtonSipButton)
	ON_CBN_CLOSEUP(IDC_COMBO_PIC_SAVE_DIR, &CConfigDialog::OnCbnCloseupComboPicSaveDir)
	ON_CBN_SELENDOK(IDC_COMBO_PIC_SAVE_DIR, &CConfigDialog::OnCbnSelendokComboPicSaveDir)
	ON_CBN_DROPDOWN(IDC_COMBO_PIC_SAVE_DIR, &CConfigDialog::OnCbnDropdownComboPicSaveDir)
	ON_EN_SETFOCUS(IDC_EDIT_SERVER, &CConfigDialog::OnEnSetfocusEditServer)
	ON_EN_SETFOCUS(IDC_EDIT_USER_REGION, &CConfigDialog::OnEnSetfocusEditUserRegion)
	ON_EN_KILLFOCUS(IDC_EDIT_SERVER, &CConfigDialog::OnEnKillfocusEditServer)
	ON_EN_KILLFOCUS(IDC_EDIT_USER_REGION, &CConfigDialog::OnEnKillfocusEditUserRegion)
END_MESSAGE_MAP()


// CConfigDialog 消息处理程序
// CConfigDialog 消息处理程序

void CConfigDialog::OnOk()
{
	// TODO: 在此添加命令处理程序代码
	this->SaveConfig();

    __super::OnOK();
}

void CConfigDialog::OnCancel()
{
	// TODO: 在此添加命令处理程序代码
    __super::OnCancel();
}

BOOL CConfigDialog::SaveConfig()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 保存。
	UpdateData( TRUE );

	// msp
	CConfig::Instance()->SetServer( (LPCTSTR)m_strServerUrl );

	// 是否显示登录对话框.
	CConfig::Instance()->SetLoginInterface( TRUE == m_bShowLoginDialog );

	// 域客户
	CConfig::Instance()->SetReginId( (LPCTSTR)m_strRegionId );

	// ptz
	CConfig::Instance()->SetPtzSpeed( m_nPtzSpeed );
	// len
	CConfig::Instance()->SetLenSpeed( m_nLenSpeed );

	int nSavePicSel = this->m_cmbPicSaveDir.GetCurSel();
	DWORD dwData = m_cmbPicSaveDir.GetItemData( nSavePicSel );
	BOOL bSaveCustom = FALSE;
	CString strDir;
	switch( dwData )
	{
	case E_PicSaveCard:
		strDir = _T( "\\Storage Card\\M_CU\\" );
		bSaveCustom = FALSE;
		break;
	case E_PicSavePhone:
		strDir = _T( "\\My Documents\\M_CU\\" );
		bSaveCustom = FALSE;
		break;
	case E_PicSaveCustom:
		this->m_cmbPicSaveDir.GetWindowText( strDir );
		bSaveCustom = TRUE;
	    break;

	default:
	    break;
	}

	CConfig::Instance()->SetCaptureDir( strDir );
	CConfig::Instance()->SetIsCaptureDirCustom( bSaveCustom );

	return TRUE;
}

BOOL CConfigDialog::OnInitDialog()
{
	CUIDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 获取配置，显示在界面上。

	// 是否用自己的登录界面.
	BOOL bShowLoginDlg;
	CConfig::Instance()->GetLoginInterface( bShowLoginDlg );
	m_bShowLoginDialog = bShowLoginDlg;

	// 服务器url.
	tstring strUrl;
	CConfig::Instance()->GetServer( strUrl );
	m_strServerUrl = strUrl.c_str();

	// ptz和镜头控制速度.
	m_sliderPtzSpeed.SetRange( 3, 10 );
	m_sliderLenSpeed.SetRange( 3, 10 );

	CConfig::Instance()->GetPtzSpeed( m_nPtzSpeed );
	if (m_nPtzSpeed<3)
	{
		m_nPtzSpeed = 3;
		CConfig::Instance()->SetPtzSpeed(m_nPtzSpeed);
	}
	m_sliderPtzSpeed.SetPos( m_nPtzSpeed );

	CConfig::Instance()->GetLenSpeed( m_nLenSpeed );
	if (m_nLenSpeed<3)
	{
		m_nLenSpeed = 3;
		CConfig::Instance()->SetLenSpeed(m_nLenSpeed);
	}
	m_sliderLenSpeed.SetPos( m_nLenSpeed );

	// 客户的域id.
	tstring strRegionId;
	CConfig::Instance()->GetRegionId( strRegionId );
	m_strRegionId = strRegionId.c_str();

	// 设置图片。
	//const COLORREF conBgCor = RGB( 33,65,181 );
	//// 设置背景色。
	//this->SetColor( conBgCor );

	// 设置背景图片。
	tstring strMainBkImg = _T( "main_bg.JPG" );
	this->SetImage( strMainBkImg.c_str(), FALSE );

	// 保存图片设置！
	LPCTSTR strCardPath = _T( "\\Storage Card\\" );

	LPCTSTR strCardSaveShow = _T( "存储卡" );
	LPCTSTR strPhoneSaveShow = _T( "手机" );
	LPCTSTR strCustomSaveShow = _T( "浏览..." );

	// 手机保存。
	int nItem = this->m_cmbPicSaveDir.AddString( strPhoneSaveShow );
	this->m_cmbPicSaveDir.SetItemData( nItem, E_PicSavePhone );

	// 是否存在存储卡。
	BOOL bHasCard = ::HasStorageCard();
	if ( bHasCard )
	{
		nItem = this->m_cmbPicSaveDir.AddString( strCardSaveShow );
		this->m_cmbPicSaveDir.SetItemData( nItem, E_PicSaveCard );
	}
	
	// 用户自定义。
	nItem = this->m_cmbPicSaveDir.AddString( strCustomSaveShow );
	this->m_cmbPicSaveDir.SetItemData( nItem, E_PicSaveExplore );

	this->m_cmbPicSaveDir.SetCurSel( 0 );

	tstring strPicDir ;
	BOOL bSaveCustom;
	CConfig::Instance()->GetCaptureDir( strPicDir );
	CConfig::Instance()->GetIsCaptureDirCustom( bSaveCustom );
	if ( !bSaveCustom )
	{
		if ( m_cmbPicSaveDir.GetCount() > 1 )
		{
			if ( strPicDir.find( strCardPath ) != -1 )
			{
				m_cmbPicSaveDir.SelectString( 0, strCardSaveShow );
			}
			else
			{
				m_cmbPicSaveDir.SelectString( 0, strPhoneSaveShow );
			}
		}
	}
	else
	{
		int nItem = this->m_cmbPicSaveDir.AddString( strPicDir.c_str() );
		m_cmbPicSaveDir.SetItemData( nItem, E_PicSaveCustom );
		m_cmbPicSaveDir.SetCurSel( nItem );
	}


	UpdateData( FALSE );

	// 设置控件颜色。
	this->m_stServerAddrTitle.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );
	this->m_stClientIdTitle.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );
	this->m_stCaptureDirTitle.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );
	this->m_stPTZSpeedTitle.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );
	this->m_stLenSpeedTitle.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );
	this->m_stLenSpeedValue.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );
	this->m_stPTZSpeedValue.SetTextColor( RGB( 255,255,255 ), RGB( 255,255,255 ), FALSE );

	// 底部菜单条及按钮。
	this->m_stBottomBg.SetBkTransparent( FALSE, FALSE );
	this->m_stBottomBg.SetPicPath( _T( "bottombg.JPG" ) );

	this->m_btnSave.SetBkTransparent( FALSE, FALSE );
	this->m_btnSave.SetImage( _T( "btn_save_normal.jpg" ), _T( "btn_save_focus.jpg" ), _T( "btn_save_disable.jpg" ), FALSE );

	this->m_btnReturn.SetBkTransparent( FALSE, FALSE );
	this->m_btnReturn.SetImage( _T( "btn_return_normal.jpg" ), _T( "btn_return_focus.jpg" ), _T( "btn_return_normal.jpg" ), FALSE );
	// 输入法控制按钮。
	this->m_btnSip.SetBkTransparent( FALSE, FALSE );
	this->m_btnSip.SetImage( _T( "btn_sip_normal.jpg" ), _T( "btn_sip_focus.jpg" ), _T( "btn_sip_disable.jpg" ), FALSE );

	// 隐藏系统菜单。
	this->FullScreen( FS_HideMenuBar | FS_HideSipButton );

	//SHMENUBARINFO shmbi;
	//// Create our softkey bar
	//// 如果不是最后创建可能就显示不出来了.
	//ZeroMemory(&shmbi, sizeof(shmbi));
	//shmbi.cbSize = sizeof(shmbi);
	//shmbi.hwndParent = GetParent()->GetSafeHwnd();
	//shmbi.dwFlags = SHCMBF_HMENU;
	//shmbi.nToolBarId = IDR_MENU_CONFIG;
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

//void CConfigDialog::OnTRBNThumbPosChangingSliderPtzSpeed(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// 此功能要求 Windows Vista 或更高版本。
//	// _WIN32_WINNT 符号必须 >= 0x0600。
////	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	m_nPtzSpeed = m_sliderPtzSpeed.GetPos();
//	UpdateData( FALSE );
//
//	*pResult = 0;
//}

//void CConfigDialog::OnTRBNThumbPosChangingSliderLenSpeed(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// 此功能要求 Windows Vista 或更高版本。
//	// _WIN32_WINNT 符号必须 >= 0x0600。
////	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//
//	m_nLenSpeed = m_sliderLenSpeed.GetPos();
//	UpdateData( FALSE );
//
//	*pResult = 0;
//}
//
//void CConfigDialog::OnNMReleasedcaptureSliderPtzSpeed(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

void CConfigDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData();

	if ( int(&m_sliderPtzSpeed) == int(pScrollBar) )
	{
		m_nPtzSpeed = m_sliderPtzSpeed.GetPos();
	}
	else if( int( &m_sliderLenSpeed ) == int( pScrollBar ) )
	{
		m_nLenSpeed = m_sliderLenSpeed.GetPos();
	}

	UpdateData( FALSE );

	CUIDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CConfigDialog::OnSize(UINT nType, int cx, int cy)
{
	CUIDialog::OnSize(nType, cx, cy);

	this->UpdateLayout( CRect( 0,0, cx,cy ) );
	// TODO: 在此处添加消息处理程序代码
}

void CConfigDialog::UpdateLayout( LPCRECT prcClient /* = NULL */ )
{
	CRect rcClient;
	if ( prcClient )
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
	//if( m_logoDlg.GetSafeHwnd() )
	//{
	//	int nLogoHeight = m_logoDlg.GetHeight();
	//	m_logoDlg.MoveWindow( 0,0, rcClient.Width(), nLogoHeight );

	//	// 在剩余空间中去除logo占用的空间。
	//	rcLeftSpace.top += nLogoHeight;
	//}

	// 调整菜单条的位置。
	if ( m_stBottomBg.GetSafeHwnd() && m_btnSave.GetSafeHwnd() && m_btnReturn.GetSafeHwnd() && m_btnSip.GetSafeHwnd() )
	{
		// 底部按钮条（替代以前的菜单）的高度	
		const float conFMenubarHeight = 0.082;
		BOOL bResult = FALSE;
		int nMenubarHeight = int( conFMenubarHeight * nScreenLong );

		CRect rcBottomBg = rcClient;
		rcBottomBg.top = rcBottomBg.bottom - nMenubarHeight;
		m_stBottomBg.MoveWindow( rcBottomBg );

		CRect rcBtnReturn;
		bResult = this->m_btnReturn.GetImageSize( rcBtnReturn );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnReturn.Width() / rcBtnReturn.Height();
			rcBtnReturn = rcBottomBg;
			rcBtnReturn.left = rcBtnReturn.right - nNewWidth;
			this->m_btnReturn.SetWindowPos( &CWnd::wndTop, rcBtnReturn.left, rcBtnReturn.top, rcBtnReturn.Width(), rcBtnReturn.Height(), NULL );
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

		CRect rcBtnSave;
		bResult = this->m_btnSave.GetImageSize( rcBtnSave );
		if ( bResult )
		{
			int nNewWidth = nMenubarHeight * rcBtnSave.Width() / rcBtnSave.Height();
			rcBtnSave = rcBottomBg;
			rcBtnSave.right = rcBtnSave.left + nNewWidth;
			this->m_btnSave.SetWindowPos( &CWnd::wndTop, rcBtnSave.left, rcBtnSave.top, rcBtnSave.Width(), rcBtnSave.Height(), NULL );
		}

		// 在剩余空间中去除菜单条占用的空间。
		rcLeftSpace.bottom -= nMenubarHeight;
	}

}

void CConfigDialog::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SaveConfig();
//	this->EndDialog( 0 );
    this->OnOk();
}

void CConfigDialog::OnBnClickedButtonReturn()
{
	// TODO: 在此添加控件通知处理程序代码
    this->OnCancel();
}

void CConfigDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
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

void CConfigDialog::OnBnClickedCheckShowLoginDialog()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CConfigDialog::OnBnClickedButtonSipButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( this->IsSipRaise() )
	{
		this->LowerSip();
	}
	else
	{
		this->RaiseSip();
	}
}

void CConfigDialog::OnClickLeftFunKey()
{
	this->OnBnClickedButtonSave();
}

void CConfigDialog::OnClickRightFunKey()
{
	this->OnBnClickedButtonReturn();
}
void CConfigDialog::OnCbnCloseupComboPicSaveDir()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = this->m_cmbPicSaveDir.GetCurSel();
	DWORD dwData = this->m_cmbPicSaveDir.GetItemData( nSel );
	if ( E_PicSaveExplore == dwData )
	{
		CDirSelectDialog dlg(this);
		if ( IDOK == dlg.DoModal() )
		{
			tstring strDir = dlg.GetSelectedDir();
			
			int nItem = this->m_cmbPicSaveDir.FindString( 0, strDir.c_str() );
			if ( -1 == nItem )
			{
				nItem = this->m_cmbPicSaveDir.AddString( strDir.c_str() );
			}			
			this->m_cmbPicSaveDir.SetItemData( nItem, E_PicSaveCustom );
			this->m_cmbPicSaveDir.SetCurSel( nItem );
//			this->m_cmbPicSaveDir.SetWindowText( strDir.c_str() );

		}
		else
		{
			// 用户取消了，恢复刚才的选项。
			this->m_cmbPicSaveDir.SetCurSel( m_nOldPicSaveDirSelected );

		}
		this->FullScreen( FS_HideSipButton );
	}
}

void CConfigDialog::OnCbnSelendokComboPicSaveDir()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CConfigDialog::OnCbnDropdownComboPicSaveDir()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nOldPicSaveDirSelected = this->m_cmbPicSaveDir.GetCurSel();
}


void CConfigDialog::OnEnSetfocusEditServer()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bRaise = this->IsSipRaise();
	if ( !bRaise )
	{
		this->RaiseSip();
	}
}

void CConfigDialog::OnEnSetfocusEditUserRegion()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bRaise = this->IsSipRaise();
	if ( !bRaise )
	{
		this->RaiseSip();
	}
}

void CConfigDialog::OnEnKillfocusEditServer()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bRaise = this->IsSipRaise();
	if ( bRaise )
	{
		this->LowerSip();
	}
}

void CConfigDialog::OnEnKillfocusEditUserRegion()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bRaise = this->IsSipRaise();
	if ( bRaise )
	{
		this->LowerSip();
	}
}
