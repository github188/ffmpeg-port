#pragma once
#include "afxwin.h"
#include "pictruestatic.h"
#include "logodialog.h"
#include "staticex.h"
#include "checkbutton.h"
#include "uibutton.h"

// CPPCLoginDialog �Ի���

#define CBasePPCLoginDialog CUIDialog 
class CPPCLoginDialog : public CBasePPCLoginDialog
{
	DECLARE_DYNAMIC(CPPCLoginDialog)

public:
	enum ELoginType
	{
		LOGIN_NORMAL = 0,		// ��ͨ��¼��
		LOGIN_LOCAL_PHONE = 1,	// ������¼��
		LOGIN_CANCEL,			// ȡ����¼��

	};

public:
	CPPCLoginDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPPCLoginDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �û��˻�ID
	CString m_strUserId;
	// ����
	CString m_strPassword;
	// ����/��������
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLoginThisPhone();
protected:
	virtual void OnOK();

	/** �û��������߹��ܼ����൱�ڵ����Ļ�ײ���߰�ť */
	virtual void OnClickLeftFunKey();
	/** �û�������ұ߹��ܼ����൱�ڵ������Ļ�ײ��ұ߰�ť�� */
	virtual void OnClickRightFunKey();

public:
	virtual BOOL OnInitDialog();

private:
	void ReadConfig();
	void SaveConfig();

	/** �û����ͼƬ�� */
	void OnPicManage();
	/** �û�������á� */
	void OnConfig();

	/** ����������ʾ�û��� 
	*	����ֵ�� TRUE ͨ���� FALSE ����У�����
	*/
	BOOL CheckAndAlert( ELoginType eLoginType );

private:


public:
	// �Ƿ񱣴��û���¼��Ϣ
	BOOL m_bSaveLoginInfo;
	// �Ƿ���3G������,�ۿ���������Ƶ.
	BOOL m_b3GStream;

	// �û��ĵ�¼��ʽ.
	ELoginType m_eLoginType;

	/** soft key bar. */
//	HWND m_hSoftKeyBar;
	afx_msg void OnMenuLogin();
	afx_msg void OnMenuConfig();
	// ����Logo
//	CPictureStatic m_cLogo;

	// logo �Ի���
	CLogoDialog m_logoDlg;

	afx_msg void OnSize(UINT nType, int cx, int cy);

	// ���ڸ��²���.
	void UpdateLayout( LPCRECT prcClient = NULL );

	CStaticEx m_stUserIdTitle;
	CStaticEx m_stPasswordTitle;
	CStaticEx m_stMCUVer;
	CCheckButton m_ckSaveLoginInfo;
	CCheckButton m_ckStreamType;
	// �ʺŵ�¼
	CUIButton m_btnLogin;
	// һ����¼�������ֻ��������
	CUIButton m_btnLoginThisPhone;
	// �û�ID��logoͼƬ
	CPictureStatic m_stUserIdLogo;
	// �����logoͼƬ
	CPictureStatic m_stPasswordLogo;
	// �û�ID����ؼ�
	CEdit m_edUserId;
	// ��������ؼ���
	CEdit m_edPassword;
	afx_msg void OnMenuPic();
	// �˵����е�ͼƬ/¼�����ť��
	CUIButton m_btnPicRecManage;
	// �˵���������ð�ť��
	CUIButton m_btnConfig;
	// �˵����ı�����
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonPicRecManage();
	afx_msg void OnBnClickedButtonConfig();
	// ���뷨��ť��
	CUIButton m_btnSip;
	afx_msg void OnBnClickedButtonSipButton();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnEnSetfocusEditUerid();
	afx_msg void OnEnSetfocusEditPassword();
	afx_msg void OnEnKillfocusEditUerid();
	afx_msg void OnEnKillfocusEditPassword();
};
