#pragma once

#include "mcudefine.h"
//#include "sdl.h"
/** 区域锁类. */

#define SCOPE_LOCK( x ) CScopeLock lock_wleonb503012l3jgnonvdo230( x ) 

#ifndef INFINITE
#define INFINITE            0xffffffffL  // Infinite timeout
#endif

struct SDL_mutex;

/** 禁用对象拷贝. */
class MU_DECLSPEC CNoCopy
{
public:
	CNoCopy();
private:
	CNoCopy( const CNoCopy& );
	CNoCopy& operator=( const CNoCopy& );
};

class MU_DECLSPEC CSDLMutex : public CNoCopy
{
public:
	CSDLMutex();
	~CSDLMutex();

	BOOL Lock( DWORD dwTimeOut = INFINITE );
	BOOL Unlock();
private:
	SDL_mutex *m_pMutex;
};


class MU_DECLSPEC CScopeLock
{
public:
	CScopeLock( CSDLMutex& pLock );
	virtual ~CScopeLock(void);
private:
	CSDLMutex *m_pMutex;
};
