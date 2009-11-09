#pragma once

#include "uidialog.h"
#include "afxwin.h"
#include "pictruestatic.h"


// CLogoDialog �Ի���
#define CBaseLogoDialog CUIDialog

class CLogoDialog : public CBaseLogoDialog
{
	DECLARE_DYNAMIC(CLogoDialog)

public:
	CLogoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogoDialog();

	/** ��ȡlogo�߶ȡ� */
	int GetHeight() const;

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��Ӫ��logo
	CPictureStatic m_picLogo;
	virtual BOOL OnInitDialog();

protected:
	// ���²��֡�
	void UpdateLayout( CRect *prcClient = NULL );
};
