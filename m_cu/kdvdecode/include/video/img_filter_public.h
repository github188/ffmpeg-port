/******************************************************************************
模块名	    ：img_filter_public
文件名	    ：img_filter_public.h            
相关文件	：
文件实现功能：定义图像滤波的参数结构体,目前制包含添加马赛克
作者		：zwenyi
版本		：1.0
-------------------------------------------------------------------------------
修改记录:
日  期			版本			修改人		走读人    修改内容
2007/09/14		1.0				zwenyi                  创建
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
	l32 l32WindowLeft;		//加马赛克区域的左边界
	l32 l32WindowTop;		//加马赛克区域的上边界
	l32 l32WindowWidth;		//加马赛克区域的宽度
	l32 l32WindowHeight;	//加马赛克区域的高度
	l32 l32Granularity;		//马赛克的粒度
}TMosaicInfo;

//加马赛克的参数
typedef struct  
{
	l32 l32Width;			//图像的宽度
	l32 l32Height;			//图像的高度
	l32 l32YStride;			//图像Y分量的跨度
	l32 l32UVStride;		//图像UV分量的跨度
	l32 l32YUVType;			//图像的图像类型
	l32 l32IsInterlace;		//图像的帧场类型

	l32 l32MosaicNum;		//添加马赛克的实际路数
	TMosaicInfo tMosaicInfo[32];
}TImageMosaicParam;

//加马赛克模块状态结构体
#define TImageMosaicStatusParam TImageMosaicParam

//加马赛克的输入参数
typedef struct
{
	u8 *pu8Input;			//源图像
	u8 *pu8Output;			//目标图象(源图像和目标图像可以一致)
}TImageMosaicInput;

//加马赛克的输出参数
typedef struct
{
	u32 u32Reserved;		//保留参数
}TImageMosaicOutput;

//OSD filter的参数
typedef struct
{
    u8  u8Mask11;           //平滑滤波强度
    s16 s16Brightness;      //亮度参数
    s16 s16Contrast;        //对比度参数
    l32 l32Width;           //图像的宽度
    l32 l32Height;          //图像的高度
    u32 *pu32RefPic;        //参考图像地址
    u8  u8Sign;             //是否有新的参考图像传入标志位    
}TFilterOSDParam;

//OSD filter的输入参数
typedef struct
{
    u32 *pu32Input;			//源图像
	u32 *pu32Output;		//目标图象(源图像和目标图像可以一致)
}TFilterOSDInput;

//OSD filter的输出参数
typedef struct
{
	u32 u32Reserved;		//保留参数
}TFilterOSDOutput;

#ifdef __cplusplus
}
#endif

#endif //_IMG_FILTER_PUBLIC_H_
