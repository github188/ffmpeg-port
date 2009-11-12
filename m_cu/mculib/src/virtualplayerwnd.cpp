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

	mcu::RegisterMessageCallback( this, OnMessage );
}

CVirtualPlayerWnd::~CVirtualPlayerWnd(void)
{
	SCOPE_LOCK( m_threadSafeLock );

	// 停止监测。
	this->StopCheckThread();

	mcu::UnregisterMessageCallback( this );
}

BOOL CVirtualPlayerWnd::StartPlay( CVideoSession *pVideoSession, EMCU_ErrorCode& pErrorCode )
{
	SCOPE_LOCK( m_threadSafeLock );

	// 不管如何，开始监测状态。反正不会重复开始。
	this->StartCheckThread();

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
		mcu::tlog << _T( "player wnd startplay() fail! video session 0! " ) << endl;
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
		mcu::tlog << _T( "CVirtualPlayerWnd::PausePlay already pause/restart!" ) << endl;
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

	mcu::tlog << _T( "CVirtualPlayerWnd::StopPlay" ) << endl;
	return this->m_MediaNet.CloseRTSP();
}


BOOL CVirtualPlayerWnd::YUVCallbackS( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo, void *param )
{

	CVirtualPlayerWnd *pThis = ( CVirtualPlayerWnd * )param;
	if ( pThis )
	{
		pThis->OnVideoPicture( pic, pFrameInfo );
	}
	return TRUE;
}

BOOL CVirtualPlayerWnd::SendPtzCmd( EPTZCmdId eCmd )
{
	SCOPE_LOCK( m_threadSafeLock );

	BOOL bHasPtzPower =	CMCUSession::Instance()->CurVideoSession()->PtzControl();
	if ( !bHasPtzPower )
	{
		mcu::tlog << _T( "用户没有PTZ权限！" ) << endl;
		return FALSE;
	}

	BOOL bPtz;
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
		mcu::tlog << _T( "Ptz Control Don't suport the cmd: " ) << eCmd << endl;
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


BOOL CVirtualPlayerWnd::Capture( EMCU_ErrorCode& eErrorCode )
{
	SCOPE_LOCK( m_threadSafeLock );

	time_t now = GetCurTime();
	tstring strTime = TimeToStr( now );

	tstring strDir;
	CConfig::Instance()->GetCaptureDir( strDir );

	tstring strPuName;
	CMCUSession::Instance()->CurVideoSession()->PuName( strPuName );

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
	ssFileName << strDir << strPuName << _T( "_" ) << strTime << strRadom << _T( "." ) << _T( "jpg" );

	BOOL bResult = this->Capture( ssFileName.str().c_str(), eErrorCode );
	mcu::tlog << _T( "Capture file: " ) << ssFileName.str() << "result" << bResult  << endl;
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
	SCOPE_LOCK( m_threadSafeLock );

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
	mcu::tlog << _T( "CVirtualPlayerWnd CheckThread run!" ) << endl;
	CVirtualPlayerWnd *pThis = ( CVirtualPlayerWnd*) pParam;
	if ( pThis )
	{
		while( pThis->GetCheckThreadSwitch() )
		{
			pThis->CheckStatsu();

			SDL_Delay( 5 );
		}	
		
	}

	mcu::tlog << _T( "CVirtualPlayerWnd CheckThread exit!" ) << endl;
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
		mcu::PostMsg( this, WM_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Fail, eStatus );
		m_bCheckStatus = FALSE;
		break;

	case RTSPStatus_Error_Server_Full:	// 服务器达到了最大能力
		mcu::PostMsg( this, WM_VIDEO_OPEN_FAIL, MCU_Error_Rtsp_Server_Full, eStatus );
		m_bCheckStatus = FALSE;
		break;

	default:
		if ( m_eLastRtspStatus != eStatus )
		{
			m_eLastRtspStatus = eStatus;
			mcu::PostMsg( this, WM_VIDEO_PLAY_STATUS, 0, eStatus );
		}
	    break;
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

		
		default:
		    break;
		}
		
	}
	return 0;
}


