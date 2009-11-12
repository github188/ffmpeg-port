#include "virtualpicmanagewnd.h"
#include "log.h"
#include "mcuconfig.h"

CVirtualPicManageWnd::CVirtualPicManageWnd(void)
{
	m_nCurPageNum = 0;
	m_nPicArrayRow = 0;
	m_nPicArrayCol = 0;
}

CVirtualPicManageWnd::~CVirtualPicManageWnd(void)
{
}

void CVirtualPicManageWnd::SetPicFolder( LPCTSTR strFolder )
{
	this->m_strPicFolder = strFolder;

	this->CheckUpdatePicFile();
}

void CVirtualPicManageWnd::UpdatePicShow()
{
	for ( int nRow = 0; nRow < m_nPicArrayRow; ++nRow )
	{
		for ( int nCol = 0; nCol < m_nPicArrayCol; ++nCol )
		{
			tstring strPicName = this->GetPicName( nRow, nCol );

			// 通知子类显示图片。

			this->ShowPic( nRow, nCol, strPicName.c_str() );						
		}
	}

}

void CVirtualPicManageWnd::CheckUpdatePicFile( )
{
	TFileList tAllFile;

	if ( m_strPicFolder.empty() )
	{
		mcu::tlog << _T( "UpdatePicShow, Pic folder is empty!" ) << endl;
		return ;
	}
	else
	{
		mcu::tlog << _T( "CheckUpdatePicFile enum all file" ) << endl;
		tAllFile = EnumAllFile( m_strPicFolder.c_str() );

		m_tPicFileList.clear();

		for ( size_t i=0; i<tAllFile.size(); ++i )
		{
			TFileNameInfo tInfo;
			ParsePath( tAllFile[i].c_str(), tInfo );
			if ( CompareNoCase( tInfo.m_strExtName, _T( "jpg" ) ) == 0 )
			{
//				mcu::tlog << _T( "find pic: " ) << tAllFile[i].c_str() << endl;
				m_tPicFileList.push_back( tAllFile[i] );
			}
		}

		this->Reset();

		this->OnPicInfo( GetTotalPageNum(), GetCurPageNum() );

		this->UpdatePicShow();

	}
}

void CVirtualPicManageWnd::Reset()
{
	m_nCurPageNum = 0;
	//m_nPicArrayCol = 0;
	//m_nPicArrayRow = 0;

}

tstring CVirtualPicManageWnd::GetPicName( int nRow, int nCol )
{
	size_t nIndex = this->GetPicIndex( nRow, nCol );
	if ( nIndex < m_tPicFileList.size() )
	{
		return m_tPicFileList[ nIndex ];
	}
	else
		return _T( "" );
}

int CVirtualPicManageWnd::GetPicIndex( int nRow, int nCol )
{
	int nNumPerPage = m_nPicArrayRow * m_nPicArrayCol;
	int nIndex = m_nCurPageNum * nNumPerPage + ( nRow * m_nPicArrayCol ) + nCol;
	return nIndex;
}

void CVirtualPicManageWnd::SetSize( int nRow, int nCol )
{
	m_nPicArrayRow = nRow;
	m_nPicArrayCol = nCol;
}

void CVirtualPicManageWnd::PageNext()
{
	int nTotalNum = this->GetTotalPageNum();
	if ( m_nCurPageNum < nTotalNum - 1 )
	{
		++m_nCurPageNum;
		this->OnPicInfo( GetTotalPageNum(), GetCurPageNum() );
		this->UpdatePicShow();
	}
}

void CVirtualPicManageWnd::PageForward()
{
	if ( m_nCurPageNum > 0 )
	{
		--m_nCurPageNum;
		this->OnPicInfo( GetTotalPageNum(), GetCurPageNum() );
		this->UpdatePicShow();
	}
}

int CVirtualPicManageWnd::GetCurPageNum() const
{
	return m_nCurPageNum;
}

int CVirtualPicManageWnd::GetTotalPageNum()
{
	int nPicNum = m_tPicFileList.size();
	int nPicPerPage = m_nPicArrayRow * m_nPicArrayCol;
	if ( nPicPerPage == 0 )
	{
		mcu::tlog << _T( "CVirtualPicManageWnd Row or Col can't be 0!" ) << endl;
		return 0;
	}

	// 页数从1开始，1张图片也没有也是一页。
	int nPageNum = ( nPicNum + nPicPerPage - 1 ) / nPicPerPage;

	if( nPageNum < 1 )
	{
		nPageNum = 1;
	}

	return nPageNum;
}

BOOL CVirtualPicManageWnd::DelPic( int nRow, int nCol )
{	
	tstring strPic = this->GetPicName( nRow, nCol );
	if ( strPic.empty() )
	{
		mcu::tlog << _T( "CVirtualPicManageWnd::DelPic file name empty!" ) << endl;
		return FALSE;
	}
	else
	{
		// 通知子类释放图片。
		this->ReleasePic( nRow, nCol, strPic.c_str() );

		// 删除文件。
		BOOL bDel = DelFile( strPic.c_str() );
		if ( !bDel )
		{
			mcu::tlog << _T( "CVirtualPicManageWnd::DelPic del file fail!" ) << endl;
			return FALSE;
		}

		// 删除记录。
		int nIndex = this->GetPicIndex( nRow, nCol );
		TFileList::iterator iterRm = m_tPicFileList.begin() + nIndex;
		this->m_tPicFileList.erase( iterRm );

		// 判断页数。
		if ( this->GetCurPageNum() >= this->GetTotalPageNum() )
		{
			m_nCurPageNum --;
		}

		// 刷新显示。
		this->OnPicInfo( this->GetTotalPageNum(), this->GetCurPageNum() );
		this->UpdatePicShow();
	}

	return TRUE;
}

void CVirtualPicManageWnd::CheckConfigChange()
{
	tstring strPicFolderNow;
	CConfig::Instance()->GetCaptureDir( strPicFolderNow );
//	if ( m_strPicFolder != strPicFolderNow )
	{
		m_strPicFolder = strPicFolderNow;
		this->CheckUpdatePicFile();		
	}
//	mcu::tlog << _T( "CVirtualPicManageWnd::CheckConfigChange " ) << m_strPicFolder << strPicFolderNow << endl;
}





