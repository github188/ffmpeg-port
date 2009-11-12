
#include "scopelock.h"

#ifdef _WIN32_WCE
#include "SDL.h"
#else
#include <SDL/SDL.h>
#endif

CNoCopy::CNoCopy()
{

}

CMutex::CMutex()
{
	this->m_pMutex = SDL_CreateMutex();
}

CMutex::~CMutex()
{
	if ( m_pMutex )
	{
		SDL_DestroyMutex( m_pMutex );
	 }
}

BOOL CMutex::Lock( const mu_uint32 dwTimeOut /* = INFINITE */ )
{
//	mu_int32 ss = dwTimeOut;
	return SDL_LockMutex( m_pMutex );
}

BOOL CMutex::Unlock()
{
	return SDL_UnlockMutex( m_pMutex );
}

CScopeLock::CScopeLock( CMutex& pLock )
{
	m_pMutex = &pLock;
	m_pMutex->Lock();
}

CScopeLock::~CScopeLock(void)
{
	m_pMutex->Unlock();
}

