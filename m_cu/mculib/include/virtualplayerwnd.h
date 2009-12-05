#pragma once

#include "mcudefine.h"
#include "decoder.h"
#include "medianet.h"
#include "videosession.h"
#include "ptzcontrol.h"
#include "message.h"

class CVirtualPlayerWnd
{
public:
	
public:
	CVirtualPlayerWnd(void);
	virtual ~CVirtualPlayerWnd(void);

	/** ��RTSP���ӡ� */
	BOOL StartPlay( CVideoSession *pVideoSession, EMCU_ErrorCode& eErrorCode );

    /** �������ӡ� */
    BOOL RestartPlay();

	BOOL PausePlay( BOOL bPause );
	BOOL IsPause() const;

	/** �ر�RTSP�� */
	BOOL StopPlay();

	/** ץ�ġ� */
	BOOL Capture( tstring& strPicPath, EMCU_ErrorCode& eErrorCode );
	BOOL Capture( LPCTSTR strPicPath, EMCU_ErrorCode& eErrorCode );

	/** ¼�� */

    /** ¼��ӿ�1���ڲ�ȷ���ļ����������ļ������ء��� */
    BOOL StartRecord( tstring& strRecPath, EMCU_ErrorCode& eErrorCode );
    /** ¼��ӿ�2��������ȷ���ļ����� */
	BOOL StartRecord( LPCTSTR strRecPath, EMCU_ErrorCode& eErrorCode );
	BOOL StopRecord( EMCU_ErrorCode& pErrorCode );
    BOOL IsRecording() const;

	/** Ptz���ơ� */
	BOOL SendPtzCmd( EPTZCmdId eCmd );
	BOOL SetDigitalPtz( BOOL bDigital );
	BOOL IsDigitalPtz() const;
	BOOL HasPtzPower() const;

protected:
	/** ͼ��ص��� */
	virtual void OnVideoPicture( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo ) = 0;

	/** ���� ״̬,�¼����ص��� */
	virtual void OnRtspStatus( const ERTSPStatus eRtspStatus, const EMCU_ErrorCode eErrorCode ) = 0;

    /** ¼��״̬�ص��� */
    virtual void OnRecordStatus( BOOL bSuccess, EMCU_ErrorCode er ) = 0;

private:
	/** ����̵߳��á� */
	void CheckStatsu();	

    /** ����������� */
    void CheckDiskSpace();

	/** ��ʼ���������̡߳�*/
	void StartCheckThread();
	void StopCheckThread();

private:
	/** ͼ��ص��� */
	static BOOL YUVCallbackS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *param );

	/** ����̺߳����� */
	static int CheckThread( void * pThis );

	/** �Ƿ��������̡߳� */
	BOOL GetCheckThreadSwitch() const;

	/** ��Ϣ�ص��� */
	static mu_int32 OnMessage( const mcu::TMsg& tMsg );

    /** ��ȡץ��/¼����ļ����� */
    tstring GetFileName( LPCTSTR strExtName );

private:
	CMediaNet m_MediaNet;

	CPtzControl m_ptzControl;

	BOOL m_bPause;

	/** ״̬����̡߳� 
	*	���rtsp״̬��
	*	ͼ���ϱ������
	*/
	SDL_Thread *m_pStatusCheckThread;
	volatile BOOL m_bStatusCheckThreadRun;

	/** �Ƿ���״̬�� */
	volatile BOOL m_bCheckStatus;

    /** �Ƿ����������*/
    volatile BOOL m_bCheckDiskSpace;

	/** ���һ�ε�rtsp״̬�� */
	ERTSPStatus m_eLastRtspStatus;

	/** ���һ��ͼ��ص�ʱ�䡣 */
	__time64_t m_timeLastPic;

	/** �߳�ͬ������ */
	mutable CMCUMutex m_threadSafeLock;

};

