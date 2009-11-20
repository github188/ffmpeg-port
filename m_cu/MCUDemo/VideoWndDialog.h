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

// CVideoWndDialog �Ի���

class CVideoWndDialog : public CDialog
{
	DECLARE_DYNAMIC(CVideoWndDialog)
public:
	CVideoWndDialog( CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoWndDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_VIDEO_WND };

	/** ��������. */
	enum EConstDef
	{
		TIMEOUT_RTSP_FAIL = 2,			// RTSP��ʱʱ��.
		TIMEOUT_NO_PACKET = 20,			// ������ʱ�ĳ�ʱʱ��.
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	/** ����Ҫ�򿪵�rtsp����,�����ڴ��ڴ���֮ǰ����. */
	void SetRstpUrl( LPCTSTR strRtspUrl );

	/** ��ͣ����. */
	BOOL Pause();

	/** �ָ�����. */
	BOOL Resume();

	/** ��ǰ�Ƿ���ͣ��. */
	BOOL IsPause();

	/** ����ȫ��״̬.*/
	void SetFullScreen( BOOL bFullScreen );

	/** ץ��ͼƬ. */
	BOOL PicCapture( LPCTSTR strPicPath );

	/** ��ʼ¼��. */
	BOOL StartRecord( LPCTSTR strRecordPath );

	/** ֹͣ¼��. */
	BOOL StopRecord();

	/** ��ǰ�Ƿ���¼��. */
	BOOL IsRecoding();

private:

	enum SDL_MCU_Event
	{
		SDL_WM_FULLSCREEN = SDL_USEREVENT + 1,	// ȫ������. data1: ȫ��
	
	};

	enum WM_MCU_VIDEO_WND_MESSAGE
	{
		WM_VIDEO_WND_VIDEO_OPEN_FAIL = WM_VIDEO_OPEN_FAIL,	// ��Ƶ���ʧ�ܡ�WParam: EVideoPlayErrorCode�������롣
		WM_VIDEO_WND_RECORD_FAIL = WM_RECORD_FAIL,			// ¼��ʧ�ܡ�
	};


private:
	CMediaNet m_MediaNet;
	CString m_strRtspUrl;

	/** SDL �߳̾���� */
	SDL_Thread *m_pSDLThread;

	/** SDL��Overlay��*/
	SDL_Overlay *m_pSDLOverlay;

	/** ��ʾ�����С. */
	SDL_Rect m_rcVideoShow; 

	/** SDL surface */
	SDL_Surface *m_pSDLSurface;
	
	/** �Ƿ���ȫ��ģʽ. */
	BOOL m_bFullScreenMode;

	/** ptz���ƶԻ���. */
//	CPTZDialog m_dlgPtz;

	/** �̰߳�ȫ��.*/
	CMCUMutex m_threadSafeLock;

	/** ״̬ˢ�¶�ʱ��ID */
	UINT_PTR m_dwStatusUpdateTimerId;

	/** ���һ�δ�������״̬��ʱ��. */
	CTime m_timeLastWork;

	/** ���һ�λ���ͼ�ε�ʱ��. */
	CTime m_timeLastDrawVideo;

	/** RTSP��Ч��ʱʱ��. */
	int m_nTimeoutRTSPFail;

	/** ����������ĳ�ʱʱ��. */
	int m_nTimeoutNoPacket;

	/** ��ǰ��¼��RTSP״̬. */
	ERTSPStatus m_eRtspStatus;

	/** �ȴ�������Ƶ����ʱ����Ϣ. */
	BOOL m_bCheckRtspTimeout;

	/** �ڴ�����Ƶ����ʧ�ܵ���Ϣ��*/
	BOOL m_bCheckRtspError;

	/** �ȴ�����û������������Ϣ. */
	BOOL m_bCheckNoPacketTimeout;

	/** ģ��˫����Ϣ��Ҫ��¼�ϴε�ʱ�䡣 */
	clock_t m_timeLastMouseClick;

	/** �Ƿ���ͣ��. */
	BOOL m_bPause;


protected:
	virtual void PostNcDestroy();

private:
	/** ��ʼ��SDL�� */
	BOOL InitSDL();
	/** ����SDL�� */
	BOOL DestroySDL();

	/** SDL�߳�*/
	static int SDL_ThreadFunc( void *param );

	/** ͼ������ص����������ڽ��ս����Ľ������ص��� */
	static BOOL YUVCallbackS( 
		const CDecoder::TVideoPicture *pic, 
		const CBaseCodec::TVideoFrameInfo* pFrameInfo,
		void *param );

	/** �ص������ĳ�Ա�������� */
	void YUVCallback( const CDecoder::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo* pFrameInfo );
	

	/** ״̬��Ѳ��ܹ�����. */
	void StatusCheck();

	/** �洢�ռ��顣 */
	void StorageSpaceCheck();

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��Ƶ״̬.
	CStaticEx m_staticVideoStatus;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
