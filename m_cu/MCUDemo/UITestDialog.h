#pragma once
#include "afxwin.h"

#include "uibutton.h"
#include "uidialog.h"
// CUITestDialog 对话框

class CUITestDialog : public CUIDialog
{
	DECLARE_DYNAMIC(CUITestDialog)

public:
	CUITestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUITestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_UI_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlayerDlgTest();
	afx_msg void OnBnClickedButtonLoginDialogTest();
	afx_msg void OnBnClickedButtonHtmlDialogTest();
	afx_msg void OnBnClickedButtonConfigDialogTest();
	afx_msg void OnBnClickedButtonMainDialogTest();
	CButton m_btnOk;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CUIButton m_btnImage;
	afx_msg void OnBnClickedButtonImage();
	afx_msg void OnBnClickedButtonControlTest();
	afx_msg void OnBnClickedButtonPlayerDlgNoPtz();
	afx_msg void OnBnClickedButtonDirSelTest();
};
