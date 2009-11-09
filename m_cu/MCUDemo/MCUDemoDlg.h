// MCUDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <list>
#include <stack>

#include "HtmlWnd.h"
#include "PlayerDialog.h"

// CMCUDemoDlg �Ի���
class CMCUDemoDlg : public CUIDialog
{
// ����
public:
	CMCUDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MCUDEMO_DIALOG };

#ifdef WIN32_PLATFORM_WFSP
protected:  // �ؼ���Ƕ���Ա
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	CExplorer1 m_ctrlExplorer;
	afx_msg void OnBnClickedButtonTest();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnDelayedEvent( int nEventId );
	void RestoreRtspPlayerRegKey();
	void RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);

private:

	/** ��¼�� */
	void Login();

	/** ��������. */
	void UpdateLayout( LPRECT lprcClient = NULL );

public:
	// Ҫ���Ե�Url��
	CComboBox m_cmbUrl;

private:
//	HWND m_hHtmlCtrlWnd;

	///** �����ַ��ʷ��¼��*/
	//typedef std::stack< CString > TUrlHistoryList;
	//TUrlHistoryList m_listUrlHistory; 

	/** html �ؼ�����. */
	CHtmlWnd m_htmlWnd;

	/** soft key bar wnd */
//	HWND m_hSoftKeyBar;

	/** ����������. */
	CPlayerDialog *m_pPlayerDlg;

	/** logo �Ի���*/
	CLogoDialog m_logoDlg;

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMenuUdpTest();
	afx_msg void OnMenuVideoPlay();
	afx_msg void OnMenuConfig();
	afx_msg void OnMenuLogin();
	afx_msg void OnMenuBack();
	afx_msg void OnMenuVideoWndTest();
	afx_msg void OnMenuPtz();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnHtmlPreLoad( WPARAM strUrl, LPARAM );
	afx_msg LRESULT OnHtmlMeta( WPARAM wParam, LPARAM lParam );
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnMenuForward();
	afx_msg void OnMenuPlayFile();
	afx_msg void OnMenuRefresh();
//	afx_msg void OnMenuPic();
protected:
	virtual void OnOK();
public:
	CUIButton m_btnPic;
	CUIButton m_btnConfig;
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonPic();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonSipButton();
	// ���뷨���ư�ť
	CUIButton m_btnSip;
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
};
