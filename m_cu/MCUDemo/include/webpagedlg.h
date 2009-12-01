#pragma once

#include "browserctrl.h"
#include "uidialog.h"
#include "mculib.h"
#include "logodialog.h"
#include "uibutton.h"
// CWebpageDlg 对话框

class CWebpageDlg : public CUIDialog, public CVirtualWebpageCtrl
{
	DECLARE_DYNAMIC(CWebpageDlg)

public:
	CWebpageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebpageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WEBPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    /** 载入页面，需要子类实现。*/
    virtual BOOL OpenUrl( LPCTSTR strUrl );

protected:
    /** 打开播放器。 */
    virtual BOOL OpenPlayer( CVideoSession *pVideoSession ) ;

    /** 历史。 */
    virtual BOOL HistoryBack( int nStep );

    /** 隐藏窗口消息。 */
    virtual void OnShowWindowCmd( int nSWCmd );

    // 更新布局。
    void UpdateLayout( CRect *prcClient = NULL );
private:
    /** 创建浏览器控件。 */
    BOOL CreateBrowserCtrl();

    /** 关闭窗口，带提示。 */
    BOOL CloseWindowWithWarnning();

   
    afx_msg void OnBnClickedButtonPic();
    afx_msg void OnBnClickedButtonConfig();
    afx_msg void OnBnClickedButtonSipButton();
    
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
   
    /** 响应创建webbrowser控件的命令消息。 
    *   在窗口的初始化时直接创建会失败！（不知道Why）所以通过发送消息的方式创建。这里就是对消息的响应。
    */
    afx_msg LRESULT OnCreateBrowserCtrlCmd( WPARAM, LPARAM );

    /** 响应关闭窗口的命令消息。 */
    afx_msg LRESULT OnCloseWindowCmd( WPARAM, LPARAM );

    /** 响应html控件的url载入。 */
    afx_msg LRESULT OnHtmlPreLoad( WPARAM wParam, LPARAM );

private:
    /** 浏览器控件。 */
    CBrowserCtrl m_browserCtrl;

    /** logo 对话框。*/
    CLogoDialog m_logoDlg;

    CUIButton m_btnPic;
    CUIButton m_btnConfig;
    CPictureStatic m_stBottomBg;
    // 输入法控制按钮
    CUIButton m_btnSip;

protected:
    virtual void OnOK();

public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
};
