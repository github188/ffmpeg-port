// mcu.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
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
// �йش����ʵ�֣������ MCUDemo.cpp
//

class CMCUApp : public CWinApp
{
public:
	CMCUApp();
	
// ��д
public:
	virtual BOOL InitInstance();
	static void BackupRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
//	static void RestoreRegValue(HKEY root, PWCHAR szSubKey, PWCHAR szValueName, PWCHAR szBakValueName);
	static void BackupRtspPlayerRegKey();
//	static void RestoreRtspPlayerRegKey();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int Run();
	BOOL ExitInstance(void);
	BOOL IsAppNormalExit();

private:
	/** ��Ԫ����. 
	*	ֻ��֧�ֵ�Ԫ���Եı������ò�ʵ�ֲ������������.��������Գ�����е�Ԫ����.
	*/
	void UnitTest();

private:
	/** ά�ֵ�ʵ���Ļ�����. */
	HANDLE m_hSingletonAppMutex;

};

extern CMCUApp theApp;
