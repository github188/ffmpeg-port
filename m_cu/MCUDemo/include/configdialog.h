#pragma once

#include "uidialog.h"
#include "uisliderctrl.h"
#include "staticex.h"
#include "afxwin.h"
#include "uibutton.h"
#include "pictruestatic.h"
// CConfigDialog 对话框

class CConfigDialog : public CUIDialog
{
	DECLARE_DYNAMIC(CConfigDialog)

public:
	CConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CFG_SYS };

public:
	/** 保存图片路径。 */
	enum EPicSavePath
	{
		E_PicSavePhone,	// 保存在手机上。
		E_PicSaveCard,	// 保存在卡上。
		E_PicSaveCustom,// 用户自定义位置。
		E_PicSaveExplore, // 浏览。
	};

protected:
	/** 用户点击了左边功能键。相当于点击屏幕底部左边按钮 */
	virtual void OnClickLeftFunKey();
	/** 用户点击了右边功能键，相当于点击了屏幕底部右边按钮。 */
	virtual void OnClickRightFunKey();

private:
	/** 保存参数。 */
	virtual BOOL SaveConfig();

	/** 窗口更新布局. */
	void UpdateLayout( LPCRECT prcClient = NULL );

private:

	/** 当前的路径。 */
	int m_nOldPicSaveDirSelected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	
	virtual BOOL OnInitDialog();
	// 服务器的url地址。
	CString m_strServerUrl;
	// 是否显示登录对话框，否则直接显示msp页面（登录页面）。
	BOOL m_bShowLoginDialog;
	// 云台控制速度
	CUISliderCtrl m_sliderPtzSpeed;
	// 镜头控制速度
	CUISliderCtrl m_sliderLenSpeed;
	// 云台控制速度
	int m_nPtzSpeed;
	// 镜头控制速度
	int m_nLenSpeed;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// 客户的域id.
	CString m_strRegionId;
	// 抓拍图片的存放路径.
	//	CString m_strPicSavePath;
	// 服务器地址标题
	CStaticEx m_stServerAddrTitle;
	// 客户ID标题
	CStaticEx m_stClientIdTitle;
	// 抓拍图片保存路径标题
	CStaticEx m_stCaptureDirTitle;
	// 云台控制步长标题
	CStaticEx m_stPTZSpeedTitle;
	// 镜头控制速度标题
	CStaticEx m_stLenSpeedTitle;
	// 云台速度值显示
	CStaticEx m_stPTZSpeedValue;
	// 镜头速度值显示
	CStaticEx m_stLenSpeedValue;
	CComboBox m_cmbPicSaveDir;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	CUIButton m_btnSave;
	CUIButton m_btnReturn;
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonReturn();
	CUIButton m_btnSip;
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnBnClickedCheckShowLoginDialog();
	afx_msg void OnBnClickedButtonSipButton();
	afx_msg void OnCbnCloseupComboPicSaveDir();
	afx_msg void OnCbnSelendokComboPicSaveDir();
	afx_msg void OnCbnDropdownComboPicSaveDir();
	afx_msg void OnEnSetfocusEditServer();
	afx_msg void OnEnSetfocusEditUserRegion();
	afx_msg void OnEnKillfocusEditServer();
	afx_msg void OnEnKillfocusEditUserRegion();
};
