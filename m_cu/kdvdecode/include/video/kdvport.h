
#include "mcudefine.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	/* Type definition */
	/*-----------------------------------------------------------------------------
	系统公用文件，开发人员严禁修改
	------------------------------------------------------------------------------*/

	typedef int		s32,BOOL32;
	typedef unsigned long   u32;
	typedef long l32;
	typedef unsigned char	u8;
	typedef unsigned short  u16;
	typedef short           s16;
	typedef char            s8;

#ifdef _MSC_VER
	typedef __int64			s64;
#else 
	typedef long long		s64;
#endif 

#ifdef _MSC_VER
	typedef unsigned __int64 u64;
#else 
	typedef unsigned long long u64;
#endif

#ifndef _MSC_VER
#ifndef LPSTR
#define LPSTR   char *
#endif
#ifndef LPCSTR
#define LPCSTR  const char *
#endif
#endif



#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif



#ifdef __cplusplus
}
#endif  /* __cplusplus */