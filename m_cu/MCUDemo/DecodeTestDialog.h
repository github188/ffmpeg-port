#pragma once
#include "afxwin.h"
#include "sdl.h"
#include "VideoWndDialog.h"

// CDecodeTestDialog �Ի���

class CDecodeTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CDecodeTestDialog)

public:
	CDecodeTestDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDecodeTestDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_DECODE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExplore();
	afx_msg void OnBnClickedButtonPlay();
	// �Ƿ�Ҫ��ʾͼ��
	BOOL m_bShowVideo;
	CString m_strFilePath;
	CComboBox m_cmbFilePath;

private:
	// ����.
	void Reset();

private:
	static int DecodeThread( void * pThis );

	SDL_Thread *m_pDecodeThread;

	// ��Ƶ��ʾ����.
	CVideoWndDialog m_cVideoWndDlg;

	// ���б�־.
	BOOL m_bRunFlag;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedCheckRecord();
	// �Ƿ�¼��
	BOOL m_bRecord;
};
