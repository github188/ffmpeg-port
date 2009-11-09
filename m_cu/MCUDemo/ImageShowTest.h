#pragma once
#include "afxwin.h"
// #include "ImageShow.h"

class CImageShow;
// CImageShowTest �Ի���

class CImageShowTest : public CDialog
{
	DECLARE_DYNAMIC(CImageShowTest)

public:
	CImageShowTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageShowTest();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMAGE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	CComboBox m_cmbImagePath;
	CString m_strImagePath;

private:
	CImageShow *m_pImgShow;
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
