#pragma once

#include "portabledefine.h"
#ifdef _WIN32_WCE
#include "stdafx.h"
#endif

/** by lizhixing @09.10.06
*	���������ʹ�õĳ����ȡ�
*/


enum EStreamType
{
	STREAM_1X = 1,
	STREAM_EVDO,
	STREAM_RESERVED1,
};

enum ELoginType
{
	LOGIN_NORMAL = 0,		// ��ͨ��¼��
	LOGIN_LOCAL_PHONE = 1,	// ������¼��
//	LOGIN_CANCEL,			// ȡ����¼��

};

#ifndef _WIN32

#define WM_USER (0x0400)

#endif

/** ��Ϣ����. 
*	only for wince 
*/

enum WM_MCU_MESSAGE
{
	WM_PTZ_BUTTON_DOWN = WM_USER + 100, // ptz��ť����,wparam:�ؼ�id.
	WM_PTZ_BUTTON_UP,					// ptz��ţ̌��,wparam:�ؼ�id.

    WM_WINDOW_CLOSE_CMD,                // �رյ�ǰ���ڡ�

	WM_HTML_CLICK,						// �û��ĵ��.wparam: LPCTSTR ���ַ���.
	WM_HTML_PRE_LOAD,					// ��Ӧ��ҳ������֮ǰ. wparam:  LPCTSTR���ַ���.
	WM_HTML_META,						// Meta��ǩ.	wparam: LPCTSTR ��ǰurl, lparam: LPCTSTR meta�е�����
//    WM_HTML_CREATE_CMD,                 // ������ҳ�ؼ����

	WM_VIDEO_PLAY_STATUS,				// ��Ƶ����״̬.
	WM_VIDEO_OPEN_FAIL,					// ����Ƶʧ��.WParam: EMCU_ErrorCode�������롣
	WM_RECORD_FAIL,						// ¼��ʧ�ܡ�

	WM_ACTIVE_WINDOW,					// �������.

	WM_FULLSCREEN,						// ȫ������.
	WM_SCREEN_DISPLAY_MODE,				// ��Ļ��ʾģʽ(����)�ı�.
	WM_FS_SET_WINDOW_POS,				// ȫ����ʱ�������Ϣ�����ڵ�λ��/��С�ı�.
	WM_ENUM_WINDOWS_NOTIFY,				// ö�ٴ���֪ͨ. WPARAM: �õ��Ĵ��ھ��.

	WM_VIDEO_SHOW_CLICK,				// ��Ƶ���ڵ��
	WM_VIDEO_CREATE,					// ������Ƶ���ڡ�
	WM_VIDEO_DESTROY,					// ������Ƶ���ڡ�
};


/** �����롣 */
enum EMCU_ErrorCode
{
	MCU_Sucess,
	MCU_Error_Unknow,				// δ֪����
	MCU_Error_Storage_Full,			// �洢�ռ�����
	MCU_Error_Rtsp_Server_Full,		// �������ﵽ�����ת��������
	MCU_Error_Decoder_Null,			// �����û�г�ʼ����
	MCU_Error_Rtsp_Fail,			// RTSP����ʧ�ܡ�
    MCU_Error_PlayStop,             // ¼�������ֹͣ���š�
};

//M_CU����VAU������ͷ��������
enum EPTZCmdId
{
	PTZ_MOVEUP =      1,		//�����ƶ�
	PTZ_MOVEDOWN =    2,		//�����ƶ�
	PTZ_MOVELEFT =    3,		//�����ƶ�
	PTZ_MOVERIGHT =   4,		//�����ƶ�
	PTZ_BRIGHTUP =    5,		//�������
	PTZ_BRIGHTDOWN =  6,		//�������
	PTZ_ZOOMTELE =    7,		//��������ͷ
	PTZ_ZOOMWIDE =    8,		//��Զ����ͷ
	PTZ_FOCUSNEAR =   9,		//���������
	PTZ_FOCUSFAR =    10,		//�������Զ

	PTZ_STOP =        15,		//ֹͣ�ƶ�
	PTZ_PRESETSET =   16,		//����ͷԤ��
	PTZ_PRESETCALL =  17,		//������ͷԤ��
	PTZ_CAMERASET =   18,		//��ʼ������ͷ
	PTZ_SAVESET =     19,		//����Ԥ��λ
	PTZ_SCANOFF =     20,		//ɨ��ֹͣ
	PTZ_SCANON =      21,		//ɨ�迪ʼ
	PTZ_SWITCHON =    22,		//�򿪵ƹ�
	PTZ_SWITCHOFF =   23,		//�ص��ƹ�
};

/** ����⵱ǰ��״̬�� */
enum ERTSPStatus
{
	RTSPStatus_Idle = 0,	// ����״̬.
	RTSPStatus_Init,		// ���ڳ�ʼ��.
	RTSPStatus_Opition,	// ���ڵȴ�OPITION����Ļظ�.
	RTSPStatus_Description,	// �ȴ�Description
	RTSPStatus_Setup,		// �ȴ�Setup.
	RTSPStatus_Play,		// �ȴ�����.
	RTSPStatus_Running,		// ����������.
    RTSPStatus_WaitingPacket,   // �ȴ�������

	RTSPStatus_Error_Unknown,		// δ֪����
	RTSPStatus_Error_Server_Full,	// �������ﵽ���������
	RTSPStatus_Error_Init_Fail,		// ��ʼ��ʧ�ܡ�
	RTSPStatus_Error_Opition,		// Opition ����ʧ�ܡ�
	RTSPStatus_Error_Description,
	RTSPStatus_Error_Setup,
	RTSPStatus_Error_Play,			
	RTSPStatus_Error_SDP,			// ����sdp��Ϣ����
	RTSPStatus_Error_Create_Rcv,	// �������մ���ʧ�ܡ�
    RTSPStatus_Error_WaitPacket,    // �ȴ�����ʧ�ܣ���ʱ��
    RTSPStatus_Error_Decoder_Fail,  // ����������

};


/** ��������. */
extern LPCTSTR MAIN_DECODER_NAME;

extern LPCTSTR MAIN_KEDA_DECODER_NAME;

/** ����Ľ���ͼƬĿ¼�� */
extern LPCTSTR UI_PIC_DIR ;

/** ��С�Ĵ洢ʣ��ռ䷧ֵ��С�ڴ˷�ֵ��ץ�ĺ�¼��ȫ��ֹͣ��ʧ�ܡ� 
*	���ֵҲ�����������ļ��ж�ȡ���������ļ����������ȼ����ߣ�
*	��������ļ���û�����ò�ʹ��Ԥ��ġ�
*/
extern const int MIN_STORAGE_SPACE ;



// ������Կ��
extern char s_EncKey[16] ;


