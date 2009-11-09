#pragma once

#include "uidialog.h"
#include "pictruestatic.h"
#include "logodialog.h"
#include "afxwin.h"
#include "uibutton.h"
#include "staticex.h"
#include "recordstatic.h"

// CPicManageDialog 对话框
#define CBasePicManageDialog CUIDialog 

class CPicManageDialog : public CBasePicManageDialog
{
	DECLARE_DYNAMIC(CPicManageDialog)

public:
	CPicManageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicManageDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PIC_MANAGE };

public:
	/** 设置图片搜索路径。 */
	void SetPicDir( LPCTSTR strPicDir ) ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPrePage();
	afx_msg void OnBnClickedButtonNextPage();

protected:
	/** 用户点击了左边功能键。相当于点击屏幕底部左边按钮 */
	virtual void OnClickLeftFunKey();
	/** 用户点击了右边功能键，相当于点击了屏幕底部右边按钮。 */
	virtual void OnClickRightFunKey();

private:

	/** 图片路径列表。 */
	typedef std::vector< tstring > TFilePathList;
//	typedef std::set< tstring > TFilePathSet;

	/** 文件类型. */
	enum EFileType
	{
		FT_Unknow,	// 未知文件格式.
		FT_Pic,		// 图片文件.
		FT_Rec,		// 录像文件.
	};

	/** 布局窗口中的控件。 */
	void UpdateLayout( LPRECT lpRcClient = NULL );

	/** 获取控件位置。 
	*	LPRECT lprcPicSpace : 所有图片所处的空间。
	*	int nIndex ： 图片的序号。
	*/
	CRect GetPicCtrlPos( int nIndex ) const;

	/** 获取图片空间（所有图片所在的地方）。 */
	CRect GetPicSpace() const;

	/** 获取文件夹下的所有图片。 */
	TFilePathList FindFile( LPCTSTR strDir, LPCTSTR strExtName );

	/** 更新图片显示。 
	*	bReSearchFile : 是否重新搜索文件夹。
	*/
	void UpdatePicShow( BOOL bReSearchFile );

	/** 删除图片。 */
	BOOL DelFile( int nPicIndex );

	/** 判断文件格式. */
	EFileType GetFileType( LPCTSTR strFileName );

	/** 打开*/

	/** 删除图片/录像操作处理。 */
	void OnDelete();
	/** 返回。 */
	void OnReturn();

private:

	/** 每页中图片控件数量。 */
	enum
	{
		E_RowNum = 3,
		E_ColumnNum = 3,
		E_PicNumPerPage = E_RowNum * E_ColumnNum,
	};

	/** 图片控件。 */
	CPictureStatic m_arPicSt[ E_PicNumPerPage ];

	/** 录像文件控件. */
	CRecordStatic m_arRecSt[ E_PicNumPerPage ];

	/** 当前显示出的*/

	/** 当前选中的图片序号。 */
	int m_nCurSelFile;

	/** 当前页数。 */
	int m_nCurPage;

	///** 图片列表。 */	
	//TFilePathList m_listPic;

	///** 录像列表. */
	//TFilePathList m_listRecord;

	/** 文件列表. 
	*	包括图片和录像.
	*/
	TFilePathList m_tFileList;



	/** logo 对话框。 */
	CLogoDialog m_logoDlg;

	/** 当前的搜索路径。 */
	tstring m_strPicDir;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 前一页图片。
	CUIButton m_btnPrePage;
	// 下一页图片。
	CUIButton m_btnNextPage;
	// 页数信息。
	CStaticEx m_stPageInfo;
	// 页码信息。
	CString m_strPageInfo;
	afx_msg void OnMenuDeletePicFile();
	afx_msg void OnMenuReturn();
	// 删除图片按钮
	CUIButton m_btnDelete;
	// 返回按钮。
	CUIButton m_btnReturn;
	// 底部的菜单条。
	CPictureStatic m_stBottomBg;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonReturn();
};
