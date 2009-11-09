#include "StdAfx.h"
#include "ImageFactory.h"

#include "ImageShow.h"

CImageFactory *CImageFactory::s_instance = NULL;

CImageFactory::CImageFactory(void)
{
}

CImageFactory::~CImageFactory(void)
{
	for ( TImageTable::iterator iter = m_tImageTable.begin(); iter != m_tImageTable.end(); ++iter )
	{
		if ( iter->second )
		{
			delete iter->second;
			iter->second = NULL;
		}		
	}
}

CImageFactory *CImageFactory::Instance()
{
	if ( NULL == s_instance )
	{
		s_instance = new CImageFactory();
	}
	return s_instance;
}

void CImageFactory::Release()
{
	if ( s_instance )
	{
		delete s_instance;
		s_instance = NULL;
	}
}

CImageShow *CImageFactory::GetImage( LPCTSTR lpImagePath )
{
	if ( lpImagePath == NULL || _tcslen( lpImagePath ) == 0 )
	{
		return NULL;
	}
	
	tstring strImagePath = lpImagePath;

	if( strImagePath.find( _T( "\\" ) ) == -1 )
	{
		tstring strPicPath = m_strDefaultPicDirectory + lpImagePath;
		strImagePath = strPicPath.c_str();
	}

	TImageTable::iterator iter = m_tImageTable.find( strImagePath );
	if ( iter != m_tImageTable.end() && iter->second != NULL )
	{
		return iter->second;
	}
	else
	{
		CImageShow *pImgShow = new CImageShow();
		BOOL bResult = pImgShow->LoadImage( strImagePath.c_str() );
		if ( bResult )
		{
			m_tImageTable[ strImagePath ] = pImgShow;
		}
		else
		{
			delete pImgShow;
			pImgShow = NULL;
		}
		return pImgShow;
	}
}

BOOL CImageFactory::ReleaseImage( LPCTSTR strImagePath )
{
	TImageTable::iterator iter = m_tImageTable.find( strImagePath );
	if ( iter != m_tImageTable.end() && iter->second != NULL )
	{
		delete iter->second;
		iter->second = NULL;
		m_tImageTable.erase(iter);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CImageFactory::SetDefaultPicDir( LPCTSTR strImageDir )
{
	this->m_strDefaultPicDirectory = strImageDir;
}