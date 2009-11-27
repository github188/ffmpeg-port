#pragma once
#include "afxwin.h"

#include "uibutton.h"
#include "uidialog.h"
// CUITestDialog �Ի���

class CUITestDialog : public CUIDialog
{
	DECLARE_DYNAMIC(CUITestDialog)

public:
	CUITestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUITestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_UI_TEST };



public:
    CButton m_btnOk;
    CUIButton m_btnImage;
public:
    virtual BOOL OnInitDialog();

protected:
    void OnShowWindowCmd( int nSWCmd );

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedButtonPlayerDlgTest();
	afx_msg void OnBnClickedButtonLoginDialogTest();
	afx_msg void OnBnClickedButtonHtmlDialogTest();
	afx_msg void OnBnClickedButtonConfigDialogTest();
	afx_msg void OnBnClickedButtonMainDialogTest();	
	afx_msg void OnBnClickedOk();		
	afx_msg void OnBnClickedButtonImage();
	afx_msg void OnBnClickedButtonControlTest();
	afx_msg void OnBnClickedButtonPlayerDlgNoPtz();
	afx_msg void OnBnClickedButtonDirSelTest();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnDestroy();

    
};
