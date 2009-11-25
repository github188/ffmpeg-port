#pragma once
#include "afxwin.h"

//#include "HtmlWnd.h"
#include "webpagedlg.h"

// CHtmlTestDialog �Ի���

class CHtmlTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CHtmlTestDialog)

public:
	CHtmlTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHtmlTestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_HTML_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbUrlList;
	afx_msg void OnBnClickedButtonOpenUrl();

private:
	// html ����.
//	CHtmlWnd m_cHtmlWnd;
    CWebpageDlg m_webpageDlg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonTest();
};
