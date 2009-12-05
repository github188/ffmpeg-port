/*****************************************************************************
ģ����      : getconfig
�ļ���      : getconfig.h
����ļ�    : 
�ļ�ʵ�ֹ���: ��ȡMPEG-4��H.264������Config��Ϣ
����        : sxx
�汾        : V1.0  Copyright (C) KDC 2005-2006. All rights reserved.
******************************************************************************/
#ifndef _GETCONFIG_H_
#define _GETCONFIG_H_

#include "portabledefine.h"
//�������͵Ķ���




	typedef char            s8;            
	typedef short           s16;

	typedef unsigned char   u8;   
	typedef unsigned short  u16;	

	typedef int            l32;
	typedef unsigned int   u32;

	typedef mu_int64		s64;
	typedef mu_uint64    u64;

#define INLINE




//������
enum
{
    ERR_MPEG4CONFIG_INPUTBUF = 1,       //����ָ��Ϊ��
    ERR_MPEG4CONFIG_INPUTSIZE,          //���볤��Ϊ��
    ERR_MPEG4CONFIG_GETCONFIG,          //��ȡconfigʧ��
    ERR_VID_MPEG4DEC_BITSTREAM,         //��������ʧ��

    ERR_H264CONFIG_INPUTBUF,            //����ָ��Ϊ��
    ERR_H264CONFIG_INPUTSIZE,           //���볤��Ϊ��
    ERR_H264CONFIG_GETCONFIG,           //��ȡconfigʧ��
    ERR_H264CONFIG_TOOMANYSPS_PPS,      //SPS, PPS��Ŀ̫��
    ERR_H264CONFIG_INVALID_BS,          //����������
    ERR_VID_H264DEC_BITSTREAM,          //��������ʧ��
};

// ���sps,pps�������Ŀ
#define MAXSIZE 10
typedef struct
{
    u8 *pu8Sps[MAXSIZE];                // ���ÿ��SPS�Ŀ�ʼָ��
    l32 l32SpsLen[MAXSIZE];             // ���ÿ��SPS�ĳ��ȣ���ָ��һһ��Ӧ��
    l32 l32SpsNum;                      // SPS����Ŀ
    u8 *pu8Pps[MAXSIZE];                // ���ÿ��PPS�Ŀ�ʼָ��
    l32 l32PpsLen[MAXSIZE];             // ���ÿ��PPS�ĳ��ȣ���ָ��һһ��Ӧ��
    l32 l32PpsNum;                      // PPS����Ŀ
} TH264ConfigInfo;

typedef struct
{
    u8 *pu8Config;                // Mpeg4�����е�VOL��Ϣָ��
    l32 l32ConfigLen;             // Mpeg4�����е�VOL��Ϣ����
} TMpeg4ConfigInfo;

//����ɹ������
#define  VIDEO_SUCCESS		0

/*=============================================================================
�� �� ��: GetMpeg4Config
��    ��: ��ȡMpeg4����VOL��Ϣ
ȫ�ֱ���: ��
��    ��: pu8Stream             ��������ָ��[in]
          u32BsLen              ������������[in]
          ptMpeg4ConfigInfo     ��ȡVOL��Ϣ�ṹ��ָ��[out]
�� �� ֵ: ��Ӧ�Ĵ�����
=============================================================================*/
l32 GetMpeg4Config(u8 *pu8Stream, u32 u32BsLen, TMpeg4ConfigInfo *ptMpeg4ConfigInfo);

/*=============================================================================
�� �� ��: GetH264Config
��    ��: ��ȡH264����SPS, PPS��Ϣ
ȫ�ֱ���: ��
��    ��: pu8Stream         ��������ָ��[in]
          u32BsLen          ������������[in]
          ptH264ConfigInfo  ���SPS��PPS��Ϣָ��[out]
�� �� ֵ: ����VIDEO_SUCCESS��ʾ�ɹ�������ʧ�ܣ����շ��ش����룩
=============================================================================*/
l32 GetH264Config(u8 *pu8Stream, u32 u32BsLen, TH264ConfigInfo *ptH264ConfigInfo);


#endif

