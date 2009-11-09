#pragma once

#include "MediaSink.hh"
#include "decoder.h"
#ifdef USE_FFMPEG
#include "ffplay.h"
#endif

class CMpeg4StreamDecodeSink : public MediaSink
{
public:
	/** ����. 
	*	nBandWidth: ����,��λkbps.
	*/
	static CMpeg4StreamDecodeSink *CreateNew( UsageEnvironment& env, unsigned bufferSize, int nBandWidth );

protected:
	CMpeg4StreamDecodeSink( UsageEnvironment& env, unsigned bufferSize, int nBandWidth );
	virtual ~CMpeg4StreamDecodeSink(void);

	/** ���롣 */
	Boolean DecodeData();

protected:
	static void afterGettingFrame(void* clientData, unsigned frameSize,
		unsigned numTruncatedBytes,
		timeval presentationTime,
		unsigned durationInMicroseconds);
	virtual void afterGettingFrame1(unsigned frameSize,
		timeval presentationTime);

	unsigned char* m_pBuffer;
	unsigned m_nDataBufferSize;
	unsigned char* m_pData;
	unsigned m_nDataLen;

	/** ʱ�����*/
	timeval m_timeStamp;

private: // redefined virtual functions:
	virtual Boolean continuePlaying();

private:
#ifdef USE_FFMPEG
	DecoderID m_decoderId;
#endif

	/** ������. */
	CDecoder *m_pDecoder;

	/** ����.kbps */
	int m_nBandWidth;
};
