#pragma once


#include "stdafx.h"
#include "mculib.h"

static const UINT ID_BROWSER = 1984;

class CBrowserCtrl :
    public CWindowImpl< CBrowserCtrl >,
    public IDispEventImpl< ID_BROWSER, CBrowserCtrl >//,
 //   public CVirtualWebpageCtrl
{
public:
    CBrowserCtrl();
    ~CBrowserCtrl();

    DECLARE_WND_CLASS(TEXT("MCU.BrowserCtrl"))

    BEGIN_MSG_MAP( CBrowserCtrl )
        //MESSAGE_HANDLER(WM_CREATE, OnCreate)
        //MESSAGE_HANDLER(WM_SIZE, OnSize)
        //MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
        //MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
        //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        //COMMAND_ID_HANDLER(IDM_SK1_EXIT, OnExitCommand)
        //COMMAND_ID_HANDLER(IDM_BACK, OnBackCommand)
        //COMMAND_ID_HANDLER(IDM_FORWARD, OnForwardCommand)
        //COMMAND_ID_HANDLER(IDM_HOME, OnHomeCommand)
        //COMMAND_ID_HANDLER(IDM_OPENURL, OnOpenURLCommand)
        //COMMAND_ID_HANDLER(IDM_REFRESH, OnRefreshCommand)
        //COMMAND_ID_HANDLER(IDM_STOP, OnStopCommand)
    END_MSG_MAP()

public:
    BEGIN_SINK_MAP( CBrowserCtrl )
        //SINK_ENTRY(ID_BROWSER, DISPID_BEFORENAVIGATE2, &CMainWindow::OnBeforeNavigate2)
        //SINK_ENTRY(ID_BROWSER, DISPID_TITLECHANGE, &CMainWindow::OnBrowserTitleChange)
        //SINK_ENTRY(ID_BROWSER, DISPID_NAVIGATECOMPLETE2, &CMainWindow::OnNavigateComplete2)
        //SINK_ENTRY(ID_BROWSER, DISPID_DOCUMENTCOMPLETE, &CMainWindow::OnDocumentComplete)
        //SINK_ENTRY(ID_BROWSER, DISPID_COMMANDSTATECHANGE, &CMainWindow::OnCommandStateChange)
    END_SINK_MAP()

public:
//    /** ����ҳ�棬��Ҫ����ʵ�֡�*/
//    virtual BOOL OpenUrl( LPCTSTR strUrl ) ;
//protected:
//    /** �򿪲������� */
//    virtual BOOL OpenPlayer( CVideoSession *pVideoSession );
//
//    /** ��ʷ�� */
//    virtual BOOL HistoryBack( int nStep );

private:

};