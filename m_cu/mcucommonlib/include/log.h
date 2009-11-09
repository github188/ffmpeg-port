#pragma once

#include "mcudefine.h"
#include <fstream>
#include <iostream>
#include <afx.h>


/** ��־����� */
namespace mcu
{

class MU_DECLSPEC CLog
{
public:
	CLog(void);
	virtual ~CLog(void);

public:

	/** �����־�� */
	CLog& operator << ( LPCTSTR strMsg );

	CLog& operator << ( const char * strMsg );

	CLog& operator << ( const tstring& strMsg );

//	CLog& operator << ( int nNum );
	CLog& operator << ( short sNum );
	CLog& operator << ( unsigned short _Val );
	CLog& operator<<(int __w64 _Val);
	CLog& operator<<(unsigned int __w64 _Val);
	CLog& operator<<(long _Val);
	CLog& operator<<(unsigned long __w64 _Val);

	CLog& operator << ( char cChar );

	CLog& operator << ( float fNum );	
	CLog& operator<<(double _Val);
	CLog& operator<<(long double _Val);

	CLog& operator<<(const void *_Val);

	/** Ϊ�����endl�� */
	typedef basic_ostream<wchar_t, char_traits<wchar_t>> _Myt;
	CLog& operator << ( _Myt& (__cdecl *_Pfn)(_Myt&) );

	void  WriteToLog( LPCTSTR strMsg );
	
private:
	/** ���߳�ͬ������ */
//	SDL_LockMutex

	CFile m_logFile;
};

/** ������־�����ȫ�ֶ��� */
extern  MU_DECLSPEC mcu::CLog tlog;
}
