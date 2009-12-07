#pragma once
#include "afxwin.h"

//#include "PtzControl.h"
#include "PtzButton.h"
#include "uidialog.h"
#include "mculib.h"

// CPTZDialog �Ի���

class CPTZDialog : public CUIDialog/*, public CPtzControl*/
{
	DECLARE_DYNAMIC(CPTZDialog)

public:
	CPTZDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPTZDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PTZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	
    /** �������ڷ���PTZ�����ָ�롣 */
    void SetPtzSender( CVirtualPlayerWnd *pSender );

private:

	/** ���ֿؼ��� */
	void UpdateLayout( LPCRECT prcClient = NULL );

private:

    /** ���ڷ���PTZ�� */
    CVirtualPlayerWnd *m_pPtzSender;

public:
//	afx_msg void OnBnClickedButtonPtzBright();
	afx_msg LRESULT OnPtzButtonDown( WPARAM btnId, LPARAM );
	afx_msg LRESULT OnPtzButtonUp( WPARAM btnId, LPARAM );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// ����
	CPtzButton m_btnMoveUp;
	// ����
	CPtzButton m_btnMoveDown;
	// ����.
	CPtzButton m_btnMoveLeft;
	// ����
	CPtzButton m_btnMoveRight;
	// ����
	CPtzButton m_btnLight;
	// �ڰ�����
	CPtzButton m_btnDark;
	// �Ŵ�
	CPtzButton m_btnZoomIn;
	// ��С
	CPtzButton m_btnZoomOut;
	// ����
	CPtzButton m_btnNear;
	// ��Զ
	CPtzButton m_btnFar;

	afx_msg void OnBnClickedButtonPtzDown();
//	afx_msg void OnBnClickedCheckDigitPtz();
	virtual BOOL OnInitDialog();
};
