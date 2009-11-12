#pragma once

#include <string>
//



#include "portabledefine.h"
#include "mcudefine.h"

using namespace std;

/** ǰ��������*/
struct SDL_Thread;
class Medium;
class UsageEnvironment;
class MediaSubsession;
class MediaSession;

extern Medium* createClient(UsageEnvironment& env, int verbosityLevel,
							char const* applicationName);

extern char* getOptionsResponse(Medium* client, char const* url,
								char* username, char* password);

extern char* getSDPDescriptionFromURL(Medium* client, char const* url,
									  char const* username,
									  char const* password,
									  char const* proxyServerName,
									  unsigned short proxyServerPortNum,
									  unsigned short clientStartPortNum);

extern BOOL clientSetupSubsession(Medium* client,
									 MediaSubsession* subsession,
									 BOOL streamUsingTCP,
									 unsigned *pResponseCode = NULL);

extern BOOL clientStartPlayingSession(Medium* client,
										 MediaSession* session);

extern BOOL clientTearDownSession(Medium* client,
									 MediaSession* session);

extern BOOL allowProxyServers;
extern BOOL controlConnectionUsesTCP;
extern BOOL supportCodecSelection;
extern char const* clientProtocolName;
extern unsigned statusCode;


class CMediaNet
{
public:


	enum ERTSPResponse
	{
		RTSPResp_Error_Server_Full = 552,
	};

public:
	CMediaNet(void);
	~CMediaNet(void);

	/** ��RTSP */
	BOOL OpenRTSP( LPCTSTR strRtspUrl );

	/** �ر�RTSP�� */
	BOOL CloseRTSP( );

	/** ��ȡRTSP״̬. */
	ERTSPStatus GetRtspStatus() const;

private:
	/** �̡߳� */
	static int MediaNet_Thread( void * pThis );

	/** ����Rtsp״̬. */
	void SetRtspStatus( ERTSPStatus eStat );
private:
	/** ��ǰ��RTSP URL�� */
	string m_strRTSPUrlA;

	/** �����̡߳� */
	SDL_Thread *m_pMediaNetThread;

	/** �����߳����б�־�� */
	char m_runFlag;

	/** RTSP��״̬. */
	ERTSPStatus m_eRtspStatus;

};
