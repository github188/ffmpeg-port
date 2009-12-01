#pragma once


#include "stdafx.h"
#include "mculib.h"
#include <piedocvw.h>
#include <pvdispid.h>
#include "mcu.h"

static const UINT ID_BROWSER = 1984;

class CBrowserCtrl :
    public CWindowImpl< CBrowserCtrl, CWindow, CWinTraits<WS_CLIPCHILDREN | WS_CLIPSIBLINGS>  >,
    public IDispEventImpl< ID_BROWSER, CBrowserCtrl >//,
 //   public CVirtualWebpageCtrl
{
public:
    BOOL OpenUrl( LPCTSTR strUrl );
    BOOL HistoryBack( int nStep );

    /** 设置控件的模拟父窗口。 */
    void SetWebpageParentWnd( HWND hWnd );

public:
    CBrowserCtrl();
    ~CBrowserCtrl();

    DECLARE_WND_CLASS(TEXT("MCU.BrowserCtrl"))

    BEGIN_MSG_MAP( CBrowserCtrl )
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
        //MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER( WM_COMMAND, OnCommand )
        //COMMAND_ID_HANDLER(IDM_SK1_EXIT, OnExitCommand)
        //COMMAND_ID_HANDLER(IDM_BACK, OnBackCommand)
        //COMMAND_ID_HANDLER(IDM_FORWARD, OnForwardCommand)
        //COMMAND_ID_HANDLER(IDM_HOME, OnHomeCommand)
        //COMMAND_ID_HANDLER(IDM_OPENURL, OnOpenURLCommand)
        //COMMAND_ID_HANDLER(IDM_REFRESH, OnRefreshCommand)
        //COMMAND_ID_HANDLER(IDM_STOP, OnStopCommand)

        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_ID_HANDLER(ID_OK, OnOk)
    END_MSG_MAP()

public:
    BEGIN_SINK_MAP( CBrowserCtrl )
        SINK_ENTRY(ID_BROWSER, DISPID_BEFORENAVIGATE2, &CBrowserCtrl::OnBeforeNavigate2)
        SINK_ENTRY(ID_BROWSER, DISPID_TITLECHANGE, &CBrowserCtrl::OnBrowserTitleChange)
        SINK_ENTRY(ID_BROWSER, DISPID_NAVIGATECOMPLETE2, &CBrowserCtrl::OnNavigateComplete2)
        SINK_ENTRY(ID_BROWSER, DISPID_DOCUMENTCOMPLETE, &CBrowserCtrl::OnDocumentComplete)
        SINK_ENTRY(ID_BROWSER, DISPID_COMMANDSTATECHANGE, &CBrowserCtrl::OnCommandStateChange)

        SINK_ENTRY(ID_BROWSER, DISPID_BEFORENAVIGATE, &CBrowserCtrl::OnBeforeNavigate )
    END_SINK_MAP()

public:

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // event handlers
    void __stdcall OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL, 
        VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
        VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/, 
        VARIANT_BOOL * /*pvbCancel*/);
    void __stdcall OnBrowserTitleChange(BSTR bstrTitleText);
    void __stdcall OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL);
    void __stdcall OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL);
    void __stdcall OnCommandStateChange(long lCommand, BOOL bEnable);
    void OnBeforeNavigate(      
        IDispatch *pDisp,
        VARIANT *url,
        VARIANT *Flags,
        VARIANT *TargetFrameName,
        VARIANT *PostData,
        VARIANT *Headers,
        VARIANT_BOOL *Cancel
        );

protected:
    // 更新布局。
    void UpdateLayout( CRect *prcClient = NULL );

private:
    // Represents the PIEWebBrowser control contained in the main application.
    // window. m_browser is used to manage the control and its associated 
    // "AtlAxWin" window. (AtlAxWin is a window class that ATL uses to support 
    // containment of controls in windows.)
    CAxWindow m_browser;

    // cached copy of hosted control's IWebBrowser2 interface pointer
    CComPtr<IWebBrowser2> m_spIWebBrowser2;

    /** Url */
    tstring m_strUrl;

    /** 父窗口。 
    *   因为未知原因，这个窗口作为一个对话框的子窗口时会出现错误，所以现在以一个独立窗口模拟子窗口。
    */
    HWND m_hWebpageParentWnd;

public:
    
    
    
    LRESULT OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};