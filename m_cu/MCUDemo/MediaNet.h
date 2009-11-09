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
	/** ����⵱ǰ��״̬.*/
	enum ERTSPStatus
	{
		RTSPStatus_Idle = 0,	// ����״̬.
		RTSPStatus_Init,		// ���ڳ�ʼ��.
		RTSPStatus_Opitiion,	// ���ڵȴ�OPITION����Ļظ�.
		RTSPStatus_Description,	// �ȴ�Description
		RTSPStatus_Setup,		// �ȴ�Setup.
		RTSPStatus_Play,		// �ȴ�����.
		RTSPStatus_Running,		// ����������.
		RTSPStatus_Error_Server_Full,	// �������ﵽ���������
		RTSPStatus_Error_Connect_Srv,	// ���ӷ�����ʧ��
	};

	enum ERTSPResponse
	{
		RTSPResp_Error_Server_Full = 552,
	};

public:
	CMediaNet(void);
	~CMediaNet(void);

	/** ��RTSP */
	bool OpenRTSP( LPCTSTR strRtspUrl );

	/** �ر�RTSP�� */
	bool CloseRTSP( );

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
