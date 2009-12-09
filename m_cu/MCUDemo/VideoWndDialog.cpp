// VideoWndDialog.cpp : ʵ���ļ�
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

// CVideoWndDialog �Ի���

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


// CVideoWndDialog ��Ϣ�������

BOOL CVideoWndDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���״̬��ʾ��
	m_staticVideoStatus.SetBkTransparent( FALSE, FALSE );
	m_staticVideoStatus.SetBkColor( RGB( 0,0,0 ) );
	m_staticVideoStatus.SetTextColor( RGB( 255,255,255 ), RGB( 128,128,128 ), FALSE );

	BOOL bResult;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//if ( !m_strRtspUrl.IsEmpty() )
	//{
	//	m_bCheckRtspTimeout = TRUE;
	//	m_bCheckRtspError = TRUE;
	//	m_MediaNet.OpenRTSP( m_strRtspUrl );
	//}


		
	

	bResult = this->InitSDL();
	if ( !bResult )
	{
		mcu::log << _T( "Init SDL fail! Maybe Only one video window is permit at one time������" ) << endl;
	}

    bResult = m_frameBuffer.Init( 10 );
    if ( !bResult )
    {
        mcu::log << _T( "Init frame buffer fail!!!!" ) << endl;
    }
    m_frameBuffer.SetFrameCallback( OnBufferdVideoFrameShowS, this );


	// ���ý���ص���
//	CDecoder::SetPictueCallback( &CVideoWndDialog::YUVCallbackS, this );

	// ������ʱ��.
//	m_dwStatusUpdateTimerId = this->SetTimer( 1, 500, NULL );


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ����ר�ô����/����û���
//	this->DestroySDL();

	CDialog::PostNcDestroy();
}

BOOL CVideoWndDialog::InitSDL()
{
	if ( SDL_WasInit( 0 ) )
	{
		return FALSE;
	}

	
	// ����SDL�̡߳�
	SDL_Thread  * pSdlThread = SDL_CreateThread( SDL_ThreadFunc, this );
	m_pSDLThread = pSdlThread;

	return ( 0 != m_pSDLThread );
}

BOOL CVideoWndDialog::DestroySDL()
{
	if ( m_pSDLThread )
	{
		// �����ص�������

		// ��ֹ���̳߳�ͻ���Ժ��Ż���
		SDL_Delay( 10 );

		// ����SDL��
		SDL_Event eventQuit;
		eventQuit.type = SDL_QUIT;
		SDL_PushEvent( &eventQuit );

		// �ȴ�SDL�߳��˳���
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
	// ʹ��Dib��ʽ��ʾ��
	SDL_putenv( "SDL_VIDEODRIVER=windib" );

	// ָ����ʾ����Ϊ�����ڡ�
	_ASSERT( hWnd );
	stringstream strWnd;
	strWnd << "SDL_WINDOWID=" << (int)hWnd;
	SDL_putenv( strWnd.str().c_str() );


	// ֻ��ʼ����Ƶ��ϵͳ��
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

	// ����ʾ���⡣
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

					//// д��,ȫ��ֻ�ܺ���.
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

				// SDL��û��˫����Ϣ������ģ��һ����				
				clock_t now = ::GetTickCount();
				
				clock_t timespan = now - pThis->m_timeLastMouseClick;
				if ( timespan < 1000 )
				{
					// ˫�������¿�ʼ��ʱ��
					pThis->m_timeLastMouseClick = 0;

					uMessage = WM_LBUTTONDBLCLK;
				}
				else
				{
					// �����������ڿ�ʼ��ʱ���ȴ��´ε�����
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

	//// ��ʼ���ź�����
	//g_pictq_mutex = SDL_CreateMutex();
	//g_pictq_cond = SDL_CreateCond();



	//	cur_stream = rtsp_open(input_filename, file_iformat);

	//	event_loop();
	//	return bResult;
}
BOOL CVideoWndDialog::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
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

	// TODO: �ڴ˴������Ϣ����������
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
//	// �������һ�λ���ͼ�ε�ʱ��.
//	m_timeLastDrawVideo = CTime::GetCurrentTime();
//
//	// SDL��û��������ʼ��.
//	if ( !m_pSDLSurface )
//	{
//		return;
//	}
//
//	// ����Ƿ���ͣ��.
//	if ( m_bPause )
//	{
//		return;
//	}
//
//	// ����Ƿ�Ҫ���� YUV Overlay��
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
//	// copyͼ��
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
//	// ��ʾ��
//	int nWidth = pFrameInfo->frameWidth;
//	int nHeight = pFrameInfo->frameHeight;
//	const int conBlackBorderOffset = -2;
//	SDL_Rect rect;
//
//	if ( nWidth * m_pSDLSurface->h > nHeight * m_rcVideoShow.w )
//	{
//		// ͼ���߱ȴ�����ʾ�����߱�,˵��Ҫ���տ�ȵ���.
//		rect.h = nHeight * m_rcVideoShow.w / nWidth ;
//		rect.w = m_pSDLSurface->w;
//	}
//	else
//	{
//		// ���ո߶ȵ���.
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
//	// �ж��Ƿ���Ҫ���´�����ͼ���档
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
//	//	// ���� overlay��
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
//	//// �����ݿ���overlay��
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

	// TODO: �ڴ˴������Ϣ����������
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

	// ״̬��ʾ�������.
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	//if( nIDEvent == m_dwStatusUpdateTimerId )
	//{
	//	// ˢ�º���Ѳ.
	//	this->StatusCheck();

	//	// ���ռ䡣
	//	this->StorageSpaceCheck();
	//}


	CDialog::OnTimer(nIDEvent);
}

//void CVideoWndDialog::StorageSpaceCheck()
//{
//	if ( this->IsRecoding() )
//	{
//		// �����̿ռ䡣
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
//	// ���RTSP�Ƿ���������.
//
//	// ����Ƿ�ﵽ�˷����������·����
//	if ( RTSPStatus_Error_Server_Full == eRtspStat && m_bCheckRtspError )
//	{
//		// �������ܾ��˿ͻ��˵�����ֱ�ӷ���ʧ�ܡ�
//		m_bCheckRtspError = FALSE;
//		this->GetParent()->SendMessage( WM_VIDEO_WND_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Server_Full, 0 );
//		return;
//	}
//
//	if ( m_bCheckRtspError )
//	{
//        switch( eRtspStat )
//        {
//        case RTSPStatus_Error_Unknown:		// δ֪����	
//        case RTSPStatus_Error_Init_Fail:	// ��ʼ��ʧ�ܡ�
//        case RTSPStatus_Error_Opition:		// Opition ����ʧ�ܡ�
//        case RTSPStatus_Error_Description:
//        case RTSPStatus_Error_Setup:
//        case RTSPStatus_Error_Play:		
//        case RTSPStatus_Error_SDP:			// ����sdp��Ϣ����
//        case RTSPStatus_Error_Create_Rcv:	// �������մ���ʧ��
//            // �������ܾ��˿ͻ��˵�����ֱ�ӷ���ʧ�ܡ�
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
//		// rtsp û�й���,����Ҫ�رնԻ���.
//		CTime now = CTime::GetCurrentTime();
//		CTimeSpan tIdleTime = now - m_timeLastWork;
//		if ( tIdleTime.GetTotalSeconds() > m_nTimeoutRTSPFail )
//		{
//			m_bCheckRtspTimeout = FALSE;
//			this->GetParent()->SendMessage( WM_VIDEO_WND_VIDEO_OPEN_FAIL, MCU_Error_Unknow, 0 );
//		}
//	}
//
//	// ��������Ƿ���������(ͨ������Ƿ����ͼ��.).
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
//	// ����״̬����ʾ.
//	// ֻ�з���������״̬ʱ����Ҫ��ʾ״̬��.
//	BOOL bWorkBad = ( RTSPStatus_Running != eRtspStat );	// RTSPû���ڹ���ʱ��Ҫ��ʾ.
//	BOOL bNoPacket = FALSE;
//	if( !bWorkBad )
//	{
//		CTime now = CTime::GetCurrentTime();
//		CTimeSpan noPacketTime = now - m_timeLastDrawVideo;
//		bNoPacket |= noPacketTime.GetTotalSeconds() > 5;	// һ��ʱ��û�еȵ�����.
//	}
//	bWorkBad |= bNoPacket;
//	
//	m_staticVideoStatus.ShowWindow( bWorkBad ? SW_SHOW : SW_HIDE );	// �������״̬����,��ʾ״̬��.
//
//	if( bWorkBad )
//	{
//		tstring strShowVideoStatus;
//		switch( eRtspStat )
//		{
//		case RTSPStatus_Idle:
//			strShowVideoStatus = _T( "����ֹͣ" );
//			break;
//		case RTSPStatus_Init:
//			strShowVideoStatus = _T( "��������ʼ��..." );
//			break;
//		case RTSPStatus_Opition:
//		case RTSPStatus_Description:
//			strShowVideoStatus = _T( "������Ƶ������..." );
//			break;
//		case RTSPStatus_Setup:
//		case RTSPStatus_Play:
//			strShowVideoStatus = _T( "����������Ƶ..." );
//			break;
//		case RTSPStatus_Running:
//			if ( bNoPacket )
//			{
//				strShowVideoStatus = _T( "�ȴ�����..." );
//			}
//			else
//			{
//				strShowVideoStatus = _T( "���ڲ���" );
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
//	// ������Ϣ.
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
//	// ���±����RTSP״̬.
//	this->m_eRtspStatus = eRtspStat;
//}

void CVideoWndDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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

	// SDL��û��������ʼ��.
	if ( !m_pSDLSurface )
	{
        mcu::log << _T( "SDL Surface not init when pic callback!" ) << endl;
		return;
	}

	// ����Ƿ�Ҫ���� YUV Overlay��
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

	// copyͼ��
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

	// ��ʾ��
	int nWidth = pFrameInfo->frameWidth;
	int nHeight = pFrameInfo->frameHeight;
	const int conBlackBorderOffset = -2;
	SDL_Rect rect;

	if ( nWidth * m_pSDLSurface->h > nHeight * m_prcVideoShow->w )
	{
		// ͼ���߱ȴ�����ʾ�����߱�,˵��Ҫ���տ�ȵ���.
		rect.h = nHeight * m_prcVideoShow->w / nWidth ;
		rect.w = m_pSDLSurface->w;
	}
	else
	{
		// ���ո߶ȵ���.
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
        strShowVideoStatus = _T( "����ֹͣ" );
    	break;
    case RTSPStatus_Init:
        strShowVideoStatus = _T( "��������ʼ��..." );
    	break;
    case RTSPStatus_Opition:
        strShowVideoStatus = _T( "������Ƶ������..." );
        break;
    case RTSPStatus_Description:
        strShowVideoStatus = _T( "������Ƶ������..." );
        break;
    case RTSPStatus_Setup:
        strShowVideoStatus = _T( "����������Ƶ..." );
    	break;
    case RTSPStatus_Play:
        strShowVideoStatus = _T( "�������󲥷�..." );
    	break;
    case RTSPStatus_Running:
        strShowVideoStatus = _T( "���ڲ���" );
        bShowRtspStatus = FALSE;                    // �������ŵ�ʱ����ʾ��ʾ��
        break;
    case RTSPStatus_WaitingPacket:
        strShowVideoStatus = _T( "�ȴ�����..." );
        break;
    case RTSPStatus_Error_Server_Full:
        strShowVideoStatus = _T( "��������ת����������" );
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
        strShowVideoStatus = _T( "���Ŵ���" );
        break;
    case RTSPStatus_Error_Decoder_Fail:
        this->GetParent()->PostMessage( WM_VIDEO_OPEN_FAIL, MCU_Error_Decoder_Null, 0 );
        strShowVideoStatus = _T( "����������ʧ��" );
        break;
    default:
        break;
    }

    if ( bShowRtspStatus )
    {
        m_staticVideoStatus.SetWindowText( strShowVideoStatus.c_str() );
    }
    m_staticVideoStatus.ShowWindow( bShowRtspStatus ? SW_SHOW : SW_HIDE );	// �������״̬����,��ʾ״̬��.
  
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


