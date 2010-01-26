
#include "log.h"
#include "mcucommon.h"

#if defined( __SYMBIAN32__ )
#include <e32def.h>
#include <e32debug.h>
#endif
//using namespace mcu;

//MU_DECLSPEC  CLog  mcu::log;
//__declspec(dllexport) CLog   log;
CLog& Log()
{
	static CLog s_log;
	return s_log;
}

CLog::CLog(void)
{

}

CLog::~CLog(void)
{
}

void CLog::SetLogFileDir( LPCTSTR strLogDir, LPCTSTR strPrefix )
{
    SCOPE_LOCK( m_threadSafeLock );

    if ( m_strLogDir != strLogDir )
    {
        m_strLogDir = strLogDir;

        tstring strLogFilePath = strLogDir;

        NormalizeDir( strLogFilePath );

        strLogFilePath += strPrefix;
        strLogFilePath += _T( "-" );

        strLogFilePath += TimeToStr( GetCurTime() );
        strLogFilePath += _T( ".log" );

        // 某些版本的iostram的open不支持16bit unicode版本的open。
//        this->m_fLog.open( strLogFilePath.c_str() );
        string strUtf8 = ::UTF16toUTF8( strLogFilePath );
        this->m_fLog.open( strUtf8.c_str() );

    }
    else
    {
        // already。
    }
}

CLog& CLog::operator <<( const wchar_t * strMsg)
{
    SCOPE_LOCK( m_threadSafeLock );

#ifdef _WIN32_WCE
    OutputDebugString( strMsg );
#elif defined( __SYMBIAN32__ )
    RDebug::Print( _L( "%s" ) , strMsg );
#elif	!defined( __CYGWIN__ )// cygwin 不支持unicode!
    std::wcout << strMsg;
#else
    cout << ( "CLog << fail! cygwin don't support unicode!" ) << endl;

#endif

    if ( this->m_fLog )
    {
        m_fLog << strMsg ;
        m_fLog.flush();
    }

    return *this;
}

CLog& CLog::operator <<( const char * strMsg )
{
    SCOPE_LOCK( m_threadSafeLock );
    
#ifdef _WIN32_WCE
    cout << strMsg;
	tstring strMsgUtf16 = ::UTF8toUTF16( strMsg );
	*this << strMsgUtf16;
	
#elif defined( __SYMBIAN32__ )
    RDebug::Printf( ( "%s" ) , strMsg );
#else
    cout << strMsg;
#endif

    if ( this->m_fLog )
    {
        m_fLog << strMsg ;
        m_fLog.flush();
    }

    return *this;
}



CLog& CLog::operator <<( const string& strMsg )
{	
    return ( (*this) << strMsg.c_str() );
}

CLog& CLog::operator <<( const wstring& strMsg )
{	
    return ( (*this) << strMsg.c_str() );
}

CLog& CLog::operator <<( long nNum )
{
     tstringstream ssTmp;
    ssTmp << nNum;

    *this << ssTmp.str();

    return *this;
}


/**
CLog& CLog::operator << ( FunWEndl _Pfn )
{
tcout << _Pfn;
return *this;
}/storage card/

CLog& CLog::operator << ( FunAEndl _Pfn )
{
tcout << _Pfn;
return *this;
}
*/
CLog& CLog::operator << ( FunTEndl _Pfn )
{
    tstringstream ssTmp;
    ssTmp << _Pfn;

    return ( *this << ssTmp.str() );
}




CLog& CLog::operator <<( int _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<(const void *_Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<( unsigned int _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<( unsigned long _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator<<( mu_int64 _val )
{
    tstringstream ssTmp;
    ssTmp << _val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator<<( mu_uint64 _val )
{
    tstringstream ssTmp;
    ssTmp << _val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<(double _Val)
{
    tstringstream ssTmp;
    ssTmp << _Val;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<(short _var )
{
    tstringstream ssTmp;
    ssTmp << _var;

    return ( *this << ssTmp.str() );
}

CLog& CLog::operator <<( const float _var )
{
    tstringstream ssTmp;
    ssTmp << _var;

    *this << ssTmp.str();
    
    return *this;
}



