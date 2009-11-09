#pragma once
#include "afxwin.h"
#include "sdl.h"
#include "VideoWndDialog.h"

// CDecodeTestDialog 对话框

class CDecodeTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CDecodeTestDialog)

public:
	CDecodeTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDecodeTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DECODE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExplore();
	afx_msg void OnBnClickedButtonPlay();
	// 是否要显示图像
	BOOL m_bShowVideo;
	CString m_strFilePath;
	CComboBox m_cmbFilePath;

private:
	// 销毁.
	void Reset();

private:
	static int DecodeThread( void * pThis );

	SDL_Thread *m_pDecodeThread;

	// 视频显示窗口.
	CVideoWndDialog m_cVideoWndDlg;

	// 运行标志.
	BOOL m_bRunFlag;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedCheckRecord();
	// 是否录像。
	BOOL m_bRecord;
};
