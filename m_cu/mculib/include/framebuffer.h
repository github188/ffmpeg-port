#pragma once

/** 
*   ֡���档����֡���ݣ���ʱ�����ʹͼ����ʾƽ����
*/
#include "mcucommon.h"
#include "basecodec.h"

typedef struct _SDL_TimerID *SDL_TimerID;

class CFrameBuffer
{
public:
    CFrameBuffer(void);
    virtual ~CFrameBuffer(void);

    /** ��ʼ����
    *   �����ڴ档������ʱ����
    */
    BOOL Init( int nFrameBufNum );

    /** ����ͼ�����ݡ� */
    BOOL InputFrame( const CBaseCodec::TVideoPicture& tPic, const CBaseCodec::TVideoFrameInfo& tFrameInfo );

    /** ͼ��ص��� */
    typedef void ( *FunFrameCb )( const CBaseCodec::TVideoPicture* pPic, const CBaseCodec::TVideoFrameInfo* pFrameInfo,
        void *userData );

    /** ����ͼ������ص��� */
    void SetFrameCallback( FunFrameCb pFun, void * param );

	/** ��ջ���ͼ�� */
	void Clear();

private:
    /** ��ʾ��ʱ���ص��� */
    static unsigned int OnDisplayTimerCallback( unsigned int interval, void *param);

    /** �ӻ����ж�ȡͼƬ��ʾ�� */
    mu_uint64 DisplayFromPicBuffer();

private:
    /** ����ͼ��֡���� */
    static const int MaxFrameBufferSize = 10;
    /** �Ƿ����ͼ�񻺳���Դ�ӡ�� */
    static const BOOL PicBufferDebug = FALSE;

    /** ͼƬ�����С��*/
    int m_nFrameBufferNum;

    /** �ص�������*/
    FunFrameCb m_pFunFrameCb;
    /** ������ */
    void *m_frameCbUserData;

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
    TPicBufferItem m_arPicBuffer[ MaxFrameBufferSize ];
    /** ��ǰд�αꡣ */
    int m_nWriteCursor;
    /** ��ǰ���αꡣ */
    int m_nReadCursor;

    /** ��һ����ʾ��֡��ʱ����� */
    mu_uint64 m_u64LastTimeStamp;

    /** ��һ����ʾ����֡��ʱ����� */
    mu_uint64 m_u64LastDisplayFrameTimeStamp;

    /** �߳�ͬ������ */
    CMCUMutex m_threadSafeLock;

};
