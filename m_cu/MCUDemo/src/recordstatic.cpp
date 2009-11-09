#include "StdAfx.h"
#include "recordstatic.h"

CRecordStatic::CRecordStatic(void)
{
	m_bSelected = FALSE;
	m_colorSelectedFrame = RGB( 239, 186, 0 );
}

CRecordStatic::~CRecordStatic(void)
{
}

BEGIN_MESSAGE_MAP(CRecordStatic, CStaticEx)
	ON_WM_PAINT()
	
END_MESSAGE_MAP()


void CRecordStatic::OnPaint()
{
	
	CRect rcClient;
	this->GetClientRect( rcClient );

	CPaintDC dc( this );

	// ������.
	COLORREF colNor,ColDis, colBk;
	this->GetTextColor( colNor, ColDis );
	this->GetBkColor( colBk );

	dc.FillSolidRect( rcClient, colBk );
	

	// ��ѡ�з���
	if ( m_bSelected )
	{
		CBrush brushSel( m_colorSelectedFrame );

		CRect rcSelectedFrame = rcClient;
		dc.FrameRect( rcSelectedFrame, &brushSel );

		rcSelectedFrame.DeflateRect( 1,1,1,1 );			
		dc.FrameRect( rcSelectedFrame, &brushSel );
	}

	// д��.
	// �����ļ���.
	CString strRecFileBaseName = ParsePath( this->m_strRecPath.c_str() ).m_strBaseName.c_str();

	CTime cRecTime;
	tstring strPuName;
	BOOL bResult = this->GetRecInfo( strRecFileBaseName, strPuName, cRecTime );

	CString strDate;
	CString strTime;

	if ( bResult )
	{
		strDate.Format( _T( "%04d/%02d/%02d" ), cRecTime.GetYear(), cRecTime.GetMonth(), cRecTime.GetDay() );

		strTime.Format( _T( "%02d:%02d:%02d" ), cRecTime.GetHour(), cRecTime.GetMinute(), cRecTime.GetSecond() );
	}
	else
	{
		strDate = _T( "δ֪����" );
		strTime = _T( "δ֪ʱ��" );
	}
	
	DWORD dwDrawTextStyle = DT_CENTER | DT_VCENTER;

	CFont *pFont = this->GetFont();
	CFont *pOldFont = dc.SelectObject( pFont );

	
	dc.SetTextColor( this->IsWindowEnabled() ? colNor : ColDis );
	dc.SetBkColor( colBk );

	int nTextRectHeight = rcClient.Height() / 3;
	CRect rcDateText = rcClient;
	rcDateText.bottom = rcClient.top + nTextRectHeight;

	int nHeight = dc.DrawText( strDate, rcDateText, dwDrawTextStyle );

	CRect rcRecText = rcClient;
	rcRecText.top = rcDateText.bottom;
	rcRecText.bottom = rcClient.bottom - nTextRectHeight;	
	nHeight = dc.DrawText( _T( "¼��" ), rcRecText, dwDrawTextStyle );


	CRect rcTimeText = rcClient;
	rcTimeText.top = rcClient.bottom - nTextRectHeight;

	
	nHeight = dc.DrawText( strTime, rcTimeText, dwDrawTextStyle );

//	CStaticEx::OnPaint();
	dc.SelectObject( pOldFont );

}

void CRecordStatic::SetRecordPath( LPCTSTR strRecName )
{
	m_strRecPath = strRecName;

	if ( this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
}

tstring CRecordStatic::GetRecordPath() const
{
	return m_strRecPath;
}

void CRecordStatic::SetSelected( BOOL bSelected, BOOL bRepaint/* = TRUE*/ )
{
	this->m_bSelected = bSelected;
	if ( bRepaint && this->GetSafeHwnd() )
	{
		this->Invalidate();
	}
}

BOOL CRecordStatic::GetRecInfo( LPCTSTR lpstrRecFileName, tstring& strPuName, CTime& timeRec )
{
	// �ļ����� ǰ����_������ʱ����XXX ����XXX��3λ������֡� 
	
	CString strRecFileName = lpstrRecFileName;

	// ȥ���������������
	strRecFileName = strRecFileName.Left( strRecFileName.GetLength() - 3 );

	// ʱ�����ļ��������
	const int conTimeLen = 4 + 2 + 2 + 2 + 2 + 2; // ������ʱ���롣 
	CString strDateTime = strRecFileName.Right( conTimeLen );
	strPuName = strRecFileName.Left( strRecFileName.GetLength() - conTimeLen - 1 );// ȥ��ʱ���"_"���ӷ���

	__time64_t nTime = ::StrToTime( strDateTime );
	timeRec = CTime( nTime );

	return ( nTime > 0 );
}