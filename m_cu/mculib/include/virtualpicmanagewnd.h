#pragma once

/** 
*	ͼƬ�����ڡ�
*/

#include "mcucommon.h"

class CVirtualPicManageWnd
{
public:
	CVirtualPicManageWnd(void);
	virtual ~CVirtualPicManageWnd(void);

protected:
	/** ��ʾͼƬ��
	*	��0��ʼ����
	*	�����Ӧλ��û��ͼƬ��ʾ��strPicΪ���ַ�����
	*	��Ҫ����ʵ�֡� */
	virtual BOOL ShowPic( int nRow, int nCol, LPCTSTR strPic ) = 0;

	/** �ͷ�ͼƬ�� 
	*	��0��ʼ����
	*	Ҫ�������ͷŶ�ӦͼƬ���ļ�����������ϲ���ʾ��ͼƬ��
	*/
	virtual BOOL ReleasePic( int nRow, int nCol, LPCTSTR strPic ) = 0;

	/**	ͼƬ�ļ���Ϣ�ϱ���
	*	�������ش���
	*/
	virtual BOOL OnPicInfo( int nTotalPageNum, int nCurPageNum ) = 0;

protected:
	/** ����ͼƬĿ¼�� */
	void SetPicFolder( LPCTSTR strFolder );

	/** ����ͼƬ�����С�� */
	void SetSize( int nRow, int nCol );

	/** ��ҳ�� */
	void PageNext();
	void PageForward();

	/** ��ǰҳ������0��ʼ���� */
	int GetCurPageNum() const;

	/** ��ȡ��ҳ����
	*	ҳ����1��ʼ��1��ͼƬҲû��Ҳ��һҳ��
	*/
	int GetTotalPageNum();

	/** ��ȡͼƬ���� */
	tstring GetPicName( int nRow, int nCol );

	/** ɾ���� */
	BOOL DelPic( int nRow, int nCol );

	/** ����ͼƬ��ʾ�� 
	*	�������ļ��б�
	*/
	void UpdatePicShow();

	/** ������ͼƬ�ļ��� */
	void CheckUpdatePicFile( );

	/** ���á� */
	void Reset();

	/** ��������Ƿ���£�������£����� ˢ�¡� */
	void CheckConfigChange();

private:

	int GetPicIndex( int nRow, int nCol );

private:
	/** pic folder. */
	tstring m_strPicFolder;

	/** pics. */
	TFileList m_tPicFileList;

	/** ͼƬ���� */
	int m_nPicArrayRow;
	int m_nPicArrayCol;

	/** ��ǰҳ�� */
	int m_nCurPageNum;
	

};



