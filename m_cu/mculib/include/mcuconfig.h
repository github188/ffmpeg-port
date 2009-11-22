#pragma once

#include "mcudefine.h"





/** ǰ�������� */
class CXmlParse;

class CConfig
{
public:
	static CConfig *Instance();
	static void Release();
private:
	static CConfig *s_instance;
protected:
	CConfig(void);
	~CConfig(void);

public:
	/** ���������ļ�·���� */
	void SetConfigFilePath( LPCTSTR strCfgFilePath );
	tstring GetConfigFilePath() const;

	/** �Ƿ񱣴�. */
	BOOL GetIsSave( BOOL& bIsSave );
	BOOL SetIsSave( BOOL bIsSave );

	/** �Ƿ�ʹ���Լ��ĵ�¼���ڡ� */
	BOOL GetLoginInterface( BOOL& bLoginDlg );
	BOOL SetLoginInterface( BOOL bLoginDlg );

	/** msp�ĵ�ַ�� */
	BOOL GetServer( tstring& strUrl );

	BOOL SetServer( LPCTSTR strUrl );

    BOOL GetServerFullUrl( LPTSTR ServerFullStr);
    BOOL GetServerFullSvrUrl(LPTSTR ServerFullStr);

	/** ��ȡ�û��������롣*/
	BOOL GetLoginInfo( tstring& strUserId, tstring& strPw, EStreamType& eStreamType );

	/** �����û��������롣 */
	BOOL SetLoginInfo( LPCTSTR strUserId, LPCTSTR strPw, EStreamType eStreamType );

	/** ��ȡ��ͻ���. */
	BOOL GetRegionId( tstring& strRegionId );
	BOOL SetReginId( LPCTSTR strRegionId );

	/** ¼��·����*/

	/** ץ�ı���·����*/
	BOOL GetCaptureDir( tstring& strCaptureDir );
	BOOL SetCaptureDir( LPCTSTR strCaptureDir );

	/** ץ��ͼƬ·�������͡� */
	BOOL GetIsCaptureDirCustom( BOOL& bCustom );
	BOOL SetIsCaptureDirCustom( BOOL bCustom );

	/** ptz���� */

	/** ��̨���Ʋ���. */
	BOOL GetPtzSpeed( int& nPtzSpeed );
	BOOL SetPtzSpeed( int nPtzSpeed );
	/** ��ͷ���Ʋ���. */
	BOOL GetLenSpeed( int& nLenSpeed );
	BOOL SetLenSpeed( int nLenSpeed );

	/** ������⡣ 
	*	����Ҫ���á������ȡ������ȡ�����ļ�����
	*/
	BOOL GetAppTitle( tstring& strAppTitle );

	/** ��ȡ�洢�ռ䷧ֵ��С�ڴ˷�ֵ��ץ�ĺ�¼�� */
	BOOL GetMinStorageSpace( int& nMinSpace );

	/** ҳ��Ŵ����� */
	BOOL GetWebpageZoomLevel( float& fZoom );
	BOOL SetWebpageZoomLevel( const float fZoom );

	/** rtsp���豸�б���Ҫ���˵Ĳ����� */
	BOOL GetWebpageHistoryBackStep( int& nStep );
	BOOL SetWebpageHistoryBackStep( const int nStep );

private:


	/** ��ȡXML�ļ����� */
	tstring GetDefaultConfigFilePath() const;

    /** ��ȡԤ���ļ�·���� */
    tstring GetBkCfgFilePath() const;

	/** ��ȡ�� */
	tstring ReadConfig( LPCTSTR strCfgEntryName, LPCTSTR strDefault );
	int		ReadConfig( LPCTSTR strCfgEntryName, int nDefault );

	/** д�롣 */
	BOOL WriteConfig( LPCTSTR strCfgEntryName, LPCTSTR strValue );
	BOOL WriteConfig( LPCTSTR strCfgEntryName, int nValue );

private:
	CXmlParse *m_pXmlParse;

    /** ����Ԥ�������ļ� */
    CXmlParse *m_pBkCfgxmlParse;
};
