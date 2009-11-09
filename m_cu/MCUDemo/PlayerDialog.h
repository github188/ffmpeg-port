#pragma once
#include "afxwin.h"
#include "VideoWndDialog.h"
#include "PTZDialog.h"
#include "logodialog.h"
#include <string>
#include "staticex.h"

using namespace std;
// CPlayerDialog �Ի���
/** ��Ƶ������. */

class CPlayerDialog : public CUIDialog 
{
	DECLARE_DYNAMIC(CPlayerDialog)


public:
	CPlayerDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayerDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_VIDEO_PLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL m_bIsEnding;

	DECLARE_MESSAGE_MAP()

public:
	enum EMenuItem
	{
		E_MenuPause ,
		E_MenuRecode,
		E_MenuCapture ,
		E_MenuPtz,
		E_MenuFullscreen,
		E_MenuClose,
	};

	/** �Ի����˳��Ĵ���. */
	enum EDlgEndCode
	{
		E_DlgEndNull = 10000,
		E_DlgEndRetry,	// �û������Ե�ǰ����.
	};
public:

	virtual BOOL OnInitDialog();

public:
	/** ����Ҫ���ŵ�rtsp����. */
	void SetRtspUrl( LPCTSTR strRtspUrl );

	/** ȫ��. */
	void PlayFullScreen( BOOL bFullScreen );

private:
	/** ��Ƶ��ʾ����. */
	CVideoWndDialog m_cVideoWnd;
	/** ptz������. */
	CPTZDialog m_cPtzDlg;

	/** ������. */
//	HWND m_hSoftKeyBar;

	/** �Ƿ�ȫ��ģʽ. */
	BOOL m_bFullScreenMode;

	/** ԭʼ����Ļģʽ. */
	DWORD m_nOrigRotation;

	CRect m_rcOldWidnow;

	/** logo �Ի���*/
	CLogoDialog m_logoDlg;

	/** ptz�˵��� */
	CMenu m_menuPtz;

	/** ptz �Ӳ˵��� */
	CMenu *m_pSubPtzMenu;

private:

	/** �������ڲ���. */
	void UpdateLayout( LPRECT lprcClient = NULL );

	/** �Ƿ���ptzȨ�ޡ� */
	BOOL HasPtz() const;

	/** �����˵�״̬��*/
	void UpdateMenu();

	/** ���ڱ����ص�.�ڻص��д����menu_worker����. */
	static BOOL CALLBACK  WindowsEnumCBForFulscreen( HWND hWnd, LPARAM param );

	/** ¼��ʧ�ܵ���Ϣ��Ӧ�� */
	afx_msg LRESULT OnRecordFail( WPARAM eErrorCode, LPARAM );

	/** ��Ƶ���ʧ�ܵ���Ϣ��Ӧ�� */
	afx_msg LRESULT OnVideoFail( WPARAM eErrorCode, LPARAM );

	/** ˢ��¼��ť��ͼƬ�� */
	void UpdateRecordButton();

	/** ��ȡ�ļ����� */
	tstring GetFileName( LPCTSTR strDir, LPCTSTR strPuName, LPCTSTR strFileExt );

	/** �����û����˳������� */
	void OnUserClose();

	/** ����RTSPʧ�ܵ��˳����� */
	void OnRtspClose(int nErrorCode);

public:
	//ע������
	static void BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
	static void RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
	static void BackupRtspPlayerRegKey();
	static void RestoreRtspPlayerRegKey();
	//�ж��Ƿ������˳����
	BOOL IsAppNormalExit();

	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
//	afx_msg void OnMenuFullScreen();
//	afx_msg void OnMenuClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuRestore();
	
protected:
	virtual void PostNcDestroy();
public:
//	afx_msg void OnMenuCapture();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateMenuPtz(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuDigitalPtz(CCmdUI *pCmdUI);
	afx_msg void OnMenuPtz();
	afx_msg void OnMenuDigitalPtz();
//	afx_msg void OnMenuRecoder();
//	afx_msg void OnMenuPause();
	CPictureStatic m_stBottomBg;
	CUIButton m_btnPause;
	CUIButton m_btnRecord;
	CUIButton m_btnCapture;
	CUIButton m_btnPtz;
	CUIButton m_btnFullscreen;
	CUIButton m_btnReturn;
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnBnClickedButtonFullscreen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonPtz();
protected:
	virtual void OnOK();
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
