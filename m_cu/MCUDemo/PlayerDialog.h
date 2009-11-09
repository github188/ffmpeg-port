#pragma once
#include "afxwin.h"
#include "VideoWndDialog.h"
#include "PTZDialog.h"
#include "logodialog.h"
#include <string>
#include "staticex.h"

using namespace std;
// CPlayerDialog 对话框
/** 视频播放器. */

class CPlayerDialog : public CUIDialog 
{
	DECLARE_DYNAMIC(CPlayerDialog)


public:
	CPlayerDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlayerDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIDEO_PLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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

	/** 对话框退出的代码. */
	enum EDlgEndCode
	{
		E_DlgEndNull = 10000,
		E_DlgEndRetry,	// 用户想重试当前连接.
	};
public:

	virtual BOOL OnInitDialog();

public:
	/** 设置要播放的rtsp链接. */
	void SetRtspUrl( LPCTSTR strRtspUrl );

	/** 全屏. */
	void PlayFullScreen( BOOL bFullScreen );

private:
	/** 视频显示窗口. */
	CVideoWndDialog m_cVideoWnd;
	/** ptz控制条. */
	CPTZDialog m_cPtzDlg;

	/** 工具条. */
//	HWND m_hSoftKeyBar;

	/** 是否全屏模式. */
	BOOL m_bFullScreenMode;

	/** 原始的屏幕模式. */
	DWORD m_nOrigRotation;

	CRect m_rcOldWidnow;

	/** logo 对话框。*/
	CLogoDialog m_logoDlg;

	/** ptz菜单。 */
	CMenu m_menuPtz;

	/** ptz 子菜单。 */
	CMenu *m_pSubPtzMenu;

private:

	/** 调整窗口布局. */
	void UpdateLayout( LPRECT lprcClient = NULL );

	/** 是否有ptz权限。 */
	BOOL HasPtz() const;

	/** 调整菜单状态。*/
	void UpdateMenu();

	/** 窗口遍历回调.在回调中处理掉menu_worker窗口. */
	static BOOL CALLBACK  WindowsEnumCBForFulscreen( HWND hWnd, LPARAM param );

	/** 录像失败的消息响应。 */
	afx_msg LRESULT OnRecordFail( WPARAM eErrorCode, LPARAM );

	/** 视频浏览失败的消息响应。 */
	afx_msg LRESULT OnVideoFail( WPARAM eErrorCode, LPARAM );

	/** 刷新录像按钮的图片。 */
	void UpdateRecordButton();

	/** 获取文件名。 */
	tstring GetFileName( LPCTSTR strDir, LPCTSTR strPuName, LPCTSTR strFileExt );

	/** 处理用户的退出操作。 */
	void OnUserClose();

	/** 处理RTSP失败等退出操作 */
	void OnRtspClose(int nErrorCode);

public:
	//注册表操作
	static void BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
	static void RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
	static void BackupRtspPlayerRegKey();
	static void RestoreRtspPlayerRegKey();
	//判断是否正常退出浏览
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
