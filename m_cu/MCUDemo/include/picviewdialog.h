#pragma once

#include "uidialog.h"
#include "pictruestatic.h"
#include "logodialog.h"
#include "afxwin.h"
#include "staticex.h"
#include "uibutton.h"

// CPicViewDialog �Ի���
#define CBasePicViewDialog CUIDialog
class CPicViewDialog : public CBasePicViewDialog
{
	DECLARE_DYNAMIC(CPicViewDialog)

public:
	CPicViewDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicViewDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PIC_VIEW };

public:

	/** �Ի���DoModal����ֵ�� */
	enum EDialogEndValue
	{
		E_PicViewClose,		// �����رա�
		E_PicViewDelete,	// �û�Ҫɾ����ǰͼƬ��
	};

	/** ������ʾͼƬ�� */
	void SetPic( LPCTSTR strPicPath );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	/** �û��������߹��ܼ����൱�ڵ����Ļ�ײ���߰�ť */
	virtual void OnClickLeftFunKey();
	/** �û�������ұ߹��ܼ����൱�ڵ������Ļ�ײ��ұ߰�ť�� */
	virtual void OnClickRightFunKey();

private:
	/** ͼ��ؼ��� */
	CPictureStatic m_stPic;

	/** logo �Ի��� */
	CLogoDialog m_logoDlg;

	/** �Ŵ����� */
	float m_fZoomRadio;

	/** �Ƿ��Ѿ�ȫ��. */
	BOOL m_bFullScreen;

	/** �˵������. */
//	HWND m_hMenuBarWnd;

private:
	/** �����ؼ����֡� */
	void UpdateLayout( LPCRECT prcClient = NULL );

	/** ȫ��. */
	void SetFullScreen( BOOL bFullScreen );

	/** �û�Ҫɾ��ͼƬ�� */
	void OnDeletePic();
	/** ���ء� */
	void OnReturn();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuPicViewReturn();
	afx_msg void OnMenuDeletePicFile();
	// ͼƬ��Ϣ��ͼƬ����
	CStaticEx m_stPicInfo;
	// ͼƬ�Ŵ�
	CUIButton m_btnZoomIn;
	// ͼƬ��С
	CUIButton m_btnZoomOut;
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ɾ��ͼƬ��ť
	CUIButton m_btnDelete;
	// ���ذ�ť
	CUIButton m_btnReturn;
	// �ײ�������ͼƬ
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReturn();
};
