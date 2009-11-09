#pragma once
#include "afxwin.h"


// CUDPTestDialog �Ի���

class CUDPTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CUDPTestDialog)

public:
	CUDPTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDPTestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_UDP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// Ҫ���͵�����
	CString m_strContent;
	// ��ַ�Ͷ˿�
	CString m_strAddr;
	afx_msg void OnBnClickedButtonSend();
	virtual BOOL OnInitDialog();

private:
	/** socket */
	SOCKET m_udpSocket;
protected:
	virtual void PostNcDestroy();
public:
	// ����
	CEdit m_editContent;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
