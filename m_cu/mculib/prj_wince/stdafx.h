// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

/** 消除编译器的警告。*/
#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

// 注意 - 这个值与作为目标的 Windows CE OS 版本的关联性并不强
#ifndef WINVER
#define WINVER _WIN32_WCE
#endif

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif

#include <windows.h>
#include <commctrl.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 ATLMFC 头中排除极少使用的资料 
#endif


#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件




#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

#if _WIN32_WCE < 0x500 && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#include <altcecrt.h>

// TODO: 在此处引用程序需要的其他头文件
