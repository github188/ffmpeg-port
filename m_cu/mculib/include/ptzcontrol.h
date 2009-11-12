#pragma once

#include "mcudefine.h"

class UsageEnvironment;

class CPtzControl
{
public:
	
public:
	CPtzControl(void);
	virtual ~CPtzControl(void);

	/** �Ƿ�ʹ��������̨. */
	void SetDigitPtz( BOOL bUseDigitPtz );
	/** ��ȡ��ǰ�Ƿ�������ptz�� */
	BOOL IsDigitalPtz() const;

	/** Ŀ���ַ.��ʹ��������!*/
	void SetPTZAddr( int ptzIp, int ptzPort );
	void SetPTZAddr( LPCTSTR strPtzIp, int nPtzPort );

	/** �����û���.*/
	void SetUserId( tstring strUserId );
	/** �豸id.*/
	void SetPuId( tstring strDev );
	/** ���ò�����.*/
	void SetPtzCmdId( EPTZCmdId opId );
	/** ���ò���.*/
	void SetParam( tstring strParam1, tstring strParam2 = _T( "" ) );
	void SetParam( int nParam1, int nParam2 );

	/** ����PTZ����.*/
	void SendPTZCmd();
	
private:
	/** ��ȡҪ���͵������ַ���.*/
	tstring GetPtzCmd() const;

	/** ���ͳ�����.*/
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

	/** ����ptz�����socket�� */
	int m_hPtzSocket;

	UsageEnvironment *m_pEnv;

};
