#pragma once



#include "mculib.h"

#include <webvw.h>
#include "afxwin.h"
// CHtmlWnd �Ի���

class CHtmlWnd : public CDialog
{
	DECLARE_DYNAMIC(CHtmlWnd)

public:
	CHtmlWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHtmlWnd();

// �Ի�������
	enum { IDD = IDD_DIALOG_HTML_WND };

	/** ��Ϣ. */
	enum EHtmlWndMessage
	{
		WM_HTML_WND_CLICK = WM_HTML_CLICK,		// �û��ĵ��.wparam: LPCTSTR ���ַ���.
		WM_HTML_WND_PRE_LOAD = WM_HTML_PRE_LOAD,// ��Ӧ��ҳ������֮ǰ. wparam:  LPCTSTR���ַ���.
		WM_HTML_WND_META = WM_HTML_META,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	/** ��url. */
	BOOL OpenUrl( LPCTSTR strUrl );

	/** ˢ��.*/
	BOOL Refresh();

	/** ��������. */
	BOOL Reload();

	/** ֹͣ. */
	BOOL Stop();

	/** ��ʷ����. */
	BOOL HistoryBack( int nStep = 1 );

	/** ��ʷ��ǰ. */
	BOOL HistoryForward( int nStep = 1 );

	BOOL IsCanBack(int nStep = 1 );
	BOOL IsCanForward(int nStep = 1 ) ;

	/** ���ҳ������. */
	BOOL Clear();

	/** �����ʷ��¼ */
	void ClearHistoryURL();

	/** ����ı�����. */
	BOOL AddPlainText( LPCTSTR strText );

	void HandlePopupMenu ( POINT point );

	/** ��ȡ�ӿ�. */
//	IPIEHTMLWindow2 *m_piBrowser; 
//	m_piBrowser->
private:
	/** �ٳֵ�html�ؼ���Ϣ������.. */
//	static LRESULT CALLBACK HtmlProc( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam );

	/** ԭ����html �ؼ���Ϣ������. */
//	static WNDPROC s_lpOldHtmlProc;
public :
	BOOL m_bWaittingNavigateComplete;
	BOOL m_bIsFaild;

private:
	/** html �ؼ� ����. */
	HWND m_hHtmlCtrlWnd;

	/** �Ƿ��ڵȴ�Navigate���.*/

	/** �������ʷ��¼. */
	typedef vector< tstring > THistoryUrlList;
	THistoryUrlList m_tHistoryUrl;

	/** ��ǰҳ�����ʷ��¼�α�. */
	int m_nCurUrlHistoryIndex;
	
	/** ��ʷ��¼����ʼ�α�. */
	int m_nHistoryHeadIndex;

	/** ��ʷ��¼�Ľ����α�. */
	int m_nHistoryEndIndex;


	/** ������ʷ��¼����. */
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
