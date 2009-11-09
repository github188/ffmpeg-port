#include "StdAfx.h"
#include "PtzControl.h"

#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"
#include "mcucommon.h"

CPtzControl::CPtzControl(void)
{
	m_bUseDigitPtz = false;
	m_nPtzIp = 0;
	m_nPtzPort = 0;
	m_ePtzOp = PTZ_STOP;
	
	m_hPtzSocket = INVALID_SOCKET;
}

CPtzControl::~CPtzControl(void)
{
	if ( INVALID_SOCKET != m_hPtzSocket )
	{
		closesocket( m_hPtzSocket );
		m_hPtzSocket = INVALID_SOCKET;
	}
	
}

void CPtzControl::SetPTZAddr( int ptzIp, int ptzPort )
{
	m_nPtzIp = ptzIp;
	m_nPtzPort = ptzPort;
}

void CPtzControl::SetUserId( tstring strUserId )
{
	m_strUserId = strUserId;
}

void CPtzControl::SetPuId( tstring strDev )
{
	m_strPuId = strDev;
}

void CPtzControl::SetOpId( PTZOpId opId )
{
	m_ePtzOp = opId;
}

void CPtzControl::SetParam(int nParam1, int nParam2)
{
	tstringstream temp1, temp2;
	temp1 << nParam1;
	temp2 << nParam2;
	this->SetParam( temp1.str(), temp2.str() );
}

void CPtzControl::SetParam(tstring strParam1, tstring strParam2 /* = _T */)
{
	m_strParam1 = strParam1;
	m_strParam2 = strParam2;
}

void CPtzControl::SetDigitPtz( BOOL bUseDigitPtz )
{
	if( !bUseDigitPtz )
	{
		// 发送一个停止命令，用来取消数字ptz的效果。
		PTZOpId eOldPtzCmd = this->m_ePtzOp;
		this->SetOpId( CPtzControl::PTZ_STOP );
		this->SendPTZCmd();
		this->SetOpId( eOldPtzCmd );
	}
	m_bUseDigitPtz = bUseDigitPtz;
}

BOOL CPtzControl::IsDigitalPtz() const
{
	return this->m_bUseDigitPtz;
}


void CPtzControl::SendPTZCmd()
{
	// 拼出要发送的字符串.
	tstring strPtzCmd = this->GetPtzCmd();

	// 字符编码转换.
	string strSentCmd = UTF16toUTF8( strPtzCmd );

	// 发送数据.
	this->NetSendPtzCmd( strSentCmd );

}

tstring CPtzControl::GetPtzCmd() const
{
	tstringstream ssContent;
	ssContent << _T( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n");
	ssContent << _T( "<Message Version=\"1.1\">");

	tstring strMsgRegType;
	if ( m_bUseDigitPtz )
	{
		strMsgRegType = _T( "MSG_DIGIT_PTZ_SET_REQ" );
	}
	else
	{
		strMsgRegType = _T( "MSG_PTZ_SET_REQ" );
	}

	ssContent << _T( "<IE_HEADER MessageType=\"" ) << strMsgRegType << _T( "\" " );


	//用户ID
	ssContent << _T( "UserID=\"") << m_strUserId << _T( "\" " );

	//设备
	ssContent << _T( "DestID=\"") << m_strPuId << _T( "\" " );

	// 换行.
	ssContent << _T( "/>\r\n"); // 结束 IE_HEADER.

	//操作类型
	ssContent << _T( "<IE_PTZ OpId=\"") << int( m_ePtzOp ) << _T( "\" ");

	//参数1
	ssContent << _T( "Param1=\"") << m_strParam1 << _T( "\" " );

	//参数2
	ssContent << _T( "Param2=\"") << m_strParam2 << _T( "\" />" );

	ssContent << _T( "</Message>");

	tstringstream ssSipPacket;
	ssSipPacket << _T( "INFO sip:gebroadcast@x SIP/2.0\r\n" );
	ssSipPacket << _T( "Content-Type: application/global_eye_v10+xml\r\n");
	ssSipPacket << _T( "Content-Length: " );

	int UserIdLen = m_strUserId.length();
	string strUtf8 = UTF16toUTF8( m_strUserId );
	int utf8Len = strUtf8.length();

	int nLen = ssContent.str().length() + ( utf8Len - UserIdLen);
	mcu::tlog << _T( "GetPtzCmd Len:" ) << nLen << endl;
	//int Size = strlen(ssContent.str().c_str());
	ssSipPacket << nLen << _T( "\r\n" );
	ssSipPacket << _T( "To: <sip:vau@kedacom.com>\r\n");
	ssSipPacket << _T( "From: <sip:mcu@kedacom.com>\r\n");
	ssSipPacket << _T( "CSeq: 1234 INFO");
	ssSipPacket << _T( "Call-ID: 01234567890abcdef\r\n");
	ssSipPacket << _T( "Max-Forwards: 70\r\n");
	ssSipPacket << _T( "Via: SIP/2.0/UDP 127.0.0.1;branch=z9hG4bK776asdhds\r\n");
	ssSipPacket << _T( "Contact: <sip:mcu@kedacom.com>\r\n\r\n");

	ssSipPacket << ssContent.str();
	

	ssSipPacket << _T( "\r\n" );

	return ssSipPacket.str();

}


bool CPtzControl::NetSendPtzCmd( string strCmd )
{
	if( INVALID_SOCKET == m_hPtzSocket )
	{
		m_hPtzSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	}
	
	if ( INVALID_SOCKET == m_hPtzSocket )
	{
		int nLastError = WSAGetLastError();
		mcu::tlog << _T( "Creat Socket Fail! ec: " ) << nLastError << endl;
		_ASSERT( FALSE );
		return false;
	}

	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof( serverAddr ) );


	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl( m_nPtzIp );
	serverAddr.sin_port = htons( m_nPtzPort );

	int nLen = strCmd.length();
	::sendto( m_hPtzSocket, strCmd.c_str(), nLen, 0, (sockaddr*)&serverAddr, sizeof( serverAddr ) );


	return true;
}