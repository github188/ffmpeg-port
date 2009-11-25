#pragma once

#include "browserctrl.h"
#include "uidialog.h"
#include "mculib.h"
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

    // 更新布局。
    void UpdateLayout( CRect *prcClient = NULL );
private:
    /** 创建浏览器控件。 */
    BOOL CreateBrowserCtrl();

private:
    CBrowserCtrl m_browserCtrl;
protected:
    virtual void OnOK();
public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
