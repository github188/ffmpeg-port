#pragma once

#include "browserctrl.h"
#include "uidialog.h"
#include "mculib.h"
#include "logodialog.h"
#include "uibutton.h"
// CWebpageDlg �Ի���

class CWebpageDlg : public CUIDialog, public CVirtualWebpageCtrl
{
	DECLARE_DYNAMIC(CWebpageDlg)

public:
	CWebpageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWebpageDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WEBPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    /** ����ҳ�棬��Ҫ����ʵ�֡�*/
    virtual BOOL OpenUrl( LPCTSTR strUrl );

protected:
    /** �򿪲������� */
    virtual BOOL OpenPlayer( CVideoSession *pVideoSession ) ;

    /** ��ʷ�� */
    virtual BOOL HistoryBack( int nStep );

    /** ���ش�����Ϣ�� */
    virtual void OnShowWindowCmd( int nSWCmd );

    // ���²��֡�
    void UpdateLayout( CRect *prcClient = NULL );
private:
    /** ����������ؼ��� */
    BOOL CreateBrowserCtrl();

    /** �رմ��ڣ�����ʾ�� */
    BOOL CloseWindowWithWarnning();

   
    afx_msg void OnBnClickedButtonPic();
    afx_msg void OnBnClickedButtonConfig();
    afx_msg void OnBnClickedButtonSipButton();
    
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
   
    /** ��Ӧ����webbrowser�ؼ���������Ϣ�� 
    *   �ڴ��ڵĳ�ʼ��ʱֱ�Ӵ�����ʧ�ܣ�����֪��Why������ͨ��������Ϣ�ķ�ʽ������������Ƕ���Ϣ����Ӧ��
    */
    afx_msg LRESULT OnCreateBrowserCtrlCmd( WPARAM, LPARAM );

    /** ��Ӧ�رմ��ڵ�������Ϣ�� */
    afx_msg LRESULT OnCloseWindowCmd( WPARAM, LPARAM );

    /** ��Ӧhtml�ؼ���url���롣 */
    afx_msg LRESULT OnHtmlPreLoad( WPARAM wParam, LPARAM );

private:
    /** ������ؼ��� */
    CBrowserCtrl m_browserCtrl;

    /** logo �Ի���*/
    CLogoDialog m_logoDlg;

    CUIButton m_btnPic;
    CUIButton m_btnConfig;
    CPictureStatic m_stBottomBg;
    // ���뷨���ư�ť
    CUIButton m_btnSip;

protected:
    virtual void OnOK();

public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
};
