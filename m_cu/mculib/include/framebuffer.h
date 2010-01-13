#pragma once

/** 
*   帧缓存。缓存帧数据，定时输出，使图像显示平滑。
*/
#include "mcucommon.h"
#include "basecodec.h"

typedef struct _SDL_TimerID *SDL_TimerID;

class CFrameBuffer
{
public:
    CFrameBuffer(void);
    virtual ~CFrameBuffer(void);

    /** 初始化。
    *   申请内存。启动定时器。
    */
    BOOL Init( int nFrameBufNum );

    /** 输入图像数据。 */
    BOOL InputFrame( const CBaseCodec::TVideoPicture& tPic, const CBaseCodec::TVideoFrameInfo& tFrameInfo );

    /** 图像回调。 */
    typedef void ( *FunFrameCb )( const CBaseCodec::TVideoPicture* pPic, const CBaseCodec::TVideoFrameInfo* pFrameInfo,
        void *userData );

    /** 设置图像输出回调。 */
    void SetFrameCallback( FunFrameCb pFun, void * param );

	/** 清空缓存图像。 */
	void Clear();

private:
    /** 显示定时器回调。 */
    static unsigned int OnDisplayTimerCallback( unsigned int interval, void *param);

    /** 从缓存中读取图片显示。 */
    mu_uint64 DisplayFromPicBuffer();

private:
    /** 缓存图像帧数。 */
    static const int MaxFrameBufferSize = 10;
    /** 是否输出图像缓冲调试打印。 */
    static const BOOL PicBufferDebug = FALSE;

    /** 图片缓存大小。*/
    int m_nFrameBufferNum;

    /** 回调函数。*/
    FunFrameCb m_pFunFrameCb;
    /** 参数。 */
    void *m_frameCbUserData;

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
    TPicBufferItem m_arPicBuffer[ MaxFrameBufferSize ];
    /** 当前写游标。 */
    int m_nWriteCursor;
    /** 当前读游标。 */
    int m_nReadCursor;

    /** 上一个显示的帧的时间戳。 */
    mu_uint64 m_u64LastTimeStamp;

    /** 上一个显示出的帧的时间戳。 */
    mu_uint64 m_u64LastDisplayFrameTimeStamp;

    /** 线程同步锁。 */
    CMCUMutex m_threadSafeLock;

};
