#pragma once

#include "uidialog.h"
#include "pictruestatic.h"
#include "logodialog.h"
#include "afxwin.h"
#include "uibutton.h"
#include "staticex.h"
#include "recordstatic.h"

// CPicManageDialog �Ի���
#define CBasePicManageDialog CUIDialog 

class CPicManageDialog : public CBasePicManageDialog
{
	DECLARE_DYNAMIC(CPicManageDialog)

public:
	CPicManageDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicManageDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PIC_MANAGE };

public:
	/** ����ͼƬ����·���� */
	void SetPicDir( LPCTSTR strPicDir ) ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPrePage();
	afx_msg void OnBnClickedButtonNextPage();

protected:
	/** �û��������߹��ܼ����൱�ڵ����Ļ�ײ���߰�ť */
	virtual void OnClickLeftFunKey();
	/** �û�������ұ߹��ܼ����൱�ڵ������Ļ�ײ��ұ߰�ť�� */
	virtual void OnClickRightFunKey();

private:

	/** ͼƬ·���б� */
	typedef std::vector< tstring > TFilePathList;
//	typedef std::set< tstring > TFilePathSet;

	/** �ļ�����. */
	enum EFileType
	{
		FT_Unknow,	// δ֪�ļ���ʽ.
		FT_Pic,		// ͼƬ�ļ�.
		FT_Rec,		// ¼���ļ�.
	};

	/** ���ִ����еĿؼ��� */
	void UpdateLayout( LPRECT lpRcClient = NULL );

	/** ��ȡ�ؼ�λ�á� 
	*	LPRECT lprcPicSpace : ����ͼƬ�����Ŀռ䡣
	*	int nIndex �� ͼƬ����š�
	*/
	CRect GetPicCtrlPos( int nIndex ) const;

	/** ��ȡͼƬ�ռ䣨����ͼƬ���ڵĵط����� */
	CRect GetPicSpace() const;

	/** ��ȡ�ļ����µ�����ͼƬ�� */
	TFilePathList FindFile( LPCTSTR strDir, LPCTSTR strExtName );

	/** ����ͼƬ��ʾ�� 
	*	bReSearchFile : �Ƿ����������ļ��С�
	*/
	void UpdatePicShow( BOOL bReSearchFile );

	/** ɾ��ͼƬ�� */
	BOOL DelFile( int nPicIndex );

	/** �ж��ļ���ʽ. */
	EFileType GetFileType( LPCTSTR strFileName );

	/** ��*/

	/** ɾ��ͼƬ/¼��������� */
	void OnDelete();
	/** ���ء� */
	void OnReturn();

private:

	/** ÿҳ��ͼƬ�ؼ������� */
	enum
	{
		E_RowNum = 3,
		E_ColumnNum = 3,
		E_PicNumPerPage = E_RowNum * E_ColumnNum,
	};

	/** ͼƬ�ؼ��� */
	CPictureStatic m_arPicSt[ E_PicNumPerPage ];

	/** ¼���ļ��ؼ�. */
	CRecordStatic m_arRecSt[ E_PicNumPerPage ];

	/** ��ǰ��ʾ����*/

	/** ��ǰѡ�е�ͼƬ��š� */
	int m_nCurSelFile;

	/** ��ǰҳ���� */
	int m_nCurPage;

	///** ͼƬ�б� */	
	//TFilePathList m_listPic;

	///** ¼���б�. */
	//TFilePathList m_listRecord;

	/** �ļ��б�. 
	*	����ͼƬ��¼��.
	*/
	TFilePathList m_tFileList;



	/** logo �Ի��� */
	CLogoDialog m_logoDlg;

	/** ��ǰ������·���� */
	tstring m_strPicDir;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ǰһҳͼƬ��
	CUIButton m_btnPrePage;
	// ��һҳͼƬ��
	CUIButton m_btnNextPage;
	// ҳ����Ϣ��
	CStaticEx m_stPageInfo;
	// ҳ����Ϣ��
	CString m_strPageInfo;
	afx_msg void OnMenuDeletePicFile();
	afx_msg void OnMenuReturn();
	// ɾ��ͼƬ��ť
	CUIButton m_btnDelete;
	// ���ذ�ť��
	CUIButton m_btnReturn;
	// �ײ��Ĳ˵�����
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReturn();
};
