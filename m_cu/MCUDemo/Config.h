#pragma once

// #include <MsXml.h>
#include "mcudef.h"
#include <msxml.h>
#include <atlbase.h>

/** 将普通字符串转换为XML字符串。 */
#define XML_TEXT(x) COleVariant(x).bstrVal

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
	
private:
	/** 载入XML文件。 
	*	解析出config节点。
	*/
	BOOL LoadXML();

	/** 保存。 */
	BOOL SaveXML();

	/** 获取XML文件名。 */
	tstring GetXMLFilePath();

	/** 读取。 */
	tstring ReadConfig( LPCTSTR strCfgEntryName, LPCTSTR strDefault );
	int		ReadConfig( LPCTSTR strCfgEntryName, int nDefault );

	/** 写入。 */
	BOOL WriteConfig( LPCTSTR strCfgEntryName, LPCTSTR strValue );
	BOOL WriteConfig( LPCTSTR strCfgEntryName, int nValue );

private:
	/** 解析xml。 */
	typedef CComPtr<IXMLDOMDocument> IXMLDOMDocumentPtr;
	typedef CComPtr<IXMLDOMNodeList> IXMLDOMNodeListPtr;
	typedef CComPtr<IXMLDOMNode> IXMLDOMNodePtr;
	typedef CComPtr<IXMLDOMElement> IXMLDOMElementPtr;

	IXMLDOMDocumentPtr m_pXMLDoc;
	IXMLDOMElementPtr	m_pXMLConfigNode;
};
