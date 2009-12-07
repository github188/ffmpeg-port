#pragma once
#include "afxwin.h"

//#include "PtzControl.h"
#include "PtzButton.h"
#include "uidialog.h"
#include "mculib.h"

// CPTZDialog 对话框

class CPTZDialog : public CUIDialog/*, public CPtzControl*/
{
	DECLARE_DYNAMIC(CPTZDialog)

public:
	CPTZDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPTZDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PTZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	
    /** 设置用于发送PTZ命令的指针。 */
    void SetPtzSender( CVirtualPlayerWnd *pSender );

private:

	/** 布局控件。 */
	void UpdateLayout( LPCRECT prcClient = NULL );

private:

    /** 用于发送PTZ。 */
    CVirtualPlayerWnd *m_pPtzSender;

public:
//	afx_msg void OnBnClickedButtonPtzBright();
	afx_msg LRESULT OnPtzButtonDown( WPARAM btnId, LPARAM );
	afx_msg LRESULT OnPtzButtonUp( WPARAM btnId, LPARAM );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// 向上
	CPtzButton m_btnMoveUp;
	// 向下
	CPtzButton m_btnMoveDown;
	// 向左.
	CPtzButton m_btnMoveLeft;
	// 向右
	CPtzButton m_btnMoveRight;
	// 调亮
	CPtzButton m_btnLight;
	// 黑暗降临
	CPtzButton m_btnDark;
	// 放大
	CPtzButton m_btnZoomIn;
	// 缩小
	CPtzButton m_btnZoomOut;
	// 拉近
	CPtzButton m_btnNear;
	// 拉远
	CPtzButton m_btnFar;

	afx_msg void OnBnClickedButtonPtzDown();
//	afx_msg void OnBnClickedCheckDigitPtz();
	virtual BOOL OnInitDialog();
};
