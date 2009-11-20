#pragma once

#include "scopelock.h"
#include "basecodec.h"
#include "message.h"

/** 封装了视频显示功能。
	经测试，SDL的显示资源不支持多线程访问。
	此类可能会被多个线程调用。
	所以，在创建/销毁/更改的时候都做了线程转换。
	线程转换使用的是自己写的简单message。

*	modified by lzx 2009.10.24 增加图像缓存，定时显示机制。
*	传入的图片会被写入缓冲区。如果缓冲区满，会覆盖最早的一帧。
*	根据图像的帧信息中的时间戳确定图片显示时间间隔。
*	用定时器驱动显示，每次显示最早的一帧。
*/

struct SDL_Surface;
struct SDL_Overlay;
struct SDL_Thread;
typedef struct _SDL_TimerID *SDL_TimerID;

class CVideoShowWnd
{
public:
	enum EVideoWndEvent
	{
		EventMouseButtonDown,

	};
	typedef void ( *CB_VideoWndEvent )( EVideoWndEvent eEvent, mu_uint32 uUserData );

	/** 缓存图像帧数。 */
	static const int PicBufferSize = 10;
	/** 是否输出图像缓冲调试打印。 */
	static const BOOL PicBufferDebug = FALSE;

public:
	CVideoShowWnd(void);
	virtual ~CVideoShowWnd(void);

	/** 创建。 */
	BOOL Create( int nXpos, int nYpos, int nWidth, int nHeight, BOOL bFullScreen );

	/** 更改。 */
	BOOL SetRect( int nXpos, int nYpos, int nWidth, int nHeight, BOOL bFullScreen );

	/** 显示。 */
//	BOOL ShowWindow( BOOL bShow );

	/** 销毁。 */
	BOOL Destroy();

	/** 显示。 异步，有缓存。*/
	BOOL DisplayAsync( const CBaseCodec::TVideoPicture& tPic, const CBaseCodec::TVideoFrameInfo& tFrameInfo );

	void SetEventCallback( CB_VideoWndEvent pFun, mu_uint32 uUserData );

	/** 设置图像显示每帧间隔时间,单位毫秒ms。 
	*/
	BOOL SetDisplayTimer( mu_uint32 nTime );

private:
	/** 从缓存中读取图片显示。 */
	mu_uint64 DisplayFromPicBuffer();

private:
	static int SDL_ThreadFunc( void *param );

	/** 消息处理回调。 */
	static mu_int32 OnMessageCallback( const mcu::TMsg& tMsg );

	/** 显示定时器回调。 */
	static unsigned int OnDisplayTimerCallback( unsigned int interval, void *param);

//	BOOL DestroyNotQuit();

	/** 经过线程转换后的Create。*/
	BOOL CreateThreadSafe();
	/** 经过线程转换后的Destroy。 */
	BOOL DestroyThreadSafe();

private:
	SDL_Surface *m_pSDLSurface;

	SDL_Overlay *m_pSDLOverlay;

	CMCUMutex m_threadSafeLock;

	CB_VideoWndEvent m_pCbVideoEvent;
	mu_uint32 m_uUserData;

	SDL_Thread * m_pSDLThread;

	/** 创建视频显示的参数。 */
	int m_nXpos;
	int m_nYpos;
	int m_nWidth;
	int m_nHeight;
	BOOL m_bFullscreen;

	
	/** 图像显示定时器。 */
	SDL_TimerID m_pDisplayTimerId;
	/** 定时器时间。 */
	int m_nDisplayTime;

	struct TPicBufferItem 
	{
		CBaseCodec::TVideoPicture m_tPic;			// 图片数据。
		CBaseCodec::TVideoFrameInfo m_tFrameInfo;	// 视频帧信息。
		BOOL m_bFillData;							// 是否已经填充数据，等待显示。

		TPicBufferItem()
		{
			m_bFillData = FALSE;
		}
	};
	/** 图像数据缓冲区。 */
	TPicBufferItem m_arPicBuffer[ PicBufferSize ];
	/** 当前写游标。 */
	int m_nWriteCursor;
	/** 当前读游标。 */
	int m_nReadCursor;

	/** 上一个显示的帧的时间戳。 */
	mu_uint64 m_u64LastTimeStamp;

	/** 上一个显示出的帧的时间戳。 */
	mu_uint64 m_u64LastDisplayFrameTimeStamp;

};
