#pragma once

#include "decoder.h"
#include "scopelock.h"
#include "encoder.h"
/** keda������.
*	��װkeda��������ʵ��.
*	��������������ֶ�keda����������ļ�������. 
*	
*	Ϊ�˷�ֹ�ڴ���Ƭ������keda���������Ӵ����࣬�ɱ����ظ��������������ý�������ӿڡ� 2009.10.27 by lzx
*/

extern "C" 
{ 
	typedef struct TVideoUnitDecParam TVideoUnitDecParam; 
}

class CKDVDecoderImpl
{
public:
	/** ��ȡ��������������ڣ����������ò�����. 
	*	����������򴴽���
	*/
	static CKDVDecoderImpl *GetDecoder( CDecoder::ECodecId eCodecId, LPCTSTR strName = NULL );
	/** �ͷ�. */
	static void Release( CKDVDecoderImpl *pDecoder );
	/** �������ֲ��ҽ�����. */
	static CKDVDecoderImpl *FindDecoder( LPCTSTR strName );
	/** �ͷ�����. */
	static void ReleaseAll();
private:
	static void RegDec( LPCTSTR lpName, CKDVDecoderImpl *pDec );
private:
	/** ����������-������ӳ���. */
	typedef std::map< tstring, CKDVDecoderImpl * > TDecoderNameTable;
	static TDecoderNameTable s_tDecoderNameTable;
	/** Ϊ����������ӳ��������֤�̰߳�ȫ����. */
	static CMCUMutex s_decoderNameTableLock;
public:
	CKDVDecoderImpl(void);
	~CKDVDecoderImpl(void);

	/** ����. 
	*	�����ظ����á�
	*	�ڲ����жϣ������һ�ε������ʼ������⡣������ǵ�һ����������ò����ӿڡ�
	*/
	BOOL Create( CDecoder::ECodecId eCodec );

	/** ����. */
	BOOL Release();

	/** ����һ֡. 
	*	��������,���ͼ��. 
	*	������Ҫ������������������һ֡. 
	*	ע��!�˺������ص�ͼƬ��ʹ�����ڲ�������,������һ�ν���ʱ�������ϴν����ͼƬ����..
	*/
	BOOL Decode(  const CBaseCodec::TStreamPacket& tPkt, CDecoder::TVideoPicture& tPicture, CDecoder::TVideoFrameInfo *pFrameInfo = NULL );

	/** ���������ʽ. */

	/** ץ�ġ� */
	BOOL CapturePic( LPCTSTR strPicPath, CEncoder *pEnc );

private:
	void PrintErrorInfo(void * pvHandle, mu_int32 l32ResultForAnalyze);
private:

	/** �������ͼƬ�Ĵ�С�� 
	*	+by lizhixing@2010/6/24
	*/
	mu_uint32 m_nOutputImgWidth;
	mu_uint32 m_nOutputImgHeight;

	TVideoUnitDecParam *m_pDecParam;

	/** ���������. */
	void *m_pvHandle;

	/** �̰߳�ȫ. 
	*	keda�����ӿ�ֻ�ܵ��̷߳���.
	*/
	CMCUMutex m_threadSafeLock;

	/** �ڲ�ͼƬ������. */
	mu_uint8 *m_pPicDataBuf;

	/** ����������ݵĻ������� */
	mu_uint8 *m_pCaptureDataBuf;
	/** �������ݻ�������С��*/
	mu_uint32 m_nCaptureDataBufLen;
};
