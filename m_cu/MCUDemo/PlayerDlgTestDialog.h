#pragma once
#include "afxwin.h"

#include "SDL.h"
// CPlayerDlgTestDialog �Ի���

class CPlayerDlgTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CPlayerDlgTestDialog)

public:
	CPlayerDlgTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayerDlgTestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PLAYER_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	
private:
	// ����.
	void Reset();

	static int DecodeThread( void * pThis );

	SDL_Thread *m_pDecodeThread;

	// ���б�־.
	BOOL m_bRunFlag;
public:
	CComboBox m_cmbVideoFile;
	CString m_strSelVideoPath;
	virtual BOOL OnInitDialog();
	// ֡��
	UINT m_nFPS;
};
