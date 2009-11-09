#pragma once

#include "mcudefine.h"
#include "mcudef.h"

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
		timeval	timeStamp;	// ʱ�����
	};

	/** ͼ��. */
	/**
	* four components are given, that's all.
	* the last component is alpha
	*/
	typedef struct TVideoPicture 
	{		
		mu_uint8 *data[4];		/** image data. */
		int linesize[4];       ///< number of bytes per line
		EVideoPictrueType type;
		mu_int32 picWidth;			// ͼ����.
		mu_int32 picHeight;			// ͼ��߶�.

		TVideoPicture()
		{
			memset( this, 0, sizeof( TVideoPicture ) );
		}
	} TVideoPicture;

	// ͼ��֡��Ϣ
	struct TVideoFrameInfo
	{
		mu_int32	frameWidth;     //����ͼ���ʵ�ʿ��
		mu_int32	frameHeight;    //����ͼ���ʵ�ʸ߶�
		EFrameType	frameType;		//����֡����
		timeval	frameTimeStamp;	// ʱ�����

		TVideoFrameInfo()
		{
			memset( this, 0, sizeof( TVideoFrameInfo ) );
		}
	};
};
