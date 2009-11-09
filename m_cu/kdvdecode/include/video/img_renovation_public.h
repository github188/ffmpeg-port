/******************************************************************************
模块名	    ：img_renovation_public
文件名	    ：img_renovation_public.h            
相关文件	：
文件实现功能：定义图像刷新的参数结构体
作者		：王建
版本		：1.0
-------------------------------------------------------------------------------
修改记录:
日  期			版本			修改人		走读人    修改内容
2008/11/24		1.0				王建                  创建
******************************************************************************/
#ifndef _IMG_RENOVATION_PUBLIC_H_
#define _IMG_RENOVATION_PUBLIC_H_

#include "videodefines.h"

#ifdef __cplusplus
extern "C"
{
#endif

//图像刷新设置参数结构体
typedef struct
{
    l32 l32PatternType; //图像刷新图案类型(RAINBOW、PURE_COLOR）
	l32 l32Width;       //图像宽度
	l32 l32Height;      //图像高度
	l32 l32SrcYUVType;  //YUV类型(YUV420、YUV422)
	l32 l32Interlace;   //帧场类型(FRAME_FORMAT、FIELD_FORMAT)
	u32 u32R;           //R分量值(仅纯色图像使用)
	u32 u32G;           //G分量值(仅纯色图像使用)
	u32 u32B;           //B分量值(仅纯色图像使用)
	u32 u32Reserved;    //保留参数
}TImageRenovationParam;

//图像刷新参数状态结构体
#define TImageRenovationStatusParam TImageRenovationParam

//图像刷新的输入参数
typedef struct  
{
    u8 *pu8YAddr;			//图像Y分量地址
	u8 *pu8UAddr;			//图像U分量地址
	u8 *pu8VAddr;			//图像V分量地址
	l32 l32YStride;         //图像的Y步进值
	l32 l32UVStride;		//图像的UV步进值
    u32 u32Reserved;        //保留参数
}TImageRenovationInput;

//图像刷新的输出参数
typedef struct  
{
    u32 u32Reserved;        //保留参数
}TImageRenovationOutput;

#ifdef __cplusplus
}
#endif

#endif // _IMG_RENOVATION_PUBLIC_H_
