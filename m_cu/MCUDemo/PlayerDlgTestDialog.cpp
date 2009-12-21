// PlayerDlgTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mcu.h"
#include "PlayerDlgTestDialog.h"
#include "mculib.h"
#include "PlayerDialog.h"

#ifdef USE_FFMPEG
#include "avcodec.h"
#endif

// CPlayerDlgTestDialog 对话框

IMPLEMENT_DYNAMIC(CPlayerDlgTestDialog, CDialog)

CPlayerDlgTestDialog::CPlayerDlgTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDlgTestDialog::IDD, pParent)
	, m_strSelVideoPath(_T(""))
	, m_nFPS(2)
{

}

CPlayerDlgTestDialog::~CPlayerDlgTestDialog()
{
}

void CPlayerDlgTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FILE_PATH, m_cmbVideoFile);
	DDX_CBString(pDX, IDC_COMBO_FILE_PATH, m_strSelVideoPath);
	DDX_Text(pDX, IDC_EDIT_FPS, m_nFPS);
	DDV_MinMaxUInt(pDX, m_nFPS, 1, 100);
}


BEGIN_MESSAGE_MAP(CPlayerDlgTestDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CPlayerDlgTestDialog::OnBnClickedButtonPlay)
END_MESSAGE_MAP()


// CPlayerDlgTestDialog 消息处理程序

void CPlayerDlgTestDialog::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	if( !UpdateData() )
	{
		return;
	}


	if ( m_strSelVideoPath.IsEmpty() )
	{
		MessageBox( _T( "视频文件路径是空的!!" ) );
		return;
	}

	Log() << _T( "OnBnClickedButtonPlay: " ) << LPCTSTR( m_strSelVideoPath ) << endl;

	// 销毁.
	Reset();

	Log() << _T( "before SDL_CreateThread" ) << endl;
	m_bRunFlag = TRUE;
	m_pDecodeThread = SDL_CreateThread( DecodeThread, this );

	Log() << _T( "Create Thread: " ) << int( m_pDecodeThread ) << endl;

	CPlayerDialog dlg;
	dlg.DoModal();

	this->Reset();
}

void CPlayerDlgTestDialog::Reset()
{
	m_bRunFlag = FALSE;


	if ( m_pDecodeThread  )
	{
		int nStatus;
		SDL_WaitThread( m_pDecodeThread, &nStatus );
	}
}

int CPlayerDlgTestDialog::DecodeThread( void * pvThis )
{
	Log() << _T( "DecodeThread run!!" ) << endl;
	CPlayerDlgTestDialog *pThis = (CPlayerDlgTestDialog*)pvThis;

	CString strFilePath = pThis->m_strSelVideoPath;

	CFile videoFile;
	CFileException cfex;
	BOOL bResult = videoFile.Open( strFilePath, CFile::modeRead | CFile::typeBinary, &cfex );
	if ( !bResult )
	{
		AfxMessageBox( strFilePath + _T( "\n视频文件打开失败!" ), MB_TOPMOST );
		CString strMsg;
		cfex.GetErrorMessage( strMsg.GetBuffer( 1000 ), 1000 );
		strMsg.ReleaseBuffer();
		AfxMessageBox( strMsg, MB_TOPMOST );

		return 0;
	}

	strFilePath.MakeUpper();

#ifdef USE_FFMPEG

	CodecID eCodecId;
	if ( strFilePath.Find( _T( "H264" ) ) != -1 )
	{
		eCodecId = CODEC_ID_H264;
	}
	else if( strFilePath.Find( _T( "MPEG4" ) ) != -1 ) 
	{
		eCodecId = CODEC_ID_MPEG4;
	}
	else
	{
		AfxMessageBox( _T( "不知道对应的解码器!" ) );
		return 0;
	}

	DecoderID decoderId = CreateDecoder( eCodecId );

	CTime cBeforeDecode = CTime::GetCurrentTime();

	int nFrameCount = 0;

	int nSleepTime = 1000/pThis->m_nFPS;

	const int conBufLen = 100;
	while ( pThis->m_bRunFlag )
	{
		unsigned char videoBuf[conBufLen];
		UINT nReadLen = videoFile.Read( videoBuf, conBufLen );

		if ( nReadLen == 0 )
		{
			break;
		}
		int nInputFrame = InputData( decoderId, videoBuf, nReadLen );
		nFrameCount += nInputFrame;

		if ( nInputFrame > 0 )
		{
			Sleep( nSleepTime );
		}
		

	//	Log() << _T( "frame count: " ) << nFrameCount << endl;
	}

	CTime cAfterDecode = CTime::GetCurrentTime();

	CTimeSpan cUseTime = cAfterDecode - cBeforeDecode;
	LONGLONG nSeconds = cUseTime.GetTotalSeconds();
	float fFramePS = (float)nFrameCount / (float)nSeconds;

	tstringstream ssMsg;
	ssMsg << _T( "total frame: " ) << nFrameCount << _T( " useTime(s): " ) 
		<< nSeconds << _T( "FPS: " ) << fFramePS << endl;
	Log() << ssMsg.str();
	AfxMessageBox( ssMsg.str().c_str() );

	DestroyDecoder( decoderId );

	pThis->m_pDecodeThread = 0;
#endif
	return 0;
}

BOOL CPlayerDlgTestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	this->m_cmbVideoFile.AddString( _T( "video-H264-1.264" ) );
	this->m_cmbVideoFile.AddString( _T( "video-MP4V-ES-1.mpeg4" ) );

	this->m_cmbVideoFile.SetCurSel( 0 );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
