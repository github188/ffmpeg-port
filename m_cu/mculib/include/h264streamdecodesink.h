#pragma once

#include "MediaSink.hh"
#include "decoder.h"
#ifdef USE_FFMPEG
#include "ffplay.h"
#endif

class CH264StreamDecodeSink : public MediaSink
{
public:
	/** ��������.
	*	nBandWidth: ����.��λ:kbps.
	*/
	static CH264StreamDecodeSink *CreateNew( UsageEnvironment& env, unsigned bufferSize, int nBandWidth );

protected:
	CH264StreamDecodeSink( UsageEnvironment& env, unsigned bufferSize, int nBandWidth );
	virtual ~CH264StreamDecodeSink(void);

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

	timeval m_timeStamp;

private: // redefined virtual functions:
	virtual Boolean continuePlaying();

private:
#ifdef  USE_FFMPEG
	DecoderID m_decoderId;	
#endif

	/** �����. */
	CDecoder *m_pDecoder;

	/** ����.kbps */
	int m_nBandWidth;
};
