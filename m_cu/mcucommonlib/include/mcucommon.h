#pragma once

#include "mcudefine.h"


MU_DECLSPEC string UTF16toUTF8( wstring strUTF16 );
MU_DECLSPEC wstring UTF8toUTF16( string strUTF8 );

/** ��ȡģ��·��. */
MU_DECLSPEC tstring GetModulePath( HMODULE hm = NULL );

/** �ļ�����Ϣ. */
struct TFileNameInfo 
{
	tstring m_strFilePath;	// ȫ·��.
	tstring m_strFileName;	// �ļ���.
	tstring m_strDirectory;	// �ļ������ļ���.
	tstring m_strBaseName;	// �����ļ���.
	tstring m_strExtName;	// �ļ���չ��.

	bool operator == ( const TFileNameInfo& tAnother ) const;
};
/** �����ļ���. */
MU_DECLSPEC TFileNameInfo ParsePath( LPCTSTR strPath );
MU_DECLSPEC BOOL ParsePath( LPCTSTR strPath, TFileNameInfo& tFileNameInfo );

/** ��ȡ�����ļ�����. */
MU_DECLSPEC tstring GetAppName();
/** ��ȡ�����ļ���·���� */
MU_DECLSPEC tstring GetAppDir();

/** ��ȡ��Ļ��С����*/
MU_DECLSPEC int GetScreenLong();	// �ߣ����и������Ǹ�
MU_DECLSPEC int GetScreenShort();	// �̵��Ǹ���

MU_DECLSPEC int GetScreenHeight();	// ��,y�᷽��.
MU_DECLSPEC int GetScreenWidth();	// ��,x�᷽��.

/** ���ֽ�תΪUnicode�� */
MU_DECLSPEC BOOL MBtoWC(const char* srcStr, wchar_t * dstStr, int len );

/** UnicodeתΪ���ֽڡ� */
MU_DECLSPEC BOOL WCtoMB(const wchar_t* srcStr, char *dstStr, int len );

/** ʱ��->�ַ��� */
MU_DECLSPEC tstring TimeToStr( __time64_t nTime );
MU_DECLSPEC __time64_t StrToTime( LPCTSTR strTime );

/** �ж��ֻ��Ƿ��д洢���� */
MU_DECLSPEC BOOL HasStorageCard();