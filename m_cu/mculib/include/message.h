#pragma once

/** 
*	��û����Ϣ��ƽ̨��ģ����Ϣ���ƣ���һЩ�������ֹ�����ķ�����
*	ʹ�����ûص�����ķ�ʽ��������Ϣ������Ҫע�ⲻ��ʱҪȡ��ע�ᣬ������Ȼ����ã����ܻ���ɱ�����
*	ʱ����ȣ�ֻ������M_Cu�����е�Ӧ�ã������ƣ��������á�
*	by lzx 2009.10.23  
*	���£�lzx 2009.10.23
*	���ǵ�ʹ�ã�����WIN32��������ϢĿ�꣬����ͬ��WIN32�еĴ��ڡ�
*/


#include "mcudefine.h"
#include "scopelock.h"

struct SDL_Thread;



struct SDL_semaphore;


namespace mcu
{
	/** ��Ϣ�����ߡ� */
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

	/** ��Ϣ�ص�����ԭ�͡� */
	typedef mu_int32 ( *FunMessageCallback )( const TMsg& tMsg );

	

	/** ������Ϣ���첽�� */
	BOOL PostMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

	/** ����ͬ����Ϣ�� */
	mu_int32 SendMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

	/** ע����Ϣ�ص����� */
	void RegisterMessageCallback( MessageTarget target, FunMessageCallback pFunCb );

	/** ȡ��ע�ᡣ */
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

		/** ��ʼ���� */
		BOOL Init();

		/** ���١� */
		BOOL Destroy();

		/** ������Ϣ���첽�� */
		BOOL PostMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

		/** ������Ϣ��ͬ���� 
		*	��
		*/
		mu_int32 SendMsg( MessageTarget target, mu_int32 nMessage, WPARAM wParam, LPARAM lParam );

		/** ע����Ϣ�ص����� */
		void RegisterMessageCallback( MessageTarget target, FunMessageCallback pFunCb );

		/** ȡ��ע�ᡣ */
		BOOL UnregisterMessageCallback( MessageTarget target );

		/** ȡ��Ϣ�� */
		BOOL PeekMessage( TMsg& tMsg );

		/** ������Ϣ�� */
		mu_int32 HandleMessage( const TMsg& tMsg );

		/** ����SendMessage���͵���Ϣ�� */
		BOOL CheckSentMessage();

	private:
		static int MessageHandleThread( void *pCMessagePoint );
	private:
		static CMCUMessage *s_instance;
		
		/** ����ģʽ��ȫ���� */
		static CMCUMutex s_singletonLock;
		
		/** ά���̰߳�ȫ������ */
		CMCUMutex m_threadSafeLock;
		/** ��Ϣ�����̡߳�*/
		SDL_Thread *m_pMessageHandleThread;

		/** �߳����б�־�� */
		BOOL m_bThreadSwitch;

	private:
		

		typedef std::queue<TMsg> TMessageQueue;
		typedef std::map< MessageTarget, TMsgCallback > TMessageHandleTable;

		TMessageQueue m_tMessageQueue;
		TMessageHandleTable m_tMessageHandleTable;

		/** Send Message ����Ϣ�洢. */
		TMsg m_tSentMessage;
		mu_int32 m_nSendMessageResult;
		/** ֻ����һ��SendMessage�������������� 
		*	SendMessage �������ã�����ʹ��Mutex�Ϳ��ԣ�������ͬ�̵߳ĵ��á�
		*/
		CMCUMutex m_SendMessageLock;
		/** ����SendMessage�ȴ���������Semaphore*/
		SDL_semaphore * m_semaphoreSendMessage;

	};
	

}


