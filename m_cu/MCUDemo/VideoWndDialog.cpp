// VideoWndDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "VideoWndDialog.h"
#include <time.h>
#include "config.h"

#ifdef USE_FFMPEG
#include "ffplay.h"
#include "avcodec.h"
#endif

#include "SDL.h"

#include "ScopeLock.h"

// CVideoWndDialog 对话框

IMPLEMENT_DYNAMIC(CVideoWndDialog, CDialog)

CVideoWndDialog::CVideoWndDialog(  CWnd* pParent /*=NULL*/)
	: CDialog(CVideoWndDialog::IDD, pParent),
	m_pSDLOverlay( NULL ),
	m_pSDLSurface( NULL ),
	m_pSDLThread( NULL ),
	m_bFullScreenMode( FALSE )
{
	m_timeLastMouseClick =0;
    m_prcVideoShow = NULL;
    m_prcVideoShow = new SDL_Rect();
}

CVideoWndDialog::~CVideoWndDialog()
{
}

void CVideoWndDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIDEO_STATUS, m_staticVideoStatus);
}


BEGIN_MESSAGE_MAP(CVideoWndDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CVideoWndDialog 消息处理程序

BOOL CVideoWndDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 浏览状态显示。
	m_staticVideoStatus.SetBkTransparent( FALSE, FALSE );
	m_staticVideoStatus.SetBkColor( RGB( 0,0,0 ) );
	m_staticVideoStatus.SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );

	BOOL bResult;
	// TODO:  在此添加额外的初始化
	//if ( !m_strRtspUrl.IsEmpty() )
	//{
	//	m_bCheckRtspTimeout = TRUE;
	//	m_bCheckRtspError = TRUE;
	//	m_MediaNet.OpenRTSP( m_strRtspUrl );
	//}


		
	

	bResult = this->InitSDL();
	if ( !bResult )
	{
		mcu::log << _T( "Init SDL fail! Maybe Only one video window is permit at one time！！！" ) << endl;
	}

    bResult = m_frameBuffer.Init( 10 );
    if ( !bResult )
    {
        mcu::log << _T( "Init frame buffer fail!!!!" ) << endl;
    }
    m_frameBuffer.SetFrameCallback( OnBufferdVideoFrameShowS, this );


	// 设置解码回调。
//	CDecoder::SetPictueCallback( &CVideoWndDialog::YUVCallbackS, this );

	// 开启定时器.
//	m_dwStatusUpdateTimerId = this->SetTimer( 1, 500, NULL );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CVideoWndDialog::SetRstpUrl( LPCTSTR strRtspUrl )
//{
//	m_strRtspUrl = strRtspUrl;
//}

void CVideoWndDialog::SetFullScreen( BOOL bFullScreen )
{
	m_bFullScreenMode = bFullScreen;

	

	//if ( m_bFullScreenMode )
	//{
	//	CRect rcClient;
	//	this->GetClientRect( rcClient );
	//	SDL_Event fe;
	//	fe.type = SDL_VIDEORESIZE;
	//	fe.resize.w = max( rcClient.Width(), rcClient.Height() );
	//	fe.resize.h = min( rcClient.Width(), rcClient.Height() );

	//	SDL_PushEvent( &fe );
	//}
	
}

//BOOL CVideoWndDialog::PicCapture( LPCTSTR strPicPath )
//{
//	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
//	if( pDec )
//	{	
//		return pDec->CapturePic( strPicPath );
//	}
//	else
//	{
//		return FALSE;
//	}
//}

void CVideoWndDialog::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
//	this->DestroySDL();

	CDialog::PostNcDestroy();
}

BOOL CVideoWndDialog::InitSDL()
{
	if ( SDL_WasInit( 0 ) )
	{
		return FALSE;
	}

	
	// 开启SDL线程。
	SDL_Thread  * pSdlThread = SDL_CreateThread( SDL_ThreadFunc, this );
	m_pSDLThread = pSdlThread;

	return ( 0 != m_pSDLThread );
}

BOOL CVideoWndDialog::DestroySDL()
{
	if ( m_pSDLThread )
	{
		// 消除回调函数。

		// 防止多线程冲突。以后优化！
		SDL_Delay( 10 );

		// 销毁SDL。
		SDL_Event eventQuit;
		eventQuit.type = SDL_QUIT;
		SDL_PushEvent( &eventQuit );

		// 等待SDL线程退出。
		int nStatus;
		SDL_WaitThread( m_pSDLThread, &nStatus );
	}	

	

	return TRUE;
}

int CVideoWndDialog::SDL_ThreadFunc( void *param )
{
	CVideoWndDialog *pThis = (CVideoWndDialog*)param;
	HWND hWnd = pThis->GetSafeHwnd();

	BOOL bResult = TRUE;
	// 使用Dib方式显示。
	SDL_putenv( "SDL_VIDEODRIVER=windib" );

	// 指定显示窗口为本窗口。
	_ASSERT( hWnd );
	stringstream strWnd;
	strWnd << "SDL_WINDOWID=" << (int)hWnd;
	SDL_putenv( strWnd.str().c_str() );


	// 只初始化视频子系统。
	int initFlags = SDL_INIT_VIDEO ;
	if ( SDL_Init ( initFlags ) ) 
	{
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		_ASSERT( FALSE );
		bResult = FALSE;
	}

	SDL_WM_SetCaption("M_CUDemo", "mcu" );


	SDL_Rect rcClient = *( pThis->m_prcVideoShow );

	int videoFlags = SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL;
//	int w,h;

	if( pThis->m_bFullScreenMode ) 
		videoFlags |= SDL_FULLSCREEN;
	else               
		videoFlags |= SDL_RESIZABLE;

	// 不显示标题。
	videoFlags |= SDL_NOFRAME;

	pThis->m_pSDLSurface = SDL_SetVideoMode( rcClient.w, rcClient.h, 0, videoFlags );

 	//SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
 	//SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
 	//SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
 	//SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	SDL_Event sdlevent;

	bool bRun = true;
	while( bRun ) 
	{
		SDL_WaitEvent(&sdlevent);
//		cout << "sdl event : " << (int)sdlevent.type << endl;

		switch( sdlevent.type ) 
		{
			case SDL_KEYDOWN:
			      switch( sdlevent.key.keysym.sym) 
				  {

		            case SDLK_LEFT:
		                mcu::log << _T( "left" ) << endl;
		                break; 
		            case SDLK_RIGHT:
		                mcu::log << _T( "right" ) << endl;
		                break;
		            case SDLK_UP:
						mcu::log << _T( "up" ) << endl;
		                break;
		            case SDLK_DOWN:
						mcu::log << _T( "down" ) << endl;
		                break;			            
		            default:
		                break;
		            }
		    break;
			
		case SDL_VIDEORESIZE:
			{
				SCOPE_LOCK( pThis->m_threadSafeLock );

				int nflag = 0;
				nflag |= SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL;
				if ( pThis->m_bFullScreenMode )
				{
					nflag |= SDL_FULLSCREEN;		

					//// 写死,全屏只能横着.
					//SDL_Event tempEvent = sdlevent;
					//sdlevent.resize.w = max( tempEvent.resize.w, tempEvent.resize.h );
					//sdlevent.resize.h = min( tempEvent.resize.w, tempEvent.resize.h );
				}
				else
				{
					nflag |= SDL_RESIZABLE;
				}
				if ( !pThis->m_pSDLSurface 
					|| pThis->m_pSDLSurface->w != sdlevent.resize.w 
					|| pThis->m_pSDLSurface->h != sdlevent.resize.h 
					|| pThis->m_pSDLSurface->flags != nflag ) 
				{
					pThis->m_pSDLSurface = SDL_SetVideoMode( sdlevent.resize.w, sdlevent.resize.h, 0,
						nflag );
				}				
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				UINT uMessage;

				// SDL中没有双击消息，这里模拟一个。				
				clock_t now = ::GetTickCount();
				
				clock_t timespan = now - pThis->m_timeLastMouseClick;
				if ( timespan < 1000 )
				{
					// 双击。重新开始计时。
					pThis->m_timeLastMouseClick = 0;

					uMessage = WM_LBUTTONDBLCLK;
				}
				else
				{
					// 单击。从现在开始计时，等待下次单击。
					pThis->m_timeLastMouseClick = now;

					uMessage = WM_LBUTTONDOWN;
				}
				

				HWND hParent = pThis->GetParent()->GetSafeHwnd();
				::PostMessage( hParent, uMessage, 
					WPARAM( sdlevent.button.state), 
					MAKELPARAM( sdlevent.button.x, sdlevent.button.y ) );
			}
			break;
		case SDL_QUIT:
			bRun = false;
			break;
	
		default:
			break;
		}
	}

	SDL_Quit();
	return 0;

	//	if (!display_disable) {
	//#ifdef HAVE_SDL_VIDEO_SIZE
	//		const SDL_VideoInfo *vi = SDL_GetVideoInfo();
	//		fs_screen_width = vi->current_w;
	//		fs_screen_height = vi->current_h;
	//#endif
	//	}

	// 	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	// 	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	// 	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	// 	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	//	av_init_packet(&flush_pkt);
	//	flush_pkt.data= "FLUSH";
	//
	//#ifdef LOG_OUT
	//	log_file = fopen(".\\mplayer.txt", "w");
	//#endif

	//// 初始化信号量。
	//g_pictq_mutex = SDL_CreateMutex();
	//g_pictq_cond = SDL_CreateCond();



	//	cur_stream = rtsp_open(input_filename, file_iformat);

	//	event_loop();
	//	return bResult;
}
BOOL CVideoWndDialog::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDecoder::SetPictueCallback( NULL, NULL );

	SCOPE_LOCK( m_threadSafeLock );	

	DestroySDL();

//	m_MediaNet.CloseRTSP();

	if ( m_pSDLOverlay )
	{
		SDL_FreeYUVOverlay( m_pSDLOverlay );
        m_pSDLOverlay = NULL;
	}	

	//if ( m_dwStatusUpdateTimerId )
	//{
	//	KillTimer( m_dwStatusUpdateTimerId );
	//	m_dwStatusUpdateTimerId = 0;
	//}

	return CDialog::DestroyWindow();
}

void CVideoWndDialog::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

//BOOL CVideoWndDialog::YUVCallbackS( const CDecoder::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo* pFrameInfo, void *param )
//{
//	if ( param )
//	{
//		CVideoWndDialog *pThis = (CVideoWndDialog*)param;
//		pThis->YUVCallback( pic, pFrameInfo );
//	}
//	return TRUE;
//}

//void CVideoWndDialog::YUVCallback( const CDecoder::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo* pFrameInfo )
//{
//	SCOPE_LOCK( m_threadSafeLock );
//
//	int nPicWidth = pic->picWidth;
//	int nPicHeight = pic->picHeight;
//
//	// 更新最后一次绘制图形的时间.
//	m_timeLastDrawVideo = CTime::GetCurrentTime();
//
//	// SDL还没有正常初始化.
//	if ( !m_pSDLSurface )
//	{
//		return;
//	}
//
//	// 检查是否被暂停了.
//	if ( m_bPause )
//	{
//		return;
//	}
//
//	// 检查是否要创建 YUV Overlay。
//	if ( !m_pSDLOverlay 
//		|| m_pSDLOverlay->w != nPicWidth
//		|| m_pSDLOverlay->h != nPicHeight )
//	{
//		if ( m_pSDLOverlay )
//		{
//			SDL_FreeYUVOverlay( m_pSDLOverlay );
//		}
//
////		SDL_Surface *screen = SDL_SetVideoMode( nWidth, nHeight, 0, SDL_HWSURFACE|SDL_RESIZABLE|SDL_ASYNCBLIT|SDL_HWACCEL );
//
//		m_pSDLOverlay = SDL_CreateYUVOverlay( nPicWidth, nPicHeight, SDL_IYUV_OVERLAY, m_pSDLSurface );
//
//	}
//
//	// copy图像。
//	/* get a pointer on the bitmap */
////	AVPicture pict;
//
//	SDL_LockYUVOverlay ( m_pSDLOverlay );
//
////	int dst_pix_fmt = PIX_FMT_YUV420P;
//	//pict.data[0] = m_pSDLOverlay->pixels[0];
//	//pict.data[1] = m_pSDLOverlay->pixels[1];
//	//pict.data[2] = m_pSDLOverlay->pixels[2];
//
//	//pict.linesize[0] = m_pSDLOverlay->pitches[0];
//	//pict.linesize[1] = m_pSDLOverlay->pitches[1];
//	//pict.linesize[2] = m_pSDLOverlay->pitches[2];
//
//	//av_picture_copy(&pict, (const AVPicture *)avFrame, 
//	//	dst_pix_fmt, nWidth, nHeight );
//	for( int i=0; i<3; ++i )
//	{
//		memcpy( m_pSDLOverlay->pixels[i], pic->data[i], pic->linesize[i] );
//		m_pSDLOverlay->pitches[i] = (mu_uint16)pic->linesize[i];
//	}
//	
//
//	/* update the bitmap content */
//	SDL_UnlockYUVOverlay( m_pSDLOverlay );
//
//	// 显示。
//	int nWidth = pFrameInfo->frameWidth;
//	int nHeight = pFrameInfo->frameHeight;
//	const int conBlackBorderOffset = -2;
//	SDL_Rect rect;
//
//	if ( nWidth * m_pSDLSurface->h > nHeight * m_rcVideoShow.w )
//	{
//		// 图像宽高比大于显示区域宽高比,说明要按照宽度调整.
//		rect.h = nHeight * m_rcVideoShow.w / nWidth ;
//		rect.w = m_pSDLSurface->w;
//	}
//	else
//	{
//		// 按照高度调整.
//		rect.w = nWidth * m_pSDLSurface->h / nHeight;
//		rect.h = m_pSDLSurface->h;
//	}
//
//	rect.x = conBlackBorderOffset + ( m_pSDLSurface->w - rect.w ) /2 ;
//	rect.y = conBlackBorderOffset + ( m_pSDLSurface->h - rect.h ) /2 ;
//
//	if ( m_bFullScreenMode )
//	{
////		rect.x = rect.y = 0;
//		
//	}
////	rect = m_rcVideoShow;
//	SDL_DisplayYUVOverlay( m_pSDLOverlay, &rect );
//
//	// 判断是否需要重新创建绘图表面。
//	//if ( 0 == g_videoPic[ decoderId ]->bmp 
//	//	|| g_videoPic[ decoderId ]->width != g_avCodecContext[ decoderId ]->width
//	//	|| g_videoPic[ decoderId ]->height != g_avCodecContext[ decoderId ]->height )
//	//{
//	//	g_videoPic[ decoderId ]->pts = PIX_FMT_YUV420P;
//	//	g_videoPic[ decoderId ]->width = g_avCodecContext[ decoderId ]->width;
//	//	g_videoPic[ decoderId ]->height = g_avCodecContext[ decoderId ]->height;
//	//	g_videoPic[ decoderId ]->bmp = 0;
//	//	g_videoPic[ decoderId ]->allocated = 0;
//
//
//	//	// 创建 overlay。
//	//	/* the allocation must be done in the main thread to avoid	locking problems */
//	//	sdlevent.type = FF_ALLOC_SDL_OVERLAY;
//	//	sdlevent.user.data1 = g_videoPic[ decoderId ];
//	//	SDL_PushEvent( &sdlevent );
//
//	//	/* wait until the picture is allocated */
//	//	SDL_LockMutex( g_pictq_mutex );
//	//	while (!vp->allocated ) 
//	//	{
//	//		SDL_CondWait( g_pictq_cond, g_pictq_mutex);
//	//	}
//	//	SDL_UnlockMutex( g_pictq_mutex );
//	//}
//
//	//// 将数据拷入overlay。
//
//	///* get a pointer on the bitmap */
//	//SDL_LockYUVOverlay (vp->bmp);
//
//	//dst_pix_fmt = PIX_FMT_YUV420P;
//	//pict.data[0] = vp->bmp->pixels[0];
//	//pict.data[1] = vp->bmp->pixels[2];
//	//pict.data[2] = vp->bmp->pixels[1];
//
//	//pict.linesize[0] = vp->bmp->pitches[0];
//	//pict.linesize[1] = vp->bmp->pitches[2];
//	//pict.linesize[2] = vp->bmp->pitches[1];
//
//	//av_picture_copy(&pict, (const AVPicture *)g_avFrame[ decoderId ], 
//	//	dst_pix_fmt, vp->width, vp->height );
//
//	///* update the bitmap content */
//	//SDL_UnlockYUVOverlay(vp->bmp);
//
//	//rect.x = 0;
//	//rect.y = 0;
//	//rect.w = vp->width ;
//	//rect.h = vp->height;
//	//SDL_DisplayYUVOverlay( vp->bmp, &rect );
//}
void CVideoWndDialog::OnSize(UINT nType, int cx, int cy)
{
	SCOPE_LOCK( m_threadSafeLock );

	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_prcVideoShow->x = 0;
	m_prcVideoShow->y = 0;
	m_prcVideoShow->w = cx;
	m_prcVideoShow->h = cy;


	SDL_Event sdlEvent;
	sdlEvent.resize.type = SDL_VIDEORESIZE;
	//if ( m_bFullScreenMode )
	//{
	//	sdlEvent.resize.w = max( cx,cy );
	//	sdlEvent.resize.h = min( cx,cy );
	//}
	//else
	{
		sdlEvent.resize.w = cx;
		sdlEvent.resize.h = cy;
	}
	
	SDL_PushEvent( &sdlEvent );

	// 状态显示在最底下.
	CRect rcVideoStatus;
	if ( this->m_staticVideoStatus.GetSafeHwnd() )
	{
		m_staticVideoStatus.GetWindowRect( rcVideoStatus );
		int nHeight = rcVideoStatus.Height();
		rcVideoStatus.left = 0;
		rcVideoStatus.right = cx;
		rcVideoStatus.OffsetRect( 0, cy - rcVideoStatus.bottom );

		m_staticVideoStatus.MoveWindow( rcVideoStatus );
	}
}

void CVideoWndDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	//if( nIDEvent == m_dwStatusUpdateTimerId )
	//{
	//	// 刷新和轮巡.
	//	this->StatusCheck();

	//	// 检查空间。
	//	this->StorageSpaceCheck();
	//}


	CDialog::OnTimer(nIDEvent);
}

//void CVideoWndDialog::StorageSpaceCheck()
//{
//	if ( this->IsRecoding() )
//	{
//		// 检测磁盘空间。
//		ULARGE_INTEGER nFreeSpaceToCaller, nTotalSpace, nFreeSpace;
//		tstring strRecDir;
//		CConfig::Instance()->GetCaptureDir( strRecDir );
//		BOOL bResult = ::GetDiskFreeSpaceEx( strRecDir.c_str(), &nFreeSpaceToCaller, &nTotalSpace, &nFreeSpace );
//		int nMinSpace = MIN_STORAGE_SPACE;
//		CConfig::Instance()->GetMinStorageSpace( nMinSpace ) ;
//		if ( nFreeSpaceToCaller.QuadPart < nMinSpace )
//		{
//			bResult = this->StopRecord();
//
//			this->GetParent()->SendMessage( WM_VIDEO_WND_RECORD_FAIL, MCU_Error_Storage_Full, 0 );
//		}
//	}
//	
//}
//
//void CVideoWndDialog::StatusCheck()
//{
//	ERTSPStatus eRtspStat = this->m_MediaNet.GetRtspStatus();
//
//	
//	// 检测RTSP是否正常工作.
//
//	// 检查是否达到了服务器的最大路数。
//	if ( RTSPStatus_Error_Server_Full == eRtspStat && m_bCheckRtspError )
//	{
//		// 服务器拒绝了客户端的请求，直接返回失败。
//		m_bCheckRtspError = FALSE;
//		this->GetParent()->SendMessage( WM_VIDEO_WND_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Server_Full, 0 );
//		return;
//	}
//
//	if ( m_bCheckRtspError )
//	{
//        switch( eRtspStat )
//        {
//        case RTSPStatus_Error_Unknown:		// 未知错误。	
//        case RTSPStatus_Error_Init_Fail:	// 初始化失败。
//        case RTSPStatus_Error_Opition:		// Opition 命令失败。
//        case RTSPStatus_Error_Description:
//        case RTSPStatus_Error_Setup:
//        case RTSPStatus_Error_Play:		
//        case RTSPStatus_Error_SDP:			// 解析sdp信息出错。
//        case RTSPStatus_Error_Create_Rcv:	// 码流接收创建失败
//            // 服务器拒绝了客户端的请求，直接返回失败。
//            m_bCheckRtspError = FALSE;
//            this->GetParent()->SendMessage( WM_VIDEO_WND_VIDEO_OPEN_FAIL, MCU_Error_Unknow, 0 );
//            return;
//        default:
//            break;
//        }
//		
//	}
//
//	if ( RTSPStatus_Idle != eRtspStat )	
//	{
//		m_timeLastWork = CTime::GetCurrentTime();		
//		m_bCheckRtspTimeout = TRUE;
//	}
//	else if( m_bCheckRtspTimeout )
//	{
//		// rtsp 没有工作,考虑要关闭对话框.
//		CTime now = CTime::GetCurrentTime();
//		CTimeSpan tIdleTime = now - m_timeLastWork;
//		if ( tIdleTime.GetTotalSeconds() > m_nTimeoutRTSPFail )
//		{
//			m_bCheckRtspTimeout = FALSE;
//			this->GetParent()->SendMessage( WM_VIDEO_WND_VIDEO_OPEN_FAIL, MCU_Error_Unknow, 0 );
//		}
//	}
//
//	// 检测码流是否正常接收(通过检测是否绘制图形.).
//	if ( m_bCheckNoPacketTimeout && RTSPStatus_Running == eRtspStat )
//	{
//		CTime now = CTime::GetCurrentTime();
//		CTimeSpan noPacketTime = now - m_timeLastDrawVideo;
//		if ( noPacketTime.GetTotalSeconds() > m_nTimeoutNoPacket )
//		{
//			m_bCheckNoPacketTimeout = FALSE;
//			this->GetParent()->SendMessage( WM_VIDEO_WND_VIDEO_OPEN_FAIL, MCU_Error_Unknow, 0 );
//		}
//	}
//	else
//	{
//		m_timeLastDrawVideo = CTime::GetCurrentTime();
//	}
//
//	if ( eRtspStat != m_eRtspStatus && eRtspStat == RTSPStatus_Running )
//	{
//		m_bCheckNoPacketTimeout = TRUE;
//	}
//
//	// 更新状态条显示.
//	// 只有非正常工作状态时才需要显示状态条.
//	BOOL bWorkBad = ( RTSPStatus_Running != eRtspStat );	// RTSP没有在工作时需要显示.
//	BOOL bNoPacket = FALSE;
//	if( !bWorkBad )
//	{
//		CTime now = CTime::GetCurrentTime();
//		CTimeSpan noPacketTime = now - m_timeLastDrawVideo;
//		bNoPacket |= noPacketTime.GetTotalSeconds() > 5;	// 一段时间没有等到码流.
//	}
//	bWorkBad |= bNoPacket;
//	
//	m_staticVideoStatus.ShowWindow( bWorkBad ? SW_SHOW : SW_HIDE );	// 如果工作状态不好,显示状态条.
//
//	if( bWorkBad )
//	{
//		tstring strShowVideoStatus;
//		switch( eRtspStat )
//		{
//		case RTSPStatus_Idle:
//			strShowVideoStatus = _T( "播放停止" );
//			break;
//		case RTSPStatus_Init:
//			strShowVideoStatus = _T( "播放器初始化..." );
//			break;
//		case RTSPStatus_Opition:
//		case RTSPStatus_Description:
//			strShowVideoStatus = _T( "连接视频服务器..." );
//			break;
//		case RTSPStatus_Setup:
//		case RTSPStatus_Play:
//			strShowVideoStatus = _T( "正在请求视频..." );
//			break;
//		case RTSPStatus_Running:
//			if ( bNoPacket )
//			{
//				strShowVideoStatus = _T( "等待码流..." );
//			}
//			else
//			{
//				strShowVideoStatus = _T( "正在播放" );
//			}		
//			break;
//		default:
//			break;
//		}
//		CString strStateShow;
//		m_staticVideoStatus.GetWindowText( strStateShow );
//		if ( strStateShow != strShowVideoStatus.c_str() )
//		{
//			m_staticVideoStatus.SetWindowText( strShowVideoStatus.c_str() );
//		}		
//	}
//
//	if ( eRtspStat == m_eRtspStatus )
//	{
//		return;
//	}
//
//
//	// 调试信息.
//	tstring strStatus;
//	switch( eRtspStat )
//	{
//	case RTSPStatus_Idle:
//		{
//			strStatus = _T( "Idle" );
//		}
//		break;
//	case RTSPStatus_Init:
//		strStatus = _T( "Init" );
//		break;
//	case RTSPStatus_Opition:
//		strStatus = _T( "Opition" );
//	    break;
//	case RTSPStatus_Description:
//		strStatus = _T( "Description" );
//	    break;
//	case RTSPStatus_Setup:
//		strStatus = _T( "Setup" );
//		break;
//	case RTSPStatus_Play:
//		strStatus = _T( "Play" );
//		break;
//	case RTSPStatus_Running:
//		strStatus = _T( "Running" );
//		
//		break;
//	default:
//	    break;
//	}
//	mcu::log << _T( "rtsp status: " ) << strStatus << endl;
//
//	// 更新保存的RTSP状态.
//	this->m_eRtspStatus = eRtspStat;
//}

void CVideoWndDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDblClk(nFlags, point);
}

//BOOL CVideoWndDialog::Pause()
//{ 
//	m_bPause = TRUE;
//	return TRUE;
//}
//
//BOOL CVideoWndDialog::Resume()
//{
//	m_bPause = FALSE;
//	return TRUE;
//}
//
//BOOL CVideoWndDialog::IsPause()
//{
//	return m_bPause;
//}

//BOOL CVideoWndDialog::StartRecord( LPCTSTR strRecordPath )
//{
//	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
//	if( pDec )
//	{
//		return pDec->StartRecord( strRecordPath );
//	}
//	else
//	{
//		return FALSE;
//	}	
//}
//
//BOOL CVideoWndDialog::StopRecord()
//{
//	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
//	if( pDec )
//	{
//		return pDec->StopRecord();
//	}
//	else
//	{
//		return FALSE;
//	}
//}

//BOOL CVideoWndDialog::IsRecoding()
//{
//	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
//	if( pDec )
//	{
//		return pDec->IsRecording();
//	}
//	return FALSE;
//}

void CVideoWndDialog::OnVideoPicture( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo )
{
    SCOPE_LOCK( m_threadSafeLock );

    this->m_frameBuffer.InputFrame( *pic, *pFrameInfo );
}

void CVideoWndDialog::OnBufferdVideoFrameShowS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *userData )
{
    CVideoWndDialog *pThis = ( CVideoWndDialog *)userData;
    if ( pThis )
    {
        pThis->OnBufferdVideoFrameShow( pic, pFrameInfo );
    }
    else
    {
        mcu::log << _T( "Video wnd buffer video frame show S this is NULL!" ) << endl;
    }
}

void CVideoWndDialog::OnBufferdVideoFrameShow( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo )
{
 //   mcu::log << _T( "CVideoWndDialog Video pic callback!" ) << endl;
	SCOPE_LOCK( m_threadSafeLock );

	int nPicWidth = pic->picWidth;
	int nPicHeight = pic->picHeight;

	// SDL还没有正常初始化.
	if ( !m_pSDLSurface )
	{
        mcu::log << _T( "SDL Surface not init when pic callback!" ) << endl;
		return;
	}

	// 检查是否要创建 YUV Overlay。
	if ( !m_pSDLOverlay 
		|| m_pSDLOverlay->w != nPicWidth
		|| m_pSDLOverlay->h != nPicHeight )
	{
		if ( m_pSDLOverlay )
		{
			SDL_FreeYUVOverlay( m_pSDLOverlay );
            m_pSDLOverlay = NULL;
		}

//		SDL_Surface *screen = SDL_SetVideoMode( nWidth, nHeight, 0, SDL_HWSURFACE|SDL_RESIZABLE|SDL_ASYNCBLIT|SDL_HWACCEL );
		m_pSDLOverlay = SDL_CreateYUVOverlay( nPicWidth, nPicHeight, SDL_IYUV_OVERLAY, m_pSDLSurface );

        mcu::log << _T( "Create YUV overlay! w: " ) << nPicWidth << _T( " h: " ) << nPicHeight << _T( " ret: " ) << m_pSDLOverlay << endl;

	}

	// copy图像。
	/* get a pointer on the bitmap */
//	AVPicture pict;

	SDL_LockYUVOverlay ( m_pSDLOverlay );

//	int dst_pix_fmt = PIX_FMT_YUV420P;
	//pict.data[0] = m_pSDLOverlay->pixels[0];
	//pict.data[1] = m_pSDLOverlay->pixels[1];
	//pict.data[2] = m_pSDLOverlay->pixels[2];

	//pict.linesize[0] = m_pSDLOverlay->pitches[0];
	//pict.linesize[1] = m_pSDLOverlay->pitches[1];
	//pict.linesize[2] = m_pSDLOverlay->pitches[2];

	//av_picture_copy(&pict, (const AVPicture *)avFrame, 
	//	dst_pix_fmt, nWidth, nHeight );
	for( int i=0; i<3; ++i )
	{
		memcpy( m_pSDLOverlay->pixels[i], pic->data[i], pic->linesize[i] );
		m_pSDLOverlay->pitches[i] = (mu_uint16)pic->linesize[i];
	}
	

	/* update the bitmap content */
	SDL_UnlockYUVOverlay( m_pSDLOverlay );

	// 显示。
	int nWidth = pFrameInfo->frameWidth;
	int nHeight = pFrameInfo->frameHeight;
	const int conBlackBorderOffset = -2;
	SDL_Rect rect;

	if ( nWidth * m_pSDLSurface->h > nHeight * m_prcVideoShow->w )
	{
		// 图像宽高比大于显示区域宽高比,说明要按照宽度调整.
		rect.h = nHeight * m_prcVideoShow->w / nWidth ;
		rect.w = m_pSDLSurface->w;
	}
	else
	{
		// 按照高度调整.
		rect.w = nWidth * m_pSDLSurface->h / nHeight;
		rect.h = m_pSDLSurface->h;
	}

	rect.x = conBlackBorderOffset + ( m_pSDLSurface->w - rect.w ) /2 ;
	rect.y = conBlackBorderOffset + ( m_pSDLSurface->h - rect.h ) /2 ;

	SDL_DisplayYUVOverlay( m_pSDLOverlay, &rect );
}

void CVideoWndDialog::OnRtspStatus( const ERTSPStatus eRtspStatus, const EMCU_ErrorCode eErrorCode )
{
    

    BOOL bShowRtspStatus = TRUE;
    tstring strShowVideoStatus;
    switch( eRtspStatus )
    {
    case RTSPStatus_Idle:
        strShowVideoStatus = _T( "播放停止" );
    	break;
    case RTSPStatus_Init:
        strShowVideoStatus = _T( "播放器初始化..." );
    	break;
    case RTSPStatus_Opition:
        strShowVideoStatus = _T( "连接视频服务器..." );
        break;
    case RTSPStatus_Description:
        strShowVideoStatus = _T( "连接视频服务器..." );
        break;
    case RTSPStatus_Setup:
        strShowVideoStatus = _T( "正在请求视频..." );
    	break;
    case RTSPStatus_Play:
        strShowVideoStatus = _T( "正在请求播放..." );
    	break;
    case RTSPStatus_Running:
        strShowVideoStatus = _T( "正在播放" );
        bShowRtspStatus = FALSE;                    // 正常播放的时候不显示提示。
        break;
    case RTSPStatus_WaitingPacket:
        strShowVideoStatus = _T( "等待码流..." );
        break;
    case RTSPStatus_Error_Server_Full:
        strShowVideoStatus = _T( "服务器的转码能力不足" );
        this->GetParent()->PostMessage( WM_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Server_Full, 0 );
        break;
    case RTSPStatus_Error_Unknown:    
    case RTSPStatus_Error_Init_Fail:
    case RTSPStatus_Error_Opition:
    case RTSPStatus_Error_Description:
    case RTSPStatus_Error_Setup:
    case RTSPStatus_Error_Play:
    case RTSPStatus_Error_SDP:
    case RTSPStatus_Error_Create_Rcv:
    case RTSPStatus_Error_WaitPacket:
        this->GetParent()->PostMessage( WM_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Fail, 0 );
        strShowVideoStatus = _T( "播放错误" );
        break;
    case RTSPStatus_Error_Decoder_Fail:
        this->GetParent()->PostMessage( WM_VIDEO_OPEN_FAIL, MCU_Error_Decoder_Null, 0 );
        strShowVideoStatus = _T( "解码器创建失败" );
        break;
    default:
        break;
    }

    if ( bShowRtspStatus )
    {
        m_staticVideoStatus.SetWindowText( strShowVideoStatus.c_str() );
    }
    m_staticVideoStatus.ShowWindow( bShowRtspStatus ? SW_SHOW : SW_HIDE );	// 如果工作状态不好,显示状态条.
  
    tstringstream tmp;
    tmp << _T( "CVideoWndDialog Rtso status notify status: " ) << eRtspStatus << _T( " er: " ) << eErrorCode 
        << _T( " show status: " ) << strShowVideoStatus << endl;
    mcu::log << tmp.str();
}

BOOL CVideoWndDialog::ClearScreen()
{
    mcu::log << _T( "Clear the Video screen." ) << endl;

    if ( m_pSDLOverlay )
    {
        SDL_LockYUVOverlay ( m_pSDLOverlay );

        int nSize = m_pSDLOverlay->w * m_pSDLOverlay->h;
        memset( m_pSDLOverlay->pixels[0], 0, nSize );
        m_pSDLOverlay->pitches[0] = nSize;

        memset( m_pSDLOverlay->pixels[1], 128, nSize >> 2 );
        m_pSDLOverlay->pitches[1] = nSize >> 2;

        memset( m_pSDLOverlay->pixels[2], 128, nSize >> 2 );
        m_pSDLOverlay->pitches[2] = nSize >> 2;

        SDL_UnlockYUVOverlay( m_pSDLOverlay );

        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = m_pSDLOverlay->w;
        rect.h = m_pSDLOverlay->h;
        SDL_DisplayYUVOverlay( m_pSDLOverlay, &rect );
    }
    //SDL_LockYUVOverlay ( m_pSDLOverlay );

    ////	int dst_pix_fmt = PIX_FMT_YUV420P;
    ////pict.data[0] = m_pSDLOverlay->pixels[0];
    ////pict.data[1] = m_pSDLOverlay->pixels[1];
    ////pict.data[2] = m_pSDLOverlay->pixels[2];

    ////pict.linesize[0] = m_pSDLOverlay->pitches[0];
    ////pict.linesize[1] = m_pSDLOverlay->pitches[1];
    ////pict.linesize[2] = m_pSDLOverlay->pitches[2];

    ////av_picture_copy(&pict, (const AVPicture *)avFrame, 
    ////	dst_pix_fmt, nWidth, nHeight );
    //for( int i=0; i<3; ++i )
    //{
    //    memcpy( m_pSDLOverlay->pixels[i], pic->data[i], pic->linesize[i] );
    //    m_pSDLOverlay->pitches[i] = (mu_uint16)pic->linesize[i];
    //}


    ///* update the bitmap content */
    //SDL_UnlockYUVOverlay( m_pSDLOverlay );

    return TRUE;
}

void CVideoWndDialog::OnRecordStatus( BOOL bSuccess, EMCU_ErrorCode er )
{
    if ( !bSuccess )
    {
        this->GetParent()->PostMessage( WM_RECORD_FAIL, er );
    }
}


