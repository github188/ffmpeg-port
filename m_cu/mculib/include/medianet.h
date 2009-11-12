#pragma once

#include <string>
//



#include "portabledefine.h"
#include "mcudefine.h"

using namespace std;

/** 前置声明。*/
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

	/** 打开RTSP */
	BOOL OpenRTSP( LPCTSTR strRtspUrl );

	/** 关闭RTSP。 */
	BOOL CloseRTSP( );

	/** 获取RTSP状态. */
	ERTSPStatus GetRtspStatus() const;

private:
	/** 线程。 */
	static int MediaNet_Thread( void * pThis );

	/** 设置Rtsp状态. */
	void SetRtspStatus( ERTSPStatus eStat );
private:
	/** 当前的RTSP URL。 */
	string m_strRTSPUrlA;

	/** 网络线程。 */
	SDL_Thread *m_pMediaNetThread;

	/** 网络线程运行标志。 */
	char m_runFlag;

	/** RTSP的状态. */
	ERTSPStatus m_eRtspStatus;

};
