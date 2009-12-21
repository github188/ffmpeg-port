#include "StdAfx.h"
#include "log.h"
#include "mcucommon.h"
//#include "stat.h" 
//#include "types.h"


using namespace mcu;

MU_DECLSPEC  CLog  Log();
#define LOGFILE (L"M_CU.log")
#define MAXFILESIZE 1024*1024
//__declspec(dllexport) CLog   log;

CLog::CLog(void)
{
	//flog.open("M_CU.log", ios::in | ios::out | ios::trunc);
	m_logFile.Open(LOGFILE, CFile::modeCreate|CFile::modeReadWrite, NULL);
}

CLog::~CLog(void)
{
	m_logFile.Close();
}

CLog& CLog::operator <<(LPCTSTR strMsg)
{
	OutputDebugString( strMsg );
	//flog.write( strMsg, wcslen(strMsg));	
	WriteToLog(strMsg);
	return *this;
}

CLog& CLog::operator <<( const char * strMsg )
{
	cout << strMsg;
	return *this;
}

CLog& CLog::operator <<( const tstring& strMsg )
{	
	return ( (*this) << strMsg.c_str() );
}

CLog& CLog::operator <<( long nNum )
{
	tstringstream ssNum;
	ssNum << nNum;
	( *this ) << ssNum.str();
//	tcout << nNum;
	TCHAR tmp[64];
	_itow( nNum, tmp, 10 );
	WriteToLog(tmp);
	return *this;
}

CLog& CLog::operator << ( _Myt& (__cdecl *_Pfn)(_Myt&) )
{
//	tcout << _Pfn;
	tstringstream ssTemp;
	ssTemp << _Pfn;
	( *this ) << ssTemp.str();
	return *this;
}

CLog& CLog::operator <<(__w64 int _Val)
{
	tstringstream ssTemp;
	ssTemp << _Val;
	( *this ) << ssTemp.str();
//	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<(const void *_Val)
{
	tstringstream ssTemp;
	ssTemp << _Val;
	( *this ) << ssTemp.str();
//	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<(__w64 unsigned int _Val)
{
	tstringstream ssTemp;
	ssTemp << _Val;
	( *this ) << ssTemp.str();
//	tcout << _Val;
	return *this;
}

CLog& CLog::operator <<(__w64 unsigned long _Val)
{
	tstringstream ssTemp;
	ssTemp << _Val;
	( *this ) << ssTemp.str();
//	tcout << _Val;
	return *this;
}

void  CLog::WriteToLog( LPCTSTR strMsg )
{	
	string strUrl = UTF16toUTF8(strMsg);

	if ( m_logFile.m_hFile == CFile::hFileNull )
	{
		return;
	}

	if( m_logFile.GetLength() > MAXFILESIZE)
	{
		m_logFile.SetLength (0);
	}
	char *lineend = "\r";
	m_logFile.Write(strUrl.c_str(), strlen(strUrl.c_str()));
	m_logFile.Write(lineend, strlen(lineend));
	m_logFile.Flush();
}

