#pragma once

#include "scopelock.h"
#include "basecodec.h"
#include "message.h"

/** ��װ����Ƶ��ʾ���ܡ�
	�����ԣ�SDL����ʾ��Դ��֧�ֶ��̷߳��ʡ�
	������ܻᱻ����̵߳��á�
	���ԣ��ڴ���/����/���ĵ�ʱ�������߳�ת����
	�߳�ת��ʹ�õ����Լ�д�ļ�message��

*	modified by lzx 2009.10.24 ����ͼ�񻺴棬��ʱ��ʾ���ơ�
*	�����ͼƬ�ᱻд�뻺��������������������Ḳ�������һ֡��
*	����ͼ���֡��Ϣ�е�ʱ���ȷ��ͼƬ��ʾʱ������
*	�ö�ʱ��������ʾ��ÿ����ʾ�����һ֡��
*/

struct SDL_Surface;
struct SDL_Overlay;
struct SDL_Thread;
typedef struct _SDL_TimerID *SDL_TimerID;

class CVideoShowWnd
{
public:
	enum EVideoWndEvent
	{
		EventMouseButtonDown,

	};
	typedef void ( *CB_VideoWndEvent )( EVideoWndEvent eEvent, mu_uint32 uUserData );

	/** ����ͼ��֡���� */
	static const int PicBufferSize = 10;
	/** �Ƿ����ͼ�񻺳���Դ�ӡ�� */
	static const BOOL PicBufferDebug = FALSE;

public:
	CVideoShowWnd(void);
	virtual ~CVideoShowWnd(void);

	/** ������ */
	BOOL Create( int nXpos, int nYpos, int nWidth, int nHeight, BOOL bFullScreen );

	/** ���ġ� */
	BOOL SetRect( int nXpos, int nYpos, int nWidth, int nHeight, BOOL bFullScreen );

	/** ��ʾ�� */
//	BOOL ShowWindow( BOOL bShow );

	/** ���١� */
	BOOL Destroy();

	/** ��ʾ�� �첽���л��档*/
	BOOL DisplayAsync( const CBaseCodec::TVideoPicture& tPic, const CBaseCodec::TVideoFrameInfo& tFrameInfo );

	void SetEventCallback( CB_VideoWndEvent pFun, mu_uint32 uUserData );

	/** ����ͼ����ʾÿ֡���ʱ��,��λ����ms�� 
	*/
	BOOL SetDisplayTimer( mu_uint32 nTime );

private:
	/** �ӻ����ж�ȡͼƬ��ʾ�� */
	mu_uint64 DisplayFromPicBuffer();

private:
	static int SDL_ThreadFunc( void *param );

	/** ��Ϣ����ص��� */
	static mu_int32 OnMessageCallback( const mcu::TMsg& tMsg );

	/** ��ʾ��ʱ���ص��� */
	static unsigned int OnDisplayTimerCallback( unsigned int interval, void *param);

//	BOOL DestroyNotQuit();

	/** �����߳�ת�����Create��*/
	BOOL CreateThreadSafe();
	/** �����߳�ת�����Destroy�� */
	BOOL DestroyThreadSafe();

private:
	SDL_Surface *m_pSDLSurface;

	SDL_Overlay *m_pSDLOverlay;

	CMCUMutex m_threadSafeLock;

	CB_VideoWndEvent m_pCbVideoEvent;
	mu_uint32 m_uUserData;

	SDL_Thread * m_pSDLThread;

	/** ������Ƶ��ʾ�Ĳ����� */
	int m_nXpos;
	int m_nYpos;
	int m_nWidth;
	int m_nHeight;
	BOOL m_bFullscreen;

	
	/** ͼ����ʾ��ʱ���� */
	SDL_TimerID m_pDisplayTimerId;
	/** ��ʱ��ʱ�䡣 */
	int m_nDisplayTime;

	struct TPicBufferItem 
	{
		CBaseCodec::TVideoPicture m_tPic;			// ͼƬ���ݡ�
		CBaseCodec::TVideoFrameInfo m_tFrameInfo;	// ��Ƶ֡��Ϣ��
		BOOL m_bFillData;							// �Ƿ��Ѿ�������ݣ��ȴ���ʾ��

		TPicBufferItem()
		{
			m_bFillData = FALSE;
		}
	};
	/** ͼ�����ݻ������� */
	TPicBufferItem m_arPicBuffer[ PicBufferSize ];
	/** ��ǰд�αꡣ */
	int m_nWriteCursor;
	/** ��ǰ���αꡣ */
	int m_nReadCursor;

	/** ��һ����ʾ��֡��ʱ����� */
	mu_uint64 m_u64LastTimeStamp;

	/** ��һ����ʾ����֡��ʱ����� */
	mu_uint64 m_u64LastDisplayFrameTimeStamp;

};
