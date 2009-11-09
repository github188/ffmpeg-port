#include "StdAfx.h"
#include "ScopeLock.h"
#include "SDL.h"

CNoCopy::CNoCopy()
{

}

CSDLMutex::CSDLMutex()
{
	this->m_pMutex = SDL_CreateMutex();
}

CSDLMutex::~CSDLMutex()
{
	if ( m_pMutex )
	{
		SDL_DestroyMutex( m_pMutex );
	}
}

BOOL CSDLMutex::Lock( DWORD dwTimeOut /* = INFINITE */ )
{
	dwTimeOut;
	return SDL_LockMutex( m_pMutex );
}

BOOL CSDLMutex::Unlock()
{
	return SDL_UnlockMutex( m_pMutex );
}

CScopeLock::CScopeLock( CSDLMutex& pLock )
{
	m_pMutex = &pLock;
	m_pMutex->Lock();
}

CScopeLock::~CScopeLock(void)
{
	m_pMutex->Unlock();
}

