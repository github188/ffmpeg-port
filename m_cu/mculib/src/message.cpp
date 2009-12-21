#include "message.h"
#include "mcuthread.h"
#include "log.h"

using namespace mcu;

CMCUMessage *CMCUMessage::s_instance = NULL;

CMCUMutex CMCUMessage::s_singletonLock;

namespace mcu
{



BOOL mcu::PostMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam )
{
	return CMCUMessage::Instance()->PostMsg( target, nMessage, wParam, lParam );
}

mu_int32 mcu::SendMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam )
{
	return CMCUMessage::Instance()->SendMsg( target, nMessage, wParam, lParam );
}

void mcu::RegisterMessageCallback( MessageTarget target, FunMessageCallback pFunCb )
{
	return CMCUMessage::Instance()->RegisterMessageCallback( target, pFunCb );
}

BOOL mcu::UnregisterMessageCallback( MessageTarget target )
{
	return CMCUMessage::Instance()->UnregisterMessageCallback( target );
}



CMCUMessage::CMCUMessage(void)
{
	m_pMessageHandleThread = NULL;
	m_semaphoreSendMessage = NULL;
}

CMCUMessage::~CMCUMessage(void)
{
	this->Destroy();
}

CMCUMessage *CMCUMessage::Instance()
{
	if ( NULL == s_instance )
	{
		SCOPE_LOCK( s_singletonLock );
		if ( NULL == s_instance )
		{
			s_instance = new CMCUMessage();
			s_instance->Init();
		}
	}
	return s_instance;
}

void CMCUMessage::Release()
{
	if ( s_instance )
	{
		SCOPE_LOCK( s_singletonLock );
		if ( s_instance )
		{
			s_instance->Destroy();
			delete s_instance;
			s_instance = NULL;
		}
	}
}

BOOL CMCUMessage::PostMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam )
{
	SCOPE_LOCK( m_threadSafeLock );

	TMsg tMsg;
	tMsg.m_MessageTarget = target;
	tMsg.m_nMessage = nMessage;
	tMsg.m_wParam = wParam;
	tMsg.m_lParam = lParam;

	this->m_tMessageQueue.push( tMsg );

	return TRUE;	
}

void CMCUMessage::RegisterMessageCallback( MessageTarget target, FunMessageCallback pFunCb )
{
	SCOPE_LOCK( m_threadSafeLock );

	TMsgCallback tMcb = pFunCb;

	this->m_tMessageHandleTable[ target ] = tMcb;

}

BOOL CMCUMessage::UnregisterMessageCallback( MessageTarget target )
{
	SCOPE_LOCK( m_threadSafeLock );

	TMessageHandleTable::iterator iter = this->m_tMessageHandleTable.find( target );
	if( iter != this->m_tMessageHandleTable.end() )
	{
		BOOL bResult = 0 < this->m_tMessageHandleTable.erase( target );
		return bResult;
	}
	else
	{
		return FALSE;
	}	

}

BOOL CMCUMessage::Init()
{
	SCOPE_LOCK( m_threadSafeLock );

	if ( this->m_pMessageHandleThread )
	{
		Log() << _T( "CMCUMessage::Init() already inited!" ) << endl;
		return TRUE;
	}
	else
	{
		m_bThreadSwitch = TRUE;
		Log() << _T( "before CMCUMessage SDL_CreateThread" ) <<endl;
		this->m_pMessageHandleThread = SDL_CreateThread( MessageHandleThread, this );

		this->m_semaphoreSendMessage = SDL_CreateSemaphore( 0 );
	}
	return TRUE;
}

BOOL CMCUMessage::Destroy()
{
	{
		SCOPE_LOCK( m_threadSafeLock );

		if( this->m_pMessageHandleThread )
		{
			m_bThreadSwitch = FALSE;
			int nStatus;
			SDL_WaitThread( this->m_pMessageHandleThread, &nStatus );

			this->m_pMessageHandleThread = NULL;
		}
	}
	
	// 销毁sendmessage阻塞用的信号量。
	{
		SCOPE_LOCK( this->m_SendMessageLock );
		SDL_DestroySemaphore( m_semaphoreSendMessage );
		m_semaphoreSendMessage = NULL;
	}

	return TRUE;
}

int CMCUMessage::MessageHandleThread( void *pCMessagePoint )
{
	Log() << _T( "MessageHandleThread run!" ) << endl;
	CMCUMessage *pThis = ( CMCUMessage * )pCMessagePoint;
	while ( pThis && pThis->m_bThreadSwitch )
	{
		pThis->CheckSentMessage();

		int nDelayTime = 10;
		TMsg tMsg;
		if( pThis->PeekMessage( tMsg ) )
		{
//			mu_int32 nMessageHandleResult;
			pThis->HandleMessage( tMsg );

			// 如果有消息被处理了，那么马上处理下一个。
			nDelayTime = 0;
		}
		SDL_Delay( nDelayTime );
	}
	return 0;
}

BOOL CMCUMessage::PeekMessage( TMsg& tMsg )
{
	SCOPE_LOCK( m_threadSafeLock );

	if ( !this->m_tMessageQueue.empty() )
	{
		tMsg = this->m_tMessageQueue.front();
		this->m_tMessageQueue.pop();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

mu_int32 CMCUMessage::HandleMessage( const TMsg& tMsg )
{
	SCOPE_LOCK( m_threadSafeLock );

	mu_int32 nResult = 0;
	TMessageHandleTable::iterator iter = this->m_tMessageHandleTable.find( tMsg.m_MessageTarget );
	if( iter != this->m_tMessageHandleTable.end() )
	{
		TMsgCallback tCb = iter->second;
		if ( tCb )
		{
			try
			{
				nResult = tCb( tMsg );
			}
			catch ( ... )
			{
				Log() << _T( "Call Message Callback Crash! message: " ) << tMsg.m_nMessage 
					<< _T( " wparam: " ) << tMsg.m_wParam << _T( "lparam" ) << tMsg.m_lParam
					<< _T( " callback: " ) 
					<< (void*)tCb << _T( " target: " ) 
					<< tMsg.m_MessageTarget << endl;
			}
		}		
	}
	else
	{
		Log() << _T( "HandleMessage can't find the target of the msg. target:" ) << tMsg.m_MessageTarget 
			<< _T( " msg: " ) << tMsg.m_nMessage << endl;
	}

	return nResult;
}

mu_int32 CMCUMessage::SendMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam )
{
	// Send Message 排斥其它线程的同时调用。是阻塞的，不能做与其他函数一样的互斥保护。
	SCOPE_LOCK( this->m_SendMessageLock );

	// 如果调用SendMsg的就是消息处理线程，那么就直接调用处理。
	Uint32 nThreadId = SDL_ThreadID();
	if ( nThreadId == SDL_GetThreadID( this->m_pMessageHandleThread ) )
	{
		Log() << _T( "CMCUMessage::SendMsg Called by the Message handle thread! Handle directly!" ) << endl;
		TMsg tSentMsg;
		tSentMsg.m_MessageTarget = target;
		tSentMsg.m_nMessage = nMessage;
		tSentMsg.m_wParam = wParam;
		tSentMsg.m_lParam = lParam;
		mu_int32 nResult = this->HandleMessage( tSentMsg );
		return nResult;
	}

	// 只有信号量正常的情况下才可以SendMsg。
	if( this->m_semaphoreSendMessage )
	{
		// 存储消息。
		{
			SCOPE_LOCK( this->m_threadSafeLock );
			this->m_tSentMessage.m_MessageTarget = target;
			this->m_tSentMessage.m_nMessage = nMessage;
			this->m_tSentMessage.m_wParam = wParam;
			this->m_tSentMessage.m_lParam = lParam;
		}

		Log() << _T( "SendMsg Wait for Message handle! msg: " ) << nMessage << endl;
		// 等待消息处理完成。
		SDL_SemWait( this->m_semaphoreSendMessage );
	}
	else
	{
		Log() << _T( "SendMsg m_semaphoreSendMessage = NULL!!!" ) << endl;
	}

	Log() << _T( "SendMsg  return! msg: " ) << nMessage << endl;	

	return this->m_nSendMessageResult;
}

BOOL CMCUMessage::CheckSentMessage()
{
	SCOPE_LOCK( this->m_threadSafeLock );

	if( NULL != m_tSentMessage.m_nMessage  )
	{
		Log() << _T( "CheckSentMessage find msg:" ) << m_tSentMessage.m_nMessage << endl;
		
		this->m_nSendMessageResult = this->HandleMessage( m_tSentMessage );

		SDL_SemPost( m_semaphoreSendMessage );

		// 处理完，清空。
		m_tSentMessage.m_nMessage = NULL;

		Log() << _T( "CheckSentMessage SDL_SemPost msg:" ) << m_tSentMessage.m_nMessage << endl;
	}
	return TRUE;
}

} // namespace mcu




