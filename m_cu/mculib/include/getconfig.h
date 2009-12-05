/*****************************************************************************
模块名      : getconfig
文件名      : getconfig.h
相关文件    : 
文件实现功能: 提取MPEG-4和H.264码流的Config信息
作者        : sxx
版本        : V1.0  Copyright (C) KDC 2005-2006. All rights reserved.
******************************************************************************/
#ifndef _GETCONFIG_H_
#define _GETCONFIG_H_

#include "portabledefine.h"
//数据类型的定义




	typedef char            s8;            
	typedef short           s16;

	typedef unsigned char   u8;   
	typedef unsigned short  u16;	

	typedef int            l32;
	typedef unsigned int   u32;

	typedef mu_int64		s64;
	typedef mu_uint64    u64;

#define INLINE




//错误码
enum
{
    ERR_MPEG4CONFIG_INPUTBUF = 1,       //输入指针为空
    ERR_MPEG4CONFIG_INPUTSIZE,          //输入长度为零
    ERR_MPEG4CONFIG_GETCONFIG,          //获取config失败
    ERR_VID_MPEG4DEC_BITSTREAM,         //解码码流失败

    ERR_H264CONFIG_INPUTBUF,            //输入指针为空
    ERR_H264CONFIG_INPUTSIZE,           //输入长度为零
    ERR_H264CONFIG_GETCONFIG,           //获取config失败
    ERR_H264CONFIG_TOOMANYSPS_PPS,      //SPS, PPS数目太多
    ERR_H264CONFIG_INVALID_BS,          //码流有问题
    ERR_VID_H264DEC_BITSTREAM,          //解码码流失败
};

// 输出sps,pps的最大数目
#define MAXSIZE 10
typedef struct
{
    u8 *pu8Sps[MAXSIZE];                // 存放每个SPS的开始指针
    l32 l32SpsLen[MAXSIZE];             // 存放每个SPS的长度（与指针一一对应）
    l32 l32SpsNum;                      // SPS的数目
    u8 *pu8Pps[MAXSIZE];                // 存放每个PPS的开始指针
    l32 l32PpsLen[MAXSIZE];             // 存放每个PPS的长度（与指针一一对应）
    l32 l32PpsNum;                      // PPS的数目
} TH264ConfigInfo;

typedef struct
{
    u8 *pu8Config;                // Mpeg4码流中的VOL信息指针
    l32 l32ConfigLen;             // Mpeg4码流中的VOL信息长度
} TMpeg4ConfigInfo;

//解码成功并输出
#define  VIDEO_SUCCESS		0

/*=============================================================================
函 数 名: GetMpeg4Config
功    能: 获取Mpeg4码流VOL信息
全局变量: 无
参    数: pu8Stream             输入码流指针[in]
          u32BsLen              输入码流长度[in]
          ptMpeg4ConfigInfo     获取VOL信息结构体指针[out]
返 回 值: 相应的错误码
=============================================================================*/
l32 GetMpeg4Config(u8 *pu8Stream, u32 u32BsLen, TMpeg4ConfigInfo *ptMpeg4ConfigInfo);

/*=============================================================================
函 数 名: GetH264Config
功    能: 获取H264码流SPS, PPS信息
全局变量: 无
参    数: pu8Stream         输入码流指针[in]
          u32BsLen          输入码流长度[in]
          ptH264ConfigInfo  输出SPS、PPS信息指针[out]
返 回 值: 返回VIDEO_SUCCESS表示成功，否则失败（参照返回错误码）
=============================================================================*/
l32 GetH264Config(u8 *pu8Stream, u32 u32BsLen, TH264ConfigInfo *ptH264ConfigInfo);


#endif

