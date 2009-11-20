#pragma once

#include "MediaSink.hh"
#include "decoder.h"


class CH264StreamDecodeSink : public MediaSink
{
public:
	/** 创建对象.
	*	nBandWidth: 码率.单位:kbps.
	*/
	static CH264StreamDecodeSink *CreateNew( UsageEnvironment& env, unsigned bufferSize, int nBandWidth );

protected:
	CH264StreamDecodeSink( UsageEnvironment& env, unsigned bufferSize, int nBandWidth );
	virtual ~CH264StreamDecodeSink(void);

	/** 解码。 */
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

	timeval m_timeStamp;

private: // redefined virtual functions:
	virtual Boolean continuePlaying();

private:

	/** 解码库. */
	CDecoder *m_pDecoder;

	/** 码率.kbps */
	int m_nBandWidth;
};
