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

	/** ͬ�����뷽ʽ. */
	virtual mu_int32 InputStreamSync( const TStreamPacket& pkt );

	/** ץ��. 
	*	ͬ����ץ�ĵ�ǰ֡ͼ��
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath , CEncoder *pEnc );

	/** ¼��. */
	virtual BOOL StartRecord( LPCTSTR strFileName ) ;
	virtual BOOL StopRecord();
	virtual BOOL IsRecording() const;

protected:
	/** ��������ʼ��. */
	virtual BOOL Init( int nBandWidth ) ;
private:
	//	CKDVDecoderImpl m_cKdvDecoder;
	CKDVDecoderImpl *m_pKdvDecoder;

	/** H264����������. */
	CH264StreamParse m_cH264StreamParse;

	/** ¼�� */
	CFFMpegRecorderImpl m_cFFRecoder;

	/** �Ƿ���¼�� */
	BOOL m_bRecording;

	/** �̰߳�ȫ���� */
	mutable CSDLMutex m_threadSafeLock;

	/** ���ʡ� */
	int m_nBandWidth;
};
