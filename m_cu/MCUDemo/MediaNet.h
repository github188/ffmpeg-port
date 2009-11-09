#pragma once

#include <string>
#include "liveMedia.hh"

using namespace std;


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

extern Boolean clientSetupSubsession(Medium* client,
									 MediaSubsession* subsession,
									 Boolean streamUsingTCP,
									 unsigned *pResponseCode = NULL);

extern Boolean clientStartPlayingSession(Medium* client,
										 MediaSession* session);

extern Boolean clientTearDownSession(Medium* client,
									 MediaSession* session);

extern Boolean allowProxyServers;
extern Boolean controlConnectionUsesTCP;
extern Boolean supportCodecSelection;
extern char const* clientProtocolName;
extern unsigned statusCode;

#include "SDL.h"

class CMediaNet
{
public:
	/** 网络库当前的状态.*/
	enum ERTSPStatus
	{
		RTSPStatus_Idle = 0,	// 闲置状态.
		RTSPStatus_Init,		// 正在初始化.
		RTSPStatus_Opitiion,	// 正在等待OPITION命令的回复.
		RTSPStatus_Description,	// 等待Description
		RTSPStatus_Setup,		// 等待Setup.
		RTSPStatus_Play,		// 等待播放.
		RTSPStatus_Running,		// 正常运行中.
		RTSPStatus_Error_Server_Full,	// 服务器达到了最大能力
		RTSPStatus_Error_Connect_Srv,	// 连接服务器失败
	};

	enum ERTSPResponse
	{
		RTSPResp_Error_Server_Full = 552,
	};

public:
	CMediaNet(void);
	~CMediaNet(void);

	/** 打开RTSP */
	bool OpenRTSP( LPCTSTR strRtspUrl );

	/** 关闭RTSP。 */
	bool CloseRTSP( );

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
