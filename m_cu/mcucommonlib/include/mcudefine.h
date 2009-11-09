#pragma once

#include "configure.h"



#ifdef __cplusplus

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

#endif


#ifdef _WIN32_WCE
#include <stdlib.h>
#endif

#ifdef __cplusplus

#ifdef UNICODE
#define tstring wstring
#define  tcin wcin 
#define  tcout wcout 
#define tstringstream wstringstream
#else
#define  tstring string
#define tcin cin 
#define tcout cout 
#define tstringstream stringstream
#endif

/** 只读字符串。 */
typedef const tstring ctstring;

typedef std::vector<std::tstring> tstringarray;

#endif


/** 基本类型. */
/* Basic data types */

#ifndef BOOL
typedef int BOOL;
#endif

typedef signed char mu_int8;
typedef unsigned char mu_uint8;

typedef signed short mu_int16; 
typedef unsigned short mu_uint16; 

typedef signed long mu_int32;
typedef unsigned long mu_uint32;  

typedef signed __int64 mu_int64;
typedef unsigned __int64 mu_uint64;

/** 
//////////////////////////////////////////////////////////////////////////
// 最大,最小 */
#ifndef INT_MAX
#define INT_MAX (1<<30)
#endif

#ifndef INT64_MAX
#define INT64_MAX (1<<30)
#endif

#ifndef INT_MIN
#define INT_MIN (-1<<30)
#endif

/** 比较大小. */
#define MU_MAX(a,b) ((a) > (b) ? (a) : (b))
#define MU_MIN(a,b) ((a) > (b) ? (b) : (a))

/**
//////////////////////////////////////////////////////////////////////////
// NULL */
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

/** 
// 导出.导入. */
/* Some compilers use a special export keyword */

/** 如果是静态库工程，不需要导出。 */
#ifdef MCUCOMMON_STATIC_LIB
#define MU_DECLSPEC
#endif

#ifndef MU_DECLSPEC
# if defined(__BEOS__)
#  if defined(__GNUC__)
#   define MU_DECLSPEC	__declspec(dllexport)
#  else
#   define MU_DECLSPEC	__declspec(export)
#  endif
# elif defined(__WIN32__) || defined( _WIN32_WCE )

#  ifdef __BORLANDC__
#   ifdef BUILD_MCUCOMMONLIB
#    define MU_DECLSPEC 
#   else
#    define MU_DECLSPEC	__declspec(dllimport)
#   endif
#  else
#	ifdef BUILD_MCUCOMMONLIB
#		define MU_DECLSPEC	__declspec(dllexport)
#   else
#		define MU_DECLSPEC	__declspec(dllimport)
#  endif
# endif

# elif defined(__OS2__)
#  ifdef __WATCOMC__
#   ifdef BUILD_MCUCOMMONLIB
#    define MU_DECLSPEC	__declspec(dllexport)
#   else
#    define MU_DECLSPEC
#   endif
#  else
#   define MU_DECLSPEC
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define MU_DECLSPEC	__attribute__ ((visibility("default")))
#  else
#   define MU_DECLSPEC
#  endif
# endif
#endif

/** 函数调用方式.*/
/* By default SDL uses the C calling convention */
#ifndef MU_CALL
#if defined(__WIN32__) || defined( _WIN32_WCE ) && !defined(__GNUC__)
#define MU_CALL __cdecl
#else
#ifdef __OS2__
/* But on OS/2, we use the _System calling convention */
/* to be compatible with every compiler */
#define MU_CALL _System
#else
#define MU_CALL
#endif
#endif
#endif /* MU_CALL */

#ifdef __SYMBIAN32__ 
#ifndef EKA2 
#undef MU_DECLSPEC
#define MU_DECLSPEC
#elif !defined(__WINS__)
#undef MU_DECLSPEC
#define MU_DECLSPEC __MU_DECLSPEC(dllexport)
#endif /* !EKA2 */
#endif /* __SYMBIAN32__ */

