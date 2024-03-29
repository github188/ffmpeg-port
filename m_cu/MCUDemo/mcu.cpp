// MCUDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "mcu.h"
#include "MCUSession.h"
#include "mculib.h"
#include <initguid.h>

#include "ImageFactory.h"
#include "windowfactory.h"


/** 单元测试头文件*/
#ifdef UNIT_TEST

#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#endif // #ifdef UNIT_TEST

//#include <locale>
#ifdef UNIT_TEST
#include "MCUDemo_i.c"
#else
#ifdef _DEBUG
#include "UnitTestMainDialog.h"
#endif
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMCUApp


//class CMCUDemoModule :
//	public CAtlMfcModule
//{
//public:
//	DECLARE_LIBID(LIBID_MCUDemoLib);
//	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MCUDEMO, "{B2FF6932-D2DC-4E20-8A78-80AF1772CC03}");};
//
//CMCUDemoModule _AtlModule;


class CM_CUModule :
    public CAtlMfcModule
{
public:
    DECLARE_LIBID(LIBID_MCUDemoLib);
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_M_CU, "{FDBB44A7-354A-4D07-BCA0-F2898A513884}");};

CM_CUModule _AtlModule;

BEGIN_MESSAGE_MAP(CMCUApp, CWinApp)
END_MESSAGE_MAP()



// CMCUApp 构造
CMCUApp::CMCUApp()
	: CWinApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	m_hSingletonAppMutex = NULL;
}


// 唯一的一个 CMCUDemoApp 对象
CMCUApp theApp;

#define MAX_VALUE_NAME 1024

// CMCUApp 初始化

void CMCUApp::BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
{
	HKEY hkey;
	if (RegOpenKeyEx(root, szSubKey, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		//AfxMessageBox( _T( " BackupRtspPlayerRegKey ok!"), MB_TOPMOST );
		DWORD dwType = 0;
		DWORD dwSize = 0;
		DWORD		cbValueName = MAX_VALUE_NAME;
		DWORD		retCode;

		TCHAR		ClassName[MAX_PATH];
		DWORD		dwcClassLen = MAX_PATH;
		DWORD		dwcSubKeys;
		DWORD		dwcMaxSubKey;
		DWORD		dwcMaxClass;
		DWORD		dwcValues;
		DWORD		dwcMaxValueName;
		DWORD		dwcMaxValueData;
		DWORD		dwcSecDesc;
		FILETIME	ftLastWriteTime;

		retCode =
			RegQueryInfoKey (hkey,              // Key handle.
			ClassName,         // Buffer for class name.
			&dwcClassLen,      // Length of class string.
			NULL,              // Reserved.
			&dwcSubKeys,       // Number of sub keys.
			&dwcMaxSubKey,     // Longest sub key size.
			&dwcMaxClass,      // Longest class string.
			&dwcValues,        // Number of values for this key.
			&dwcMaxValueName,  // Longest Value name.
			&dwcMaxValueData,  // Longest Value data.
			&dwcSecDesc,       // Security descriptor.
			&ftLastWriteTime); // Last write time.
		if( retCode != ERROR_SUCCESS )
		{
			Log() << _T( "RegQueryInfoKey 失败！" ) << endl;
		}
		

		BYTE* bData = (BYTE *) LocalAlloc(LMEM_FIXED, dwcMaxValueData);
		dwSize = dwcMaxValueData;
		
		//读取
		if( RegQueryValueEx(hkey, szValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//备份
			if( RegSetValueEx(hkey, szBakValueName, NULL, dwType, bData, dwSize) == ERROR_SUCCESS )
			{
				//设置为空
				WCHAR* szTmp = L"";
				RegSetValueEx(hkey, szValueName, NULL, dwType, (BYTE*)szTmp, 2);
			}
		}
		else
		{
			Log() << _T( "RegQueryValueEx 失败！" ) << endl;
		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		Log() << _T( "BackupRegValue 打开注册表失败！" ) << endl;
	}
}
/*
void CMCUApp::RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
{
	HKEY hkey;

	if (RegOpenKeyEx(root, szSubKey, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS) 
	{
		DWORD dwType = 0;
		DWORD dwSize = 0;
		DWORD		cbValueName = MAX_VALUE_NAME;
		DWORD		retCode;

		TCHAR		ClassName[MAX_PATH];
		DWORD		dwcClassLen = MAX_PATH;
		DWORD		dwcSubKeys;
		DWORD		dwcMaxSubKey;
		DWORD		dwcMaxClass;
		DWORD		dwcValues;
		DWORD		dwcMaxValueName;
		DWORD		dwcMaxValueData;
		DWORD		dwcSecDesc;
		FILETIME	ftLastWriteTime;

		retCode =
			RegQueryInfoKey (hkey,              // Key handle.
			ClassName,         // Buffer for class name.
			&dwcClassLen,      // Length of class string.
			NULL,              // Reserved.
			&dwcSubKeys,       // Number of sub keys.
			&dwcMaxSubKey,     // Longest sub key size.
			&dwcMaxClass,      // Longest class string.
			&dwcValues,        // Number of values for this key.
			&dwcMaxValueName,  // Longest Value name.
			&dwcMaxValueData,  // Longest Value data.
			&dwcSecDesc,       // Security descriptor.
			&ftLastWriteTime); // Last write time.			

		BYTE* bData = (BYTE *) LocalAlloc(LMEM_FIXED, dwcMaxValueData);
		dwSize = dwcMaxValueData;

		//读取
		if( RegQueryValueEx(hkey, szBakValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//还原
			RegSetValueEx(hkey, szValueName, NULL, dwType, bData, dwSize);

		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		Log() << _T( "RestoreRegValue 打开注册表失败！" ) << endl;
	}
}
*/
void CMCUApp::BackupRtspPlayerRegKey()
{
	BackupRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");		
	BackupRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	BackupRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");		
}
/*
void CMCUApp::RestoreRtspPlayerRegKey()
{
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");
}
*/

#define  APP_RUNNING     2
#define  APP_NORMALEXIT  1
#define  APP_FIRSTRUN    0

BOOL CMCUApp::IsAppNormalExit()
{
	UINT dwStatus = GetProfileInt(L"AppStatus",L"Status",APP_FIRSTRUN);
	if(dwStatus != APP_NORMALEXIT)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}



BOOL CMCUApp::InitInstance()
{
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // 在应用程序初始化期间，应调用一次 SHInitExtraControls 以初始化
    // 所有 Windows Mobile 特定控件，如 CAPEDIT 和 SIPPREF。
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// 通过 CoRegisterClassObject() 注册类工厂。
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	//#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	//// 通过 CoRegisterClassObject() 注册类工厂。
	//if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
	//	return FALSE;
	//#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// 应用程序是用 /Embedding 或 /Automation 开关启动的。
	// 将应用程序作为自动化服务器运行。
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// 不显示主窗口
		return TRUE;
	}

	// 应用程序是用 /Register 或 /Regserver 开关启动的。
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		_AtlModule.UpdateRegistryAppId(TRUE);
		_AtlModule.RegisterServer(TRUE);
		return FALSE;
	}


	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Kedacom"));


	

	// 初始化COM。
	HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
	if ( FAILED( hr ) )
	{
		Log() << _T( "COM初始化失败！" ) << endl;
	}

	// 设置界面图片的默认目录.
	tstring strPicPath = ::GetAppDir() + UI_PIC_DIR ;
	CImageFactory::Instance()->SetDefaultPicDir( strPicPath.c_str() );

    // 初始化窗口工厂。
 //   CWindowFactory::Instance()->CreateAllWnd();

	//CMainDialog mainDlg;
	//m_pMainWnd = &mainDlg;
	//mainDlg.DoModal();


	// 判断程序是否是单实例运行.
	m_hSingletonAppMutex = ::CreateMutex( 0, TRUE, _T( "mcu-kedacom" ) );
	if ( m_hSingletonAppMutex )
	{
		if( ERROR_ALREADY_EXISTS == GetLastError() )
		{
			// 已经在运行.
			// 找到已经运行的MCU,将其置于顶端.
			HWND hActiveWnd = GetForegroundWindow();
			HWND hEnumWnd = ::GetWindow( hActiveWnd, GW_HWNDFIRST );
			while ( hEnumWnd )
			{
				::PostMessage( hEnumWnd, WM_ACTIVE_WINDOW, 0, 0 );
				hEnumWnd = ::GetWindow( hEnumWnd, GW_HWNDNEXT );
			}

			return FALSE;
		}
	}
	else
	{
		tstring strTitle;
		CConfig::Instance()->GetAppTitle( strTitle );
		AfxMessageBox( ( strTitle + _T( " 程序初始化失败!" ) ).c_str(), MB_TOPMOST );
	}

	//备份RTSP播放器
 	if( TRUE == IsAppNormalExit() )
 	{
 		BackupRtspPlayerRegKey();
 	}

	//记录进程正常退出状态
	WriteProfileInt(L"AppStatus",L"Status",APP_RUNNING);

// #ifdef UNIT_TEST
// 
//     this->UnitTest();
// 
//  /*   CWebpageDlg dlg;
//     dlg.DoModal();*/
// 
//     CWindowFactory::Instance()->ShowWindow( WndUnittestMain, WndLogin );
// 
//     m_pMainWnd = CWindowFactory::Instance()->GetWnd( WndUnittestMain );
//     return TRUE;
//  //   CUnitTestMainDialog utDlg;
//  //   utDlg.DoModal();
// 
// #else
// 
// #ifdef _DEBUG
//     CUnitTestMainDialog utDlg;
//     utDlg.DoModal();
// #endif 
// 
// #endif

//    CWindowFactory::Instance()->ShowWindow( WndLogin, WndInvalid );
//    m_pMainWnd = CWindowFactory::Instance()->GetWnd( WndLogin );
    // 改为为非模态对话框，所以需要消息泵程序才能运行。
//    return TRUE;

    //CMCUDemoDlg dlg;
    //m_pMainWnd = &dlg;
    //INT_PTR nResponse = dlg.DoModal();
    //if (nResponse == IDOK)
    //{
    //    // TODO: 在此放置处理何时用
    //    //  “确定”来关闭对话框的代码
    //}

	// 改回模态对话框方式。
#ifdef UNIT_TEST

	tstring strLogPath = _T( "\\Storage Card\\" );
	if ( !IsFileExist( strLogPath.c_str() ) )
	{
		strLogPath = _T( "\\存储卡\\" );
	}

	if ( !IsFileExist( strLogPath.c_str() ) )
	{
		strLogPath = _T( "\\" );
	}

	Log().SetLogFileDir( strLogPath.c_str(), _T( "mculog" ) );

	this->UnitTest();
	
	CDialog *pUtDlg = CWindowFactory::Instance()->NewDlg( WndUnittestMain );
//	m_pMainWnd = &utdlg;
	pUtDlg->DoModal();
#endif
	
	CDialog *plogindlg = CWindowFactory::Instance()->NewDlg( WndLogin );
	m_pMainWnd = plogindlg;
	plogindlg->DoModal();

	return FALSE;// exit app.
	

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
//	return FALSE;
}

BOOL CMCUApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch( pMsg->message )
	{
	case WM_ACTIVE_WINDOW:
		if ( m_pMainWnd && !m_pMainWnd->IsTopParentActive())
		{
			// msdn: ms-help://MS.MSDNQTR.v90.chs/wceshellui5/html/wce50lrfsetforegroundwindow.htm
			// If the window being set to the foreground is minimized, 
			// the window may not be visible to the end user. This function also does not reactivate the last active child window by default. 
			//	To ensure that the window is restored and the last active child window is reactivated, 
			//	combine the handle to the window with 0x01 using the logical OR operator. For example:
			SetForegroundWindow((HWND)(((ULONG) m_pMainWnd->GetSafeHwnd()) | 0x01) );
		}
		break;
	default:
	    break;
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

int CMCUApp::Run()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWinApp::Run();
}

BOOL CMCUApp::ExitInstance(void)
{
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	_AtlModule.RevokeClassObjects();
#endif
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	_AtlModule.RevokeClassObjects();
#endif

	// 释放单件类。
    CWindowFactory::Release();
	CImageFactory::Release();
	CConfig::Release();
	CMCUSession::Release();

	//还原RTSP协议播放器
	//RestoreRtspPlayerRegKey();
	//记录进程正常退出状态
	WriteProfileInt(L"AppStatus",L"Status",APP_NORMALEXIT);

	// 释放互斥量.
	ReleaseMutex( m_hSingletonAppMutex );
	CloseHandle( m_hSingletonAppMutex );

	return CWinApp::ExitInstance();
}

#ifdef UNIT_TEST
void CMCUApp::UnitTest()
{

 //   // VC链接的时候总是会忽略掉单元测试类的cpp，这里把单元测试类引用一次，以便强制链接。
 //   // 链接选项修改无效，后续再做深入研究。
 //   CMCUCommonTest tmp0;
 //   CXMLParseTest tmp1;
 //   CMcuConfigTest tmp2;

	//// Get the top level suite from the registry
	//CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

	//// Adds the test to the list of test to run
	//CPPUNIT_NS::TextUi::TestRunner runner;
	//runner.addTest( suite );

	//// Change the default outputter to a compiler error format outputter
	//runner.setOutputter( new CPPUNIT_NS::CompilerOutputter( &runner.result(),
	//	CPPUNIT_NS::stdCOut() ) );
	//// Run the test.
	//bool wasSucessful = runner.run();

    BOOL wasSucessful = RunUnitTest();

	if( !wasSucessful )
	{
		AfxMessageBox( _T( "单元测试未通过!请根据错误信息检查代码!" ) );
	}
}
#endif	// #ifdef UNIT_TEST
