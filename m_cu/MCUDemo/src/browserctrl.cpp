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
    m_spIWebBrowser2->put_AddressBar(VARIANT_FALSE);


    ASSERT(SUCCEEDED(hr));
Error:
    return SUCCEEDED(hr) ? 0 : -1;
}

LRESULT CBrowserCtrl::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    int fwSizeType = wParam; 
    int nWidth = LOWORD(lParam); 
    int nHeight = HIWORD(lParam);
    CRect rcClient( 0,0,nWidth, nHeight );
    this->UpdateLayout( &rcClient );
    return 0;
}

BOOL CBrowserCtrl::OpenUrl( LPCTSTR lpstrUrl )
{
    if ( NULL == m_spIWebBrowser2 )
    {
        return FALSE;
    }

    TCHAR szTmp[1000];
    _tcscpy( szTmp, lpstrUrl );
    HRESULT hr = m_spIWebBrowser2->Navigate( szTmp, NULL, NULL, NULL, NULL );
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

    return 0;
}

LRESULT CBrowserCtrl::OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    return 0;
}

LRESULT CBrowserCtrl::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    return 0;
}

// **************************************************************************
//
// event handlers
//
// **************************************************************************

void __stdcall CBrowserCtrl::OnBeforeNavigate2(IDispatch* pDisp, VARIANT * pvtURL, 
                                              VARIANT * /*pvtFlags*/, VARIANT * pvtTargetFrameName,
                                              VARIANT * /*pvtPostData*/, VARIANT * /*pvtHeaders*/, 
                                              VARIANT_BOOL * /*pvbCancel*/)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("0x%08p %s %s"), pDisp, OLE2CT(V_BSTR(pvtURL)),
        VT_ERROR != V_VT(pvtTargetFrameName) ? OLE2CT(V_BSTR(pvtTargetFrameName)) : TEXT("(error)"));
    OutputDebugString(szOutput);

    SetWindowText(TEXT("Untitled")); 

}

void __stdcall CBrowserCtrl::OnBrowserTitleChange(BSTR bstrTitleText)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("%s"), OLE2CT(bstrTitleText));
    OutputDebugString(szOutput);

    SetWindowText(OLE2CT(bstrTitleText)); 
}

void __stdcall CBrowserCtrl::OnNavigateComplete2(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("0x%08p %s"), pDisp, OLE2CT(V_BSTR(pvtURL)));
    OutputDebugString(szOutput);
}

void __stdcall CBrowserCtrl::OnDocumentComplete(IDispatch* pDisp, VARIANT * pvtURL)
{
    USES_CONVERSION;
    TCHAR szOutput[128];

    StringCchPrintf(szOutput, ARRAYSIZE(szOutput), 
        TEXT("0x%08p %s"), pDisp, OLE2CT(V_BSTR(pvtURL)));
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