// MCUDemo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "MCUDemoDlg.h"
#include "MCUSession.h"

#include "mculib.h"
#include <initguid.h>
//#include "MCUDemo_i.c"

#include "MainDialog.h"

#include "ImageFactory.h"

/** ��Ԫ����ͷ�ļ�*/
#ifdef UNIT_TEST

#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#endif // #ifdef UNIT_TEST

//#include <locale>
#ifdef UNIT_TEST
#include "UnitTestMainDialog.h"
#include "MCUDemo_i.c"
#else
#ifdef _DEBUG
#include "UnitTestMainDialog.h"
#endif
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMCUDemoApp


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

BEGIN_MESSAGE_MAP(CMCUDemoApp, CWinApp)
END_MESSAGE_MAP()



// CMCUDemoApp ����
CMCUDemoApp::CMCUDemoApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	m_hSingletonAppMutex = NULL;
}


// Ψһ��һ�� CMCUDemoApp ����
CMCUDemoApp theApp;

#define MAX_VALUE_NAME 1024

// CMCUDemoApp ��ʼ��

void CMCUDemoApp::BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
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
			mcu::log << _T( "RegQueryInfoKey ʧ�ܣ�" ) << endl;
		}
		

		BYTE* bData = (BYTE *) LocalAlloc(LMEM_FIXED, dwcMaxValueData);
		dwSize = dwcMaxValueData;
		
		//��ȡ
		if( RegQueryValueEx(hkey, szValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//����
			if( RegSetValueEx(hkey, szBakValueName, NULL, dwType, bData, dwSize) == ERROR_SUCCESS )
			{
				//����Ϊ��
				WCHAR* szTmp = L"";
				RegSetValueEx(hkey, szValueName, NULL, dwType, (BYTE*)szTmp, 2);
			}
		}
		else
		{
			mcu::log << _T( "RegQueryValueEx ʧ�ܣ�" ) << endl;
		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		mcu::log << _T( "BackupRegValue ��ע���ʧ�ܣ�" ) << endl;
	}
}
/*
void CMCUDemoApp::RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName)
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

		//��ȡ
		if( RegQueryValueEx(hkey, szBakValueName, NULL, &dwType, bData, &dwSize) == ERROR_SUCCESS )
		{
			//��ԭ
			RegSetValueEx(hkey, szValueName, NULL, dwType, bData, dwSize);

		}
		LocalFree ((HLOCAL)bData);
		RegCloseKey(hkey);
	}
	else
	{
		mcu::log << _T( "RestoreRegValue ��ע���ʧ�ܣ�" ) << endl;
	}
}
*/
void CMCUDemoApp::BackupRtspPlayerRegKey()
{
	BackupRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");		
	BackupRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	BackupRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");		
}
/*
void CMCUDemoApp::RestoreRtspPlayerRegKey()
{
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\Shell\\Open\\Command", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"RTSP\\DefaultIcon", NULL, L"KdmBackup");
	RestoreRegValue(HKEY_CLASSES_ROOT, L"PROTOCOLS\\Handler\\RTSP", L"CLSID", L"KdmBackup");
}
*/

#define  APP_RUNNING     2
#define  APP_NORMALEXIT  1
#define  APP_FIRSTRUN    0

BOOL CMCUDemoApp::IsAppNormalExit()
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



BOOL CMCUDemoApp::InitInstance()
{
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // ��Ӧ�ó����ʼ���ڼ䣬Ӧ����һ�� SHInitExtraControls �Գ�ʼ��
    // ���� Windows Mobile �ض��ؼ����� CAPEDIT �� SIPPREF��
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// ͨ�� CoRegisterClassObject() ע���๤����
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// ͨ�� CoRegisterClassObject() ע���๤����
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// Ӧ�ó������� /Embedding �� /Automation ���������ġ�
	// ��Ӧ�ó�����Ϊ�Զ������������С�
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// ����ʾ������
		return TRUE;
	}
	//// Ӧ�ó������� /Unregserver �� /Unregister ���������ġ�
	//if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
		_AtlModule.UpdateRegistryAppId(FALSE);
		_AtlModule.UnregisterServer(TRUE);
	//{
	//	_AtlModule.UpdateRegistryAppId(FALSE);
	//	_AtlModule.UnregisterServer(TRUE);
	//	return FALSE;
	//}
	//// Ӧ�ó������� /Register �� /Regserver ���������ġ�
	//if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	//{
	//	_AtlModule.UpdateRegistryAppId(TRUE);
	//	_AtlModule.RegisterServer(TRUE);
	//	return FALSE;
	//}


	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Kedacom"));


	

	// ��ʼ��COM��
	HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
	if ( FAILED( hr ) )
	{
		mcu::log << _T( "COM��ʼ��ʧ�ܣ�" ) << endl;
	}

	// ���ý���ͼƬ��Ĭ��Ŀ¼.
	tstring strPicPath = ::GetAppDir() + UI_PIC_DIR ;
	CImageFactory::Instance()->SetDefaultPicDir( strPicPath.c_str() );


	//CMainDialog mainDlg;
	//m_pMainWnd = &mainDlg;
	//mainDlg.DoModal();
#ifdef UNIT_TEST

	this->UnitTest();

	CUnitTestMainDialog utDlg;
	utDlg.DoModal();

#else

#ifdef _DEBUG
	CUnitTestMainDialog utDlg;
	utDlg.DoModal();
#endif

#endif

	// �жϳ����Ƿ��ǵ�ʵ������.
	m_hSingletonAppMutex = ::CreateMutex( 0, TRUE, _T( "mcu-kedacom" ) );
	if ( m_hSingletonAppMutex )
	{
		if( ERROR_ALREADY_EXISTS == GetLastError() )
		{
			// �Ѿ�������.
			// �ҵ��Ѿ����е�MCU,�������ڶ���.
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
		AfxMessageBox( ( strTitle + _T( " �����ʼ��ʧ��!" ) ).c_str(), MB_TOPMOST );
	}

	//����RTSP������
 	if( TRUE == IsAppNormalExit() )
 	{
 		BackupRtspPlayerRegKey();
 	}

	//��¼���������˳�״̬
	WriteProfileInt(L"AppStatus",L"Status",APP_RUNNING);


	CMCUDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}

	

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CMCUDemoApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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

int CMCUDemoApp::Run()
{
	// TODO: �ڴ����ר�ô����/����û���

	return CWinApp::Run();
}

BOOL CMCUDemoApp::ExitInstance(void)
{
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	_AtlModule.RevokeClassObjects();
#endif
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	_AtlModule.RevokeClassObjects();
#endif

	// �ͷŵ����ࡣ
	CImageFactory::Release();
	CConfig::Release();
	CMCUSession::Release();

	//��ԭRTSPЭ�鲥����
	//RestoreRtspPlayerRegKey();
	//��¼���������˳�״̬
	WriteProfileInt(L"AppStatus",L"Status",APP_NORMALEXIT);

	// �ͷŻ�����.
	ReleaseMutex( m_hSingletonAppMutex );
	CloseHandle( m_hSingletonAppMutex );

	return CWinApp::ExitInstance();
}

#ifdef UNIT_TEST
void CMCUDemoApp::UnitTest()
{

 //   // VC���ӵ�ʱ�����ǻ���Ե���Ԫ�������cpp������ѵ�Ԫ����������һ�Σ��Ա�ǿ�����ӡ�
 //   // ����ѡ���޸���Ч���������������о���
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
		AfxMessageBox( _T( "��Ԫ����δͨ��!����ݴ�����Ϣ������!" ) );
	}
}
#endif	// #ifdef UNIT_TEST
