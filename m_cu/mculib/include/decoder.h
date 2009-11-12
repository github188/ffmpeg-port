#pragma once

/** ������. */


#include <vector>
#include <map>
#include "scopelock.h"
#include "basecodec.h"
#include "encoder.h"

class CDecoder : public CBaseCodec
{
public:


	/** ͼ��ص�����. */
	typedef BOOL  ( MU_CALL *PictrueCallback )( const TVideoPicture *pic, const TVideoFrameInfo *pFrameInfo, void *param );



protected:
	CDecoder(void);
	virtual ~CDecoder(void);

public:
	/** ����������. */
	static CDecoder *CreateDecoder( ECodecId eCodecId, int nBandWidth, LPCTSTR strName = NULL );
	/** �ͷ�. */
	static void Release( CDecoder *pDecoder );
	/** �������ֲ��ҽ�����. */
	static CDecoder *FindDecoder( LPCTSTR strName );
	/** �ͷ�����. */
	static void ReleaseAll();

	/** ��ʼ��. */
	virtual BOOL Init( int nBandWidth ) = 0;

	/** ���������ʽ. */
	virtual BOOL SetOutPictureParam( mu_int32 width, mu_int32 height ){return FALSE;};

	/** ͬ�����뷽ʽ. */
	virtual mu_int32 InputStreamSync( const TStreamPacket& pkt ) = 0;

	/** ��������.�첽����. */
	virtual BOOL InputStream( void *pStreamBuf, mu_uint32 nBufLen ){return FALSE;};

	/** ���ý���ͼ�����ݻص�. 
	*	��ʱʹ�þ�̬�Ļص�����������
	*/
	static BOOL SetPictueCallback( PictrueCallback pFun, void *param );

	/** ��ȡ���һ֡��ͼ��. */

	/** ץ��. 
	*	ͬ����ץ�ĵ�ǰ֡ͼ��
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath );



	/** ¼��. */
	virtual BOOL StartRecord( LPCTSTR strFileName ) = 0;
	virtual BOOL StopRecord() = 0;
	virtual BOOL IsRecording() const= 0;

protected:
	/** ץ��. 
	*	ͬ����ץ�ĵ�ǰ֡ͼ��
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath, CEncoder *pEnc ) = 0;

protected:
	/** ������֪ͨ. */
	virtual void OnDecodeResult( BOOL bSuccess, TVideoPicture *pPic, TVideoFrameInfo* pFrameInfo );

	static void RegDec( LPCTSTR lpName, CDecoder *pDec );
	static void UnRegDec( CDecoder * pDec );

private:
	/** �����߳�. */
	static mu_int32 ThreadDecode( void * threadParam );

	/** ͼ��ص��߳�. */
	static mu_int32 ThreadImageOut( void * threadParam );

private:

	/** ����������-������ӳ���. */
	typedef std::map< tstring, CDecoder * > TDecoderNameTable;
	static TDecoderNameTable s_tDecoderNameTable;
	/** Ϊ����������ӳ��������֤�̰߳�ȫ����. */
	static CMutex s_decoderNameTableLock;

private:
	/** �û��趨�Ļص�����. */
	static PictrueCallback m_pPicCallBack;
	static void *m_picCallbackParam;

	/** �̰߳�ȫ������. */
	//	CSDLMutex m_threadSafeLock;

	/** �����߳�ͬ���ź�. */

	/** ͼ��ص��߳�ͬ���ź�. */

	/** ͼ�񻺳���. */
	typedef std::vector< mu_uint8 > TPictureBuf;
	TPictureBuf m_picBuf;

	/** ����������. */
	//	typedef std::pair< mu_uint8 *, mu_uint32 > TStreamPacket;
	typedef std::vector< TStreamPacket > TPacketList;

	TPacketList m_tStreamPacketList;
};
