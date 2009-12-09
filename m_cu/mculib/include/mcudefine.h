#pragma once

#include "portabledefine.h"
#ifdef _WIN32_WCE
#include "stdafx.h"
#endif

/** by lizhixing @09.10.06
*	定义程序中使用的常量等。
*/


enum EStreamType
{
	STREAM_1X = 1,
	STREAM_EVDO,
	STREAM_RESERVED1,
};

enum ELoginType
{
	LOGIN_NORMAL = 0,		// 普通登录。
	LOGIN_LOCAL_PHONE = 1,	// 本机登录。
//	LOGIN_CANCEL,			// 取消登录。

};

#ifndef _WIN32

#define WM_USER (0x0400)

#endif

/** 消息定义. 
*	only for wince 
*/

enum WM_MCU_MESSAGE
{
	WM_PTZ_BUTTON_DOWN = WM_USER + 100, // ptz按钮按下,wparam:控件id.
	WM_PTZ_BUTTON_UP,					// ptz按钮抬起,wparam:控件id.

    WM_WINDOW_CLOSE_CMD,                // 关闭当前窗口。

	WM_HTML_CLICK,						// 用户的点击.wparam: LPCTSTR 的字符串.
	WM_HTML_PRE_LOAD,					// 响应在页面载入之前. wparam:  LPCTSTR的字符串.
	WM_HTML_META,						// Meta标签.	wparam: LPCTSTR 当前url, lparam: LPCTSTR meta中的内容
//    WM_HTML_CREATE_CMD,                 // 创建网页控件命令。

	WM_VIDEO_PLAY_STATUS,				// 视频播放状态.
	WM_VIDEO_OPEN_FAIL,					// 打开视频失败.WParam: EMCU_ErrorCode，错误码。
	WM_RECORD_FAIL,						// 录像失败。

	WM_ACTIVE_WINDOW,					// 激活程序.

	WM_FULLSCREEN,						// 全屏命令.
	WM_SCREEN_DISPLAY_MODE,				// 屏幕显示模式(方向)改变.
	WM_FS_SET_WINDOW_POS,				// 全屏的时候发这个消息处理窗口的位置/大小改变.
	WM_ENUM_WINDOWS_NOTIFY,				// 枚举窗口通知. WPARAM: 得到的窗口句柄.

	WM_VIDEO_SHOW_CLICK,				// 视频窗口点击
	WM_VIDEO_CREATE,					// 创建视频窗口。
	WM_VIDEO_DESTROY,					// 销毁视频窗口。
};


/** 错误码。 */
enum EMCU_ErrorCode
{
	MCU_Sucess,
	MCU_Error_Unknow,				// 未知错误。
	MCU_Error_Storage_Full,			// 存储空间满。
	MCU_Error_Rtsp_Server_Full,		// 服务器达到了最大转码能力。
	MCU_Error_Decoder_Null,			// 解码库没有初始化。
	MCU_Error_Rtsp_Fail,			// RTSP交互失败。
    MCU_Error_PlayStop,             // 录像等遇到停止播放。
};

//M_CU发给VAU的摄像头控制命令
enum EPTZCmdId
{
	PTZ_MOVEUP =      1,		//向上移动
	PTZ_MOVEDOWN =    2,		//向下移动
	PTZ_MOVELEFT =    3,		//向左移动
	PTZ_MOVERIGHT =   4,		//向右移动
	PTZ_BRIGHTUP =    5,		//画面调亮
	PTZ_BRIGHTDOWN =  6,		//画面调暗
	PTZ_ZOOMTELE =    7,		//拉近摄像头
	PTZ_ZOOMWIDE =    8,		//拉远摄像头
	PTZ_FOCUSNEAR =   9,		//将焦距调近
	PTZ_FOCUSFAR =    10,		//将焦距调远

	PTZ_STOP =        15,		//停止移动
	PTZ_PRESETSET =   16,		//摄象头预存
	PTZ_PRESETCALL =  17,		//调摄象头预存
	PTZ_CAMERASET =   18,		//初始化摄像头
	PTZ_SAVESET =     19,		//保存预置位
	PTZ_SCANOFF =     20,		//扫描停止
	PTZ_SCANON =      21,		//扫描开始
	PTZ_SWITCHON =    22,		//打开灯光
	PTZ_SWITCHOFF =   23,		//关掉灯光
};

/** 网络库当前的状态。 */
enum ERTSPStatus
{
	RTSPStatus_Idle = 0,	// 闲置状态.
	RTSPStatus_Init,		// 正在初始化.
	RTSPStatus_Opition,	// 正在等待OPITION命令的回复.
	RTSPStatus_Description,	// 等待Description
	RTSPStatus_Setup,		// 等待Setup.
	RTSPStatus_Play,		// 等待播放.
	RTSPStatus_Running,		// 正常运行中.
    RTSPStatus_WaitingPacket,   // 等待码流。

	RTSPStatus_Error_Unknown,		// 未知错误。
	RTSPStatus_Error_Server_Full,	// 服务器达到了最大能力
	RTSPStatus_Error_Init_Fail,		// 初始化失败。
	RTSPStatus_Error_Opition,		// Opition 命令失败。
	RTSPStatus_Error_Description,
	RTSPStatus_Error_Setup,
	RTSPStatus_Error_Play,			
	RTSPStatus_Error_SDP,			// 解析sdp信息出错。
	RTSPStatus_Error_Create_Rcv,	// 码流接收创建失败。
    RTSPStatus_Error_WaitPacket,    // 等待码流失败（超时）
    RTSPStatus_Error_Decoder_Fail,  // 解码器错误。

};


/** 解码器名. */
extern LPCTSTR MAIN_DECODER_NAME;

extern LPCTSTR MAIN_KEDA_DECODER_NAME;

/** 程序的界面图片目录。 */
extern LPCTSTR UI_PIC_DIR ;

/** 最小的存储剩余空间阀值。小于此阀值，抓拍和录像全部停止或失败。 
*	这个值也可以在配置文件中读取，且配置文件的配置优先级更高，
*	如果配置文件中没有配置才使用预设的。
*/
extern const int MIN_STORAGE_SPACE ;



// 加密密钥。
extern char s_EncKey[16] ;


