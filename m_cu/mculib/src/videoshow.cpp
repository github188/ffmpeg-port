#include "videoshow.h"
#include "log.h"
#ifdef _WIN32_WCE
#include "SDL.h"
#else
#include <SDL/SDL.h>
#endif
#include "message.h"
#include "mcucommon.h"

CVideoShowWnd::CVideoShowWnd(void)
{
	this->m_pSDLSurface = NULL;
	this->m_pSDLOverlay = NULL;
	this->m_pCbVideoEvent = NULL;
	m_pSDLThread = NULL;

	m_pDisplayTimerId = NULL;
	m_nDisplayTime = 40;		// Ĭ����25֡�ġ�

	m_nWriteCursor = 0;
	m_nReadCursor = 0;

	m_u64LastTimeStamp = 0;
	m_u64LastDisplayFrameTimeStamp = 0;

	mcu::RegisterMessageCallback( this, OnMessageCallback );
}

CVideoShowWnd::~CVideoShowWnd(void)
{
	SCOPE_LOCK( m_threadSafeLock );

	this->Destroy();

	mcu::UnregisterMessageCallback( this );

	// ����ͼ�񻺳�����ͼƬ������ڴ档
	for ( int i=0; i<PicBufferSize; ++i )
	{
		this->m_arPicBuffer[i].m_tPic.Free();
	}
}

BOOL CVideoShowWnd::Create( int nXpos, int nYpos, int nWidth, int nHeight, BOOL bFullScreen )
{

	//SCOPE_LOCK( m_threadSafeLock );

	return this->SetRect( nXpos, nYpos, nWidth, nHeight, bFullScreen );
}

BOOL CVideoShowWnd::SetRect( int nXpos, int nYpos, int nWidth, int nHeight, BOOL bFullScreen )
{
	// ����Ϣ����ʵ�֣��������߳�ת������������
	// ���˴洢�����⣬����Ҫ�����������������

	mcu::tlog << _T( "Vidw Show Set Rect. x: " ) << nXpos << _T( " y: " ) << nYpos
		<< _T( "w: " ) << nWidth << _T( "h: " ) << nHeight << _T( "full: " ) << bFullScreen << endl;

	// ֻ�ڱ������ʱ������
	{
		SCOPE_LOCK( m_threadSafeLock );
		m_nXpos = nXpos;
		m_nYpos = nYpos;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_bFullscreen = bFullScreen;
	}
	
	BOOL bResult = mcu::SendMsg( this, WM_VIDEO_CREATE, 0, 0 );
	return bResult;


	//this->DestroyNotQuit();
	
}

BOOL CVideoShowWnd::CreateThreadSafe()
{
	SCOPE_LOCK( m_threadSafeLock );

	this->DestroyThreadSafe();

	// ֻ��ʼ����Ƶ��ϵͳ��
	int initFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
	if( initFlags != SDL_WasInit( initFlags ) )
	{		
		if ( SDL_Init ( initFlags ) ) 
		{
			mcu::tlog << _T( "Could not initialize SDL ec: " ) << SDL_GetError() << endl;
			_ASSERT( FALSE );
		}
		else
		{
			mcu::tlog << _T( "SDL Init!" ) << endl;
		}
	}	

	// ����SDL���õĻ���������������ʾ��
	int nXpos = this->m_nXpos;
	int nYpos = this->m_nYpos;
	int nWidth = this->m_nWidth;
	int nHeight = this->m_nHeight;
	BOOL bFullScreen = this->m_bFullscreen;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// δ֪ԭ��������������黺�棡
	char szPosEnv[256] = {0};
	stringstream ssEnvWindowPos;
	ssEnvWindowPos << "SDL_VIDEO_WINDOW_POS=" << nXpos << "," << nYpos ;
	strcpy( szPosEnv, ssEnvWindowPos.str().c_str() );
	int nResult = SDL_putenv( szPosEnv );
	mcu::tlog << "sdl set env: " << szPosEnv  << "result: " << nResult << endl;

	//	SDL_putenv( "SDL_VIDEO_WINDOW_POS=0,50" );
	//	SDL_putenv( ( "SDL_VIDEO_WINDOW_POS=200,100" ) );
	mcu::tlog << "new set env2" << endl;

	SDL_WM_SetCaption("M_CU", "mcu" );



	char szSDLDrivers[1000] = {0};
	SDL_VideoDriverName( szSDLDrivers, 1000 );
	mcu::tlog << "sdl driver: " << szSDLDrivers << endl;

	//SDL_VIDEODRIVER

	int videoFlags = SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL;
	//	int w,h;

	if( bFullScreen ) 
		videoFlags |= SDL_FULLSCREEN;
	else               
		videoFlags |= SDL_RESIZABLE;

	// ����ʾ���⡣
	videoFlags |= SDL_NOFRAME;

	m_pSDLSurface = SDL_SetVideoMode( nWidth, nHeight, 0, videoFlags );

	if( NULL == m_pSDLThread )
	{
		m_pSDLThread = SDL_CreateThread( SDL_ThreadFunc, this );
	}

	// ������ʾ��ʱ����
	if ( NULL == m_pDisplayTimerId )
	{
		m_pDisplayTimerId = SDL_AddTimer( m_nDisplayTime, OnDisplayTimerCallback, this );
	}

	return ( NULL != m_pSDLSurface );
}

BOOL CVideoShowWnd::Destroy()
{
	// ʹ����Ϣ���ƣ������߳�ת������������
	// ����Ҫ�������������ܻ�������
	BOOL bResult = mcu::SendMsg( this, WM_VIDEO_DESTROY, 0, 0 );
	return bResult;


}

BOOL CVideoShowWnd::DestroyThreadSafe()
{
	SCOPE_LOCK( m_threadSafeLock );

	// ������ʾ��ʱ����
	if ( m_pDisplayTimerId )
	{
		SDL_RemoveTimer( m_pDisplayTimerId );
		m_pDisplayTimerId = NULL;
	}

	if ( m_pSDLThread )
	{
		// ����SDL��
		SDL_Event eventQuit;
		eventQuit.type = SDL_QUIT;
		SDL_PushEvent( &eventQuit );


		// �ȴ�SDL�߳��˳���
		int nStatus;
		SDL_WaitThread( m_pSDLThread, &nStatus );
		this->m_pSDLThread = NULL;
	}

//	this->DestroyNotQuit();
//	mcu::tlog << _T( "DestroyNotQuit Vidwo show !" ) << endl;

	//	this->m_pCbVideoEvent = NULL;
	//	this->m_uUserData = NULL;
	if ( this->m_pSDLOverlay )
	{
		SDL_FreeYUVOverlay( m_pSDLOverlay );
		m_pSDLOverlay = NULL;
	}

	if ( this->m_pSDLSurface )
	{
		SDL_FreeSurface( m_pSDLSurface );
		m_pSDLSurface = NULL;
	}


	SDL_Quit();

	// �������е����ݡ�
	// ֻ��Ҫ����һ�±�־�Ϳ����ˡ�
	for ( int i=0; i<PicBufferSize; ++i )
	{
		this->m_arPicBuffer[i].m_bFillData = FALSE;
	}
	m_nReadCursor = 0;
	m_nWriteCursor = 0;

	return TRUE;
}

//BOOL CVideoShowWnd::DestroyNotQuit()
//{
//	SCOPE_LOCK( m_threadSafeLock );
//
//	mcu::tlog << _T( "DestroyNotQuit Vidwo show !" ) << endl;
//
//	//	this->m_pCbVideoEvent = NULL;
//	//	this->m_uUserData = NULL;
//	if ( this->m_pSDLOverlay )
//	{
//		SDL_FreeYUVOverlay( m_pSDLOverlay );
//		m_pSDLOverlay = NULL;
//	}
//
//	if ( this->m_pSDLSurface )
//	{
//		SDL_FreeSurface( m_pSDLSurface );
//		m_pSDLSurface = NULL;
//	}
//
//
//	return TRUE;
//}

BOOL CVideoShowWnd::DisplayAsync( const CBaseCodec::TVideoPicture& tPic, const CBaseCodec::TVideoFrameInfo& tFrameInfo )
{
	SCOPE_LOCK( m_threadSafeLock );

	// ����ͼ��֡�е���Ϣ���¶�ʱ����
	if( this->m_u64LastTimeStamp != 0 )
	{
		int nNewTime = tFrameInfo.frameTimeStamp - m_u64LastTimeStamp;
		nNewTime /= 1000;	// ��Ƶ֡ʱ����ĵ�λ�� microsecond�� 1/1000000 s��

//			mcu::tlog << _T( "DisplayAsync frame time span: " ) << nNewTime 
//				<< _T( " frame time: " ) << (mu_uint32)tFrameInfo.frameTimeStamp << endl;
		
		// ��ʱ����ȡ����һ����ʱ�����Խ���ʱ������СһЩ���ö�ʱ�����Ŀ�һЩ��
		nNewTime -= 10;

		if ( nNewTime != m_nDisplayTime )
		{
			if ( nNewTime < 5 || nNewTime > 500 )
			{
				mcu::tlog << _T( "DisplayAsync Invalid nNewTime :" ) << nNewTime << endl;
			}
			else
			{
				m_nDisplayTime = nNewTime;
				mcu::tlog << _T( "DisplayAsync Timer change to " ) << m_nDisplayTime << endl;
			}
			
		}
	}
	this->m_u64LastTimeStamp = tFrameInfo.frameTimeStamp;

	

	// �ж��ǲ�����Ҫ���ǻ�û�ж�ȡ��ʾ��ͼƬ���ݡ�
	// ����ǣ�����ʾ�Ķ��α�ǰ�Ƶ���ɵ������ϡ�
	if( this->m_nReadCursor == this->m_nWriteCursor )
	{
		int nNextPos = this->m_nReadCursor + 1;
		if ( nNextPos >= PicBufferSize )
		{
			nNextPos -= PicBufferSize;
		}
		if ( m_arPicBuffer[ nNextPos ].m_bFillData )
		{
			m_nReadCursor = nNextPos;
			if( PicBufferDebug )
			{
				mcu::tlog << _T( "Write faster, read slow. write change read cursor to " ) << m_nReadCursor << endl;
			}			
		}
	}

	// ��ͼ������copy��ͼƬ���塣
	int nWriteIndex = this->m_nWriteCursor;
	this->m_arPicBuffer[ nWriteIndex ].m_tFrameInfo = tFrameInfo;
	BOOL bResult = this->m_arPicBuffer[ nWriteIndex ].m_tPic.Copy( tPic );
	if ( !bResult )
	{
		mcu::tlog << _T( "DisplayAsync copy pic failed!" ) << endl;
	}
	this->m_arPicBuffer[ nWriteIndex ].m_bFillData = bResult;
	
	if ( bResult )
	{
		this->m_nWriteCursor ++;
		if ( m_nWriteCursor >= PicBufferSize )
		{
			m_nWriteCursor -= PicBufferSize;
		}

//		mcu::tlog << _T( "write cursor move to" ) << m_nWriteCursor << endl;
	}
	return bResult;


}

void CVideoShowWnd::SetEventCallback( CB_VideoWndEvent pFun, mu_uint32 uUserData )
{
	SCOPE_LOCK( this->m_threadSafeLock );

	this->m_uUserData = uUserData;
	this->m_pCbVideoEvent = pFun;
}

int CVideoShowWnd::SDL_ThreadFunc( void *param )
{
	mcu::tlog << _T( "video show wnd thread run! " ) << endl;
	CVideoShowWnd *pThis = (CVideoShowWnd*)param;
	if ( NULL == pThis )
	{
		return 0;
	}

	SDL_Event sdlevent;

	bool bRun = true;
	while( bRun ) 
	{
		SDL_WaitEvent(&sdlevent);
		mcu::tlog << _T( "sdl event : " ) << (int)sdlevent.type << endl;

		switch( sdlevent.type ) 
		{
		case SDL_KEYDOWN:
			//switch( sdlevent.key.keysym.sym) 
			//{

			//case SDLK_LEFT:
			//	mcu::tlog << _T( "left" ) << endl;
			//	break; 
			//case SDLK_RIGHT:
			//	mcu::tlog << _T( "right" ) << endl;
			//	break;
			//case SDLK_UP:
			//	mcu::tlog << _T( "up" ) << endl;
			//	break;
			//case SDLK_DOWN:
			//	mcu::tlog << _T( "down" ) << endl;
			//	break;			            
			//default:
			//	break;
			//}
			break;

		case SDL_VIDEORESIZE:
			{
	
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				//if ( pThis->m_pCbVideoEvent )
				//{
				//	try
				//	{
				//		pThis->m_pCbVideoEvent( EventMouseButtonDown, pThis->m_uUserData );
				//	}
				//	catch( ... )
				//	{
				//		mcu::tlog << _T( "WM_VIDEO_SHOW_CLICK m_pCbVideoEvent crash!!" ) << endl;
				//	}
				//}
				mcu::PostMsg( pThis, WM_VIDEO_SHOW_CLICK, NULL, NULL );

	//			UINT uMessage;

				// SDL��û��˫����Ϣ������ģ��һ����				
	//			clock_t now = ::GetTickCount();

				//clock_t timespan = now - pThis->m_timeLastMouseClick;
				//if ( timespan < 1000 )
				//{
				//	// ˫�������¿�ʼ��ʱ��
				//	pThis->m_timeLastMouseClick = 0;

				//	uMessage = WM_LBUTTONDBLCLK;
				//}
				//else
				//{
				//	// �����������ڿ�ʼ��ʱ���ȴ��´ε�����
				//	pThis->m_timeLastMouseClick = now;

				//	uMessage = WM_LBUTTONDOWN;
				//}
				
	
			}
			break;
		case SDL_QUIT:
			bRun = false;
			break;

		default:
			break;
		}
	}

//	mcu::tlog << _T( "SDL_ThreadFunc sdl thread exit! " ) << endl;
	return 0;
}

//BOOL CVideoShowWnd::ShowWindow( BOOL bShow )
//{
//	if ( !m_pSDLSurface )
//	{
//		return FALSE;
//	}
//
//	if ( bShow )
//	{
//		
//	}
//	else
//	{
//		m_nSurfaceWidth = m_pSDLSurface->w;
//		m_nSurfaceHeight = m_pSDLSurface->h;
//		this->m_bSurfaceFullScreen = m_pSDLSurface->flags & SDL_FULLSCREEN;
//
//		this->SetRect( 0, 0, 0, 0, FALSE );
//	}
//
//	return FALSE;
//}

mu_int32 CVideoShowWnd::OnMessageCallback( const mcu::TMsg& tMsg )
{
	CVideoShowWnd *pThis = (CVideoShowWnd*)tMsg.m_MessageTarget;
	if ( NULL == pThis )
	{
		mcu::tlog << "OnMessageCallback this = 0! msg: " << tMsg.m_nMessage << endl;
		return 0;
	}

	switch( tMsg.m_nMessage )
	{
	case WM_VIDEO_SHOW_CLICK:			
		if ( pThis && pThis->m_pCbVideoEvent )
		{
			try
			{
				pThis->m_pCbVideoEvent( EventMouseButtonDown, pThis->m_uUserData );
			}
			catch( ... )
			{
				mcu::tlog << _T( "WM_VIDEO_SHOW_CLICK m_pCbVideoEvent crash!!" ) << endl;
			}
		}
		break;
	case WM_VIDEO_CREATE:
		if ( pThis )
		{
			pThis->CreateThreadSafe();
		}
	    break;
	case WM_VIDEO_DESTROY:
		if ( pThis )
		{
			pThis->DestroyThreadSafe();
		}
		break;
	default:
	    break;
	}
	
	return 0;
}

unsigned int CVideoShowWnd::OnDisplayTimerCallback(unsigned int interval, void *param)
{
	CVideoShowWnd *pThis = ( CVideoShowWnd * )param;
	if ( pThis )
	{
		mu_uint64 u64TimeStamp = pThis->DisplayFromPicBuffer();

		//if ( 0 != u64TimeStamp )
		//{
		//	int nTimeSpan = u64TimeStamp - pThis->m_u64LastDisplayFrameTimeStamp;
		//	nTimeSpan /= 1000;

		//	pThis->m_u64LastDisplayFrameTimeStamp = u64TimeStamp;

		//	if ( nTimeSpan > 10 && nTimeSpan < 500 )
		//	{
		//		mcu::tlog << _T( "timer: " ) << nTimeSpan << endl;
		//		return nTimeSpan;
		//	}
		//}

//		mcu::tlog << _T( "interval" ) << interval << _T( "m_nDisplayTime: " )<< pThis->m_nDisplayTime << endl;

		// ���ص�ǰ����ʾ���ʱ�䣬��ʱ�����Զ��������ʱ�����´εĶ�ʱ��
		return pThis->m_nDisplayTime;
	}
	return interval;
}

mu_uint64 CVideoShowWnd::DisplayFromPicBuffer()
{
	SCOPE_LOCK( m_threadSafeLock );

	// �ӻ����ж�ȡͼƬ����
	CBaseCodec::TVideoPicture *pPic = NULL;
	CBaseCodec::TVideoFrameInfo *pFrame = NULL;

	int nReadCur = this->m_nReadCursor;
	if ( this->m_arPicBuffer[nReadCur].m_bFillData )
	{
		pPic = &(this->m_arPicBuffer[ nReadCur ].m_tPic);
		pFrame = &( this->m_arPicBuffer[ nReadCur ].m_tFrameInfo );

//		mcu::tlog << _T( "read pic cursor: " ) << nReadCur << endl;

		this->m_arPicBuffer[ nReadCur ].m_bFillData = FALSE;
		// move to next.
		this->m_nReadCursor ++;
		if ( m_nReadCursor >= PicBufferSize )
		{
			m_nReadCursor -= PicBufferSize;
		}
	}

	if ( NULL == pPic || NULL == pFrame )
	{
		if( PicBufferDebug )
		{
			mcu::tlog << _T( "DisplayFromPicBuffer has no pic! " ) << endl;
		}		
		return 0;
	}


	int nPicWidth = pPic->picWidth;
	int nPicHeight = pPic->picHeight;

	// �������һ�λ���ͼ�ε�ʱ��.
	//	m_timeLastDrawVideo = CTime::GetCurrentTime();

	// SDL��û��������ʼ��.
	if ( !m_pSDLSurface )
	{
		mcu::tlog << _T( "Vidow show wnd display: Surface didn't inited! " ) << endl;
		return pFrame->frameTimeStamp;
	}


	// ����Ƿ�Ҫ���� YUV Overlay��
	if ( !m_pSDLOverlay 
		|| m_pSDLOverlay->w != nPicWidth
		|| m_pSDLOverlay->h != nPicHeight )
	{
		if ( m_pSDLOverlay )
		{
			SDL_FreeYUVOverlay( m_pSDLOverlay );
		}
		m_pSDLOverlay = SDL_CreateYUVOverlay( nPicWidth, nPicHeight, SDL_IYUV_OVERLAY, m_pSDLSurface );

		mcu::tlog << _T( "Recreate the yuv overlay! " ) << endl;
	}

	// copyͼ��
	SDL_LockYUVOverlay ( m_pSDLOverlay );

	for( int i=0; i<3; ++i )
	{
		memcpy( m_pSDLOverlay->pixels[i], pPic->data[i], pPic->linesize[i] );
		m_pSDLOverlay->pitches[i] = pPic->linesize[i];
	}

	/* update the bitmap content */
	SDL_UnlockYUVOverlay( m_pSDLOverlay );

	// ��ʾ��
	int nWidth = pFrame->frameWidth;
	int nHeight = pFrame->frameHeight;
	const int conBlackBorderOffset = -2;
	SDL_Rect rect;

	if ( nWidth * m_pSDLSurface->h > nHeight * m_pSDLSurface->w )
	{
		// ͼ���߱ȴ�����ʾ�����߱�,˵��Ҫ���տ�ȵ���.
		rect.h = nHeight * m_pSDLSurface->w / nWidth ;
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

	//	mcu::tlog << _T( "SDL_DisplayYUVOverlay time : " ) << GetTick() << endl;

	SDL_DisplayYUVOverlay( m_pSDLOverlay, &rect );

	//	mcu::tlog << _T( "after SDL_DisplayYUVOverlay time : " ) << GetTick() << endl;
	return pFrame->frameTimeStamp;
}




