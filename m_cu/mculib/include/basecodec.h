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
	/** ������id. */
	enum ECodecId
	{
		CODEC_NULL,
		CODEC_H264,
		CODEC_MPEG4,
		CODEC_JPEG,
	};

	/** ͼ���ʽ. */
	enum EVideoPictrueType
	{
		PIC_YUV420P,

	};

	//ͼ��ı�����ʽ(I֡����P֡)
	enum EFrameType
	{
		FRAME_UNKNOW = 0,	// δ֪���͡�
		FRAME_I = 0x01,		//I֡
		FRAME_P,			//P֡
		//SKIP_PICTURE,      //����֡
		//DROP_PICTURE       //��֡
	};

	/** �������� */
	struct TStreamPacket
	{
		mu_uint8 *	data;		// ָ��������������
		mu_uint32	datalen;	// �������ȡ�
		mu_uint64	timeStamp;	// ʱ�����
	};

	/** ͼ��. */
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
		mu_int32 picWidth;			// ͼ����.
		mu_int32 picHeight;			// ͼ��߶�.

		TVideoPicture()
		{
			memset( this, 0, sizeof( TVideoPicture ) );
		}
		
		/** ����һ�ݳ����� */
		BOOL Copy( const TVideoPicture& tAnother )
		{
			if ( this == &tAnother )
			{
				return TRUE;
			}
			
			this->type = tAnother.type;
			this->picWidth = tAnother.picWidth;
			this->picHeight = tAnother.picHeight;
			// �����ڴ�copy��
			for ( int i=0; i<VIDEO_PICTURE_DIM; ++i )
			{
				if ( tAnother.data[i] && tAnother.linesize[i]>0 )
				{
					// �����ڴ档
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

		/** ɾ���ڴ棬����ڴ治��������ģ��벻Ҫ���ã� */
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

	// ͼ��֡��Ϣ
	struct TVideoFrameInfo
	{
		mu_int32	frameWidth;     //����ͼ���ʵ�ʿ��
		mu_int32	frameHeight;    //����ͼ���ʵ�ʸ߶�
		EFrameType	frameType;		//����֡����
		mu_uint64	frameTimeStamp;	// ʱ�����

		TVideoFrameInfo()
		{
			memset( this, 0, sizeof( TVideoFrameInfo ) );
		}
	};
};
