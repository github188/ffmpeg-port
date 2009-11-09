#pragma once


// CControlTestDialog 对话框
#include "uidialog.h"
#include "afxwin.h"
#include "staticex.h"

class CControlTestDialog : public CUIDialog
{
	DECLARE_DYNAMIC(CControlTestDialog)

public:
	CControlTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTROL_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CStaticEx m_stGreenTextTransLeft;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonTestTransparentMaskImage();

private:
	/** 销毁旧窗口。 */
	void ResetTest();

private:
	/** 正在测试的控件。 */
	CWnd *m_pTestWnd;
public:
	afx_msg void OnBnClickedButtonTestStaticText();
	afx_msg void OnBnClickedButtonTestImageButton();
	afx_msg void OnBnClickedButtonTestCheckButton();
	afx_msg void OnBnClickedButtonTestCheckPushButton();
	afx_msg void OnBnClickedButtonTestDisableWnd();
	afx_msg void OnBnClickedButtonTestSlider();
};
