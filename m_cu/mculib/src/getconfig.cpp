/*****************************************************************************
模块名      : getconfig
文件名      : getconfig.c
相关文件    :
文件实现功能: 提取MPEG-4码流VOL头信息
作者        : sxx
版本        : V1.0  Copyright (C) KDC 2005-2006. All rights reserved.
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "getconfig.h"

#define START_CODE          0x00000100		// start code , 27 bits
#define VO_START_CODE       0x00000100	
#define VOL_START_CODE	    0x00000120	
#define VO_SEQ_START_CODE   0x000001b0
#define USERDATA_START_CODE 0x000001b2
#define GOV_START_CODE      0x000001b3
#define VSO_START_CODE      0x000001b5		// Visual Object start code
#define VOP_START_CODE      0x000001b6
#define SLICE_START_CODE    0x000001b7

//码流信息结构
typedef struct
{
    u32 u32Cur;      //当前码流数据
    u32 u32Next;     //字节反转后码流数据
    u32 u32Pos;      //码流位置
    u32 *u32Ptr;     //码流指针
    u32 *u32Buf;     //码流Buffer
    u32 u32Length;   //码流长度
    u32 u32DecBsErr; //码流解码错误标记
    u32 u32Offset;   //偏移
} TBitReader;

/*=============================================================================
函 数 名: BSWAP
功    能: 字节反转
全局变量: 无
参    数: u32Val          输入值
返 回 值: 反正后值
=============================================================================*/
static u32 INLINE BSWAP(u32 u32Val)
{
    u32Val = ((u32Val >> 24) | ((u32Val & 0xff0000) >> 8) | ((u32Val & 0xff00) << 8) | ((u32Val & 0xff) << 24));
    
    return u32Val;
}

/*=============================================================================
函 数 名: BsLength
功    能: 计算码流长度
全局变量: 无
参    数: ptBs          码流信息结构指针
返 回 值: 码流长度
=============================================================================*/
static INLINE u32 BsLength(TBitReader *const ptBs)
{
    return (((ptBs->u32Ptr - ptBs->u32Buf) << 2) - 4 - ((32 - ptBs->u32Pos) >> 3));
}

/*=============================================================================
函 数 名: BsSkip
功    能: 跳过nbit
全局变量: 无
参    数: ptBs          码流信息结构指针
          u32Bits       取出Bit数
返 回 值: 无
=============================================================================*/
static INLINE void BsSkip(TBitReader *const ptBs, const u32 u32Bits)
{
    u32 u32Consumed;
    
    ptBs->u32Pos += u32Bits;
    
    if(ptBs->u32Pos > 31) 
    {
        ptBs->u32Cur = ptBs->u32Next;
        
        ptBs->u32Next = BSWAP(*ptBs->u32Ptr++);
        ptBs->u32Pos -= 32;
        
        //计算消耗bit数
        u32Consumed = BsLength(ptBs);
        
        if(u32Consumed > ptBs->u32Length)
        {
            ptBs->u32DecBsErr = ERR_VID_MPEG4DEC_BITSTREAM; //设置解码码流错误标记
        }     
    }
}

/*=============================================================================
函 数 名: BsByteAlign
功    能: 字节对齐
全局变量: 无
参    数: ptBs          码流信息结构指针
返 回 值: 无
=============================================================================*/
static void BsByteAlign(TBitReader *const ptBs)
{
    u32 u32Remained = ptBs->u32Pos & 7; 
    
    if(u32Remained)
    {
        BsSkip(ptBs, 8 - u32Remained);
    }
}

/*=============================================================================
函 数 名: BsShow
功    能: 取出nbit值(指针不前移)
全局变量: 无
参    数: ptBs          码流信息结构指针
          u32Bits       取出Bit数
返 回 值: nbit值
=============================================================================*/
static u32 BsShow(TBitReader *const ptBs, const u32 u32Bits)
{
    l32 nbit = (l32)(u32Bits + ptBs->u32Pos) - 32;
    
    if(nbit > 0) 
    {
        return ((ptBs->u32Cur & (0xffffffff >> ptBs->u32Pos)) << nbit) | (ptBs->u32Next >> (32 - nbit));
    }
    else 
    {
        return (ptBs->u32Cur & (0xffffffff >> ptBs->u32Pos)) >> (32 - ptBs->u32Pos - u32Bits);
    }
}

/*=============================================================================
函 数 名: BsHeaders
功    能: 解析头信息
全局变量: 无
参    数: ptBs                  码流结构指针[in]  
          ptMpeg4ConfigInfo     输出Config信息结构体指针[out]
返 回 值: VIDEO_SUCCESS: 成功；其他: 相应错误码
=============================================================================*/
l32 BsHeaders(TBitReader *ptBs, TMpeg4ConfigInfo *ptMpeg4ConfigInfo)
{
    l32 l32Msg = VIDEO_SUCCESS;
    u32 u32StartCode;
    u8 *pu8Start = NULL;	// lzx + "=NULL"
    u8 *pu8End = NULL;		// lzx + "=NULL"
    l32 l32UsrData = 0;
    l32 l32IFrame = 0;
    
    while(!ptBs->u32DecBsErr)
    {
        BsByteAlign(ptBs);
        
        u32StartCode = BsShow(ptBs, 32); //获取起始码
        
        if((u32StartCode & (~0x0ff)) == START_CODE)
        {
            if(u32StartCode == VO_SEQ_START_CODE)
            {
                //获取起始地址
                pu8Start = (u8 *)(ptBs->u32Ptr - 2) + (ptBs->u32Pos / 8);
                pu8End = pu8Start;
                l32IFrame = 1;
                BsSkip(ptBs, 32);
            }
            else if(u32StartCode == VSO_START_CODE)
            {
                BsSkip(ptBs, 32);
            }
            else if((u32StartCode & (~0xf)) == VOL_START_CODE) // 0x120 ... 0x12f
            {
				//解码VOL头信息
                BsSkip(ptBs, 32);
            }
            else if(u32StartCode == GOV_START_CODE) //解码GOV头
            {
                BsSkip(ptBs, 32);		
            }
            else if(u32StartCode == VOP_START_CODE)
            {
                if((!l32UsrData) && l32IFrame)
                {
                    pu8End = (u8 *)(ptBs->u32Ptr - 2) + (ptBs->u32Pos / 8);
                    l32IFrame = 0;
                }
				//为了保持C版本和MAP版本流程一样，VOP头信息的解码都在后面解码流程中进行，MAP版本则转到VLX中进行解码
                break;
            }
            else if(u32StartCode == USERDATA_START_CODE)//解码用户数据
            {
                //获取结束地址
                pu8End = (u8 *)(ptBs->u32Ptr - 2) + (ptBs->u32Pos / 8);
                l32UsrData = 1;
                BsSkip(ptBs, 32);
            }
            else
            {
                BsSkip(ptBs, 32);
            }
        }
        else  // continue seeking by forward 8 bits
        {
            BsSkip(ptBs, 8);
        }
    }

	// lzx + "pu8Start &&"
    if( pu8Start && pu8Start != pu8End && pu8End > pu8Start)
    {
        //获取VOL信息长度
        ptMpeg4ConfigInfo->l32ConfigLen = pu8End - pu8Start;
        ptMpeg4ConfigInfo->pu8Config = pu8Start;
    }
    else
    {
        l32Msg = ERR_MPEG4CONFIG_GETCONFIG;
    }
    
    return l32Msg; 
}

// NAL类型
enum
{
    NAL_SLICE = 1,
    NAL_PARTA,
    NAL_PARTB,
    NAL_PARTC,
    NAL_IDRSLICE,
    NAL_SEI,
    NAL_SPS,
    NAL_PPS,
    NAL_AUD,
    NAL_EOSEQ,
    NAL_EOSTREAM,
    NAL_FILL
};

/*=============================================================================
函 数 名: GetH264NextStartCode
功    能: 前进到下一个起始码处
全局变量: 无
参    数: ptBs              码流结构指针[in]
返 回 值: 无
=============================================================================*/
static void GetH264NextStartCode(TBitReader *ptBs)
{
    while(!ptBs->u32DecBsErr)
    {
        if(BsShow(ptBs, 32) == 0x00000001)
        {
            return;
        }
        else
        {
            BsSkip(ptBs, 8);
        }
    }
}

/*=============================================================================
函 数 名: BsHeaders
功    能: 解析头信息
全局变量: 无
参    数: ptBs              码流结构指针[in]
          ptH264ConfigInfo  输出SPS、PPS信息指针[out]
返 回 值: VIDEO_SUCCESS: 成功；其他: 相应错误码
=============================================================================*/
l32 GetH264Headers(TBitReader *ptBs, TH264ConfigInfo *ptH264ConfigInfo)
{
//    l32 l32Msg = VIDEO_SUCCESS;
    u32 u32StartCode;
    l32 l32Tmp;
    u8 *pu8Start;
    u8 *pu8End;
    
    while(!ptBs->u32DecBsErr)
    {
        BsByteAlign(ptBs);
        
        u32StartCode = BsShow(ptBs, 32);

        // 寻找00 00 00 01特征码
        if(u32StartCode == 0x00000001)
        {
            // 跳过特征码
            BsSkip(ptBs, 32);

            // nal类型等信息
            l32Tmp = BsShow(ptBs, 8);
            switch(l32Tmp & 31)
            {
            case NAL_SPS:
                pu8Start = (u8*)(ptBs->u32Ptr - 2) - 4 + (ptBs->u32Pos / 8);
                // 开始地址
                ptH264ConfigInfo->pu8Sps[ptH264ConfigInfo->l32SpsNum] = pu8Start;
                // 找到下一个起始码
                GetH264NextStartCode(ptBs);
                // 结束地址
                pu8End = (u8*)(ptBs->u32Ptr - 2) + ptBs->u32Pos / 8;
                ptH264ConfigInfo->l32SpsLen[ptH264ConfigInfo->l32SpsNum] = pu8End - pu8Start;

                // SPS数目增加
                ptH264ConfigInfo->l32SpsNum++;

                break;
                
            case NAL_PPS:
                pu8Start = (u8*)(ptBs->u32Ptr - 2) - 4 + (ptBs->u32Pos / 8);
                // 开始地址
                ptH264ConfigInfo->pu8Pps[ptH264ConfigInfo->l32PpsNum] = pu8Start;
                // 找到下一个起始码
                GetH264NextStartCode(ptBs);
                // 结束地址
                pu8End = (u8*)(ptBs->u32Ptr - 2) + ptBs->u32Pos / 8;
                ptH264ConfigInfo->l32PpsLen[ptH264ConfigInfo->l32PpsNum] = pu8End - pu8Start;
                
                // SPS数目增加
                ptH264ConfigInfo->l32PpsNum++;
                
                break;

            // 所有在SPS、PPS之间的数据都忽略掉
            case NAL_SEI:
                break;
            case NAL_SLICE:
            case NAL_IDRSLICE:
            default:
                // 只有SPS、PPS至少有一个才认为成功
                return (ptH264ConfigInfo->l32PpsNum && ptH264ConfigInfo->l32SpsNum) ? VIDEO_SUCCESS : ERR_H264CONFIG_GETCONFIG;
            }
            
            // SPS, PPS数目太多数组存放不了
            if(ptH264ConfigInfo->l32SpsNum > MAXSIZE || ptH264ConfigInfo->l32PpsNum > MAXSIZE)
                return ERR_H264CONFIG_TOOMANYSPS_PPS;
        }
        else
        {
            BsSkip(ptBs, 8);
        }
    }

    // 没找到起始码
    return ERR_H264CONFIG_INVALID_BS; 
}

/*=============================================================================
函 数 名: BsInit
功    能: 初始码流信息
全局变量: 无
参    数: ptBs          码流信息结构指针
          pu8BsBuf      输入码流Buffer
          u32Length     码流长度
返 回 值: 无
=============================================================================*/
static void INLINE BsInit(TBitReader *const ptBs, u8 *pu8BsBuf, u32 u32Length)
{
    u32 *pu32Start = NULL;
    l32 l32PadLen;

    //将码流以32bit对齐
    pu32Start = (u32*)((u32)pu8BsBuf & (u32)~3);
    l32PadLen = (u32)pu8BsBuf - (u32)pu32Start;
    ptBs->u32Buf = ptBs->u32Ptr = (u32 *)pu32Start;
    ptBs->u32Pos = l32PadLen * 8;
    ptBs->u32Length = u32Length;
    
    ptBs->u32Cur = BSWAP(*ptBs->u32Ptr ++);
    ptBs->u32Next = BSWAP(*ptBs->u32Ptr ++);
    ptBs->u32DecBsErr = VIDEO_SUCCESS;
}

/*=============================================================================
函 数 名: GetMpeg4Config
功    能: 获取Mpeg4码流VOL信息
全局变量: 无
参    数: pu8Stream             输入码流指针[in]
          u32BsLen              输入码流长度[in]
          ptMpeg4ConfigInfo     获取VOL信息结构体指针[out]
返 回 值: 相应的错误码
=============================================================================*/
l32 GetMpeg4Config(u8 *pu8Stream, u32 u32BsLen, TMpeg4ConfigInfo *ptMpeg4ConfigInfo)
{
    TBitReader tBs;
    l32 l32Status = 0;

    if(NULL == pu8Stream || NULL == ptMpeg4ConfigInfo)
    {
        return ERR_MPEG4CONFIG_INPUTBUF;
    }

    if(u32BsLen == 0)
    {
        return ERR_MPEG4CONFIG_INPUTSIZE;
    }
    
    //初始化码流信息
    BsInit(&tBs, pu8Stream, u32BsLen);
    memset(ptMpeg4ConfigInfo, 0, sizeof(*ptMpeg4ConfigInfo));

    //解码VOL等头信息
    l32Status = BsHeaders(&tBs, ptMpeg4ConfigInfo);

    return l32Status;
}

/*=============================================================================
函 数 名: GetH264Config
功    能: 获取H264码流SPS, PPS信息
全局变量: 无
参    数: pu8Stream         输入码流指针[in]
          u32BsLen          输入码流长度[in]
          ptH264ConfigInfo  输出SPS、PPS信息指针[out]
返 回 值: 返回VIDEO_SUCCESS表示成功，否则失败（参照返回错误码）
=============================================================================*/
l32 GetH264Config(u8 *pu8Stream, u32 u32BsLen, TH264ConfigInfo *ptH264ConfigInfo)
{
    TBitReader tBs;

    if(NULL == pu8Stream || NULL == ptH264ConfigInfo)
    {
        return ERR_H264CONFIG_INPUTBUF;
    }

    if(u32BsLen == 0)
    {
        return ERR_H264CONFIG_INPUTSIZE;
    }

    //初始化码流信息
    BsInit(&tBs, pu8Stream, u32BsLen);
    memset(ptH264ConfigInfo, 0, sizeof(*ptH264ConfigInfo));

    //解码SPS, PPS等头信息
    return GetH264Headers(&tBs, ptH264ConfigInfo);
}

BOOL IsIFrameH264( u8 *pu8Stream, u32 u32BsLen )
{
	TH264ConfigInfo tH264ConfigInfo;
	l32 lRet = GetH264Config( pu8Stream, u32BsLen, &tH264ConfigInfo );

	return 0 == lRet;
}

BOOL IsIFrameMPG4( u8 *pu8Stream, u32 u32BsLen )
{
	TMpeg4ConfigInfo tMpeg4ConfigInfo;
	l32 lRet = GetMpeg4Config( pu8Stream, u32BsLen, &tMpeg4ConfigInfo );

	return 0 == lRet;
}