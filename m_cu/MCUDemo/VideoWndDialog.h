#pragma once


#include "staticex.h"
#include "PTZDialog.h"
//#include "afxwin.h"

#include "mculib.h"

struct SDL_Rect;

// CVideoWndDialog �Ի���

class CVideoWndDialog : public CDialog, public CVirtualPlayerWnd
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

    /** �����Ļ��ʾ�� */
    BOOL ClearScreen();

	/** ����Ҫ�򿪵�rtsp����,�����ڴ��ڴ���֮ǰ����. */
//	void SetRstpUrl( LPCTSTR strRtspUrl );

	/** ��ͣ����. */
//	BOOL Pause();

	/** �ָ�����. */
//	BOOL Resume();

	/** ��ǰ�Ƿ���ͣ��. */
//	BOOL IsPause();

	/** ����ȫ��״̬.*/
	void SetFullScreen( BOOL bFullScreen );

	/** ץ��ͼƬ. */
//	BOOL PicCapture( LPCTSTR strPicPath );

	/** ��ʼ¼��. */
//	BOOL StartRecord( LPCTSTR strRecordPath );

	/** ֹͣ¼��. */
//	BOOL StopRecord();

	/** ��ǰ�Ƿ���¼��. */
//	BOOL IsRecoding();

protected:
    /** ͼ��ص��� */
    virtual void OnVideoPicture( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo ) ;

    /** ���� ״̬,�¼����ص��� */
    virtual void OnRtspStatus( const ERTSPStatus eRtspStatus, const EMCU_ErrorCode eErrorCode ) ;

    /** ¼��״̬�ص��� */
    virtual void OnRecordStatus( BOOL bSuccess, EMCU_ErrorCode er );

    /** ͼ��֡���ݻص����������档 */
    static void OnBufferdVideoFrameShowS( const CBaseCodec::TVideoPicture *pic, 
        const CBaseCodec::TVideoFrameInfo *pFrameInfo,
        void *userData );

    /** ͼ��֡���ݻص����������档 */
    void OnBufferdVideoFrameShow( const CBaseCodec::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo *pFrameInfo );

private:

	enum SDL_MCU_Event
	{
//		SDL_WM_FULLSCREEN = SDL_USEREVENT + 1,	// ȫ������. data1: ȫ��
	
	};


private:
//	CMediaNet m_MediaNet;
//	CString m_strRtspUrl;

    /** ֡���档 */
    CFrameBuffer m_frameBuffer;

	/** SDL �߳̾���� */
	SDL_Thread *m_pSDLThread;

	/** SDL��Overlay��*/
	SDL_Overlay *m_pSDLOverlay;

	/** ��ʾ�����С. */
	SDL_Rect* m_prcVideoShow; 

	/** SDL surface */
	SDL_Surface *m_pSDLSurface;
	
	/** �Ƿ���ȫ��ģʽ. */
	BOOL m_bFullScreenMode;

	/** ptz���ƶԻ���. */
//	CPTZDialog m_dlgPtz;

	/** �̰߳�ȫ��.*/
	CMCUMutex m_threadSafeLock;

	/** ״̬ˢ�¶�ʱ��ID */
//	UINT_PTR m_dwStatusUpdateTimerId;

	/** ���һ�δ�������״̬��ʱ��. */
//	CTime m_timeLastWork;

	/** ���һ�λ���ͼ�ε�ʱ��. */
//	CTime m_timeLastDrawVideo;

	/** RTSP��Ч��ʱʱ��. */
//	int m_nTimeoutRTSPFail;

	/** ����������ĳ�ʱʱ��. */
//	int m_nTimeoutNoPacket;

	/** ��ǰ��¼��RTSP״̬. */
//	ERTSPStatus m_eRtspStatus;

	/** �ȴ�������Ƶ����ʱ����Ϣ. */
//	BOOL m_bCheckRtspTimeout;

	/** �ڴ�����Ƶ����ʧ�ܵ���Ϣ��*/
//	BOOL m_bCheckRtspError;

	/** �ȴ�����û������������Ϣ. */
//	BOOL m_bCheckNoPacketTimeout;

	/** ģ��˫����Ϣ��Ҫ��¼�ϴε�ʱ�䡣 */
	clock_t m_timeLastMouseClick;

	/** �Ƿ���ͣ��. */
//	BOOL m_bPause;


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
	//static BOOL YUVCallbackS( 
	//	const CDecoder::TVideoPicture *pic, 
	//	const CBaseCodec::TVideoFrameInfo* pFrameInfo,
	//	void *param );

	/** �ص������ĳ�Ա�������� */
//    void YUVCallback( const CDecoder::TVideoPicture *pic, const CBaseCodec::TVideoFrameInfo* pFrameInfo );
	

	/** ״̬��Ѳ��ܹ�����. */
//	void StatusCheck();

	/** �洢�ռ��顣 */
//	void StorageSpaceCheck();

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��Ƶ״̬.
	CStaticEx m_staticVideoStatus;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
