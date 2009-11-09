#pragma once

#include "mcudef.h"
#include "Groupsock.hh"

class CPtzControl
{
public:
	//M_CU����VAU������ͷ��������
	enum PTZOpId
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
public:
	CPtzControl(void);
	virtual ~CPtzControl(void);

	/** �Ƿ�ʹ��������̨. */
	void SetDigitPtz( BOOL bUseDigitPtz );
	/** ��ȡ��ǰ�Ƿ�������ptz�� */
	BOOL IsDigitalPtz() const;

	/** Ŀ���ַ.��ʹ��������!*/
	void SetPTZAddr( int ptzIp, int ptzPort );

	/** �����û���.*/
	void SetUserId( tstring strUserId );
	/** �豸id.*/
	void SetPuId( tstring strDev );
	/** ���ò�����.*/
	void SetOpId( PTZOpId opId );
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

	PTZOpId m_ePtzOp;

	BOOL m_bUseDigitPtz;

	/** ����ptz�����socket�� */
	SOCKET m_hPtzSocket;

};
