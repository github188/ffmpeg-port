#pragma once

#include "scopelock.h"
#include "mcudefine.h"
#include "videosession.h"

/** mcu的一次登录会话. */
class CMCUSession
{
public:
	static CMCUSession *Instance();
	static void Release();
private:
	static CMCUSession *s_instance;
	static CMCUMutex s_threadSafeLock;
protected:
	CMCUSession(void);
	virtual ~CMCUSession(void);

public:
	/** 用户名. */
	tstring UserId() const ;
	void UserId(tstring val);
	/** 密码. */
	tstring Password() const { return m_strPassword; }
	void Password(tstring val) { m_strPassword = val; }

//	const CVideoSession * CurVideoSession() const { return &m_curVideoSession; }
	CVideoSession * CurVideoSession() { return &m_curVideoSession; }
private:
	/** 用户名*/
	tstring m_strUserId;
	
	/** 密码. */
	tstring m_strPassword;

	/** 码流类型. */

	/** 登录方式. */

	/** 当前视频会话. */
	CVideoSession m_curVideoSession;
	
};
