/******************************************************************************
ģ����	    ��img_renovation_public
�ļ���	    ��img_renovation_public.h            
����ļ�	��
�ļ�ʵ�ֹ��ܣ�����ͼ��ˢ�µĲ����ṹ��
����		������
�汾		��1.0
-------------------------------------------------------------------------------
�޸ļ�¼:
��  ��			�汾			�޸���		�߶���    �޸�����
2008/11/24		1.0				����                  ����
******************************************************************************/
#ifndef _IMG_RENOVATION_PUBLIC_H_
#define _IMG_RENOVATION_PUBLIC_H_

#include "videodefines.h"

#ifdef __cplusplus
extern "C"
{
#endif

//ͼ��ˢ�����ò����ṹ��
typedef struct
{
    l32 l32PatternType; //ͼ��ˢ��ͼ������(RAINBOW��PURE_COLOR��
	l32 l32Width;       //ͼ����
	l32 l32Height;      //ͼ��߶�
	l32 l32SrcYUVType;  //YUV����(YUV420��YUV422)
	l32 l32Interlace;   //֡������(FRAME_FORMAT��FIELD_FORMAT)
	u32 u32R;           //R����ֵ(����ɫͼ��ʹ��)
	u32 u32G;           //G����ֵ(����ɫͼ��ʹ��)
	u32 u32B;           //B����ֵ(����ɫͼ��ʹ��)
	u32 u32Reserved;    //��������
}TImageRenovationParam;

//ͼ��ˢ�²���״̬�ṹ��
#define TImageRenovationStatusParam TImageRenovationParam

//ͼ��ˢ�µ��������
typedef struct  
{
    u8 *pu8YAddr;			//ͼ��Y������ַ
	u8 *pu8UAddr;			//ͼ��U������ַ
	u8 *pu8VAddr;			//ͼ��V������ַ
	l32 l32YStride;         //ͼ���Y����ֵ
	l32 l32UVStride;		//ͼ���UV����ֵ
    u32 u32Reserved;        //��������
}TImageRenovationInput;

//ͼ��ˢ�µ��������
typedef struct  
{
    u32 u32Reserved;        //��������
}TImageRenovationOutput;

#ifdef __cplusplus
}
#endif

#endif // _IMG_RENOVATION_PUBLIC_H_
