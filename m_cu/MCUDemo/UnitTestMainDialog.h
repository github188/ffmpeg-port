#pragma once


// CUnitTestMainDialog �Ի���

class CUnitTestMainDialog : public CDialog
{
	DECLARE_DYNAMIC(CUnitTestMainDialog)

public:
	CUnitTestMainDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUnitTestMainDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_UNIT_TEST_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDecodeTest();
	afx_msg void OnBnClickedButtonUdpTest();
	afx_msg void OnBnClickedButtonRtspTest();
	afx_msg void OnBnClickedButtonUiTest();
	afx_msg void OnBnClickedButtonPlayerDialogTest();
	afx_msg void OnBnClickedButtonHtmlTest();
	afx_msg void OnBnClickedButtonImageTest();
};
