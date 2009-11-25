#pragma once
#include "afxwin.h"

//#include "HtmlWnd.h"
#include "webpagedlg.h"

// CHtmlTestDialog 对话框

class CHtmlTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CHtmlTestDialog)

public:
	CHtmlTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHtmlTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_HTML_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbUrlList;
	afx_msg void OnBnClickedButtonOpenUrl();

private:
	// html 窗口.
//	CHtmlWnd m_cHtmlWnd;
    CWebpageDlg m_webpageDlg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonTest();
};
