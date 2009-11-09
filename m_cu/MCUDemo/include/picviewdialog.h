#pragma once

#include "uidialog.h"
#include "pictruestatic.h"
#include "logodialog.h"
#include "afxwin.h"
#include "staticex.h"
#include "uibutton.h"

// CPicViewDialog 对话框
#define CBasePicViewDialog CUIDialog
class CPicViewDialog : public CBasePicViewDialog
{
	DECLARE_DYNAMIC(CPicViewDialog)

public:
	CPicViewDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicViewDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PIC_VIEW };

public:

	/** 对话框DoModal返回值。 */
	enum EDialogEndValue
	{
		E_PicViewClose,		// 正常关闭。
		E_PicViewDelete,	// 用户要删除当前图片。
	};

	/** 设置显示图片。 */
	void SetPic( LPCTSTR strPicPath );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	/** 用户点击了左边功能键。相当于点击屏幕底部左边按钮 */
	virtual void OnClickLeftFunKey();
	/** 用户点击了右边功能键，相当于点击了屏幕底部右边按钮。 */
	virtual void OnClickRightFunKey();

private:
	/** 图像控件。 */
	CPictureStatic m_stPic;

	/** logo 对话框。 */
	CLogoDialog m_logoDlg;

	/** 放大倍数。 */
	float m_fZoomRadio;

	/** 是否已经全屏. */
	BOOL m_bFullScreen;

	/** 菜单栏句柄. */
//	HWND m_hMenuBarWnd;

private:
	/** 调整控件布局。 */
	void UpdateLayout( LPCRECT prcClient = NULL );

	/** 全屏. */
	void SetFullScreen( BOOL bFullScreen );

	/** 用户要删除图片。 */
	void OnDeletePic();
	/** 返回。 */
	void OnReturn();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuPicViewReturn();
	afx_msg void OnMenuDeletePicFile();
	// 图片信息（图片名）
	CStaticEx m_stPicInfo;
	// 图片放大。
	CUIButton m_btnZoomIn;
	// 图片缩小
	CUIButton m_btnZoomOut;
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 删除图片按钮
	CUIButton m_btnDelete;
	// 返回按钮
	CUIButton m_btnReturn;
	// 底部背景条图片
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReturn();
};
