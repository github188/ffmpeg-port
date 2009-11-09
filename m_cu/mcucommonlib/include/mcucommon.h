#pragma once

#include "mcudefine.h"


MU_DECLSPEC string UTF16toUTF8( wstring strUTF16 );
MU_DECLSPEC wstring UTF8toUTF16( string strUTF8 );

/** 获取模块路径. */
MU_DECLSPEC tstring GetModulePath( HMODULE hm = NULL );

/** 文件名信息. */
struct TFileNameInfo 
{
	tstring m_strFilePath;	// 全路径.
	tstring m_strFileName;	// 文件名.
	tstring m_strDirectory;	// 文件所在文件夹.
	tstring m_strBaseName;	// 基本文件名.
	tstring m_strExtName;	// 文件扩展名.

	bool operator == ( const TFileNameInfo& tAnother ) const;
};
/** 解析文件名. */
MU_DECLSPEC TFileNameInfo ParsePath( LPCTSTR strPath );
MU_DECLSPEC BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo );

/** 获取程序文件名。. */
MU_DECLSPEC tstring GetAppName();
/** 获取程序文件夹路径。 */
MU_DECLSPEC tstring GetAppDir();

/** 获取屏幕大小（）*/
MU_DECLSPEC int GetScreenLong();	// 高，宽中更长的那个
MU_DECLSPEC int GetScreenShort();	// 短的那个。

MU_DECLSPEC int GetScreenHeight();	// 高,y轴方向.
MU_DECLSPEC int GetScreenWidth();	// 宽,x轴方向.

/** 多字节转为Unicode。 */
MU_DECLSPEC BOOL MBtoWC(const char* srcStr, wchar_t * dstStr, int len );

/** Unicode转为多字节。 */
MU_DECLSPEC BOOL WCtoMB(const wchar_t* srcStr, char *dstStr, int len );

/** 时间->字符串 */
MU_DECLSPEC tstring TimeToStr( __time64_t nTime );
MU_DECLSPEC __time64_t StrToTime( LPCTSTR strTime );

/** 判断手机是否有存储卡。 */
MU_DECLSPEC BOOL HasStorageCard();