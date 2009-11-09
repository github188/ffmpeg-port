#pragma once
#include "staticex.h"
#include "mcucommon.h"

class CRecordStatic :
	public CStaticEx
{
public:
	CRecordStatic(void);
	virtual ~CRecordStatic(void);

public:
	/** ¼���ļ�. */
	void SetRecordPath( LPCTSTR strRecPath );

	/** ��ȡ¼����. */
	tstring GetRecordPath() const;

	/** ����ѡ��. */
	void SetSelected( BOOL bSelected, BOOL bRepaint = TRUE );

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	/** ͨ���ļ�����ȡ¼��ʱ���ǰ������ */
	BOOL GetRecInfo( LPCTSTR strRecFileName, tstring& strPuName, CTime& timeRec );
private:
	/** �ļ���. */
	tstring m_strRecPath;

	/** �Ƿ�ѡ��. */
	BOOL m_bSelected;

	/** ѡ�п���ɫ�� */
	COLORREF m_colorSelectedFrame;
};
