#pragma once

#include "mcudefine.h"
#include "mcudef.h"

class CBaseCodec
{
public:
	CBaseCodec(void);
	virtual ~CBaseCodec(void);

public:
	/** 解码器id. */
	enum ECodecId
	{
		CODEC_NULL,
		CODEC_H264,
		CODEC_MPEG4,
		CODEC_JPEG,
	};

	/** 图像格式. */
	enum EVideoPictrueType
	{
		PIC_YUV420P,

	};

	//图像的编解码格式(I帧或者P帧)
	enum EFrameType
	{
		FRAME_UNKNOW = 0,	// 未知类型。
		FRAME_I = 0x01,		//I帧
		FRAME_P,			//P帧
		//SKIP_PICTURE,      //跳过帧
		//DROP_PICTURE       //丢帧
	};

	/** 码流包。 */
	struct TStreamPacket
	{
		mu_uint8 *	data;		// 指向码流缓冲区。
		mu_uint32	datalen;	// 码流长度。
		timeval	timeStamp;	// 时间戳。
	};

	/** 图像. */
	/**
	* four components are given, that's all.
	* the last component is alpha
	*/
	typedef struct TVideoPicture 
	{		
		mu_uint8 *data[4];		/** image data. */
		int linesize[4];       ///< number of bytes per line
		EVideoPictrueType type;
		mu_int32 picWidth;			// 图像宽度.
		mu_int32 picHeight;			// 图像高度.

		TVideoPicture()
		{
			memset( this, 0, sizeof( TVideoPicture ) );
		}
	} TVideoPicture;

	// 图像帧信息
	struct TVideoFrameInfo
	{
		mu_int32	frameWidth;     //解码图象的实际宽度
		mu_int32	frameHeight;    //解码图象的实际高度
		EFrameType	frameType;		//解码帧类型
		timeval	frameTimeStamp;	// 时间戳。

		TVideoFrameInfo()
		{
			memset( this, 0, sizeof( TVideoFrameInfo ) );
		}
	};
};
