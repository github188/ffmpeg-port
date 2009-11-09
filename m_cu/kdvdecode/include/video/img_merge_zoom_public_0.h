/******************************************************************************
ģ����	    ��img_merge_zoom_public
�ļ���	    ��img_merge_zoom_public.h            
����ļ�	��
�ļ�ʵ�ֹ��ܣ����廭��ϳɲ����ṹ��
����		������ϼ
�汾		��1.0
-------------------------------------------------------------------------------
�޸ļ�¼:
��  ��			�汾			�޸���		�߶���    �޸�����
2007/09/12		1.0		        ����ϼ                 ����
2007/11/02		1.1		        ����ϼ                 ��ӻ���ϳ����ñ߽��߿�Ȳ���
******************************************************************************/
#ifndef _IMG_MERGE_ZOOM_PUBLIC_H_
#define _IMG_MERGE_ZOOM_PUBLIC_H_

#include "videodefines.h"
#include "img_common_public.h"

//ǰ��ͼ����Ϣ�ṹ��
typedef struct
{
	l32 l32Width;         //ͼ��Ŀ��
	l32 l32Height;        //ͼ��ĸ߶�
	l32 l32IsInterlace;   //ͼ�������(֡��ʽ���߳���ʽ)
	l32 l32YUVType;       //ͼ��ͼ���ʽ(YUV422����YUV420)
	l32 l32DrawFocusFlag; //�����Ƿ񹴻��߿�ı��,u8DrawfocusΪ1ʱ����
	l32 l32FocuslineR;	  //����߿�ɫR
	l32 l32FocuslineG;	  //����߿�ɫG
	l32 l32FocuslineB;	  //����߿�ɫB
    l32 l32FocusWidth;    //�߿�ͳһ��ȣ��ݲ������ã�
	l32 l32Pos;			  //����λ�ñ��
	l32 l32NoPicInput;    //��ǰ��ͼ�������־λIMG_MERGE_NO_PIC_INPUT
	l32 l32SingleZoomStyle;  //����ģʽ�����ڶ��Buffer����ʱ��������Сͼ������ģʽ��PIC_ZOOM_ONE��ȫ�����ţ�PIC_ZOOM_TWO�����ֱ������첢����ͼ��������PIC_ZOOM_THREE�����ֱ������첢����Ŀ������PIC_ZOOM_FOUR������ԭ�гߴ磩
	l32 l32Reserved;	  //��������
} TMergeZoomFgParam;

//����ϳ�ģ���ʼ�������ṹ
typedef struct
{  
	l32	l32MergeStyle;	    //����ϳ�ģʽ
    //l32 l32InBufStyle;      //�ϳ�����Bufģʽ��MERGE_INBUF_ONE:����Buf,MERGE_INBUF_MULTI:���Buf��
    //l32 l32MultiZoomStyle;  //����ģʽ�����ڵ���Buffer����ʱͳһ����Сͼ������ģʽ��PIC_ZOOM_ONE��ȫ�����ţ�PIC_ZOOM_TWO�����ֱ������첢����ͼ��������PIC_ZOOM_THREE�����ֱ������첢����Ŀ������PIC_ZOOM_FOUR������ԭ�гߴ磩
	l32 l32Width;           //����ͼ��Ŀ��
	l32 l32Height;          //����ͼ��ĸ߶�
	l32	l32IsInterlace;     //����ͼ�������(֡��ʽ���߳���ʽ)
	l32 l32YUVType;         //����ͼ���ʽ(YUV422����YUV420)
	l32 l32BackgroundR;	    //����ϳɱ���ɫR
	l32 l32BackgroundG;	    //����ϳɱ���ɫG
	l32 l32BackgroundB;	    //����ϳɱ���ɫB
    l32 l32BoundaryR;       //����ϳɸ�·ͼ��ͳһ�߽�����ɫR
    l32 l32BoundaryG;       //����ϳɸ�·ͼ��ͳһ�߽�����ɫG
    l32 l32BoundaryB;       //����ϳɸ�·ͼ��ͳһ�߽�����ɫB
    l32 l32BoundaryWidth;   //����ϳɱ߽��߿�ȣ�ȱʡֵΪ0������ΪCIFʱ�߽��߿��Ĭ��Ϊ2������ΪD1ʱ�߽��߿��Ĭ��Ϊ4��
    l32 l32PreprocessPic;   //PREPROCESS_NO_PIC_FILL_BG ǰ������ͼ����䱳��ɫ
    l32 l32Drawsides;       //ǰ�����л��߿� PREPROCESS_DRAWSIDES
    l32 l32Drawfocus;       //ǰ��ͼ�񹴻�ѡ���߿� ��ֵΪPREPROCESS_DRAWFOCUS��ǰ��ͼ��Ż���ݸ��Ե����û��߿�
	TMergeZoomFgParam atImageInput[PIC_MERGE_NUM_MAX];  //ǰ��ͼ����Ϣ
} TImageMergeZoomParam;

//����ϳ�ģ��״̬��ѯ�ṹ��
#define TImageMergeZoomStatusParam TImageMergeZoomParam

//����ϳ�ģ������ṹ��
typedef struct 
{
    u8 *apu8InY[PIC_MERGE_NUM_MAX];   //����ǰ��N·ͼ���Y������ַ
    u8 *apu8InU[PIC_MERGE_NUM_MAX];   //����ǰ��N·ͼ���U������ַ(���YUV��ַ������ſɽ�U��ַ����ΪNULL)
    u8 *apu8InV[PIC_MERGE_NUM_MAX];   //����ǰ��N·ͼ���V������ַ(���YUV��ַ������ſɽ�V��ַ����ΪNULL)
    u8 *pu8OutY;                //����ϳ�ͼ���Y������ַ
    u8 *pu8OutU;                //����ϳ�ͼ���U������ַ(���YUV��ַ������ſɽ�U��ַ����ΪNULL)
    u8 *pu8OutV;                //����ϳ�ͼ���V������ַ(���YUV��ַ������ſɽ�V��ַ����ΪNULL)
    //TBufGroupInfo atChip[MAX_BUF_GROUPS]; //�����BUF������Ϣ
    //l32 l32BufGroups;                        //�����BUF������Ŀ
    //l32 l32OffsetY;
    u32 u32Reserved;
} TImageMergeZoomInput;
 
//����ϳ�ģ������ṹ��
typedef struct 
{
    u32 u32Reserved;   //�������� 
} TImageMergeZoomOutput;

#endif
