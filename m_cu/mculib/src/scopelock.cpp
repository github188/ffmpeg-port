
#include "scopelock.h"
#include "mcuthread.h"



CNoCopy::CNoCopy()
{

}

CMCUMutex::CMCUMutex()
{
	this->m_pMutex = SDL_CreateMutex();
}

CMCUMutex::~CMCUMutex()
{
	if ( m_pMutex )
	{
		SDL_DestroyMutex( m_pMutex );
	 }
}

BOOL CMCUMutex::Lock( const mu_uint32 dwTimeOut /* = INFINITE */ )
{
//	mu_int32 ss = dwTimeOut;
	return SDL_LockMutex( m_pMutex );
}

BOOL CMCUMutex::Unlock()
{
	return SDL_UnlockMutex( m_pMutex );
}

CScopeLock::CScopeLock( CMCUMutex& pLock )
{
	m_pMutex = &pLock;
	m_pMutex->Lock();
}

CScopeLock::~CScopeLock(void)
{
	m_pMutex->Unlock();
}

