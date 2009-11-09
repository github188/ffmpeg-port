/******************************************************************************
模块名	    ：img_merge_zoom_public
文件名	    ：img_merge_zoom_public.h            
相关文件	：
文件实现功能：定义画面合成参数结构体
作者		：孙晓霞
版本		：1.0
-------------------------------------------------------------------------------
修改记录:
日  期			版本			修改人		走读人    修改内容
2007/09/12		1.0		        孙晓霞                 创建
2007/11/02		1.1		        孙晓霞                 添加画面合成设置边界线宽度参数
******************************************************************************/
#ifndef _IMG_MERGE_ZOOM_PUBLIC_H_
#define _IMG_MERGE_ZOOM_PUBLIC_H_

#include "videodefines.h"
#include "img_common_public.h"

//前景图像信息结构体
typedef struct
{
	l32 l32Width;         //图像的宽度
	l32 l32Height;        //图像的高度
	l32 l32IsInterlace;   //图像的类型(帧格式或者场格式)
	l32 l32YUVType;       //图标图像格式(YUV422或者YUV420)
	l32 l32DrawFocusFlag; //画面是否勾画边框的标记,u8Drawfocus为1时可用
	l32 l32FocuslineR;	  //画面边框色R
	l32 l32FocuslineG;	  //画面边框色G
	l32 l32FocuslineB;	  //画面边框色B
    l32 l32FocusWidth;    //边框统一宽度（暂不可设置）
	l32 l32Pos;			  //画面位置编号
	l32 l32NoPicInput;    //无前景图像输入标志位IMG_MERGE_NO_PIC_INPUT
	l32 l32SingleZoomStyle;  //缩放模式（用在多个Buffer输入时单个控制小图像缩放模式，PIC_ZOOM_ONE：全屏缩放，PIC_ZOOM_TWO：保持比例拉伸并保持图像完整，PIC_ZOOM_THREE：保持比例拉伸并充满目标区域，PIC_ZOOM_FOUR：保持原有尺寸）
	l32 l32Reserved;	  //保留参数
} TMergeZoomFgParam;

//画面合成模块初始化参数结构
typedef struct
{  
	l32	l32MergeStyle;	    //画面合成模式
    //l32 l32InBufStyle;      //合成输入Buf模式（MERGE_INBUF_ONE:单个Buf,MERGE_INBUF_MULTI:多个Buf）
    //l32 l32MultiZoomStyle;  //缩放模式（用在单个Buffer输入时统一控制小图像缩放模式，PIC_ZOOM_ONE：全屏缩放，PIC_ZOOM_TWO：保持比例拉伸并保持图像完整，PIC_ZOOM_THREE：保持比例拉伸并充满目标区域，PIC_ZOOM_FOUR：保持原有尺寸）
	l32 l32Width;           //背景图像的宽度
	l32 l32Height;          //背景图像的高度
	l32	l32IsInterlace;     //背景图像的类型(帧格式或者场格式)
	l32 l32YUVType;         //背景图像格式(YUV422或者YUV420)
	l32 l32BackgroundR;	    //画面合成背景色R
	l32 l32BackgroundG;	    //画面合成背景色G
	l32 l32BackgroundB;	    //画面合成背景色B
    l32 l32BoundaryR;       //画面合成各路图像统一边界线颜色R
    l32 l32BoundaryG;       //画面合成各路图像统一边界线颜色G
    l32 l32BoundaryB;       //画面合成各路图像统一边界线颜色B
    l32 l32BoundaryWidth;   //画面合成边界线宽度（缺省值为0，背景为CIF时边界线宽度默认为2，背景为D1时边界线宽度默认为4）
    l32 l32PreprocessPic;   //PREPROCESS_NO_PIC_FILL_BG 前处理无图像处填充背景色
    l32 l32Drawsides;       //前处理中画边框 PREPROCESS_DRAWSIDES
    l32 l32Drawfocus;       //前景图像勾画选定边框 赋值为PREPROCESS_DRAWFOCUS，前景图像才会根据各自的设置画边框
	TMergeZoomFgParam atImageInput[PIC_MERGE_NUM_MAX];  //前景图像信息
} TImageMergeZoomParam;

//画面合成模块状态查询结构体
#define TImageMergeZoomStatusParam TImageMergeZoomParam

//画面合成模块输入结构体
typedef struct 
{
    u8 *apu8InY[PIC_MERGE_NUM_MAX];   //输入前景N路图像的Y分量地址
    u8 *apu8InU[PIC_MERGE_NUM_MAX];   //输入前景N路图像的U分量地址(如果YUV地址连续存放可将U地址设置为NULL)
    u8 *apu8InV[PIC_MERGE_NUM_MAX];   //输入前景N路图像的V分量地址(如果YUV地址连续存放可将V地址设置为NULL)
    u8 *pu8OutY;                //输出合成图像的Y分量地址
    u8 *pu8OutU;                //输出合成图像的U分量地址(如果YUV地址连续存放可将U地址设置为NULL)
    u8 *pu8OutV;                //输出合成图像的V分量地址(如果YUV地址连续存放可将V地址设置为NULL)
    //TBufGroupInfo atChip[MAX_BUF_GROUPS]; //输入的BUF分组信息
    //l32 l32BufGroups;                        //输入的BUF分组数目
    //l32 l32OffsetY;
    u32 u32Reserved;
} TImageMergeZoomInput;
 
//画面合成模块输出结构体
typedef struct 
{
    u32 u32Reserved;   //保留参数 
} TImageMergeZoomOutput;

#endif
