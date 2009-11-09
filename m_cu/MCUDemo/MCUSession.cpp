#include "StdAfx.h"
#include "MCUSession.h"

CMCUSession *CMCUSession::s_instance = 0;
CSDLMutex CMCUSession::s_threadSafeLock;

CMCUSession::CMCUSession(void)
{
}

CMCUSession::~CMCUSession(void)
{
}

CMCUSession *CMCUSession::Instance()
{
	if ( NULL == s_instance )
	{
		SCOPE_LOCK( s_threadSafeLock );
		if ( NULL == s_instance )
		{
			s_instance = new CMCUSession();
		}
	}

	return s_instance;
}

void CMCUSession::Release()
{
	if ( s_instance )
	{
		SCOPE_LOCK( s_threadSafeLock );
		if ( s_instance )
		{
			delete s_instance;
			s_instance = NULL;
		}
	}
}

tstring CMCUSession::UserId() const
{
	return m_strUserId;
}

void CMCUSession::UserId(std::wstring val)
{ 
	m_strUserId = val; 
}