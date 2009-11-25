// UDPTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "UDPTestDialog.h"

#include "PtzControl.h"

#include <iostream>

using namespace std;

// CUDPTestDialog 对话框

IMPLEMENT_DYNAMIC(CUDPTestDialog, CDialog)

CUDPTestDialog::CUDPTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPTestDialog::IDD, pParent)
	, m_strContent(_T("中文不好输入，所以直接在代码里写吧。"))
//	, m_strAddr(_T("172.16.163.34:1984"))
//	, m_strAddr(_T("218.242.128.205:8000"))
, m_strAddr( _T( "172.16.160.125:5060" ) )

	, m_udpSocket( INVALID_SOCKET )
{

}

CUDPTestDialog::~CUDPTestDialog()
{
}

void CUDPTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_strAddr);
	DDX_Control(pDX, IDC_EDIT_CONTENT, m_editContent);
}


BEGIN_MESSAGE_MAP(CUDPTestDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CUDPTestDialog::OnBnClickedButtonSend)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUDPTestDialog 消息处理程序

void CUDPTestDialog::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

//	MessageBox( m_strContent );

	// 解析网址中的ip和端口。
	int nPos = m_strAddr.Find( TEXT( ":" ) );
	if ( -1 == nPos )
	{
		MessageBox( TEXT( "解析网址出错！" ) );
		return;
	}
	CString strIp = m_strAddr.Left( nPos );
	CString strPort = m_strAddr.Right( m_strAddr.GetLength() - nPos - 1 );

//	MessageBox( strIp, strPort );

//	sockaddr serverAddr;
	

	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof( serverAddr ) );


	int nAddrLen = sizeof( serverAddr );
	int nResult = WSAStringToAddress( strIp.GetBuffer(), AF_INET, NULL, (sockaddr*)&serverAddr, &nAddrLen );
	if( SOCKET_ERROR == nResult )
	{
		int nErrorCode = WSAGetLastError();
		int sdfs = nErrorCode;
	}
	strIp.ReleaseBuffer();

	serverAddr.sin_family = AF_INET;
	//	serverAddr.sin_addr.s_addr = inet_addr( strIp );
	serverAddr.sin_port = htons( (short)_ttoi( strPort ) );

	int nLen = m_strContent.GetLength();
	LPTSTR lpBuf = m_strContent.GetBuffer();
//	::sendto( m_udpSocket, (const char*)lpBuf,nLen * sizeof( lpBuf[0] ), 0, (sockaddr*)&serverAddr, nAddrLen );
	m_strContent.ReleaseBuffer();

	// ptz
	CPtzControl ptzTest;
	ptzTest.SetPTZAddr( ntohl( serverAddr.sin_addr.s_addr ), _ttoi( strPort ) );
	ptzTest.SetPtzCmdId( EPTZCmdId( 1 ) );
	ptzTest.SetParam( 3, 1 );
	ptzTest.SetPuId( _T( "27000000000000000011200003300000-1" ) );
	ptzTest.SetUserId( _T( "admin@888.js" ) );
	ptzTest.SendPTZCmd();
}

BOOL CUDPTestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化网络。
	// 这个工程已经在进程初始化的时候初始化了网络库。
	// Initialize Winsock.
	//int iRet = WSAStartup( MAKEWORD(2,2), &wsaData);
	//if(iRet != NO_ERROR )
	//{
	//	MessageBox( TEXT( "初始化网络失败！" ) );
	//}

	m_udpSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( INVALID_SOCKET == m_udpSocket )
	{
		int nLastError = WSAGetLastError();
		_ASSERT( FALSE );
	}

	sockaddr_in name;
	memset( &name, 0, sizeof( name ) );

	name.sin_family = AF_INET;
	name.sin_port = htons( 6984 );


	int nResult = ::bind( m_udpSocket, (sockaddr*)&name, sizeof( name ) );
	if( SOCKET_ERROR == nResult )
	{
		wcout << _T( "bind error: " ) << WSAGetLastError() << endl;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUDPTestDialog::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( INVALID_SOCKET != m_udpSocket )
	{
		closesocket( m_udpSocket );
//		CloseHandle( m_udpSocket );
	}

	CDialog::PostNcDestroy();
}

void CUDPTestDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if ( m_editContent.GetSafeHwnd() )
	{
		CRect rcClient;
		GetClientRect( rcClient );

		rcClient.bottom -= 10;

		m_editContent.MoveWindow( rcClient );
	}
}
