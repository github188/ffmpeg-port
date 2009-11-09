#pragma once
#include "decoder.h"
#include "kdvdecoderimpl.h"
#include "mpeg4streamparse.h"
#include "ffmpegrecorderimpl.h"

class CMpeg4Decoder :
	public CDecoder
{
public:
	CMpeg4Decoder(void);
	virtual ~CMpeg4Decoder(void);

	/** 同步解码方式. */
	virtual mu_int32 InputStreamSync( const TStreamPacket& pkt );

	/** 录像. */
	virtual BOOL StartRecord( LPCTSTR strFileName ) ;
	virtual BOOL StopRecord() ;
	virtual BOOL IsRecording() const;

protected:
	/** 解码器初始化. */
	virtual BOOL Init( int nBandWidth ) ;


	/** 抓拍. 
	*	同步。抓拍当前帧图像。
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath , CEncoder *pEnc );

private:
	/** keda 解码器. */
	CKDVDecoderImpl *m_pKdvDecoder;

	/** mpeg4码流解析. */
	CMPEG4StreamParse m_cMediaStreamParse;

	/** 录像。 */
	CFFMpegRecorderImpl m_cFFRecoder;

	/** 是否在录像。 */
	BOOL m_bRecording;

	/** 线程安全锁。 */
	mutable CSDLMutex m_threadSafeLock;

	/** 码率.。*/
	int m_nBandWidth;
};
