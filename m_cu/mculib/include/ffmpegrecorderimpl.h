#pragma once

#include "basecodec.h"
#include "mcudefine.h"

// FFMpeg�ṹ��������
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

	/** ��ʼ����
	*/
	BOOL Init( CBaseCodec::ECodecId eCodec, int nBandWidth, LPCTSTR strFileName );

	/** ��֡����������
	*	Я������ʱ�õ�����Ϣ��
	*/
	BOOL WriteFrame( mu_uint8 *buf, int len, const CBaseCodec::TVideoFrameInfo& tFrameInfo );

	/** ������*/
	BOOL CloseFile();

private:
	/** �ͷ���Դ�� */
	BOOL Release();

	/** ��ȡFFMpeg�е�codec id */
	mu_int32 GetFFCodecId( CBaseCodec::ECodecId eMUCodecId ) const;

	/** ʱ���ת���� */
//	mu_uint64 GetTimeStamp( const timeval &time ) const;

private:
#ifdef _WIN32_WCE
	/** ffmpeg��¼����Ϣ��*/
	AVFormatContext *m_pavFormatContext;

	/** ���� */
	AVStream *m_pavStream;
#endif
	/** ��ʼʱ�䡣��λ�Ǻ��롣 */
	mu_uint64 m_timeBegin;

	/** ����ʱ�䣬��λ�Ǻ��롣 */
	mu_uint64 m_timeEnd;

	/** �������� */
	mu_uint64 m_nDataSize;

	/** ֡��ͳ�ơ� */
	mu_int32 m_nFrameCount;

	/** ���ʡ� */
	mu_int32 m_nBandWidth;

	/** ¼���ļ�. */
	tstring m_strRecordFilePath;

    /** �߳�ͬ������*/
    CMCUMutex m_threadSafeLock;

	/** �ļ�Э���Ƿ�ע�����*/
	static BOOL s_bFileProtocolReg;

};
