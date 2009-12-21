#include "StdAfx.h"
#include "ImageShow.h"
#include <InitGuid.h>
#include <imgguids.h>
#include "mculib.h"

CImageShow::CImageShow(void)
{
	m_pImage = NULL;
	m_pImgFactory = NULL;

	m_bTransparent = FALSE;
	m_colorMask = RGB( 255,255,255 );

//	m_hLastDC = NULL;
}

CImageShow::~CImageShow(void)
{
	if ( m_pImgFactory )
	{
		ULONG kkk = m_pImgFactory->Release();
	}

	if ( m_pImage )
	{
		ULONG kkk = m_pImage->Release();
	}
}

BOOL CImageShow::LoadImage( LPCTSTR strImage )
{
	if ( _tcslen( strImage ) == 0 )
	{
		return FALSE;
	}

	// ����ͼƬ�๤��.
	if ( !m_pImgFactory )
	{
		if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IImagingFactory,
			(void **)&this->m_pImgFactory )))
		{
			
		}
		else
		{
			Log() << _T( "Create Image Facotry Fail!" ) << endl;
			_ASSERT( FALSE );
		}
	}

	// ��������ͼƬ����.
	if ( m_pImage )
	{
		m_pImage->Release();
		m_pImage = NULL;
	}

	// �����µ�ͼƬ����.
	if ( m_pImgFactory )
	{
		// Load the image from the JPG file.
		LRESULT lResult = m_pImgFactory->CreateImageFromFile(
			strImage,
			&m_pImage);
		if (SUCCEEDED( lResult ))
		{
			
		}
		else
		{
			Log() << _T( "Load Image : " ) << strImage << _T( " Fail! return: " ) << lResult << endl;
//			_ASSERT( FALSE );
			if ( m_pImage )
			{
				m_pImage->Release();
				m_pImage = NULL;
			}			
		}
	}
	return m_pImage != NULL;
}

BOOL CImageShow::Draw( HDC hdc, LPRECT pRect /* = NULL */ )
{
	LRESULT lResult;
	if ( m_pImage )
	{
		// �õ�Ҫ���Ƴ���ͼ��Ĵ�С��
		CRect rcDraw;
		if ( NULL == pRect )
		{
			ImageInfo info;
			m_pImage->GetImageInfo( &info );
			rcDraw.top = rcDraw.left = 0;
			rcDraw.bottom = info.Height;
			rcDraw.right = info.Width;
		}
		else
		{
			rcDraw = *pRect;
		}

		if ( m_bTransparent )
		{
			// ͸������

			// ����ͼ��
			CDC dcImage;
			BOOL bSuc =  dcImage.CreateCompatibleDC( CDC::FromHandle( hdc ) );
			if ( !bSuc )
			{
				Log() << _T( "Create compatible Dc Fail!" ) << endl;
			}
			_ASSERT( bSuc );

			// ����ͼ��λͼ�����ҵõ�ԭʼͼ���λͼ��
			// ����ʹ��ԭʼDC�����ڣ�����λͼ����Ϊ�ոմ����ļ����ڴ�DC���������ԣ�
			// A memory device context is a device context that exists only in memory. 
			// When the memory device context is created, its display surface is exactly one monochrome pixel wide and one monochrome pixel high. 
			CBitmap bmpImage;
			bmpImage.CreateCompatibleBitmap( CDC::FromHandle( hdc ), rcDraw.Width(), rcDraw.Height() );

			BITMAP hB;
			bmpImage.GetBitmap( &hB );


			CBitmap *pOldBmp = dcImage.SelectObject( &bmpImage );
			CRect rcCombineDC( 0, 0 , rcDraw.Width(), rcDraw.Height() );
			lResult = m_pImage->Draw( dcImage, &rcCombineDC, NULL );

			// ͼ��ϳɡ�
			BOOL bResult = ::TransparentBlt( hdc, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(), 
												dcImage.GetSafeHdc(), 0, 0, rcDraw.Width(), rcDraw.Height(), m_colorMask);
			if( !bResult )
			{
				DWORD dwError = ::GetLastError();
				Log() << _T( "Draw Image Fail! Error code: " ) << dwError << endl;
			}			


			_ASSERT( bResult );

			// �ָ���
			dcImage.SelectObject( pOldBmp );

			// ���١�
			bmpImage.DeleteObject();			
		}
		else
		{
			lResult = m_pImage->Draw( hdc, rcDraw, NULL );

			
		}		
	}
	if ( S_OK != lResult )
	{
		Log() << _T( "Draw Image fail!!" ) << endl;
		_ASSERT( FALSE );
	}
	 
	return ( S_OK == lResult );
}



BOOL CImageShow::IsOk() const
{
	return m_pImage != NULL;
}

BOOL CImageShow::GetImageInfo( CImageInfo& imgInfo )
{
	if ( m_pImage )
	{
		ImageInfo info;
		m_pImage->GetImageInfo( &info );
		imgInfo.Width = info.Width;
		imgInfo.Height = info.Height;

		return TRUE;
	}
	return FALSE;
}

BOOL CImageShow::SetTransparentMask( BOOL bTransparent, COLORREF maskColor /* = NULL */ )
{
	m_bTransparent = bTransparent;
	m_colorMask = maskColor;
	return TRUE;
}