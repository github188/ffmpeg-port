#pragma once
#include "afxwin.h"

#include "SDL.h"
// CPlayerDlgTestDialog 对话框

class CPlayerDlgTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CPlayerDlgTestDialog)

public:
	CPlayerDlgTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerDlgTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PLAYER_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	
private:
	// 销毁.
	void Reset();

	static int DecodeThread( void * pThis );

	SDL_Thread *m_pDecodeThread;

	// 运行标志.
	BOOL m_bRunFlag;
public:
	CComboBox m_cmbVideoFile;
	CString m_strSelVideoPath;
	virtual BOOL OnInitDialog();
	// 帧率
	UINT m_nFPS;
};
