#pragma once
#include "afxwin.h"


// CRTSPTestDialog 对话框

class CRTSPTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CRTSPTestDialog)

public:
	CRTSPTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRTSPTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_RTSP_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbRtspUrl;
	CString m_strRtspUrl;
	afx_msg void OnBnClickedButtonOpn();
	virtual BOOL OnInitDialog();
};
