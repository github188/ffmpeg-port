#pragma once

#include "mcudefine.h"
#include "mem.h"
#include "log.h"

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
		mu_uint64	timeStamp;	// 时间戳。
	};

	/** 图像. */
	/**
	* four components are given, that's all.
	* the last component is alpha
	*/
	typedef struct TVideoPicture 
	{		
#define VIDEO_PICTURE_DIM (4)
		mu_uint8 *data[VIDEO_PICTURE_DIM];		/** image data. */
		mu_uint32 linesize[VIDEO_PICTURE_DIM];       ///< number of bytes per line
		EVideoPictrueType type;
		mu_int32 picWidth;			// 图像宽度.
		mu_int32 picHeight;			// 图像高度.

		TVideoPicture()
		{
			memset( this, 0, sizeof( TVideoPicture ) );
		}
		
		/** 拷贝一份出来。 */
		BOOL Copy( const TVideoPicture& tAnother )
		{
			if ( this == &tAnother )
			{
				return TRUE;
			}
			
			this->type = tAnother.type;
			this->picWidth = tAnother.picWidth;
			this->picHeight = tAnother.picHeight;
			// 进行内存copy。
			for ( int i=0; i<VIDEO_PICTURE_DIM; ++i )
			{
				if ( tAnother.data[i] && tAnother.linesize[i]>0 )
				{
					// 申请内存。
					this->data[i] = (mu_uint8*)mu_fast_realloc( this->data[i], &this->linesize[i], tAnother.linesize[i] );
					if ( NULL == this->data[i] )
					{
						mcu::tlog << _T( "TVideoPicture malloc memery failed when copy!!" ) << endl;
						return FALSE;
					}
					this->linesize[i] = tAnother.linesize[i];
					memcpy( this->data[i], tAnother.data[i], this->linesize[i] );
				}
			}		
			return TRUE;
		}

		/** 删除内存，如果内存不是它申请的，请不要调用！ */
		BOOL Free()
		{
			for ( int i=0; i<VIDEO_PICTURE_DIM; ++i )
			{
				mu_freep( &this->data[i] );
				this->linesize[i] = 0;
			}
			return TRUE;
		}

	} TVideoPicture;

	// 图像帧信息
	struct TVideoFrameInfo
	{
		mu_int32	frameWidth;     //解码图象的实际宽度
		mu_int32	frameHeight;    //解码图象的实际高度
		EFrameType	frameType;		//解码帧类型
		mu_uint64	frameTimeStamp;	// 时间戳。

		TVideoFrameInfo()
		{
			memset( this, 0, sizeof( TVideoFrameInfo ) );
		}
	};
};
