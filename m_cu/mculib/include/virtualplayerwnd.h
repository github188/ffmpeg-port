#pragma once

#include "mcudefine.h"
#include "decoder.h"
#include "medianet.h"
#include "videosession.h"
#include "ptzcontrol.h"
#include "message.h"
#include "framebuffer.h"

class CVirtualPlayerWnd
{
public:
	
public:
	CVirtualPlayerWnd(void);
	virtual ~CVirtualPlayerWnd(void);

	/** 打开RTSP链接。 */
	BOOL StartPlay( CVideoSession *pVideoSession, EMCU_ErrorCode& eErrorCode );

    /** 重新连接。 */
    BOOL RestartPlay();

	BOOL PausePlay( BOOL bPause );
	BOOL IsPause() const;

	/** 关闭RTSP。 */
	BOOL StopPlay();

	/** 全屏。 */
	/** 设置全屏状态.*/
	void SetFullScreen( BOOL bFullScreen );
	BOOL IsFullScreen() const;

	/** 抓拍。 */
	BOOL Capture( tstring& strPicPath, EMCU_ErrorCode& eErrorCode );
	BOOL Capture( LPCTSTR strPicPath, EMCU_ErrorCode& eErrorCode );

	/** 录像。 */

    /** 录像接口1，内部确定文件名，并将文件名返回。。 */
    BOOL StartRecord( tstring& strRecPath, EMCU_ErrorCode& eErrorCode );
    /** 录像接口2，调用者确定文件名。 */
	BOOL StartRecord( LPCTSTR strRecPath, EMCU_ErrorCode& eErrorCode );
	BOOL StopRecord( EMCU_ErrorCode& pErrorCode );
    BOOL IsRecording() const;

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

    /** 录像状态回调。 */
    virtual void OnRecordStatus( BOOL bSuccess, EMCU_ErrorCode er ) = 0;

private:
	/** 检测线程调用。 */
	void CheckStatsu();	

    /** 检测磁盘情况。 */
    void CheckDiskSpace();

	/** 开始与结束检测线程。*/
	void StartCheckThread();
	void StopCheckThread();

private:
	/** 图像回调。 */
	static BOOL YUVCallbackS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *param );

	BOOL YUVCallback( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo );

	/** 图像帧数据回调。经过缓存。 */
	static void OnBufferdVideoFrameShowS( const CBaseCodec::TVideoPicture *pic, 
		const CBaseCodec::TVideoFrameInfo *pFrameInfo,
		void *userData );

	void OnBufferdVideoFrameShow( const CBaseCodec::TVideoPicture *pic, 
		const CBaseCodec::TVideoFrameInfo *pFrameInfo );

	/** 监测线程函数。 */
	static int CheckThread( void * pThis );

	/** 是否允许检测线程。 */
	BOOL GetCheckThreadSwitch() const;

	/** 消息回调。 */
	static mu_int32 OnMessage( const mcu::TMsg& tMsg );

    /** 获取抓拍/录像的文件名。 */
    tstring GetFileName( LPCTSTR strExtName );

private:
	CMediaNet m_MediaNet;

	CPtzControl m_ptzControl;

	/** 帧缓存。 */
	CFrameBuffer m_frameBuffer;

	BOOL m_bPause;

	/** 状态监测线程。 
	*	检测rtsp状态。
	*	图像上报情况。
	*/
	SDL_Thread *m_pStatusCheckThread;
	volatile BOOL m_bStatusCheckThreadRun;

	/** 是否检测状态。 */
	volatile BOOL m_bCheckStatus;

    /** 是否检测磁盘满。*/
    volatile BOOL m_bCheckDiskSpace;

	/** 最近一次的rtsp状态。 */
	ERTSPStatus m_eLastRtspStatus;

	/** 最近一次图像回调时间。 */
	__time64_t m_timeLastPic;

	/** 线程同步锁。 */
	mutable CMCUMutex m_threadSafeLock;

	/** 是否是全屏模式. */
	BOOL m_bFullScreenMode;

	/** 旋转图像时使用的缓存。 */
	CBaseCodec::TVideoPicture m_tVideoRollPicBuf;

};

