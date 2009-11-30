/************************************************************************
/* ֧����ͼ�ĶԻ���                                                                     
*	2009.9.2 ����ȫ������ת��Ļ����.����ȫ������ת��Ļ��ϵͳ������һ�������е����޷��ﵽЧ��.
*	��Ϊ��Ϣ��ʽ,����Ϣ�����е���ϵͳ����. by thinkingl.
*	2009.9.18 sip button ��������OnInitDialog�е��õ�ʱ��û��Ч������ȡ��ʱ���õķ�ʽ�ڶԻ�����ʾ֮����á�
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
	/** ���ڼ���ģʽ. 
	*	������"|"���ʹ��,�����غ���ʾ�������ǳ�ͻ��.
	*/
	enum EFullScreenCmd
	{
		FS_HideTaskBar = 1 << 0,	// �����TaskBar��ָ�ֻ����������һ��,������ʼ��ť,������.
		FS_ShowTaskBar = 1 << 1,
		FS_HideMenuBar = 1 << 2,	// MenuBarָ���������һ��,һ�����˵�ʹ��.
		FS_ShowMenuBar = 1 << 3,
		FS_HideSipButton = 1 << 4,	// ����ϵͳ�����뷨��ť��
		FS_ShowSipButton = 1 << 5,

		FS_HorizontalScreen = 1 << 8,	// ����Ļ����.(����ת90��).
		FS_ResumeScreen = 1 << 9,		// �ָ���Ļ����.

		// ���õ����.
		FS_FullScreen = FS_HideMenuBar | FS_HideTaskBar | FS_HideSipButton,	// ȫ��.
//		FS_Resume = FS_ShowMenuBar | FS_ShowTaskBar ,		// �ָ�����ģʽ.
	};

	enum
	{
		DelayEvent_HideSipButton = 89809,	// �������뷨��ť����ʱ�¼�id��
	};

public:
	/** ����ͼƬ��*/
	BOOL SetImage( LPCTSTR strImagePath, BOOL bRePaint = TRUE );

	/** ���õ�ɫ�� */
	BOOL SetColor( COLORREF bgColor, BOOL bRePaint = TRUE );

	/** ȫ�����߻ָ�. 
	*	�������EFullScreenCmd�����.
	*/
	BOOL FullScreen( DWORD dwFullScreenCmd );

	/** ��������ʱ�Ƿ��Զ��ָ�����. */
	void SetAutoResume( BOOL bAutoResume );

    /** ���ô���id�� */
    void SetWindowId( EWindowId eWndId );
    /** ��ȡ����id��*/
    EWindowId GetWindowId() const;

    /** ���ñ����ڹرպ�Ҫ�򿪵ĺ������ڡ� */
    void SetWndAfterClose( EWindowId eWndId );
    /** ��ȡ�������ڡ� */
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
	/** ����һ����ʱ�¼��� */
	BOOL CreateDelayedEvent( int nEventId, int nDelayTime );
	/** ȡ��һ����ʱ�¼��� */
	BOOL CancelDelayedEvent( int nEventId );
	/** ��ʱ�¼��ص��� */
	virtual void OnDelayedEvent( int nEventId );

	/** �û�������ߵĹ��ܼ��� */
	virtual void OnClickLeftFunKey();
	/** �û������ұ߹��ܼ��� */
	virtual void OnClickRightFunKey();

    /** ֪ͨ���࣬ShowWindow�������ˡ� */
    virtual void OnShowWindowCmd( int nSWCmd );


	/** ����������� */
	BOOL LowerSip();

	/** ����������� */
	BOOL RaiseSip( void );

	BOOL IsSipRaise();


private:
	/** ��ʱ�¼�id���ϡ� */
	typedef std::set< int > TEventIdSet;
	TEventIdSet m_setEventId;
	
private:
	/** ��ʼ��. */
	void Init();

	/** ȫ��. */
	LRESULT OnFullScreenCmd( WPARAM, LPARAM );

	/** ��ת��Ļ. */
	LRESULT OnScreenOrientation( WPARAM, LPARAM );

	LRESULT OnSetWindowPosCmd( WPARAM, LPARAM );

	/** ���ڱ����ص�. call WindowsEnumCBForFulscreen.
	*	param: CFullScreen * Point to self.
	*/
	static BOOL CALLBACK  WindowsEnumCBForFulscreenS( HWND hWnd, LPARAM param );
	/** �����menu_worker����. */
	LRESULT OnWindowsEnumNotify( WPARAM hWnd, LPARAM );

	/** �Ƿ���ĳ������. 
	*	eCmd �����Ƕ�� EFullScreenCmd ������.ֻ�������ȫ���������,�ŷ���TRUE.
	*/
	BOOL HasCmd( DWORD eCmd );

	/** �Զ��ָ�. */
	void AutoResume();

private:
	/** ͼƬ·���� */
	tstring m_strImagePath;
	/** ������ɫ�� */
	COLORREF m_corBg;
	/** ������ɫ�Ƿ����ù��� */
	BOOL m_bCorSet;

	/** ԭʼ���ڴ�С. */
	CRect m_rcOldWndRect;

	/** ��ǰ��ȫ������. EFullScreenCmd�����. */
	DWORD m_dwFullScreenCmd;

	/** �洢menu_worker���ھ��. */
	typedef std::vector< HWND > TWndList;
	TWndList m_tMenuWorkerWndList;

	/** ����ʱ�Ƿ��Զ��ָ�����. */
	BOOL m_bAutoResume;

	/** ��Ļ����. */
	DWORD m_dwOriginalScreenMode;
	/** �Ƿ�ı�����Ļ�ķ���. */
	BOOL m_bChangeScreenMode;

    /** ����id�� */
    EWindowId m_eWndId;
    /** �����ڹرպ���ʾ�Ĵ��ڡ� */
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
