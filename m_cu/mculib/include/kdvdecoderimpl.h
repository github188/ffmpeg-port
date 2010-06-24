#pragma once

#include "decoder.h"
#include "scopelock.h"
#include "encoder.h"
/** keda解码器.
*	封装keda解码器的实现.
*	剥离程序其它部分对keda解码器相关文件的依赖. 
*	
*	为了防止内存碎片发生，keda解码器增加创建类，可避免重复创建。增加设置解码参数接口。 2009.10.27 by lzx
*/

extern "C" 
{ 
	typedef struct TVideoUnitDecParam TVideoUnitDecParam; 
}

class CKDVDecoderImpl
{
public:
	/** 获取解码器，如果存在，则重新设置参数。. 
	*	如果不存在则创建。
	*/
	static CKDVDecoderImpl *GetDecoder( CDecoder::ECodecId eCodecId, LPCTSTR strName = NULL );
	/** 释放. */
	static void Release( CKDVDecoderImpl *pDecoder );
	/** 根据名字查找解码器. */
	static CKDVDecoderImpl *FindDecoder( LPCTSTR strName );
	/** 释放所有. */
	static void ReleaseAll();
private:
	static void RegDec( LPCTSTR lpName, CKDVDecoderImpl *pDec );
private:
	/** 解码器名称-解码器映射表. */
	typedef std::map< tstring, CKDVDecoderImpl * > TDecoderNameTable;
	static TDecoderNameTable s_tDecoderNameTable;
	/** 为解码器名称映射表加锁保证线程安全的锁. */
	static CMCUMutex s_decoderNameTableLock;
public:
	CKDVDecoderImpl(void);
	~CKDVDecoderImpl(void);

	/** 创建. 
	*	可以重复调用。
	*	内部作判断，如果第一次调用则初始化解码库。如果不是第一次则调用设置参数接口。
	*/
	BOOL Create( CDecoder::ECodecId eCodec );

	/** 销毁. */
	BOOL Release();

	/** 解码一帧. 
	*	传入码流,输出图像. 
	*	解码器要求输入码流是完整的一帧. 
	*	注意!此函数返回的图片会使用类内部缓冲区,所以下一次解码时会清理上次解码的图片数据..
	*/
	BOOL Decode(  const CBaseCodec::TStreamPacket& tPkt, CDecoder::TVideoPicture& tPicture, CDecoder::TVideoFrameInfo *pFrameInfo = NULL );

	/** 设置输出格式. */

	/** 抓拍。 */
	BOOL CapturePic( LPCTSTR strPicPath, CEncoder *pEnc );

private:
	void PrintErrorInfo(void * pvHandle, mu_int32 l32ResultForAnalyze);
private:

	/** 解码输出图片的大小。 
	*	+by lizhixing@2010/6/24
	*/
	mu_uint32 m_nOutputImgWidth;
	mu_uint32 m_nOutputImgHeight;

	TVideoUnitDecParam *m_pDecParam;

	/** 解码器句柄. */
	void *m_pvHandle;

	/** 线程安全. 
	*	keda解码库接口只能单线程访问.
	*/
	CMCUMutex m_threadSafeLock;

	/** 内部图片缓冲区. */
	mu_uint8 *m_pPicDataBuf;

	/** 缓存编码数据的缓冲区。 */
	mu_uint8 *m_pCaptureDataBuf;
	/** 编码数据缓冲区大小。*/
	mu_uint32 m_nCaptureDataBufLen;
};
