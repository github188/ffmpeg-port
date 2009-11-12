#pragma once

#include "mcudefine.h"

/** ��������. */

#define SCOPE_LOCK( x ) CScopeLock lock_wleonb503012l3jgnonvdo230( x ) 

#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

/** ���ö��󿽱�. */
class MU_DECLSPEC CNoCopy
{
public:
	CNoCopy();
private:
	CNoCopy( const CNoCopy& );
	CNoCopy& operator=( const CNoCopy& );
};

/** ǰ������������ͷ�ļ�������*/
struct SDL_mutex;
class MU_DECLSPEC CMutex : public CNoCopy
{
public:
	CMutex();
	~CMutex();

	BOOL Lock( const mu_uint32 dwTimeOut = INFINITE );
	BOOL Unlock();
private:
	SDL_mutex *m_pMutex;
};


class MU_DECLSPEC CScopeLock
{
public:
	CScopeLock( CMutex& pLock );
	virtual ~CScopeLock(void);
private:
	CMutex *m_pMutex;
};
