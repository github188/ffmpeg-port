#pragma once

#include "uidialog.h"
#include "afxwin.h"
#include "pictruestatic.h"


// CLogoDialog 对话框
#define CBaseLogoDialog CUIDialog

class CLogoDialog : public CBaseLogoDialog
{
	DECLARE_DYNAMIC(CLogoDialog)

public:
	CLogoDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogoDialog();

	/** 获取logo高度。 */
	int GetHeight() const;

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 运营商logo
	CPictureStatic m_picLogo;
	virtual BOOL OnInitDialog();

protected:
	// 更新布局。
	void UpdateLayout( CRect *prcClient = NULL );
};
