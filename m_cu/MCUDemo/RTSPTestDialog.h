#pragma once
#include "afxwin.h"


// CRTSPTestDialog �Ի���

class CRTSPTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CRTSPTestDialog)

public:
	CRTSPTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRTSPTestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_RTSP_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbRtspUrl;
	CString m_strRtspUrl;
	afx_msg void OnBnClickedButtonOpn();
	virtual BOOL OnInitDialog();
};
