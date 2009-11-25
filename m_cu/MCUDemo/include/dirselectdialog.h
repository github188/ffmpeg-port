#pragma once

// CDirSelectDialog �Ի���
#include "mcu.h"
#include "uidialog.h"
#include "afxwin.h"
#include "afxcmn.h"

class CDirSelectDialog : public CUIDialog
{
	DECLARE_DYNCREATE(CDirSelectDialog)

public:
	CDirSelectDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDirSelectDialog();
// ��д


// �Ի�������
	enum { IDD = IDD_DIALOG_DIR_SELECT };

public:
	/** ��ȡ�û�ѡ�е�·���� */
	tstring GetSelectedDir() const;

private:
	/** �������ڲ���. */
	void UpdateLayout( LPRECT lprcClient = NULL );

	/** ˢ���ļ�����ʾ�� */
	void UpdateDirShow( LPCTSTR strRootPath );
private:
	/** ��ǰ�ļ��С� */
	tstring m_strCurDir;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
