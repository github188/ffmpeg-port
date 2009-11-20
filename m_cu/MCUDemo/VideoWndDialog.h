#pragma once

#include "MediaNet.h"
#include "SDL.h"
#include "decoder.h"
#include "staticex.h"

#ifdef USE_FFMPEG
#include "ffplay.h"
#endif

#include "PTZDialog.h"
#include "ScopeLock.h"
#include "afxwin.h"

// CVideoWndDialog 对话框

class CVideoWndDialog : public CDialog
{
	DECLARE_DYNAMIC(CVideoWndDialog)
public:
	CVideoWndDialog( CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoWndDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIDEO_WND };

	/** 常数定义. */
	enum EConstDef
	{
		TIMEOUT_RTSP_FAIL = 2,			// RTSP超时时间.
		TIMEOUT_NO_PACKET = 20,			// 无码流时的超时时间.
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	/** 设置要打开的rtsp链接,必须在窗口创建之前设置. */
	void SetRstpUrl( LPCTSTR strRtspUrl );

	/** 暂停播放. */
	BOOL Pause();

	/** 恢复播放. */
	BOOL Resume();

	/** 当前是否暂停了. */
	BOOL IsPause();

	/** 设置全屏状态.*/
	void SetFullScreen( BOOL bFullScreen );

	/** 抓拍图片. */
	BOOL PicCapture( LPCTSTR strPicPath );

	/** 开始录像. */
	BOOL StartRecord( LPCTSTR strRecordPath );

	/** 停止录像. */
	BOOL StopRecord();

	/** 当前是否在录像. */
	BOOL IsRecoding();

private:

	enum SDL_MCU_Event
	{
		SDL_WM_FULLSCREEN = SDL_USEREVENT + 1,	// 全屏控制. data1: 全屏
	
	};

	enum WM_MCU_VIDEO_WND_MESSAGE
	{
		WM_VIDEO_WND_VIDEO_OPEN_FAIL = WM_VIDEO_OPEN_FAIL,	// 视频浏览失败。WParam: EVideoPlayErrorCode，错误码。
		WM_VIDEO_WND_RECORD_FAIL = WM_RECORD_FAIL,			// 录像失败。
	};


private:
	CMediaNet m_MediaNet;
	CString m_strRtspUrl;

	/** SDL 线程句柄。 */
	SDL_Thread *m_pSDLThread;

	/** SDL的Overlay。*/
	SDL_Overlay *m_pSDLOverlay;

	/** 显示区域大小. */
	SDL_Rect m_rcVideoShow; 

	/** SDL surface */
	SDL_Surface *m_pSDLSurface;
	
	/** 是否是全屏模式. */
	BOOL m_bFullScreenMode;

	/** ptz控制对话框. */
//	CPTZDialog m_dlgPtz;

	/** 线程安全锁.*/
	CMCUMutex m_threadSafeLock;

	/** 状态刷新定时器ID */
	UINT_PTR m_dwStatusUpdateTimerId;

	/** 最后一次处在正常状态的时间. */
	CTime m_timeLastWork;

	/** 最后一次绘制图形的时间. */
	CTime m_timeLastDrawVideo;

	/** RTSP无效超时时间. */
	int m_nTimeoutRTSPFail;

	/** 码流不到达的超时时间. */
	int m_nTimeoutNoPacket;

	/** 当前记录的RTSP状态. */
	ERTSPStatus m_eRtspStatus;

	/** 等待处理视频请求超时的消息. */
	BOOL m_bCheckRtspTimeout;

	/** 在处理视频请求失败的消息。*/
	BOOL m_bCheckRtspError;

	/** 等待处理没有码流包的消息. */
	BOOL m_bCheckNoPacketTimeout;

	/** 模拟双击消息需要记录上次的时间。 */
	clock_t m_timeLastMouseClick;

	/** 是否暂停了. */
	BOOL m_bPause;


protected:
	virtual void PostNcDestroy();

private:
	/** 初始化SDL。 */
	BOOL InitSDL();
	/** 销毁SDL。 */
	BOOL DestroySDL();

	/** SDL线程*/
	static int SDL_ThreadFunc( void *param );

	/** 图像输入回调函数，用于接收解码库的解码结果回调。 */
	static BOOL YUVCallbackS( 
		const CDecoder::TVideoPicture *pic, 
		const CBaseCodec::TVideoFrameInfo* pFrameInfo,
		void *param );

	/** 回调函数的成员函数处理。 */
	void YUVCallback( const CDecoder::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo* pFrameInfo );
	

	/** 状态轮巡监管管理函数. */
	void StatusCheck();

	/** 存储空间检查。 */
	void StorageSpaceCheck();

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 视频状态.
	CStaticEx m_staticVideoStatus;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
