#pragma once

#include "basecodec.h"
#include "ScopeLock.h"
#include "mcudefine.h"

/** keda编码器.
*	封装keda编码器的实现.
*	剥离程序其它部分对keda编码器相关文件的依赖. */

extern "C" 
{ 
	typedef struct TVideoUnitEncParam TVideoUnitEncParam; 
}

class CKDVEncoderImpl
{
public:
	CKDVEncoderImpl(void);
	virtual ~CKDVEncoderImpl(void);


	/** 创建. */
	BOOL Create( CBaseCodec::ECodecId eCodec );

	/** 销毁. */
	BOOL Release();

	/** 编码一帧. 
	*	传入图像,输出编码后的数据. 
	*	
	*/
	BOOL Encode( const CBaseCodec::TVideoPicture& tPicture, void *pStramBuf, mu_uint32 & nBufLen );


	/** 编码后需要的内存空间大小。 */
	virtual mu_uint32 GetMemSize( const CBaseCodec::TVideoPicture& tVideoPic );
private:
	void PrintErrorInfo(void * pvHandle, mu_int32 l32ResultForAnalyze);
private:
	TVideoUnitEncParam *m_pEncParam;

	/** 解码器句柄. */
	void * m_pvHandle;

	/** 线程安全. 
	*	keda解码库接口只能单线程访问.
	*/
	CSDLMutex m_threadSafeLock;

	/** 内部缓冲区.存储编码后的码流 */
	mu_uint8 *m_pDataBuf;
	/** 缓冲区大小。 */
	mu_uint32 m_nDataBufLen;

	/** 图片缓冲区。 */
	mu_uint8 *m_pPicBuf;
	/** 图片缓冲区大小。*/
	mu_uint32 m_nPicBufLen;
};
