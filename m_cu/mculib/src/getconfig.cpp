/*****************************************************************************
ģ����      : getconfig
�ļ���      : getconfig.c
����ļ�    :
�ļ�ʵ�ֹ���: ��ȡMPEG-4����VOLͷ��Ϣ
����        : sxx
�汾        : V1.0  Copyright (C) KDC 2005-2006. All rights reserved.
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

//������Ϣ�ṹ
typedef struct
{
    u32 u32Cur;      //��ǰ��������
    u32 u32Next;     //�ֽڷ�ת����������
    u32 u32Pos;      //����λ��
    u32 *u32Ptr;     //����ָ��
    u32 *u32Buf;     //����Buffer
    u32 u32Length;   //��������
    u32 u32DecBsErr; //�������������
    u32 u32Offset;   //ƫ��
} TBitReader;

/*=============================================================================
�� �� ��: BSWAP
��    ��: �ֽڷ�ת
ȫ�ֱ���: ��
��    ��: u32Val          ����ֵ
�� �� ֵ: ������ֵ
=============================================================================*/
static u32 INLINE BSWAP(u32 u32Val)
{
    u32Val = ((u32Val >> 24) | ((u32Val & 0xff0000) >> 8) | ((u32Val & 0xff00) << 8) | ((u32Val & 0xff) << 24));
    
    return u32Val;
}

/*=============================================================================
�� �� ��: BsLength
��    ��: ������������
ȫ�ֱ���: ��
��    ��: ptBs          ������Ϣ�ṹָ��
�� �� ֵ: ��������
=============================================================================*/
static INLINE u32 BsLength(TBitReader *const ptBs)
{
    return (((ptBs->u32Ptr - ptBs->u32Buf) << 2) - 4 - ((32 - ptBs->u32Pos) >> 3));
}

/*=============================================================================
�� �� ��: BsSkip
��    ��: ����nbit
ȫ�ֱ���: ��
��    ��: ptBs          ������Ϣ�ṹָ��
          u32Bits       ȡ��Bit��
�� �� ֵ: ��
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
        
        //��������bit��
        u32Consumed = BsLength(ptBs);
        
        if(u32Consumed > ptBs->u32Length)
        {
            ptBs->u32DecBsErr = ERR_VID_MPEG4DEC_BITSTREAM; //���ý�������������
        }     
    }
}

/*=============================================================================
�� �� ��: BsByteAlign
��    ��: �ֽڶ���
ȫ�ֱ���: ��
��    ��: ptBs          ������Ϣ�ṹָ��
�� �� ֵ: ��
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
�� �� ��: BsShow
��    ��: ȡ��nbitֵ(ָ�벻ǰ��)
ȫ�ֱ���: ��
��    ��: ptBs          ������Ϣ�ṹָ��
          u32Bits       ȡ��Bit��
�� �� ֵ: nbitֵ
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
�� �� ��: BsHeaders
��    ��: ����ͷ��Ϣ
ȫ�ֱ���: ��
��    ��: ptBs                  �����ṹָ��[in]  
          ptMpeg4ConfigInfo     ���Config��Ϣ�ṹ��ָ��[out]
�� �� ֵ: VIDEO_SUCCESS: �ɹ�������: ��Ӧ������
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
        
        u32StartCode = BsShow(ptBs, 32); //��ȡ��ʼ��
        
        if((u32StartCode & (~0x0ff)) == START_CODE)
        {
            if(u32StartCode == VO_SEQ_START_CODE)
            {
                //��ȡ��ʼ��ַ
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
				//����VOLͷ��Ϣ
                BsSkip(ptBs, 32);
            }
            else if(u32StartCode == GOV_START_CODE) //����GOVͷ
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
				//Ϊ�˱���C�汾��MAP�汾����һ����VOPͷ��Ϣ�Ľ��붼�ں�����������н��У�MAP�汾��ת��VLX�н��н���
                break;
            }
            else if(u32StartCode == USERDATA_START_CODE)//�����û�����
            {
                //��ȡ������ַ
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
        //��ȡVOL��Ϣ����
        ptMpeg4ConfigInfo->l32ConfigLen = pu8End - pu8Start;
        ptMpeg4ConfigInfo->pu8Config = pu8Start;
    }
    else
    {
        l32Msg = ERR_MPEG4CONFIG_GETCONFIG;
    }
    
    return l32Msg; 
}

// NAL����
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
�� �� ��: GetH264NextStartCode
��    ��: ǰ������һ����ʼ�봦
ȫ�ֱ���: ��
��    ��: ptBs              �����ṹָ��[in]
�� �� ֵ: ��
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
�� �� ��: BsHeaders
��    ��: ����ͷ��Ϣ
ȫ�ֱ���: ��
��    ��: ptBs              �����ṹָ��[in]
          ptH264ConfigInfo  ���SPS��PPS��Ϣָ��[out]
�� �� ֵ: VIDEO_SUCCESS: �ɹ�������: ��Ӧ������
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

        // Ѱ��00 00 00 01������
        if(u32StartCode == 0x00000001)
        {
            // ����������
            BsSkip(ptBs, 32);

            // nal���͵���Ϣ
            l32Tmp = BsShow(ptBs, 8);
            switch(l32Tmp & 31)
            {
            case NAL_SPS:
                pu8Start = (u8*)(ptBs->u32Ptr - 2) - 4 + (ptBs->u32Pos / 8);
                // ��ʼ��ַ
                ptH264ConfigInfo->pu8Sps[ptH264ConfigInfo->l32SpsNum] = pu8Start;
                // �ҵ���һ����ʼ��
                GetH264NextStartCode(ptBs);
                // ������ַ
                pu8End = (u8*)(ptBs->u32Ptr - 2) + ptBs->u32Pos / 8;
                ptH264ConfigInfo->l32SpsLen[ptH264ConfigInfo->l32SpsNum] = pu8End - pu8Start;

                // SPS��Ŀ����
                ptH264ConfigInfo->l32SpsNum++;

                break;
                
            case NAL_PPS:
                pu8Start = (u8*)(ptBs->u32Ptr - 2) - 4 + (ptBs->u32Pos / 8);
                // ��ʼ��ַ
                ptH264ConfigInfo->pu8Pps[ptH264ConfigInfo->l32PpsNum] = pu8Start;
                // �ҵ���һ����ʼ��
                GetH264NextStartCode(ptBs);
                // ������ַ
                pu8End = (u8*)(ptBs->u32Ptr - 2) + ptBs->u32Pos / 8;
                ptH264ConfigInfo->l32PpsLen[ptH264ConfigInfo->l32PpsNum] = pu8End - pu8Start;
                
                // SPS��Ŀ����
                ptH264ConfigInfo->l32PpsNum++;
                
                break;

            // ������SPS��PPS֮������ݶ����Ե�
            case NAL_SEI:
                break;
            case NAL_SLICE:
            case NAL_IDRSLICE:
            default:
                // ֻ��SPS��PPS������һ������Ϊ�ɹ�
                return (ptH264ConfigInfo->l32PpsNum && ptH264ConfigInfo->l32SpsNum) ? VIDEO_SUCCESS : ERR_H264CONFIG_GETCONFIG;
            }
            
            // SPS, PPS��Ŀ̫�������Ų���
            if(ptH264ConfigInfo->l32SpsNum > MAXSIZE || ptH264ConfigInfo->l32PpsNum > MAXSIZE)
                return ERR_H264CONFIG_TOOMANYSPS_PPS;
        }
        else
        {
            BsSkip(ptBs, 8);
        }
    }

    // û�ҵ���ʼ��
    return ERR_H264CONFIG_INVALID_BS; 
}

/*=============================================================================
�� �� ��: BsInit
��    ��: ��ʼ������Ϣ
ȫ�ֱ���: ��
��    ��: ptBs          ������Ϣ�ṹָ��
          pu8BsBuf      ��������Buffer
          u32Length     ��������
�� �� ֵ: ��
=============================================================================*/
static void INLINE BsInit(TBitReader *const ptBs, u8 *pu8BsBuf, u32 u32Length)
{
    u32 *pu32Start = NULL;
    l32 l32PadLen;

    //��������32bit����
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
�� �� ��: GetMpeg4Config
��    ��: ��ȡMpeg4����VOL��Ϣ
ȫ�ֱ���: ��
��    ��: pu8Stream             ��������ָ��[in]
          u32BsLen              ������������[in]
          ptMpeg4ConfigInfo     ��ȡVOL��Ϣ�ṹ��ָ��[out]
�� �� ֵ: ��Ӧ�Ĵ�����
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
    
    //��ʼ��������Ϣ
    BsInit(&tBs, pu8Stream, u32BsLen);
    memset(ptMpeg4ConfigInfo, 0, sizeof(*ptMpeg4ConfigInfo));

    //����VOL��ͷ��Ϣ
    l32Status = BsHeaders(&tBs, ptMpeg4ConfigInfo);

    return l32Status;
}

/*=============================================================================
�� �� ��: GetH264Config
��    ��: ��ȡH264����SPS, PPS��Ϣ
ȫ�ֱ���: ��
��    ��: pu8Stream         ��������ָ��[in]
          u32BsLen          ������������[in]
          ptH264ConfigInfo  ���SPS��PPS��Ϣָ��[out]
�� �� ֵ: ����VIDEO_SUCCESS��ʾ�ɹ�������ʧ�ܣ����շ��ش����룩
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

    //��ʼ��������Ϣ
    BsInit(&tBs, pu8Stream, u32BsLen);
    memset(ptH264ConfigInfo, 0, sizeof(*ptH264ConfigInfo));

    //����SPS, PPS��ͷ��Ϣ
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