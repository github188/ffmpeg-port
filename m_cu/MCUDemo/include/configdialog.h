#pragma once

#include "uidialog.h"
#include "uisliderctrl.h"
#include "staticex.h"
#include "afxwin.h"
#include "uibutton.h"
#include "pictruestatic.h"
// CConfigDialog �Ի���

class CConfigDialog : public CUIDialog
{
	DECLARE_DYNAMIC(CConfigDialog)

public:
	CConfigDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CFG_SYS };

public:
	/** ����ͼƬ·���� */
	enum EPicSavePath
	{
		E_PicSavePhone,	// �������ֻ��ϡ�
		E_PicSaveCard,	// �����ڿ��ϡ�
		E_PicSaveCustom,// �û��Զ���λ�á�
		E_PicSaveExplore, // �����
	};

protected:
	/** �û��������߹��ܼ����൱�ڵ����Ļ�ײ���߰�ť */
	virtual void OnClickLeftFunKey();
	/** �û�������ұ߹��ܼ����൱�ڵ������Ļ�ײ��ұ߰�ť�� */
	virtual void OnClickRightFunKey();

private:
	/** ��������� */
	virtual BOOL SaveConfig();

	/** ���ڸ��²���. */
	void UpdateLayout( LPCRECT prcClient = NULL );

private:

	/** ��ǰ��·���� */
	int m_nOldPicSaveDirSelected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	
	virtual BOOL OnInitDialog();
	// ��������url��ַ��
	CString m_strServerUrl;
	// �Ƿ���ʾ��¼�Ի��򣬷���ֱ����ʾmspҳ�棨��¼ҳ�棩��
	BOOL m_bShowLoginDialog;
	// ��̨�����ٶ�
	CUISliderCtrl m_sliderPtzSpeed;
	// ��ͷ�����ٶ�
	CUISliderCtrl m_sliderLenSpeed;
	// ��̨�����ٶ�
	int m_nPtzSpeed;
	// ��ͷ�����ٶ�
	int m_nLenSpeed;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// �ͻ�����id.
	CString m_strRegionId;
	// ץ��ͼƬ�Ĵ��·��.
	//	CString m_strPicSavePath;
	// ��������ַ����
	CStaticEx m_stServerAddrTitle;
	// �ͻ�ID����
	CStaticEx m_stClientIdTitle;
	// ץ��ͼƬ����·������
	CStaticEx m_stCaptureDirTitle;
	// ��̨���Ʋ�������
	CStaticEx m_stPTZSpeedTitle;
	// ��ͷ�����ٶȱ���
	CStaticEx m_stLenSpeedTitle;
	// ��̨�ٶ�ֵ��ʾ
	CStaticEx m_stPTZSpeedValue;
	// ��ͷ�ٶ�ֵ��ʾ
	CStaticEx m_stLenSpeedValue;
	CComboBox m_cmbPicSaveDir;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	CUIButton m_btnSave;
	CUIButton m_btnReturn;
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonReturn();
	CUIButton m_btnSip;
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnBnClickedCheckShowLoginDialog();
	afx_msg void OnBnClickedButtonSipButton();
	afx_msg void OnCbnCloseupComboPicSaveDir();
	afx_msg void OnCbnSelendokComboPicSaveDir();
	afx_msg void OnCbnDropdownComboPicSaveDir();
	afx_msg void OnEnSetfocusEditServer();
	afx_msg void OnEnSetfocusEditUserRegion();
	afx_msg void OnEnKillfocusEditServer();
	afx_msg void OnEnKillfocusEditUserRegion();
};
