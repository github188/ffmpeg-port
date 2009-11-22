#pragma once

#include "mcudefine.h"





/** 前置声明。 */
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
	/** 设置配置文件路径。 */
	void SetConfigFilePath( LPCTSTR strCfgFilePath );
	tstring GetConfigFilePath() const;

	/** 是否保存. */
	BOOL GetIsSave( BOOL& bIsSave );
	BOOL SetIsSave( BOOL bIsSave );

	/** 是否使用自己的登录窗口。 */
	BOOL GetLoginInterface( BOOL& bLoginDlg );
	BOOL SetLoginInterface( BOOL bLoginDlg );

	/** msp的地址。 */
	BOOL GetServer( tstring& strUrl );

	BOOL SetServer( LPCTSTR strUrl );

    BOOL GetServerFullUrl( LPTSTR ServerFullStr);
    BOOL GetServerFullSvrUrl(LPTSTR ServerFullStr);

	/** 获取用户名，密码。*/
	BOOL GetLoginInfo( tstring& strUserId, tstring& strPw, EStreamType& eStreamType );

	/** 设置用户名，密码。 */
	BOOL SetLoginInfo( LPCTSTR strUserId, LPCTSTR strPw, EStreamType eStreamType );

	/** 获取域客户名. */
	BOOL GetRegionId( tstring& strRegionId );
	BOOL SetReginId( LPCTSTR strRegionId );

	/** 录像路径。*/

	/** 抓拍保存路径。*/
	BOOL GetCaptureDir( tstring& strCaptureDir );
	BOOL SetCaptureDir( LPCTSTR strCaptureDir );

	/** 抓拍图片路径的类型。 */
	BOOL GetIsCaptureDirCustom( BOOL& bCustom );
	BOOL SetIsCaptureDirCustom( BOOL bCustom );

	/** ptz参数 */

	/** 云台控制步长. */
	BOOL GetPtzSpeed( int& nPtzSpeed );
	BOOL SetPtzSpeed( int nPtzSpeed );
	/** 镜头控制步长. */
	BOOL GetLenSpeed( int& nLenSpeed );
	BOOL SetLenSpeed( int nLenSpeed );

	/** 程序标题。 
	*	不需要设置。如果获取不到，取程序文件名。
	*/
	BOOL GetAppTitle( tstring& strAppTitle );

	/** 获取存储空间阀值，小于此阀值不抓拍和录像。 */
	BOOL GetMinStorageSpace( int& nMinSpace );

	/** 页面放大倍数。 */
	BOOL GetWebpageZoomLevel( float& fZoom );
	BOOL SetWebpageZoomLevel( const float fZoom );

	/** rtsp到设备列表需要后退的步数。 */
	BOOL GetWebpageHistoryBackStep( int& nStep );
	BOOL SetWebpageHistoryBackStep( const int nStep );

private:


	/** 获取XML文件名。 */
	tstring GetDefaultConfigFilePath() const;

    /** 获取预设文件路径。 */
    tstring GetBkCfgFilePath() const;

	/** 读取。 */
	tstring ReadConfig( LPCTSTR strCfgEntryName, LPCTSTR strDefault );
	int		ReadConfig( LPCTSTR strCfgEntryName, int nDefault );

	/** 写入。 */
	BOOL WriteConfig( LPCTSTR strCfgEntryName, LPCTSTR strValue );
	BOOL WriteConfig( LPCTSTR strCfgEntryName, int nValue );

private:
	CXmlParse *m_pXmlParse;

    /** 程序预设配置文件 */
    CXmlParse *m_pBkCfgxmlParse;
};
