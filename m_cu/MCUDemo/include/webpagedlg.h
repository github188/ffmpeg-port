#pragma once

#include "browserctrl.h"
#include "uidialog.h"
#include "mculib.h"
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

    // ���²��֡�
    void UpdateLayout( CRect *prcClient = NULL );
private:
    /** ����������ؼ��� */
    BOOL CreateBrowserCtrl();

private:
    CBrowserCtrl m_browserCtrl;
protected:
    virtual void OnOK();
public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
