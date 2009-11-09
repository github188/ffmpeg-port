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

	/** ͬ�����뷽ʽ. */
	virtual mu_int32 InputStreamSync( const TStreamPacket& pkt );

	/** ¼��. */
	virtual BOOL StartRecord( LPCTSTR strFileName ) ;
	virtual BOOL StopRecord() ;
	virtual BOOL IsRecording() const;

protected:
	/** ��������ʼ��. */
	virtual BOOL Init( int nBandWidth ) ;


	/** ץ��. 
	*	ͬ����ץ�ĵ�ǰ֡ͼ��
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath , CEncoder *pEnc );

private:
	/** keda ������. */
	CKDVDecoderImpl *m_pKdvDecoder;

	/** mpeg4��������. */
	CMPEG4StreamParse m_cMediaStreamParse;

	/** ¼�� */
	CFFMpegRecorderImpl m_cFFRecoder;

	/** �Ƿ���¼�� */
	BOOL m_bRecording;

	/** �̰߳�ȫ���� */
	mutable CSDLMutex m_threadSafeLock;

	/** ����.��*/
	int m_nBandWidth;
};
