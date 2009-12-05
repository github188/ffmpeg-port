#include "StdAfx.h"
#include "windowfactory.h"
#include "mcu.h"

#include "uidialog.h"

#include "logindialog.h"
#include "picmanagedialog.h"
#include "picviewdialog.h"
#include "configdialog.h"
#include "PlayerDialog.h"
#include "webpagedlg.h"

#ifdef UNIT_TEST
#include "HtmlTestDialog.h"
#include "RTSPTestDialog.h"
#include "UnitTestMainDialog.h"
#include "UITestDialog.h"

#endif

CWindowFactory * CWindowFactory::s_instance = NULL;

CWindowFactory::CWindowFactory(void)
{
}

CWindowFactory::~CWindowFactory(void)
{
    for ( int i=0; i<WndAllNum; ++i )
    {
        CUIDialog *pWnd = this->GetWnd( EWindowId( i ) );
        if ( pWnd && pWnd->GetSafeHwnd() )
        {
            pWnd->DestroyWindow();
        }
        if ( pWnd )
        {
            delete pWnd;
        }
    }
    m_tWndTable.clear();
}

CWindowFactory * CWindowFactory::Instance()
{
    if ( NULL == s_instance )
    {
        s_instance = new CWindowFactory();
    }
    return s_instance;
}

void CWindowFactory::Release()
{
    if ( s_instance )
    {
        delete s_instance;
        s_instance = NULL;
    }
}

CUIDialog *CWindowFactory::CreateWnd( EWindowId eWndId )
{
    CUIDialog *pDlg = NULL;
    if ( NULL == pDlg )
    {
        switch( eWndId )
        {
        case WndLogin:
            pDlg = new CLoginDialog();
            if ( pDlg )
            {
                pDlg->Create( CLoginDialog::IDD );
            }                
            break;
        case WndWebpage:
            pDlg = new CWebpageDlg();
            if ( pDlg )
            {
                pDlg->Create( CWebpageDlg::IDD );
            } 
            break;
        case WndPicManage:
            pDlg = new CPicManageDialog();
            if ( pDlg )
            {
                pDlg->Create( CPicManageDialog::IDD );
            } 
            break;
        case WndPicView:
            pDlg = new CPicViewDialog();
            if ( pDlg )
            {
                pDlg->Create( CPicViewDialog::IDD );
            } 
            break;
        case WndPlayer:
            pDlg = new CPlayerDialog();
            if ( pDlg )
            {
                pDlg->Create( CPlayerDialog::IDD );
            } 
            break;
        case WndConfig:
            pDlg = new CConfigDialog();
            if ( pDlg )
            {
                pDlg->Create( CConfigDialog::IDD );
            } 
            break;
#ifdef UNIT_TEST
        case WndUnittestMain:
            pDlg = new CUnitTestMainDialog();
            if ( pDlg )
            {
                pDlg->Create( CUnitTestMainDialog::IDD );
            }
            break;
        case WndUnittestUI:                
            pDlg = new CUITestDialog();
            if ( pDlg )
            {
                pDlg->Create( CUITestDialog::IDD );
            }
            break;
        case WndUnittestWebpage:
            pDlg = new CHtmlTestDialog();
            if ( pDlg )
            {
                pDlg->Create( CHtmlTestDialog::IDD );
            }
            break;
        case WndUnittestRtsp:
            pDlg = new CRTSPTestDialog();
            if( pDlg )
            {
                pDlg->Create( CRTSPTestDialog::IDD );
            }
            break;
#endif
        default:
            break;
        }
    }

    return pDlg;
}

BOOL CWindowFactory::CreateAllWnd()
{
    for ( int i=0; i<WndAllNum; ++i )
    {
        EWindowId eWndId = EWindowId( i );

        CUIDialog *pDlg = this->CreateWnd( eWndId );

        if ( pDlg )
        {
            pDlg->SetWindowId( eWndId );
            pDlg->ShowWindow( SW_HIDE );
            this->m_tWndTable[ eWndId ] = pDlg;
        }
        
    }
    return TRUE;
}

CUIDialog *CWindowFactory::GetWnd( EWindowId eWndId )
{
    CUIDialog *pWnd = this->m_tWndTable[ eWndId ];


    return pWnd;
}

BOOL CWindowFactory::ShowWindow( EWindowId eShowWndId, EWindowId eShowAfter )
{
    CUIDialog *pWnd = this->GetWnd( eShowWndId );

    if ( NULL == pWnd )
    {
        // 如果没有，尝试创建。
        pWnd = this->CreateWnd( eShowWndId );
        pWnd->SetWindowId( eShowWndId );
        this->m_tWndTable[ eShowWndId ] = pWnd;
    }
   

    if ( pWnd )
    {
        pWnd->SetWndAfterClose( eShowAfter );
        pWnd->ShowWindow( SW_SHOW );   
  //      pWnd->SetForegroundWindow();
  //      SetForegroundWindow((HWND)((ULONG) pWnd->GetSafeHwnd() | 0x00000001));
    }
    else
    {
        mcu::log << _T( "CWindowFactory::ShowWindow can't find the window: " ) << eShowWndId << endl;
        return FALSE;
    }

    // 隐藏其它窗口。
    for ( int i=0; i<WndAllNum; ++i )
    {
        if ( i != eShowWndId )
        {
            CUIDialog *pHideWnd = GetWnd( EWindowId( i ) );
            if ( pHideWnd )
            {
                pHideWnd->ShowWindow( SW_HIDE );
            }
        }
    }
    return TRUE;
}

