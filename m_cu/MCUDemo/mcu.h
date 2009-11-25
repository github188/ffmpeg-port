// mcu.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 
#include "MCUDemo_i.h"
//#include "m_cu_i.h"
#ifdef SMARTPHONE2003_UI_MODEL
#include "resourcesp.h"
#endif

// CMCUApp:
// 有关此类的实现，请参阅 MCUDemo.cpp
//

class CMCUApp : public CWinApp
{
public:
	CMCUApp();
	
// 重写
public:
	virtual BOOL InitInstance();
	static void BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
//	static void RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
	static void BackupRtspPlayerRegKey();
//	static void RestoreRtspPlayerRegKey();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int Run();
	BOOL ExitInstance(void);
	BOOL IsAppNormalExit();

private:
	/** 单元测试. 
	*	只有支持单元测试的编译配置才实现并调用这个函数.这个函数对程序进行单元测试.
	*/
	void UnitTest();

private:
	/** 维持单实例的互斥量. */
	HANDLE m_hSingletonAppMutex;

};

extern CMCUApp theApp;
