#include "virtualplayerwnd.h"
#include "mcusession.h"
#include "mcuconfig.h"
#include "mcucommon.h"
#include "mcuthread.h"

CVirtualPlayerWnd::CVirtualPlayerWnd(void)
{
	m_bPause = FALSE;
	m_pStatusCheckThread = FALSE;
	m_bStatusCheckThreadRun = FALSE;
	m_eLastRtspStatus = RTSPStatus_Idle;
	m_timeLastPic = 0;
	m_bCheckStatus = FALSE;
    m_bCheckDiskSpace = FALSE;
	m_bFullScreenMode = FALSE;


	mcu::RegisterMessageCallback( this, OnMessage );

	BOOL bResult = m_frameBuffer.Init( 10 );
	
	if ( !bResult )
	{
		Log() << _T( "Init frame buffer fail!!!!" ) << endl;
	}
	m_frameBuffer.SetFrameCallback( OnBufferdVideoFrameShowS, this );
}

CVirtualPlayerWnd::~CVirtualPlayerWnd(void)
{
    // 不能加锁，否则等待退出线程时会死锁。
//	SCOPE_LOCK( m_threadSafeLock );

	// 停止监测。
	this->StopCheckThread();

	mcu::UnregisterMessageCallback( this );

	m_tVideoRollPicBuf.Free();
}

BOOL CVirtualPlayerWnd::StartPlay( CVideoSession *pVideoSession, EMCU_ErrorCode& pErrorCode )
{
	SCOPE_LOCK( m_threadSafeLock );

	// 不管如何，开始监测状态。反正不会重复开始。
	this->StartCheckThread();


    // 暂停改成False。
    this->m_bPause = FALSE;

	BOOL bResult = FALSE;
	if( pVideoSession )
	{
		CDecoder::SetPictueCallback( (CVirtualPlayerWnd::YUVCallbackS), this );
		bResult = this->m_MediaNet.OpenRTSP( pVideoSession->RtspUrl().c_str() );

		this->m_ptzControl.SetPTZAddr( pVideoSession->PtzIP().c_str(), pVideoSession->PtzPort() );
		this->m_ptzControl.SetPuId( pVideoSession->PuId() );
		this->m_ptzControl.SetUserId( CMCUSession::Instance()->UserId() );

		pErrorCode = MCU_Sucess;
	}
	else
	{
		pErrorCode = MCU_Error_Unknow;
		Log() << _T( "player wnd startplay() fail! video session 0! " ) << endl;
	}

	// 是否开始监测。
	m_bCheckStatus = bResult;

	return bResult;
}

BOOL CVirtualPlayerWnd::PausePlay( BOOL bPause )
{
	SCOPE_LOCK( m_threadSafeLock );

	// 服务器不支持暂停，直接停止。
	if ( m_bPause == bPause )
	{
		Log() << _T( "CVirtualPlayerWnd::PausePlay already pause/restart!" ) << endl;
		return TRUE;
	}
	m_bPause = bPause;

	BOOL bResult = FALSE;
	if ( bPause )
	{
		bResult = this->StopPlay();
	}
	else
	{
		EMCU_ErrorCode eErrorCode;
		bResult = this->StartPlay( CMCUSession::Instance()->CurVideoSession(), eErrorCode );
	}
	
	return bResult;
}

BOOL CVirtualPlayerWnd::IsPause() const
{
	SCOPE_LOCK( m_threadSafeLock );

	return m_bPause;
}

BOOL CVirtualPlayerWnd::StopPlay()
{
	SCOPE_LOCK( m_threadSafeLock );

    if ( this->IsRecording() )
    {
        EMCU_ErrorCode er;
        BOOL bResult = this->StopRecord( er );

        mcu::PostMsg( this, WM_RECORD_FAIL, MCU_Error_PlayStop, 0 );

        Log() << _T( "Stop play when recording post WM_RECORD_FAIL msg.. stop record ret:" ) << bResult << _T( " er: " ) << er << endl;
    }

	Log() << _T( "CVirtualPlayerWnd::StopPlay" ) << endl;
	this->m_MediaNet.CloseRTSP();

    this->StopCheckThread();
    Log() << _T( "Stop Check status.!" ) << endl;
    
    return TRUE;
}


BOOL CVirtualPlayerWnd::YUVCallbackS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *param )
{

	CVirtualPlayerWnd *pThis = ( CVirtualPlayerWnd * )param;
	if ( pThis )
	{
        // 单元测试。测试获取不到图像时候的表现。
#define UNIT_TEST_STATUS_CHECK 0
#if     UNIT_TEST_STATUS_CHECK
        static int s_count = 0;
        if( s_count > 199 )
        {
            s_count = 0;
        }
        if ( s_count > 100 )
        {
            return FALSE;
        }
        s_count ++;

#endif

        // 记录最近一次的图像回调时间。
        pThis->m_timeLastPic = ::GetCurTime();

// 		pThis->OnVideoPicture( pic, pFrameInfo );
		pThis->YUVCallback( pic, pFrameInfo );
	}
	return TRUE;
}

BOOL CVirtualPlayerWnd::YUVCallback( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo )
{
	// 不需要加锁！因为frameBuffer里面会做多线程同步处理。
	// 加锁后反而会造成frameBuffer回调时死锁！
	//    SCOPE_LOCK( m_threadSafeLock );

	this->m_frameBuffer.InputFrame( *pic, *pFrameInfo );
	return TRUE;
}

BOOL CVirtualPlayerWnd::SendPtzCmd( EPTZCmdId eCmd )
{
	SCOPE_LOCK( m_threadSafeLock );

	BOOL bHasPtzPower =	CMCUSession::Instance()->CurVideoSession()->PtzControl();
	if ( !bHasPtzPower )
	{
		Log() << _T( "Have no ptz right. can't control ptz." ) << endl;
		return FALSE;
	}

	BOOL bPtz = TRUE;
	switch( eCmd )
	{
	case PTZ_MOVEUP:
	case PTZ_MOVEDOWN:
	case PTZ_MOVELEFT:
	case PTZ_MOVERIGHT:
	case PTZ_STOP:
		bPtz = TRUE;
		break;
	case PTZ_BRIGHTUP:
	case PTZ_BRIGHTDOWN:
	case PTZ_ZOOMTELE:
	case PTZ_ZOOMWIDE:
	case PTZ_FOCUSNEAR:
	case PTZ_FOCUSFAR:
		bPtz = FALSE;
		break;
	default:
		Log() << _T( "Ptz Control Don't suport the cmd: " ) << eCmd << endl;
	    break;
	}

	int nSpeed;
	if ( bPtz )
	{
		CConfig::Instance()->GetPtzSpeed( nSpeed );
	}
	else
	{
		CConfig::Instance()->GetLenSpeed( nSpeed );
	}

	this->m_ptzControl.SetPtzCmdId( eCmd );
	this->m_ptzControl.SetParam( nSpeed, CMCUSession::Instance()->CurVideoSession()->StreamType() );

	this->m_ptzControl.SendPTZCmd();

	return TRUE;
}

BOOL CVirtualPlayerWnd::IsDigitalPtz()const
{
	SCOPE_LOCK( m_threadSafeLock );

	return this->m_ptzControl.IsDigitalPtz();
}

BOOL CVirtualPlayerWnd::HasPtzPower() const
{
	SCOPE_LOCK( m_threadSafeLock );

	return CMCUSession::Instance()->CurVideoSession()->PtzControl();
}

BOOL CVirtualPlayerWnd::SetDigitalPtz( BOOL bDigital )
{
	SCOPE_LOCK( m_threadSafeLock );

	this->m_ptzControl.SetDigitPtz( bDigital );
	return TRUE;
}


BOOL CVirtualPlayerWnd::Capture( tstring& strPicPath, EMCU_ErrorCode& eErrorCode )
{
	SCOPE_LOCK( m_threadSafeLock );

    strPicPath = this->GetFileName( _T( "jpg" ) );

	BOOL bResult = this->Capture( strPicPath.c_str(), eErrorCode );    


	Log() << _T( "Capture file: " ) << strPicPath << "result" << bResult  << endl;
	return bResult;
}

BOOL CVirtualPlayerWnd::Capture( LPCTSTR strPicPath, EMCU_ErrorCode& eErrorCode )
{
	SCOPE_LOCK( m_threadSafeLock );

	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if( pDec )
	{	
		BOOL bResult = pDec->CapturePic( strPicPath );
		return bResult;
	}
	else
	{
		eErrorCode = MCU_Error_Decoder_Null;
		return FALSE;
	}
}

void CVirtualPlayerWnd::StartCheckThread()
{
	SCOPE_LOCK( m_threadSafeLock );

	m_bStatusCheckThreadRun = TRUE;
	this->m_bCheckStatus = TRUE;
	if ( NULL == m_pStatusCheckThread )
	{		
		m_pStatusCheckThread = SDL_CreateThread( CheckThread, this );
	}	
}

void CVirtualPlayerWnd::StopCheckThread()
{
    // 这里不能加锁，否则会造成锁死！！！
//	SCOPE_LOCK( m_threadSafeLock );

	if ( m_pStatusCheckThread )
	{
		m_bCheckStatus = FALSE;
		m_bStatusCheckThreadRun = FALSE;
		int nStatus;
		SDL_WaitThread( m_pStatusCheckThread, &nStatus );

		m_pStatusCheckThread = NULL;

	}
}

int CVirtualPlayerWnd::CheckThread( void * pParam )
{
	Log() << _T( "CVirtualPlayerWnd CheckThread run!" ) << endl;
	CVirtualPlayerWnd *pThis = ( CVirtualPlayerWnd*) pParam;
	if ( pThis )
	{
		while( pThis->GetCheckThreadSwitch() )
		{
			pThis->CheckStatsu();

            pThis->CheckDiskSpace();

			SDL_Delay( 200 );
		}	
		
	}

	Log() << _T( "CVirtualPlayerWnd CheckThread exit!" ) << endl;
	return 0;

}

BOOL CVirtualPlayerWnd::GetCheckThreadSwitch() const
{
	SCOPE_LOCK( m_threadSafeLock );
	return m_bStatusCheckThreadRun;
}

void CVirtualPlayerWnd::CheckStatsu()
{
	SCOPE_LOCK( m_threadSafeLock );

	if ( !m_bCheckStatus )
	{
		return;
	}

    EMCU_ErrorCode eErrorCode = MCU_Sucess;
	ERTSPStatus eStatus = m_MediaNet.GetRtspStatus();
	switch( eStatus )
	{
	case RTSPStatus_Error_Unknown:		// 未知错误。	
	case RTSPStatus_Error_Init_Fail:	// 初始化失败。
	case RTSPStatus_Error_Opition:		// Opition 命令失败。
	case RTSPStatus_Error_Description:
	case RTSPStatus_Error_Setup:
	case RTSPStatus_Error_Play:		
	case RTSPStatus_Error_SDP:			// 解析sdp信息出错。
	case RTSPStatus_Error_Create_Rcv:	// 码流接收创建失败
    case RTSPStatus_Error_Decoder_Fail:
//		mcu::PostMsg( this, WM_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Fail, eStatus );
        eErrorCode = MCU_Error_Rtsp_Fail;
		m_bCheckStatus = FALSE;
		break;

	case RTSPStatus_Error_Server_Full:	// 服务器达到了最大能力
//		mcu::PostMsg( this, WM_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Server_Full, eStatus );
        eErrorCode = MCU_Error_Rtsp_Server_Full;
		m_bCheckStatus = FALSE;
		break;

    case RTSPStatus_Running:
        if ( this->m_eLastRtspStatus != eStatus 
            && this->m_eLastRtspStatus != RTSPStatus_WaitingPacket )
        {
            // 如果当前状态变为正常时，记录时间，以便开始计时。
            Log() << _T( "The media stream detecter reset the net timeout timer." ) << endl;
            this->m_timeLastPic = ::GetCurTime();
        }
        break;
	default:
        
	    break;
	}
	
    // 对码流时间进行检测
    if( RTSPStatus_Running == this->m_eLastRtspStatus || RTSPStatus_WaitingPacket == this->m_eLastRtspStatus )
    {
        __time64_t tNow = ::GetCurTime();
        const __time64_t timeoutNoPacket = 20;  // 超过这个时间就判定码流不来。
        const __time64_t timeoutWaitPacket = 10;// 超过这个时间就判定开始等待码流。
        __time64_t timeSpan = tNow - this->m_timeLastPic;

        if ( timeSpan > timeoutNoPacket && RTSPStatus_WaitingPacket == this->m_eLastRtspStatus )
        {
            Log() << _T( "Time out for waiting packet. stop the play!" ) << endl;
            eStatus = RTSPStatus_Error_WaitPacket;
            //           mcu::PostMsg( this, WM_VIDEO_PLAY_STATUS, MCU_Error_Rtsp_Fail, eStatus );
            eErrorCode = MCU_Error_Rtsp_Fail;
            // 停止。
            this->m_bCheckStatus = FALSE;
        }
        else if( timeSpan > timeoutWaitPacket  )
        {
            Log() << _T( "Time out for packet .start to wait packet." ) << endl;
            eStatus = RTSPStatus_WaitingPacket;
            
 //           mcu::PostMsg( this, WM_VIDEO_PLAY_STATUS, 0, eStatus );

        }
        
    }

    if ( this->m_eLastRtspStatus != eStatus )
    {
        // 通知。
        mcu::PostMsg( this, WM_VIDEO_PLAY_STATUS, eErrorCode, eStatus );

    }
    
    m_eLastRtspStatus = eStatus;
}

mu_int32 CVirtualPlayerWnd::OnMessage( const mcu::TMsg& tMsg )
{
	CVirtualPlayerWnd * pThis = (CVirtualPlayerWnd*)tMsg.m_MessageTarget;
	if ( pThis )
	{
		switch( tMsg.m_nMessage )
		{
		case WM_VIDEO_OPEN_FAIL:
		case WM_VIDEO_PLAY_STATUS:
			pThis->OnRtspStatus( (ERTSPStatus)tMsg.m_lParam, (EMCU_ErrorCode)tMsg.m_wParam );
			break;	
        case WM_RECORD_FAIL:
            pThis->OnRecordStatus( FALSE, (EMCU_ErrorCode)tMsg.m_wParam );
		
		default:
		    break;
		}
		
	}
	return 0;
}

BOOL CVirtualPlayerWnd::StartRecord( LPCTSTR strRecPath, EMCU_ErrorCode& eErrorCode )
{
 //   Log() << _T( "Start Record Has no code" ) << endl;
    if ( this->IsRecording() )
    {
        Log() << _T( "StartRecord called twice!!!!" ) << endl;
        return TRUE;
    }

	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if( pDec )
	{
		BOOL bResult = pDec->StartRecord( strRecPath );

        // 是否开始检测磁盘情况。
        m_bCheckDiskSpace = bResult;

        return bResult;

	}
	else
	{
		return FALSE;
	}	

//    Log() << _T( "start record " ) << strRecPath << _T( " er: " ) << eErrorCode << endl;
}

BOOL CVirtualPlayerWnd::StartRecord( tstring& strRecPath, EMCU_ErrorCode& eErrorCode )
{
//    Log() << _T( "Start Record Has no code" ) << endl;
    strRecPath = this->GetFileName( _T( "3gp" ) );

    return this->StartRecord( strRecPath.c_str(), eErrorCode );

}

BOOL CVirtualPlayerWnd::IsRecording() const
{
//    Log() << _T( "Is Recording Has no code" ) << endl;
	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if( pDec )
	{
		return pDec->IsRecording();
	}
	return FALSE;
}

BOOL CVirtualPlayerWnd::RestartPlay()
{
//    Log() << _T( "Restart play Has no code" ) << endl;

    BOOL bResult;
    bResult = this->StopPlay();
    if( !bResult )
    {
        Log() << _T( "Restart play can't StopPlay " ) << endl;
    }
    EMCU_ErrorCode er;
    bResult = this->StartPlay( CMCUSession::Instance()->CurVideoSession(),er );
    if ( !bResult )
    {
        Log() << _T( "Restart play can't Start Play! er: " ) << er << endl;
    }
    return bResult;
}

BOOL CVirtualPlayerWnd::StopRecord( EMCU_ErrorCode& pErrorCode )
{
    // 停止磁盘空间检测。
    this->m_bCheckDiskSpace = FALSE;

	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if( pDec )
	{
		return pDec->StopRecord();
	}
	else
	{
		return FALSE;
	}
 //   Log() << _T( " CVirtualPlayerWnd::StopRecord has no code!" ) << endl;
    return FALSE;
}

tstring CVirtualPlayerWnd::GetFileName( LPCTSTR strExtName )
{
    time_t now = GetCurTime();
    tstring strTime = TimeToStr( now );

    // 时间只保留 月 日 小时 分钟。
    strTime = strTime.substr( 4, 8 );

    tstring strDir;
    CConfig::Instance()->GetCaptureDir( strDir );

    tstring strPuName = CMCUSession::Instance()->CurVideoSession()->PuName( );

    // 前端名只保留5个字。需要区别unicode和非unicode下的中文，英文。
    if ( !strPuName.empty() )
    {
        strPuName = strPuName.substr( 0, 10 / sizeof( strPuName[0] )  );
    }


    tstringstream ssFileName;

    // 文件名最后要加3位随机数。
    int nRadom = (int)GetTick();
    nRadom &= 0x03FF;		// 取后10位。最大是1023
    if ( nRadom > 999 )
    {
        nRadom -= 24;		// 不允许超出3位数。
    }
    tstringstream ssRadom;
    ssRadom << setw( 3 ) << nRadom;
    tstring strRadom = ssRadom.str();

    // 如果图片所在目录不存在，创建。
    if ( !IsFileExist( strDir.c_str() ) )
    {
        MakeDir( strDir.c_str() );
    }	

    //strPicDir + strPuName + _T( "_" ) + (LPCTSTR)strTime + strRadom + _T( ".jpg" );
    ssFileName << strDir << strPuName << _T( "_" ) << strTime << strRadom << _T( "." ) << strExtName;

    tstring strPicPath = ssFileName.str();

    return strPicPath;
}

void CVirtualPlayerWnd::CheckDiskSpace()
{
    if ( this->m_bCheckDiskSpace && this->IsRecording() )
    {
        // 检查。
		// 检测磁盘空间。
        tstring strRecDir;
        CConfig::Instance()->GetCaptureDir( strRecDir );

        mu_uint64 nFreeSpace = ::GetDirFreeSpace( strRecDir.c_str() );
        
		mu_uint32 nMinSpace = MIN_STORAGE_SPACE;
		CConfig::Instance()->GetMinStorageSpace( nMinSpace ) ;
		if ( nFreeSpace < nMinSpace )
		{
            Log() << _T( "CheckDiskSpace dir " ) << strRecDir << _T( " don't have enough space to rec! left: " )
                << nFreeSpace << _T( " min: " ) << nMinSpace << endl;

            EMCU_ErrorCode er;
			BOOL bResult = this->StopRecord( er );

			if( !bResult )
				{
			Log() << _T( "Stop Recorder Fail!!" ) << endl;
				}

            // 不再检测磁盘空间。
			this->m_bCheckDiskSpace = FALSE;

            // 通知。
            mcu::PostMsg( this, WM_RECORD_FAIL, MCU_Error_Storage_Full, 0 );
		}
    }
}

void CVirtualPlayerWnd::OnBufferdVideoFrameShowS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *userData )
{
	CVirtualPlayerWnd *pThis = ( CVirtualPlayerWnd *)userData;
	if ( pThis )
	{
		pThis->OnBufferdVideoFrameShow( pic, pFrameInfo );
	}
	else
	{
		Log() << _T( "Video wnd buffer video frame show S this is NULL!" ) << endl;
	}
}

void RollClockwise( int nWidth, int nHeight,  void *pSrc, void *pDst )
{
	int i = 0;
	for ( int y = 0; y < nWidth; ++y )
	{
		for ( int x=0; x<nHeight; ++x )
		{
			int nIndex = nWidth * nHeight - nWidth * x - nWidth + y;
			(( unsigned char * )pDst)[i] = ( (unsigned char* )pSrc )[ nIndex ];
			i++;
		}
	}
}

void RollYUV420Clockwise( int nWidth, int nHeight, void * pSrc, void * pDst )
{
	int nOffset = 0;

	int nYWidth = nWidth;
	int nYHeight = nHeight;
	RollClockwise( nYWidth, nYHeight, pSrc, pDst );
	nOffset += nYWidth * nYHeight;

	int nUWidth = nWidth / 2;
	int nUHeight = nHeight / 2;
	RollClockwise( nUWidth, nUHeight, (char*)pSrc + nOffset, (char*)pDst + nOffset );
	nOffset += nUWidth * nUHeight;

	int nVWidth = nWidth / 2;
	int nVHeight = nHeight / 2;
	RollClockwise( nVWidth, nVHeight, (char*)pSrc + nOffset, (char*)pDst + nOffset );
}

void CVirtualPlayerWnd::OnBufferdVideoFrameShow( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo )
{
	const CBaseCodec::TVideoPicture *pVideoPic = pic;
	CBaseCodec::TVideoFrameInfo tFrameInfo = *pFrameInfo;
	if ( this->IsFullScreen() )
	{
		// 旋转图像。
		// 判断是否需要旋转。
		int nScreenWidth = ::GetScreenWidth();
		int nScreenHeight = ::GetScreenHeight();
		if ( ( nScreenWidth - nScreenHeight ) * ( pFrameInfo->frameWidth - pFrameInfo->frameHeight ) < 0  )
		{
			// 需要旋转。顺时针旋转90度。
			// 数组中图像是YUV格式的。0是Y，1，2是U,V.

			// 初始化缓存,这里copy目的是申请内存而不是拷贝数据。
			if ( this->m_tVideoRollPicBuf.picWidth != pic->picWidth 
				|| this->m_tVideoRollPicBuf.picHeight != pic->picHeight )
			{
				this->m_tVideoRollPicBuf.Copy( *pic );
			}

			for ( int i=0; i<VIDEO_PICTURE_DIM; ++i )
			{
				if ( i == 0 )
				{
					RollClockwise( pic->picWidth, pic->picHeight, pic->data[i], m_tVideoRollPicBuf.data[i] );					
				}
				else if ( i == 1 || i == 2 )
				{
					int nDimWidth = pic->picWidth / 2;
					int nDimHeight = pic->picHeight / 2;
					RollClockwise( nDimWidth, nDimHeight, pic->data[i], m_tVideoRollPicBuf.data[i] );
				}
			}

			// 构造新的图片结构体。
			m_tVideoRollPicBuf.picWidth = pic->picHeight;
			m_tVideoRollPicBuf.picHeight = pic->picWidth;
			pVideoPic = &m_tVideoRollPicBuf;

			int nTmp = tFrameInfo.frameHeight;
			tFrameInfo.frameHeight = tFrameInfo.frameWidth;
			tFrameInfo.frameWidth = nTmp;

		}
	}
	this->OnVideoPicture( pVideoPic, &tFrameInfo );
}

void CVirtualPlayerWnd::SetFullScreen( BOOL bFullScreen )
{
	m_bFullScreenMode = bFullScreen;
}

BOOL CVirtualPlayerWnd::IsFullScreen() const
{
	return m_bFullScreenMode;
}
