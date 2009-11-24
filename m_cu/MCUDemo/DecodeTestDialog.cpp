// DecodeTestDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCUDemo.h"
#include "DecodeTestDialog.h"
#include "decoder.h"
#ifdef USE_FFMPEG
#include "avcodec.h"
#endif


// CDecodeTestDialog �Ի���

IMPLEMENT_DYNAMIC(CDecodeTestDialog, CDialog)

CDecodeTestDialog::CDecodeTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDecodeTestDialog::IDD, pParent)
	, m_bShowVideo(TRUE)
	, m_strFilePath(_T("video-H264-1.264"))
	, m_bRecord(FALSE)
{
	m_bRunFlag = FALSE;
	m_pDecodeThread = 0;
}

CDecodeTestDialog::~CDecodeTestDialog()
{
}

void CDecodeTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SHOW_VIDEO, m_bShowVideo);
	DDX_CBString(pDX, IDC_COMBO_FILE_PATH, m_strFilePath);
	DDX_Control(pDX, IDC_COMBO_FILE_PATH, m_cmbFilePath);
	DDX_Check(pDX, IDC_CHECK_RECORD, m_bRecord);
}


BEGIN_MESSAGE_MAP(CDecodeTestDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXPLORE, &CDecodeTestDialog::OnBnClickedButtonExplore)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CDecodeTestDialog::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_CHECK_RECORD, &CDecodeTestDialog::OnBnClickedCheckRecord)
END_MESSAGE_MAP()


// CDecodeTestDialog ��Ϣ�������

void CDecodeTestDialog::OnBnClickedButtonExplore()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fdlg( TRUE );
	fdlg.DoModal();

	CString strPath = fdlg.GetPathName();

}

void CDecodeTestDialog::OnBnClickedButtonPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if ( m_strFilePath.IsEmpty() )
	{
		MessageBox( _T( "��Ƶ�ļ�·���ǿյ�!!" ) );
		return;
	}

	mcu::log << _T( "OnBnClickedButtonPlay: " ) << LPCTSTR( m_strFilePath ) << endl;

	// ����.
	Reset();

	if ( m_bShowVideo )
	{
		m_cVideoWndDlg.Create( CVideoWndDialog::IDD, this );

		CRect rcClient;
		this->GetClientRect( rcClient );
		rcClient.bottom -= 50;

		m_cVideoWndDlg.MoveWindow( rcClient );
		m_cVideoWndDlg.ShowWindow( SW_SHOW );
	}

	mcu::log << _T( "before SDL_CreateThread" ) << endl;
	m_bRunFlag = TRUE;
	m_pDecodeThread = SDL_CreateThread( DecodeThread, this );

	mcu::log << _T( "Create Thread: " ) << int( m_pDecodeThread ) << endl;
	
}

int CDecodeTestDialog::DecodeThread( void * pvThis )
{
	mcu::log << _T( "DecodeThread run!!" ) << endl;
	CDecodeTestDialog *pThis = (CDecodeTestDialog*)pvThis;

	CString strFilePath = pThis->m_strFilePath;

	CFile videoFile;
	CFileException cfex;
	BOOL bResult = videoFile.Open( strFilePath, CFile::modeRead | CFile::typeBinary, &cfex );
	if ( !bResult )
	{
		AfxMessageBox( strFilePath + _T( "\n��Ƶ�ļ���ʧ��!" ) );
		CString strMsg;
		cfex.GetErrorMessage( strMsg.GetBuffer( 1000 ), 1000 );
		strMsg.ReleaseBuffer();
		AfxMessageBox( strMsg );

		return 0;
	}

	strFilePath.MakeUpper();

	CDecoder::ECodecId eCodecId;
	if ( strFilePath.Find( _T( "264" ) ) != -1 )
	{
		eCodecId = CDecoder::CODEC_H264;
	}
	else if( strFilePath.Find( _T( "MPEG4" ) ) != -1 || strFilePath.Find( _T( "M4V" ) ) != -1 ) 
	{
		eCodecId = CDecoder::CODEC_MPEG4;
	}
	else
	{
		AfxMessageBox( _T( "��֪����Ӧ�Ľ�����!" ) );
		return 0;
	}

	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if ( NULL == pDec )
	{
		pDec = CDecoder::CreateDecoder( eCodecId, 1000, MAIN_DECODER_NAME );
	}

	if ( NULL == pDec )
	{
		AfxMessageBox( _T( "����������ʧ��!!!" ) );
		return 0;
	}

	if ( pThis->m_bRecord )
	{
		pDec->StartRecord( _T( "0000test.3gp" ) );
	}

	CTime cBeforeDecode = CTime::GetCurrentTime();

	int nFrameCount = 0;

	const int conBufLen = 100;
	while ( pThis->m_bRunFlag )
	{
		unsigned char videoBuf[conBufLen];
		UINT nReadLen = videoFile.Read( videoBuf, conBufLen );

		if ( nReadLen == 0 )
		{
			break;
		}

		CBaseCodec::TStreamPacket tPkt;
		tPkt.data = videoBuf;
		tPkt.datalen = nReadLen;
	//	tPkt.timeStamp.tv_usec = ( nFrameCount * 100 ) % 1000;	// �����衣
		tPkt.timeStamp = nFrameCount * 125 * 1000;

		int nOldFrameCount = nFrameCount;
		nFrameCount += pDec->InputStreamSync( tPkt );

		if ( nFrameCount > nOldFrameCount )
		{
			mcu::log << _T( "frame count: " ) << nFrameCount << endl;
		}
		
	}

	CTime cAfterDecode = CTime::GetCurrentTime();

	CTimeSpan cUseTime = cAfterDecode - cBeforeDecode;
	int nSeconds = (int) cUseTime.GetTotalSeconds();
	float fFramePS = (float)nFrameCount / (float)nSeconds;

	tstringstream ssMsg;
	ssMsg << _T( "total frame: " ) << nFrameCount << _T( " useTime(s): " ) 
		<< nSeconds << _T( "FPS: " ) << fFramePS << endl;
	mcu::log << ssMsg.str();
	AfxMessageBox( ssMsg.str().c_str() );

	CDecoder::Release( pDec );

	pThis->m_pDecodeThread = 0;

#ifdef USE_FFMPEG
	CodecID eCodecId;
	if ( strFilePath.Find( _T( "264" ) ) != -1 )
	{
		eCodecId = CODEC_ID_H264;
	}
	else if( strFilePath.Find( _T( "MPEG4" ) ) != -1 ) 
	{
		eCodecId = CODEC_ID_MPEG4;
	}
	else
	{
		AfxMessageBox( _T( "��֪����Ӧ�Ľ�����!" ) );
		return 0;
	}

	DecoderID decoderId = CreateDecoder( eCodecId );

	CTime cBeforeDecode = CTime::GetCurrentTime();

	int nFrameCount = 0;

	const int conBufLen = 10000;
	while ( pThis->m_bRunFlag )
	{
		unsigned char videoBuf[conBufLen];
		UINT nReadLen = videoFile.Read( videoBuf, conBufLen );
		
		if ( nReadLen == 0 )
		{
			break;
		}
		nFrameCount += InputData( decoderId, videoBuf, nReadLen );

		mcu::log << _T( "frame count: " ) << nFrameCount << endl;
	}

	CTime cAfterDecode = CTime::GetCurrentTime();

	CTimeSpan cUseTime = cAfterDecode - cBeforeDecode;
	int nSeconds = (int) cUseTime.GetTotalSeconds();
	float fFramePS = (float)nFrameCount / (float)nSeconds;

	tstringstream ssMsg;
	ssMsg << _T( "total frame: " ) << nFrameCount << _T( " useTime(s): " ) 
		<< nSeconds << _T( "FPS: " ) << fFramePS << endl;
	mcu::log << ssMsg.str();
	AfxMessageBox( ssMsg.str().c_str() );

	DestroyDecoder( decoderId );

	pThis->m_pDecodeThread = 0;

#endif

	return 0;
}

void CDecodeTestDialog::Reset()
{
	m_bRunFlag = FALSE;


	if ( m_pDecodeThread  )
	{
		int nStatus;
		SDL_WaitThread( m_pDecodeThread, &nStatus );
	}

	if ( m_cVideoWndDlg.GetSafeHwnd() )
	{
		m_cVideoWndDlg.DestroyWindow();
	}

}
BOOL CDecodeTestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->m_cmbFilePath.AddString( _T( "video-MP4V-ES-1.mpeg4" ) );
	this->m_cmbFilePath.AddString( _T( "video-H264-1.264" ) );

	this->m_cmbFilePath.AddString( _T( "\\Storage Card\\testfile\\shangyansuo.5.20.264" ) );
	this->m_cmbFilePath.AddString( _T( "\\Storage Card\\testfile\\shangyansuo.5.20-st1.264" ) );

	this->m_cmbFilePath.AddString( _T( "akiyo_cif_1m.m4v" ) );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDecodeTestDialog::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->m_cVideoWndDlg.DestroyWindow();

	return CDialog::DestroyWindow();
}


void CDecodeTestDialog::OnBnClickedCheckRecord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	CDecoder *pDec = CDecoder::FindDecoder( MAIN_DECODER_NAME );
	if( pDec )
	{
		BOOL bSuccess = FALSE;
		if ( m_bRecord )
		{
			bSuccess = pDec->StartRecord( _T( "00rectest.3gp" ) );
		}
		else
		{
			bSuccess = pDec->StopRecord();
		}
		
		if ( !bSuccess )
		{
			mcu::log << _T( "record operation fail!" ) << endl;
		}
	}
}
