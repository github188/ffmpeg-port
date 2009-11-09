#pragma once

#include "mcudef.h"

/** ��Ƶ����Ự. */
class CVideoSession
{
public:
	CVideoSession(void);
	virtual ~CVideoSession(void);

	tstring PuId() const { return m_strPuId; }
	void PuId(tstring val) { m_strPuId = val; }

	/** ptz ip addr */
	tstring PtzAddr() const { return m_tPtzAddr; }
	void PtzAddr(tstring val) { m_tPtzAddr = val; }

	/** ptz port, ������ host byte order */
	int PtzPort() const { return m_ptzPort; }
	void PtzPort(int val) { m_ptzPort = val; }

	/** stream type*/
	EStreamType StreamType() const { return m_eStreamType; }
	void StreamType(EStreamType val) { m_eStreamType = val; }

	/** ptzȨ�� */
	BOOL PtzControl() const { return m_bPtzControl; }
	void PtzControl(BOOL val) { m_bPtzControl = val; }

	/** ǰ������ */
	tstring PuName() const { return m_strPuName; }
	void PuName(tstring val) 
	{ 
		m_strPuName = val; 
		mcu::tlog << _T( "pu name: " ) << m_strPuName << endl;
	}
private:
	/** ptz ip addr */
	tstring m_tPtzAddr;

	/** ptz port, ������ host byte order */
	int m_ptzPort;

	/** puid */
	tstring m_strPuId;

	/** rtsp url */
	tstring m_strRtspUrl;

	/** pu name*/
	tstring m_strPuName;

	/** stream type*/
	EStreamType m_eStreamType;

	/** ptz Ȩ��*/
	BOOL m_bPtzControl;

	

};
