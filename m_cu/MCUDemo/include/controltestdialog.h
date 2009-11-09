#pragma once


// CControlTestDialog �Ի���
#include "uidialog.h"
#include "afxwin.h"
#include "staticex.h"

class CControlTestDialog : public CUIDialog
{
	DECLARE_DYNAMIC(CControlTestDialog)

public:
	CControlTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControlTestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONTROL_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CStaticEx m_stGreenTextTransLeft;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonTestTransparentMaskImage();

private:
	/** ���پɴ��ڡ� */
	void ResetTest();

private:
	/** ���ڲ��ԵĿؼ��� */
	CWnd *m_pTestWnd;
public:
	afx_msg void OnBnClickedButtonTestStaticText();
	afx_msg void OnBnClickedButtonTestImageButton();
	afx_msg void OnBnClickedButtonTestCheckButton();
	afx_msg void OnBnClickedButtonTestCheckPushButton();
	afx_msg void OnBnClickedButtonTestDisableWnd();
	afx_msg void OnBnClickedButtonTestSlider();
};
