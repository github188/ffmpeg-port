#pragma once

// CDirSelectDialog 对话框
#include "mcu.h"
#include "uidialog.h"
#include "afxwin.h"
#include "afxcmn.h"

class CDirSelectDialog : public CUIDialog
{
	DECLARE_DYNCREATE(CDirSelectDialog)

public:
	CDirSelectDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirSelectDialog();
// 重写


// 对话框数据
	enum { IDD = IDD_DIALOG_DIR_SELECT };

public:
	/** 获取用户选中的路径。 */
	tstring GetSelectedDir() const;

private:
	/** 调整窗口布局. */
	void UpdateLayout( LPRECT lprcClient = NULL );

	/** 刷新文件夹显示。 */
	void UpdateDirShow( LPCTSTR strRootPath );
private:
	/** 当前文件夹。 */
	tstring m_strCurDir;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_editDir;
	CListCtrl m_listctrlDir;
	afx_msg void OnLvnItemchangedListDir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListDir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuOk();
	afx_msg void OnMenuCancel();
};
