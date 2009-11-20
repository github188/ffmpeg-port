#pragma once

#include "mculib.h"

class CImageShow;

class CImageFactory
{
public:
	static CImageFactory *Instance();
	static void Release();

	/** ��ȡͼƬ��.
	*	����������Ƶ�֧�֡��������ͼƬ����û�С�\�����͵����õ�Ĭ��Ŀ¼��ȥѰ�Ҹ�ͼƬ��
	*/
	CImageShow *GetImage( LPCTSTR strImagePath );

	/** ����ͼƬ�ࡣ */
	BOOL ReleaseImage( LPCTSTR strImagePath );

	/** ����Ĭ�ϵ�ͼƬ����·���� */
	void SetDefaultPicDir( LPCTSTR strImageDir );

private:
	static CImageFactory *s_instance;

	/** ͼƬӳ���. */
	typedef map< tstring, CImageShow * > TImageTable;
	TImageTable m_tImageTable;
	
	/** Ĭ�ϵ�ͼƬĿ¼. */
	tstring m_strDefaultPicDirectory;

protected:
	CImageFactory(void);
	~CImageFactory(void);
};
