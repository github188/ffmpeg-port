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
	/** 录像文件. */
	void SetRecordPath( LPCTSTR strRecPath );

	/** 获取录像名. */
	tstring GetRecordPath() const;

	/** 设置选中. */
	void SetSelected( BOOL bSelected, BOOL bRepaint = TRUE );

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	/** 通过文件名获取录像时间和前端名。 */
	BOOL GetRecInfo( LPCTSTR strRecFileName, tstring& strPuName, CTime& timeRec );
private:
	/** 文件名. */
	tstring m_strRecPath;

	/** 是否选中. */
	BOOL m_bSelected;

	/** 选中框颜色。 */
	COLORREF m_colorSelectedFrame;
};
