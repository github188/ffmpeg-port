#pragma once



#include "mculib.h"

#include <webvw.h>
#include "afxwin.h"
// CHtmlWnd 对话框

class CHtmlWnd : public CDialog
{
	DECLARE_DYNAMIC(CHtmlWnd)

public:
	CHtmlWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHtmlWnd();

// 对话框数据
	enum { IDD = IDD_DIALOG_HTML_WND };

	/** 消息. */
	enum EHtmlWndMessage
	{
		WM_HTML_WND_CLICK = WM_HTML_CLICK,		// 用户的点击.wparam: LPCTSTR 的字符串.
		WM_HTML_WND_PRE_LOAD = WM_HTML_PRE_LOAD,// 响应在页面载入之前. wparam:  LPCTSTR的字符串.
		WM_HTML_WND_META = WM_HTML_META,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	/** 打开url. */
	BOOL OpenUrl( LPCTSTR strUrl );

	/** 刷新.*/
	BOOL Refresh();

	/** 重新载入. */
	BOOL Reload();

	/** 停止. */
	BOOL Stop();

	/** 历史倒退. */
	BOOL HistoryBack( int nStep = 1 );

	/** 历史向前. */
	BOOL HistoryForward( int nStep = 1 );

	BOOL IsCanBack(int nStep = 1 );
	BOOL IsCanForward(int nStep = 1 ) ;

	/** 清空页面内容. */
	BOOL Clear();

	/** 清空历史记录 */
	void ClearHistoryURL();

	/** 添加文本内容. */
	BOOL AddPlainText( LPCTSTR strText );

	void HandlePopupMenu ( POINT point );

	/** 获取接口. */
//	IPIEHTMLWindow2 *m_piBrowser; 
//	m_piBrowser->
private:
	/** 劫持的html控件消息处理函数.. */
//	static LRESULT CALLBACK HtmlProc( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam );

	/** 原本的html 控件消息处理函数. */
//	static WNDPROC s_lpOldHtmlProc;
public :
	BOOL m_bWaittingNavigateComplete;
	BOOL m_bIsFaild;

private:
	/** html 控件 窗口. */
	HWND m_hHtmlCtrlWnd;

	/** 是否在等待Navigate完成.*/

	/** 浏览的历史记录. */
	typedef vector< tstring > THistoryUrlList;
	THistoryUrlList m_tHistoryUrl;

	/** 当前页面的历史记录游标. */
	int m_nCurUrlHistoryIndex;
	
	/** 历史记录的起始游标. */
	int m_nHistoryHeadIndex;

	/** 历史记录的结束游标. */
	int m_nHistoryEndIndex;


	/** 最大的历史记录个数. */
	enum EHistory
	{
		MAX_HISTORY = 100,
		INVALID_INDEX = -1,
	};

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL DestroyWindow();
};
