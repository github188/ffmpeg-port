#pragma once

#include "uidialog.h"
#include "logodialog.h"
// CMainDialog �Ի���

typedef CUIDialog CBaseMainDialog;
class CMainDialog : public CBaseMainDialog
{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	static BOOL CALLBACK  WindowsEnumCB( HWND hWnd, LPARAM param );

	/** ���������Ͽؼ�λ�á� */
	void UpdateCtrlPos();

private:
	CLogoDialog m_logoDlg;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
