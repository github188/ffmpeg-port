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

	/** 打开RTSP链接。 */
	BOOL StartPlay( CVideoSession *pVideoSession, EMCU_ErrorCode& eErrorCode );

	BOOL PausePlay( BOOL bPause );
	BOOL IsPause() const;

	/** 关闭RTSP。 */
	BOOL StopPlay();

	/** 抓拍。 */
	BOOL Capture( EMCU_ErrorCode& eErrorCode );
	BOOL Capture( LPCTSTR strPicPath, EMCU_ErrorCode& eErrorCode );

	/** 录像。 */
	BOOL StartRecord( LPCTSTR strRecPath, EMCU_ErrorCode& eErrorCode );
	BOOL StopRecord( EMCU_ErrorCode& pErrorCode );

	/** Ptz控制。 */
	BOOL SendPtzCmd( EPTZCmdId eCmd );
	BOOL SetDigitalPtz( BOOL bDigital );
	BOOL IsDigitalPtz() const;
	BOOL HasPtzPower() const;

protected:
	/** 图像回调。 */
	virtual void OnVideoPicture( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo ) = 0;

	/** 播放 状态,事件。回调。 */
	virtual void OnRtspStatus( const ERTSPStatus eRtspStatus, const EMCU_ErrorCode eErrorCode ) = 0;

private:
	/** 检测线程调用。 */
	void CheckStatsu();	

	/** 开始与结束检测线程。*/
	void StartCheckThread();
	void StopCheckThread();

private:
	/** 图像回调。 */
	static BOOL YUVCallbackS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *param );

	/** 监测线程函数。 */
	static int CheckThread( void * pThis );

	/** 是否允许检测线程。 */
	BOOL GetCheckThreadSwitch() const;

	/** 消息回调。 */
	static mu_int32 OnMessage( const mcu::TMsg& tMsg );

private:
	CMediaNet m_MediaNet;

	CPtzControl m_ptzControl;

	BOOL m_bPause;

	/** 状态监测线程。 
	*	检测rtsp状态。
	*	图像上报情况。
	*/
	SDL_Thread *m_pStatusCheckThread;
	volatile BOOL m_bStatusCheckThreadRun;

	/** 是否检测状态。 */
	volatile BOOL m_bCheckStatus;

	/** 最近一次的rtsp状态。 */
	ERTSPStatus m_eLastRtspStatus;

	/** 最近一次图像回调时间。 */
	time_t m_timeLastPic;

	/** 线程同步锁。 */
	mutable CMCUMutex m_threadSafeLock;

};

