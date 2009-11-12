#pragma once

/** 解码器. */


#include <vector>
#include <map>
#include "scopelock.h"
#include "basecodec.h"
#include "encoder.h"

class CDecoder : public CBaseCodec
{
public:


	/** 图像回调函数. */
	typedef BOOL  ( MU_CALL *PictrueCallback )( const TVideoPicture *pic, const TVideoFrameInfo *pFrameInfo, void *param );



protected:
	CDecoder(void);
	virtual ~CDecoder(void);

public:
	/** 创建解码器. */
	static CDecoder *CreateDecoder( ECodecId eCodecId, int nBandWidth, LPCTSTR strName = NULL );
	/** 释放. */
	static void Release( CDecoder *pDecoder );
	/** 根据名字查找解码器. */
	static CDecoder *FindDecoder( LPCTSTR strName );
	/** 释放所有. */
	static void ReleaseAll();

	/** 初始化. */
	virtual BOOL Init( int nBandWidth ) = 0;

	/** 设置输出格式. */
	virtual BOOL SetOutPictureParam( mu_int32 width, mu_int32 height ){return FALSE;};

	/** 同步解码方式. */
	virtual mu_int32 InputStreamSync( const TStreamPacket& pkt ) = 0;

	/** 输入码流.异步解码. */
	virtual BOOL InputStream( void *pStreamBuf, mu_uint32 nBufLen ){return FALSE;};

	/** 设置解码图像数据回调. 
	*	暂时使用静态的回调函数及参数
	*/
	static BOOL SetPictueCallback( PictrueCallback pFun, void *param );

	/** 获取最近一帧的图像. */

	/** 抓拍. 
	*	同步。抓拍当前帧图像。
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath );



	/** 录像. */
	virtual BOOL StartRecord( LPCTSTR strFileName ) = 0;
	virtual BOOL StopRecord() = 0;
	virtual BOOL IsRecording() const= 0;

protected:
	/** 抓拍. 
	*	同步。抓拍当前帧图像。
	*/
	virtual BOOL CapturePic( LPCTSTR strPicPath, CEncoder *pEnc ) = 0;

protected:
	/** 解码结果通知. */
	virtual void OnDecodeResult( BOOL bSuccess, TVideoPicture *pPic, TVideoFrameInfo* pFrameInfo );

	static void RegDec( LPCTSTR lpName, CDecoder *pDec );
	static void UnRegDec( CDecoder * pDec );

private:
	/** 解码线程. */
	static mu_int32 ThreadDecode( void * threadParam );

	/** 图像回调线程. */
	static mu_int32 ThreadImageOut( void * threadParam );

private:

	/** 解码器名称-解码器映射表. */
	typedef std::map< tstring, CDecoder * > TDecoderNameTable;
	static TDecoderNameTable s_tDecoderNameTable;
	/** 为解码器名称映射表加锁保证线程安全的锁. */
	static CMutex s_decoderNameTableLock;

private:
	/** 用户设定的回调函数. */
	static PictrueCallback m_pPicCallBack;
	static void *m_picCallbackParam;

	/** 线程安全互斥量. */
	//	CSDLMutex m_threadSafeLock;

	/** 解码线程同步信号. */

	/** 图像回调线程同步信号. */

	/** 图像缓冲区. */
	typedef std::vector< mu_uint8 > TPictureBuf;
	TPictureBuf m_picBuf;

	/** 码流缓冲区. */
	//	typedef std::pair< mu_uint8 *, mu_uint32 > TStreamPacket;
	typedef std::vector< TStreamPacket > TPacketList;

	TPacketList m_tStreamPacketList;
};
