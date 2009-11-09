#pragma once
#include "afxwin.h"


// CUDPTestDialog 对话框

class CUDPTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CUDPTestDialog)

public:
	CUDPTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDPTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_UDP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 要发送的内容
	CString m_strContent;
	// 地址和端口
	CString m_strAddr;
	afx_msg void OnBnClickedButtonSend();
	virtual BOOL OnInitDialog();

private:
	/** socket */
	SOCKET m_udpSocket;
protected:
	virtual void PostNcDestroy();
public:
	// 内容
	CEdit m_editContent;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
