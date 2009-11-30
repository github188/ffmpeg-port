/************************************************************************
/* 支持贴图的对话框。                                                                     
*	2009.9.2 增加全屏和旋转屏幕功能.发现全屏和旋转屏幕的系统功能在一个函数中调用无法达到效果.
*	改为消息方式,在消息处理中调用系统功能. by thinkingl.
*	2009.9.18 sip button 的隐藏在OnInitDialog中调用的时候没有效果，采取延时调用的方式在对话框显示之后调用。
/************************************************************************/


#pragma once
#include "afxwin.h"
#include "mculib.h"
#include <set>

#include "windowfactory.h"

class CUIDialog :
	public CDialog
{
	DECLARE_DYNAMIC(CUIDialog)

public:
	/** 窗口几种模式. 
	*	可以用"|"组合使用,但隐藏和显示的命令是冲突的.
	*/
	enum EFullScreenCmd
	{
		FS_HideTaskBar = 1 << 0,	// 这里的TaskBar是指手机程序上面的一条,包括开始按钮,任务栏.
		FS_ShowTaskBar = 1 << 1,
		FS_HideMenuBar = 1 << 2,	// MenuBar指程序下面的一条,一般做菜单使用.
		FS_ShowMenuBar = 1 << 3,
		FS_HideSipButton = 1 << 4,	// 隐藏系统的输入法按钮。
		FS_ShowSipButton = 1 << 5,

		FS_HorizontalScreen = 1 << 8,	// 将屏幕横置.(即旋转90度).
		FS_ResumeScreen = 1 << 9,		// 恢复屏幕方向.

		// 常用的组合.
		FS_FullScreen = FS_HideMenuBar | FS_HideTaskBar | FS_HideSipButton,	// 全屏.
//		FS_Resume = FS_ShowMenuBar | FS_ShowTaskBar ,		// 恢复正常模式.
	};

	enum
	{
		DelayEvent_HideSipButton = 89809,	// 隐藏输入法按钮的延时事件id。
	};

public:
	/** 设置图片。*/
	BOOL SetImage( LPCTSTR strImagePath, BOOL bRePaint = TRUE );

	/** 设置底色。 */
	BOOL SetColor( COLORREF bgColor, BOOL bRePaint = TRUE );

	/** 全屏或者恢复. 
	*	传入的是EFullScreenCmd的组合.
	*/
	BOOL FullScreen( DWORD dwFullScreenCmd );

	/** 设置销毁时是否自动恢复窗口. */
	void SetAutoResume( BOOL bAutoResume );

    /** 设置窗口id。 */
    void SetWindowId( EWindowId eWndId );
    /** 获取窗口id。*/
    EWindowId GetWindowId() const;

    /** 设置本窗口关闭后要打开的后续窗口。 */
    void SetWndAfterClose( EWindowId eWndId );
    /** 获取后续窗口。 */
    EWindowId GetWndAfterClose( ) const;


public:
	CUIDialog(void);
	~CUIDialog(void);

//	explicit CDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	explicit CUIDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

public:
    BOOL ShowWindow(int nCmdShow );

    // termination
    void EndDialog(int nResult);

protected:
	/** 创建一个延时事件。 */
	BOOL CreateDelayedEvent( int nEventId, int nDelayTime );
	/** 取消一个延时事件。 */
	BOOL CancelDelayedEvent( int nEventId );
	/** 延时事件回调。 */
	virtual void OnDelayedEvent( int nEventId );

	/** 用户按了左边的功能键。 */
	virtual void OnClickLeftFunKey();
	/** 用户按了右边功能键。 */
	virtual void OnClickRightFunKey();

    /** 通知子类，ShowWindow被调用了。 */
    virtual void OnShowWindowCmd( int nSWCmd );


	/** 隐藏输入面板 */
	BOOL LowerSip();

	/** 升起输入面板 */
	BOOL RaiseSip( void );

	BOOL IsSipRaise();


private:
	/** 延时事件id集合。 */
	typedef std::set< int > TEventIdSet;
	TEventIdSet m_setEventId;
	
private:
	/** 初始化. */
	void Init();

	/** 全屏. */
	LRESULT OnFullScreenCmd( WPARAM, LPARAM );

	/** 翻转屏幕. */
	LRESULT OnScreenOrientation( WPARAM, LPARAM );

	LRESULT OnSetWindowPosCmd( WPARAM, LPARAM );

	/** 窗口遍历回调. call WindowsEnumCBForFulscreen.
	*	param: CFullScreen * Point to self.
	*/
	static BOOL CALLBACK  WindowsEnumCBForFulscreenS( HWND hWnd, LPARAM param );
	/** 处理掉menu_worker窗口. */
	LRESULT OnWindowsEnumNotify( WPARAM hWnd, LPARAM );

	/** 是否有某个命令. 
	*	eCmd 可以是多个 EFullScreenCmd 或的组合.只有组合中全部都满足的,才返回TRUE.
	*/
	BOOL HasCmd( DWORD eCmd );

	/** 自动恢复. */
	void AutoResume();

private:
	/** 图片路径。 */
	tstring m_strImagePath;
	/** 背景颜色。 */
	COLORREF m_corBg;
	/** 背景颜色是否设置过。 */
	BOOL m_bCorSet;

	/** 原始窗口大小. */
	CRect m_rcOldWndRect;

	/** 当前的全屏命令. EFullScreenCmd的组合. */
	DWORD m_dwFullScreenCmd;

	/** 存储menu_worker窗口句柄. */
	typedef std::vector< HWND > TWndList;
	TWndList m_tMenuWorkerWndList;

	/** 销毁时是否自动恢复窗口. */
	BOOL m_bAutoResume;

	/** 屏幕方向. */
	DWORD m_dwOriginalScreenMode;
	/** 是否改变了屏幕的方向. */
	BOOL m_bChangeScreenMode;

    /** 窗口id。 */
    EWindowId m_eWndId;
    /** 本窗口关闭后显示的窗口。 */
    EWindowId m_eWndAfterClose;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// for translating Windows messages in main message pump
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    virtual void OnOK();
    virtual void OnCancel();

protected:
    DECLARE_MESSAGE_MAP()


    afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    afx_msg LRESULT OnCloseWindowCmd( WPARAM, LPARAM );
    
public:
    afx_msg void OnClose();
};
