#pragma once

#include "mcudefine.h"

class UsageEnvironment;

class CPtzControl
{
public:
	
public:
	CPtzControl(void);
	virtual ~CPtzControl(void);

	/** 是否使用数字云台. */
	void SetDigitPtz( BOOL bUseDigitPtz );
	/** 获取当前是否是数字ptz。 */
	BOOL IsDigitalPtz() const;

	/** 目标地址.都使用主机序!*/
	void SetPTZAddr( int ptzIp, int ptzPort );
	void SetPTZAddr( LPCTSTR strPtzIp, int nPtzPort );

	/** 设置用户名.*/
	void SetUserId( tstring strUserId );
	/** 设备id.*/
	void SetPuId( tstring strDev );
	/** 设置操作码.*/
	void SetPtzCmdId( EPTZCmdId opId );
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

	EPTZCmdId m_ePtzOp;

	BOOL m_bUseDigitPtz;

	/** 发送ptz命令的socket。 */
	int m_hPtzSocket;

	UsageEnvironment *m_pEnv;

};
