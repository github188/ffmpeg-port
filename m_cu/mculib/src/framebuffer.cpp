#include "framebuffer.h"

#include "SDL.h"

CFrameBuffer::CFrameBuffer(void)
{
    this->m_pFunFrameCb = NULL;
    this->m_frameCbUserData = NULL;
    this->m_nDisplayTime = 40;  // Ĭ����25֡�ġ�
    this->m_pDisplayTimerId = NULL;
    this->m_nWriteCursor = 0;
    this->m_nReadCursor = 0;
    this->m_u64LastTimeStamp = 0;
    this->m_u64LastDisplayFrameTimeStamp = 0;
    m_nFrameBufferNum = 0;
}

CFrameBuffer::~CFrameBuffer(void)
{
    SCOPE_LOCK( m_threadSafeLock );

    // ������ʾ��ʱ����
    if ( m_pDisplayTimerId )
    {
        SDL_RemoveTimer( m_pDisplayTimerId );
        m_pDisplayTimerId = NULL;
    }

    // ����ͼ�񻺳�����ͼƬ������ڴ档
    for ( int i=0; i<MaxFrameBufferSize; ++i )
    {
        this->m_arPicBuffer[i].m_tPic.Free();
    }
}

BOOL CFrameBuffer::Init( int nFrameBufNum )
{
    SCOPE_LOCK( m_threadSafeLock );

    this->m_nFrameBufferNum = nFrameBufNum;
    if ( m_nFrameBufferNum > MaxFrameBufferSize )
    {
        Log() << _T( "Init framebuf num is biger than Max frame buffer size! cut to Max size. Req: " )
            << m_nFrameBufferNum << _T( " maxsize: " ) << MaxFrameBufferSize << endl;
        m_nFrameBufferNum = MaxFrameBufferSize;
    }

    // ֻ��ʼ����ʱ����ϵͳ��
    int initFlags = SDL_INIT_TIMER ;
    if( initFlags != SDL_WasInit( initFlags ) )
    {		
        if ( SDL_Init ( initFlags ) ) 
        {
            Log() << _T( "Could not initialize SDL timer subsys err: " ) << SDL_GetError() << endl;
            _ASSERT( FALSE );
        }
        else
        {
            Log() << _T( "SDL Timer Init!" ) << endl;
        }
    }	
    else
    {
        Log() << _T( "SDL Timer already inited!" ) << endl;
    }

    // ������ʾ��ʱ����
    if ( NULL == m_pDisplayTimerId )
    {
        m_pDisplayTimerId = SDL_AddTimer( m_nDisplayTime, OnDisplayTimerCallback, this );
    }
    else
    {
        Log() << _T( "Display timer already run!" ) << endl;
    }

    return m_pDisplayTimerId != NULL;

}

BOOL CFrameBuffer::InputFrame( const CBaseCodec::TVideoPicture& tPic, const CBaseCodec::TVideoFrameInfo& tFrameInfo )
{
    SCOPE_LOCK( m_threadSafeLock );

    // ����ͼ��֡�е���Ϣ���¶�ʱ����
    if( this->m_u64LastTimeStamp != 0 )
    {
        int nNewTime = int( tFrameInfo.frameTimeStamp - m_u64LastTimeStamp );
        nNewTime /= 1000;	// ��Ƶ֡ʱ����ĵ�λ�� microsecond�� 1/1000000 s��

        //			Log() << _T( "DisplayAsync frame time span: " ) << nNewTime 
        //				<< _T( " frame time: " ) << (mu_uint32)tFrameInfo.frameTimeStamp << endl;

        // ��ʱ����ȡ����һ����ʱ�����Խ���ʱ������СһЩ���ö�ʱ�����Ŀ�һЩ��
        nNewTime -= 10;

        if ( nNewTime != m_nDisplayTime )
        {
            if ( nNewTime < 5 || nNewTime > 500 )
            {
                Log() << _T( "DisplayAsync Invalid nNewTime :" ) << nNewTime << endl;
            }
            else
            {
                m_nDisplayTime = nNewTime;
                Log() << _T( "DisplayAsync Timer change to " ) << m_nDisplayTime << endl;
            }

        }
    }
    this->m_u64LastTimeStamp = tFrameInfo.frameTimeStamp;

    int nPicBufferNum = this->m_nFrameBufferNum;


    // �ж��ǲ�����Ҫ���ǻ�û�ж�ȡ��ʾ��ͼƬ���ݡ�
    // ����ǣ�����ʾ�Ķ��α�ǰ�Ƶ���ɵ������ϡ�
    if( this->m_nReadCursor == this->m_nWriteCursor )
    {
        int nNextPos = this->m_nReadCursor + 1;
        if ( nNextPos >= nPicBufferNum )
        {
            nNextPos -= nPicBufferNum;
        }
        if ( m_arPicBuffer[ nNextPos ].m_bFillData )
        {
            m_nReadCursor = nNextPos;
            if( PicBufferDebug )
            {
                Log() << _T( "Write faster, read slow. write change read cursor to " ) << m_nReadCursor << endl;
            }			
        }
    }

    // ��ͼ������copy��ͼƬ���塣
    int nWriteIndex = this->m_nWriteCursor;
    this->m_arPicBuffer[ nWriteIndex ].m_tFrameInfo = tFrameInfo;
    BOOL bResult = this->m_arPicBuffer[ nWriteIndex ].m_tPic.Copy( tPic );
    if ( !bResult )
    {
        Log() << _T( "DisplayAsync copy pic failed!" ) << endl;
    }
    this->m_arPicBuffer[ nWriteIndex ].m_bFillData = bResult;

    if ( bResult )
    {
        this->m_nWriteCursor ++;
        if ( m_nWriteCursor >= nPicBufferNum )
        {
            m_nWriteCursor -= nPicBufferNum;
        }

        //		Log() << _T( "write cursor move to" ) << m_nWriteCursor << endl;
    }
    return bResult;
}

unsigned int CFrameBuffer::OnDisplayTimerCallback(unsigned int interval, void *param)
{
    CFrameBuffer *pThis = ( CFrameBuffer * )param;
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
        //		Log() << _T( "timer: " ) << nTimeSpan << endl;
        //		return nTimeSpan;
        //	}
        //}

        //		Log() << _T( "interval" ) << interval << _T( "m_nDisplayTime: " )<< pThis->m_nDisplayTime << endl;

        // ���ص�ǰ����ʾ���ʱ�䣬��ʱ�����Զ��������ʱ�����´εĶ�ʱ��
        return pThis->m_nDisplayTime;
    }
    return interval;
}

mu_uint64 CFrameBuffer::DisplayFromPicBuffer()
{
    SCOPE_LOCK( m_threadSafeLock );

    // �ӻ����ж�ȡͼƬ����
    CBaseCodec::TVideoPicture *pPic = NULL;
    CBaseCodec::TVideoFrameInfo *pFrame = NULL;

    int nFrameBufNum = this->m_nFrameBufferNum;

    int nReadCur = this->m_nReadCursor;
    if ( this->m_arPicBuffer[nReadCur].m_bFillData )
    {
        pPic = &(this->m_arPicBuffer[ nReadCur ].m_tPic);
        pFrame = &( this->m_arPicBuffer[ nReadCur ].m_tFrameInfo );

        //		Log() << _T( "read pic cursor: " ) << nReadCur << endl;

        this->m_arPicBuffer[ nReadCur ].m_bFillData = FALSE;
        // move to next.
        this->m_nReadCursor ++;
        if ( m_nReadCursor >= nFrameBufNum )
        {
            m_nReadCursor -= nFrameBufNum;
        }
    }

    if ( NULL == pPic || NULL == pFrame )
    {
        if( PicBufferDebug )
        {
            Log() << _T( "DisplayFromPicBuffer has no pic! " ) << endl;
        }		
        return 0;
    }

    if ( this->m_pFunFrameCb && pPic && pFrame )
    {
        this->m_pFunFrameCb( pPic, pFrame, this->m_frameCbUserData );
    }
    else
    {
        Log() << _T( "Display Callback function is null!" ) << endl;
    }

    return pFrame->frameTimeStamp;
}

void CFrameBuffer::SetFrameCallback( FunFrameCb pFun, void * param )
{
    SCOPE_LOCK( m_threadSafeLock );

    this->m_pFunFrameCb = pFun;
    this->m_frameCbUserData = param;

}

