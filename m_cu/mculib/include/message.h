#pragma once

/** 
*	在没有消息的平台上模拟消息机制，在一些处理里防止死锁的发生。
*	使用设置回调处理的方式来处理消息，必须要注意不用时要取消注册，否则仍然会调用，可能会造成崩溃。
*	时间紧迫，只考虑了M_Cu中现有的应用，不完善，切勿滥用。
*	by lzx 2009.10.23  
*	更新：lzx 2009.10.23
*	考虑到使用，仿照WIN32，增加消息目标，它等同于WIN32中的窗口。
*/


#include "mcudefine.h"
#include "scopelock.h"

struct SDL_Thread;



struct SDL_semaphore;


namespace mcu
{
	/** 消息接收者。 */
	typedef void * MessageTarget;

	struct TMsg
	{
		MessageTarget m_MessageTarget;
		mu_int32 m_nMessage;
		WPARAM m_wParam;
		LPARAM m_lParam;

		TMsg()
		{
			m_MessageTarget = 0;
			m_nMessage = 0;
			m_wParam = 0;
			m_lParam = 0;
		}
	};

	/** 消息回调函数原型。 */
	typedef mu_int32 ( *FunMessageCallback )( const TMsg& tMsg );

	

	/** 发送消息，异步。 */
	BOOL PostMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

	/** 发送同步消息。 */
	mu_int32 SendMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

	/** 注册消息回调处理。 */
	void RegisterMessageCallback( MessageTarget target, FunMessageCallback pFunCb );

	/** 取消注册。 */
	BOOL UnregisterMessageCallback( MessageTarget target );

	class CMCUMessage
	{
	public:
		static CMCUMessage *Instance();
		static void Release();

	private:

		typedef FunMessageCallback TMsgCallback;
		//struct TMsgCallback
		//{
		//	FunMessageCallback m_pFunMessageCb;

		//	TMsgCallback()
		//	{
		//		m_pFunMessageCb = NULL;
		//	}

		//	bool operator < ( const TMsgCallback& another ) const
		//	{
		//		return ( this->m_pFunMessageCb < another.m_pFunMessageCb );
		//
		//	}
		//};

		

	public:
		CMCUMessage(void);
		virtual ~CMCUMessage(void);

		/** 初始化。 */
		BOOL Init();

		/** 销毁。 */
		BOOL Destroy();

		/** 发送消息，异步。 */
		BOOL PostMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

		/** 发送消息，同步。 
		*	。
		*/
		mu_int32 SendMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

		/** 注册消息回调处理。 */
		void RegisterMessageCallback( MessageTarget target, FunMessageCallback pFunCb );

		/** 取消注册。 */
		BOOL UnregisterMessageCallback( MessageTarget target );

		/** 取消息。 */
		BOOL PeekMessage( TMsg& tMsg );

		/** 处理消息。 */
		mu_int32 HandleMessage( const TMsg& tMsg );

		/** 处理SendMessage发送的消息。 */
		BOOL CheckSentMessage();

	private:
		static int MessageHandleThread( void *pCMessagePoint );
	private:
		static CMCUMessage *s_instance;
		
		/** 单件模式安全锁。 */
		static CMCUMutex s_singletonLock;
		
		/** 维持线程安全的锁。 */
		CMCUMutex m_threadSafeLock;
		/** 消息处理线程。*/
		SDL_Thread *m_pMessageHandleThread;

		/** 线程运行标志。 */
		BOOL m_bThreadSwitch;

	private:
		

		typedef std::queue<TMsg> TMessageQueue;
		typedef std::map< MessageTarget, TMsgCallback > TMessageHandleTable;

		TMessageQueue m_tMessageQueue;
		TMessageHandleTable m_tMessageHandleTable;

		/** Send Message 的消息存储. */
		TMsg m_tSentMessage;
		mu_int32 m_nSendMessageResult;
		/** 只能有一个SendMessage被处理。互斥锁。 
		*	SendMessage 阻塞调用，所以使用Mutex就可以，不考虑同线程的调用。
		*/
		CMCUMutex m_SendMessageLock;
		/** 阻塞SendMessage等待处理结果的Semaphore*/
		SDL_semaphore * m_semaphoreSendMessage;

	};
	

}


