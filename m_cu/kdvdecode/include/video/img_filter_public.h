/******************************************************************************
ģ����	    ��img_filter_public
�ļ���	    ��img_filter_public.h            
����ļ�	��
�ļ�ʵ�ֹ��ܣ�����ͼ���˲��Ĳ����ṹ��,Ŀǰ�ư������������
����		��zwenyi
�汾		��1.0
-------------------------------------------------------------------------------
�޸ļ�¼:
��  ��			�汾			�޸���		�߶���    �޸�����
2007/09/14		1.0				zwenyi                  ����
******************************************************************************/
#ifndef _IMG_FILTER_PUBLIC_H_
#define _IMG_FILTER_PUBLIC_H_

#include "videodefines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	l32 l32WindowLeft;		//���������������߽�
	l32 l32WindowTop;		//��������������ϱ߽�
	l32 l32WindowWidth;		//������������Ŀ��
	l32 l32WindowHeight;	//������������ĸ߶�
	l32 l32Granularity;		//�����˵�����
}TMosaicInfo;

//�������˵Ĳ���
typedef struct  
{
	l32 l32Width;			//ͼ��Ŀ��
	l32 l32Height;			//ͼ��ĸ߶�
	l32 l32YStride;			//ͼ��Y�����Ŀ��
	l32 l32UVStride;		//ͼ��UV�����Ŀ��
	l32 l32YUVType;			//ͼ���ͼ������
	l32 l32IsInterlace;		//ͼ���֡������

	l32 l32MosaicNum;		//��������˵�ʵ��·��
	TMosaicInfo tMosaicInfo[32];
}TImageMosaicParam;

//��������ģ��״̬�ṹ��
#define TImageMosaicStatusParam TImageMosaicParam

//�������˵��������
typedef struct
{
	u8 *pu8Input;			//Դͼ��
	u8 *pu8Output;			//Ŀ��ͼ��(Դͼ���Ŀ��ͼ�����һ��)
}TImageMosaicInput;

//�������˵��������
typedef struct
{
	u32 u32Reserved;		//��������
}TImageMosaicOutput;

//OSD filter�Ĳ���
typedef struct
{
    u8  u8Mask11;           //ƽ���˲�ǿ��
    s16 s16Brightness;      //���Ȳ���
    s16 s16Contrast;        //�ԱȶȲ���
    l32 l32Width;           //ͼ��Ŀ��
    l32 l32Height;          //ͼ��ĸ߶�
    u32 *pu32RefPic;        //�ο�ͼ���ַ
    u8  u8Sign;             //�Ƿ����µĲο�ͼ�����־λ    
}TFilterOSDParam;

//OSD filter���������
typedef struct
{
    u32 *pu32Input;			//Դͼ��
	u32 *pu32Output;		//Ŀ��ͼ��(Դͼ���Ŀ��ͼ�����һ��)
}TFilterOSDInput;

//OSD filter���������
typedef struct
{
	u32 u32Reserved;		//��������
}TFilterOSDOutput;

#ifdef __cplusplus
}
#endif

#endif //_IMG_FILTER_PUBLIC_H_
