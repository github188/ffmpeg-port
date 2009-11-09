#pragma once

#include "uidialog.h"
#include "logodialog.h"
// CMainDialog 对话框

typedef CUIDialog CBaseMainDialog;
class CMainDialog : public CBaseMainDialog
{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	static BOOL CALLBACK  WindowsEnumCB( HWND hWnd, LPARAM param );

	/** 调整窗口上控件位置。 */
	void UpdateCtrlPos();

private:
	CLogoDialog m_logoDlg;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
