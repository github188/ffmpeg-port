#pragma once

#include <imaging.h>

struct CImageInfo
{
public:
	int Width;
	int Height;

	CImageInfo()
	{
		Width = 0;
		Height = 0;
	}
};

class CImageShow
{
public:
	//enum EImageDrawFlag
	//{
	//	IDF_Orientation_0 = 0,						// ͼƬ��ת,����ת.
	//	IDF_Orientation_90,							// ͼƬ˳ʱ����ת90��.
	//	IDF_Orientation_180,						// ͼƬ��ת180��.
	//	IDF_Orientation_270,						// ͼƬ˳ʱ����ת270��.
	//	IDF_Orientation_Default = IDF_Orientation_0,// Ĭ�������ͼƬ����ת.

	//};
public:
	CImageShow(void);
	virtual ~CImageShow(void);

	BOOL LoadImage( LPCTSTR strImage );
	BOOL Draw( HDC hdc, LPRECT pRect = NULL/*, EImageDrawFlag eIDF = IDF_Orientation_Default*/ );

	BOOL IsOk() const;

	/** ��ȡͼƬ��Ϣ. */
	BOOL GetImageInfo( CImageInfo& imgInfo );

	/** ����͸��ɫ�� */
	BOOL SetTransparentMask( BOOL bTransparent, COLORREF maskColor = NULL );


private:
	IImage *m_pImage;
	IImagingFactory *m_pImgFactory;

	/** ͸��ɫ�� */
	COLORREF m_colorMask;
	/** �Ƿ�͸���� */
	BOOL m_bTransparent;

	///** �ϳɺ��͸��������ͼ�� */
	//CBitmap m_bmpFinal;
	///** �ϳ�ǰ��ͼƬ�� */
	//CBitmap m_bmpImage;

	///** �ϳ��õ�DC�� */
	//CDC m_dcCombine;

	///** ���һ��ʹ�õ�Ŀ���ͼDC�� */
	//HDC m_hLastDC;

};
