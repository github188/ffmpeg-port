#pragma once

#include "scopelock.h"
#include "mcudefine.h"
#include "videosession.h"

/** mcu��һ�ε�¼�Ự. */
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
	/** �û���. */
	tstring UserId() const ;
	void UserId(tstring val);
	/** ����. */
	tstring Password() const { return m_strPassword; }
	void Password(tstring val) { m_strPassword = val; }

//	const CVideoSession * CurVideoSession() const { return &m_curVideoSession; }
	CVideoSession * CurVideoSession() { return &m_curVideoSession; }
private:
	/** �û���*/
	tstring m_strUserId;
	
	/** ����. */
	tstring m_strPassword;

	/** ��������. */

	/** ��¼��ʽ. */

	/** ��ǰ��Ƶ�Ự. */
	CVideoSession m_curVideoSession;
	
};
