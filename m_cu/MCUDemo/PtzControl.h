#pragma once

#include "mcudef.h"
#include "Groupsock.hh"

class CPtzControl
{
public:
	//M_CU发给VAU的摄像头控制命令
	enum PTZOpId
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
public:
	CPtzControl(void);
	virtual ~CPtzControl(void);

	/** 是否使用数字云台. */
	void SetDigitPtz( BOOL bUseDigitPtz );
	/** 获取当前是否是数字ptz。 */
	BOOL IsDigitalPtz() const;

	/** 目标地址.都使用主机序!*/
	void SetPTZAddr( int ptzIp, int ptzPort );

	/** 设置用户名.*/
	void SetUserId( tstring strUserId );
	/** 设备id.*/
	void SetPuId( tstring strDev );
	/** 设置操作码.*/
	void SetOpId( PTZOpId opId );
	/** 设置参数.*/
	void SetParam( tstring strParam1, tstring strParam2 = _T( "" ) );
	void SetParam( int nParam1, int nParam2 );

	/** 发送PTZ命令.*/
	void SendPTZCmd();
	
private:
	/** 获取要发送的命令字符串.*/
	tstring GetPtzCmd() const;

	/** 发送出命令.*/
	bool NetSendPtzCmd( string strCmd );

private:
	tstring m_strUserId;
	tstring m_strPuId;
	tstring m_strParam1;
	tstring m_strParam2;

	int m_nPtzIp;
	int m_nPtzPort;

	PTZOpId m_ePtzOp;

	BOOL m_bUseDigitPtz;

	/** 发送ptz命令的socket。 */
	SOCKET m_hPtzSocket;

};
