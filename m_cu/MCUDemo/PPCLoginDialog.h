#pragma once
#include "afxwin.h"
#include "pictruestatic.h"
#include "logodialog.h"
#include "staticex.h"
#include "checkbutton.h"
#include "uibutton.h"

// CPPCLoginDialog 对话框

#define CBasePPCLoginDialog CUIDialog 
class CPPCLoginDialog : public CBasePPCLoginDialog
{
	DECLARE_DYNAMIC(CPPCLoginDialog)

public:
	enum ELoginType
	{
		LOGIN_NORMAL = 0,		// 普通登录。
		LOGIN_LOCAL_PHONE = 1,	// 本机登录。
		LOGIN_CANCEL,			// 取消登录。

	};

public:
	CPPCLoginDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPPCLoginDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用户账户ID
	CString m_strUserId;
	// 密码
	CString m_strPassword;
	// 码流/连接类型
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLoginThisPhone();
protected:
	virtual void OnOK();

	/** 用户点击了左边功能键。相当于点击屏幕底部左边按钮 */
	virtual void OnClickLeftFunKey();
	/** 用户点击了右边功能键，相当于点击了屏幕底部右边按钮。 */
	virtual void OnClickRightFunKey();

public:
	virtual BOOL OnInitDialog();

private:
	void ReadConfig();
	void SaveConfig();

	/** 用户点击图片。 */
	void OnPicManage();
	/** 用户点击设置。 */
	void OnConfig();

	/** 检查参数并提示用户。 
	*	返回值： TRUE 通过。 FALSE 参数校验错误。
	*/
	BOOL CheckAndAlert( ELoginType eLoginType );

private:


public:
	// 是否保存用户登录信息
	BOOL m_bSaveLoginInfo;
	// 是否是3G的网络,观看清晰的视频.
	BOOL m_b3GStream;

	// 用户的登录方式.
	ELoginType m_eLoginType;

	/** soft key bar. */
//	HWND m_hSoftKeyBar;
	afx_msg void OnMenuLogin();
	afx_msg void OnMenuConfig();
	// 厂商Logo
//	CPictureStatic m_cLogo;

	// logo 对话框。
	CLogoDialog m_logoDlg;

	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 窗口更新布局.
	void UpdateLayout( LPCRECT prcClient = NULL );

	CStaticEx m_stUserIdTitle;
	CStaticEx m_stPasswordTitle;
	CStaticEx m_stMCUVer;
	CCheckButton m_ckSaveLoginInfo;
	CCheckButton m_ckStreamType;
	// 帐号登录
	CUIButton m_btnLogin;
	// 一键登录，利用手机号码完成
	CUIButton m_btnLoginThisPhone;
	// 用户ID的logo图片
	CPictureStatic m_stUserIdLogo;
	// 密码的logo图片
	CPictureStatic m_stPasswordLogo;
	// 用户ID输入控件
	CEdit m_edUserId;
	// 密码输入控件。
	CEdit m_edPassword;
	afx_msg void OnMenuPic();
	// 菜单栏中的图片/录像管理按钮。
	CUIButton m_btnPicRecManage;
	// 菜单栏里的配置按钮。
	CUIButton m_btnConfig;
	// 菜单栏的背景。
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonPicRecManage();
	afx_msg void OnBnClickedButtonConfig();
	// 输入法按钮。
	CUIButton m_btnSip;
	afx_msg void OnBnClickedButtonSipButton();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnEnSetfocusEditUerid();
	afx_msg void OnEnSetfocusEditPassword();
	afx_msg void OnEnKillfocusEditUerid();
	afx_msg void OnEnKillfocusEditPassword();
};
