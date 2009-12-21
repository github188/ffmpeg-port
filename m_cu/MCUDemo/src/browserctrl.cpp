#include "browserctrl.h"

#define _ErrorLabel Error


#define CHR(hResult) \
    if(FAILED(hResult)) { hr = (hResult); goto _ErrorLabel;} 


#define CPR(pPointer) \
    if(NULL == (pPointer)) { hr = (E_OUTOFMEMORY); goto _ErrorLabel;} 


#define CBR(fBool) \
    if(!(fBool)) { hr = (E_FAIL); goto _ErrorLabel;} 


#define ARRAYSIZE(s) (sizeof(s) / sizeof(s[0]))


#define RELEASE_OBJ(s)  \
    if (s != NULL)      \
    {                   \
    s->Release();   \
    s = NULL;       \
    }

CBrowserCtrl::CBrowserCtrl()
{
    m_spIWebBrowser2 = NULL;
    this->m_hWebpageParentWnd = NULL;

}

CBrowserCtrl::~CBrowserCtrl()
{

}

//BOOL CBrowserCtrl::OpenUrl( LPCTSTR strUrl )
//{
//    return FALSE;
//}
//
//BOOL CBrowserCtrl::OpenPlayer( CVideoSession *pVideoSession )
//{
//    return FALSE;
//}
//
//BOOL CBrowserCtrl::HistoryBack( int nStep )
//{
//    return FALSE;
//}
//
void CBrowserCtrl::SetWebpageParentWnd( HWND hWnd )
{
    this->m_hWebpageParentWnd = hWnd;
}


LRESULT CBrowserCtrl::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HRESULT hr = S_OK;
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    // In one step, create an "AtlAxWin" window for the PIEWebBrowser control, 
    // and also create the control itself. (AtlAxWin is a window class that 
    // ATL uses to support containment of controls in windows.)
    m_browser.Create(m_hWnd,
        CWindow::rcDefault, // proper sizing is done in CBrowserCtrl::OnSize
        TEXT("Microsoft.PIEDocView"), // ProgID of the control
        WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
        ID_BROWSER);
    CBR(m_browser.m_hWnd != NULL);

    // cache IWebBrowser2 interface pointer
    hr = m_browser.QueryControl(&m_spIWebBrowser2);
    CHR(hr);

    // set up connection point
    hr = AtlAdviseSinkMap(this, true);
    CHR(hr);

    // set initial properties for the control
    m_spIWebBrowser2->put_AddressBar(VARIANT_TRUE);

//    m_spIWebBrowser2->get_

 //   m_spIWebBrowser2->put_MenuBar( VARIANT_FALSE );


    ASSERT(SUCCEEDED(hr));

    
Error:
    LRESULT hResult = SUCCEEDED(hr) ? 0 : -1;
    if ( 0 == hResult && !m_strUrl.empty() )
    {
        this->OpenUrl( m_strUrl.c_str() );
    }
    return hResult;
}

LRESULT CBrowserCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    int fwSizeType = wParam; 
    int nWidth = LOWORD(lParam); 
    int nHeight = HIWORD(lParam);
    CRect rcClient( 0,0,nWidth, nHeight );
    this->UpdateLayout( &rcClient );

    Log() << _T( "CBrowserCtrl::OnSize. type: " ) << fwSizeType << 
        _T( " w: ") << nWidth << _T( " h: " ) << nHeight << endl;


    if ( SIZE_MINIMIZED == fwSizeType )
    {
        Log() << _T( "web browser control is minimized!" ) << endl;

        if ( this->m_hWebpageParentWnd )
        {
//            bHandled = TRUE;

            //WPARAM wParam = MAKELONG( wID, wNotifyCode );
            //LPARAM lParam = (LPARAM)hWndCtl;
            UINT uMessage = WM_WINDOW_CLOSE_CMD;

            BOOL bClose = ::SendMessage( m_hWebpageParentWnd, WM_WINDOW_CLOSE_CMD, 0, 0 );

            Log() << _T( "Send WM_WINDOW_CLOSE_CMD parent! msg: " ) 
                << uMessage << _T( " wp: " ) << 0 << _T( " lp: " ) << 0 << _T( " ret: " ) << bClose << endl;

            if ( !bClose )
            {
                bHandled = TRUE;
 //               this->ShowWindow( SW_HIDE );
                this->ShowWindow( SW_SHOW );
            }
            else
            {
                bHandled = TRUE;
                this->ShowWindow( SW_HIDE );
            }

 //           ::PostMessage( m_hWebpageParentWnd, uMessage, 0, 0 );
 //           Log() << _T( "Minimize the web browser,hide parent wnd! " ) << m_hWebpageParentWnd << endl;
 //           ::ShowWindow( m_hWebpageParentWnd, SW_HIDE );
        }
        else
        {
            Log() << _T( "The webpage parent wnd is NULL! When minimize the web browser!" ) << endl;
        }
    }

    return 0;
}

BOOL CBrowserCtrl::OpenUrl( LPCTSTR lpstrUrl )
{
    m_strUrl = lpstrUrl;

    if ( NULL == m_spIWebBrowser2 )
    {
        Log() << _T( "CBrowserCtrl::OpenUrl m_spIWebBrowser2 is NULL!" ) << endl;
        return FALSE;
    }

    tstring strUrl = StringToUrl( lpstrUrl );

   

    TCHAR szTmp[1000] = {0};
    _tcscpy( szTmp, strUrl.c_str() );
    HRESULT hr = m_spIWebBrowser2->Navigate( szTmp, NULL, NULL, NULL, NULL );

    Log() << _T( "Open Url: " ) << lpstrUrl << _T( " ret: " ) << hr << endl;
    return SUCCEEDED( hr );
}

BOOL CBrowserCtrl::HistoryBack( int nStep )
{
    return FALSE;
}

void CBrowserCtrl::UpdateLayout( CRect *prcClient /* = NULL */ )
{
    CRect rcClient;
    if ( NULL == prcClient )
    {
        this->GetClientRect( rcClient );
    }
    else
    {
        rcClient = *prcClient;
    }

    if ( m_browser.IsWindow() )
    {
        m_browser.MoveWindow( rcClient );
    }
}


LRESULT CBrowserCtrl::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    // Tear down connection points while controls are still alive.
    VERIFY(SUCCEEDED(AtlAdviseSinkMap(this, false)));

    m_spIWebBrowser2 = NULL;
    m_browser = NULL;
    return 0;
}

LRESULT CBrowserCtrl::OnActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    // Notify shell of our WM_ACTIVATE message
 //   SHHandleWMActivate(m_hWnd, wParam, lParam, &m_sai, 0);
    Log() << _T( "CBrowserCtrl::OnActivate" ) << endl;
    return 0;
}

LRESULT CBrowserCtrl::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    bHandled = TRUE;

    return 0;
}

LRESULT CBrowserCtrl::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    WORD wNotifyCode = HIWORD(wParam); 
    WORD wID = LOWORD(wParam); 
    HWND hwndCtl = (HWND) lParam;

    if( wID == IDOK )
    {
        int dfw = 323;
    }
    return 0;
}

// **************************************************************************
//
// event handlers
//
// **************************************************************************

void CBrowserCtrl::OnBeforeNavigate(IDispatch* pDisp, 
                                     VARIANT * pvtURL, VARIANT * , VARIANT * pvtTargetFrameName, 
                                     VARIANT * , VARIANT * , VARIANT_BOOL * pvbCancel )
{   
    tstring strUrl =  OLE2CT(V_BSTR(pvtURL));

Log() << _T( "CBrowserCtrl::OnBeforeNavigate url: " ) << strUrl << endl;


if ( m_hWebpageParentWnd )
{
    BOOL bHandled = ::SendMessage( m_hWebpageParentWnd, WM_HTML_PRE_LOAD, (WPARAM)strUrl.c_str(), NULL );

 //   this->m_spIWebBrowser2->Stop();
    *pvbCancel = -1;
}
else
{
    Log() << _T( "CBrowserCtrl::OnBeforeNavigate2 when webpage parent wnd is NULL!" ) << endl;
}

}

void __stdcall CBrowserCtrl::OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL, 
                                              VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
                                              VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/, 
                                              VARIANT_BOOL * pvbCancel)
{
    //USES_CONVERSION;
    //TCHAR szOutput[512] = { 0 };

    //StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
    //    TEXT("OnBeforeNavigate2 0x%08p %s %s\n"), pDisp, OLE2CT(V_BSTR(pvtURL)),
    //    VT_ERROR != V_VT(pvtTargetFrameName) ? OLE2CT(V_BSTR(pvtTargetFrameName)) : TEXT("(error)"));
    //OutputDebugString(szOutput);
    tstring strUrl =  OLE2CT(V_BSTR(pvtURL));

    Log() << _T( "CBrowserCtrl::OnBeforeNavigate2 url: " ) << strUrl << endl;

    CComPtr<IWebBrowser2> spIWebBrowser2 = NULL;
    pDisp->QueryInterface( &spIWebBrowser2 );

    if ( m_hWebpageParentWnd  )
    {
        BOOL bHandled = ::SendMessage( m_hWebpageParentWnd, WM_HTML_PRE_LOAD, (WPARAM)strUrl.c_str(), NULL );

        if ( bHandled )
        {
            //if ( spIWebBrowser2 )
            //{
            //    spIWebBrowser2->Stop();
            //}
            //this->m_spIWebBrowser2->Stop();
//            static const CComBSTR newURL = L"http://www.g.cn";
//            this->m_spIWebBrowser2->Navigate(newURL, NULL, NULL, NULL, NULL);
            //       this->m_spIWebBrowser2->Stop();
            *pvbCancel = VARIANT_TRUE;
        }
        
    }
    else
    {
        Log() << _T( "CBrowserCtrl::OnBeforeNavigate2 when webpage parent wnd is NULL!" ) << endl;
    }

//   SetWindowText(TEXT("Untitled")); 

}

void __stdcall CBrowserCtrl::OnBrowserTitleChange(BSTR bstrTitleText)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("OnBrowserTitleChange %s\n"), OLE2CT(bstrTitleText));
    OutputDebugString(szOutput);

//    SetWindowText(OLE2CT(bstrTitleText)); 
}

void __stdcall CBrowserCtrl::OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("OnNavigateComplete2 0x%08p %s\n"), pDisp, OLE2CT(V_BSTR(pvtURL)));
    OutputDebugString(szOutput);
}

void __stdcall CBrowserCtrl::OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("OnDocumentComplete 0x%08p %s\n"), pDisp, OLE2CT(V_BSTR(pvtURL)));
    OutputDebugString(szOutput);

//    VERIFY(SetEnabledState(IDM_STOP, FALSE));
}

void __stdcall CBrowserCtrl::OnCommandStateChange(long lCommand, BOOL bEnable)
{
    if (CSC_NAVIGATEBACK == lCommand)
    {
 //       VERIFY(SetEnabledState(IDM_BACK, bEnable));
    }
    else if (CSC_NAVIGATEFORWARD == lCommand)
    {
 //       VERIFY(SetEnabledState(IDM_FORWARD, bEnable));
    }
}
LRESULT CBrowserCtrl::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    // TODO: 在此添加命令处理程序代码
  /*  if(uMsg == WM_COMMAND && id == LOWORD(wParam)) \
    { \
    bHandled = TRUE; \
    lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
    if(bHandled) \
        return TRUE; \
    }*/


    return 0;
}
