#pragma once

#include "basecodec.h"
#include "mcudefine.h"

// FFMpeg结构的声明。
extern "C"
{
	struct AVFormatContext;
	struct AVStream;
//	enum CodecID;
};

class CFFMpegRecorderImpl
{
public:
	CFFMpegRecorderImpl(void);
	virtual ~CFFMpegRecorderImpl(void);

	/** 初始化。
	*/
	BOOL Init( CBaseCodec::ECodecId eCodec, int nBandWidth, LPCTSTR strFileName );

	/** 按帧输入码流。
	*	携带解码时得到的信息。
	*/
	BOOL WriteFrame( mu_uint8 *buf, int len, const CBaseCodec::TVideoFrameInfo& tFrameInfo );

	/** 结束。*/
	BOOL CloseFile();

private:
	/** 释放资源。 */
	BOOL Release();

	/** 获取FFMpeg中的codec id */
	mu_int32 GetFFCodecId( CBaseCodec::ECodecId eMUCodecId ) const;

	/** 时间戳转换。 */
//	mu_uint64 GetTimeStamp( const timeval &time ) const;

private:
#ifdef _WIN32_WCE
	/** ffmpeg的录像信息。*/
	AVFormatContext *m_pavFormatContext;

	/** 流。 */
	AVStream *m_pavStream;
#endif
	/** 开始时间。单位是毫秒。 */
	mu_uint64 m_timeBegin;

	/** 结束时间，单位是毫秒。 */
	mu_uint64 m_timeEnd;

	/** 数据量。 */
	mu_uint64 m_nDataSize;

	/** 帧数统计。 */
	mu_int32 m_nFrameCount;

	/** 码率。 */
	mu_int32 m_nBandWidth;

	/** 录像文件. */
	tstring m_strRecordFilePath;

    /** 线程同步锁。*/
    CMCUMutex m_threadSafeLock;

	/** 文件协议是否注册过。*/
	static BOOL s_bFileProtocolReg;

};
