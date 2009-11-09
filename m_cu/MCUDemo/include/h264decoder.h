#pragma once
#include "decoder.h"

#include "kdvdecoderimpl.h"
#include "h264streamparse.h"
#include "ffmpegrecorderimpl.h"

class CH264Decoder :
	public CDecoder
{

public:
	CH264Decoder(void);
	virtual ~CH264Decoder(void);

	/** 同步解码方式. */
	virtual mu_int32 InputStreamSync( const TStreamPacket& pkt );

	/** 抓拍. 
	*	同步。抓拍当前帧图像。
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath , CEncoder *pEnc );

	/** 录像. */
	virtual BOOL StartRecord( LPCTSTR strFileName ) ;
	virtual BOOL StopRecord();
	virtual BOOL IsRecording() const;

protected:
	/** 解码器初始化. */
	virtual BOOL Init( int nBandWidth ) ;
private:
	//	CKDVDecoderImpl m_cKdvDecoder;
	CKDVDecoderImpl *m_pKdvDecoder;

	/** H264码流解析类. */
	CH264StreamParse m_cH264StreamParse;

	/** 录像。 */
	CFFMpegRecorderImpl m_cFFRecoder;

	/** 是否在录像。 */
	BOOL m_bRecording;

	/** 线程安全锁。 */
	mutable CSDLMutex m_threadSafeLock;

	/** 码率。 */
	int m_nBandWidth;
};
