#pragma once

// #include <MsXml.h>
#include "mcudef.h"
#include <msxml.h>
#include <atlbase.h>

/** ����ͨ�ַ���ת��ΪXML�ַ����� */
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
	
private:
	/** ����XML�ļ��� 
	*	������config�ڵ㡣
	*/
	BOOL LoadXML();

	/** ���档 */
	BOOL SaveXML();

	/** ��ȡXML�ļ����� */
	tstring GetXMLFilePath();

	/** ��ȡ�� */
	tstring ReadConfig( LPCTSTR strCfgEntryName, LPCTSTR strDefault );
	int		ReadConfig( LPCTSTR strCfgEntryName, int nDefault );

	/** д�롣 */
	BOOL WriteConfig( LPCTSTR strCfgEntryName, LPCTSTR strValue );
	BOOL WriteConfig( LPCTSTR strCfgEntryName, int nValue );

private:
	/** ����xml�� */
	typedef CComPtr<IXMLDOMDocument> IXMLDOMDocumentPtr;
	typedef CComPtr<IXMLDOMNodeList> IXMLDOMNodeListPtr;
	typedef CComPtr<IXMLDOMNode> IXMLDOMNodePtr;
	typedef CComPtr<IXMLDOMElement> IXMLDOMElementPtr;

	IXMLDOMDocumentPtr m_pXMLDoc;
	IXMLDOMElementPtr	m_pXMLConfigNode;
};
