#pragma once

/** 
*	图片管理窗口。
*/

#include "mcucommon.h"

class CVirtualPicManageWnd
{
public:
	CVirtualPicManageWnd(void);
	virtual ~CVirtualPicManageWnd(void);

protected:
	/** 显示图片。
	*	从0开始数。
	*	如果对应位置没有图片显示，strPic为空字符串。
	*	需要子类实现。 */
	virtual BOOL ShowPic( int nRow, int nCol, LPCTSTR strPic ) = 0;

	/** 释放图片。 
	*	从0开始数。
	*	要求子类释放对应图片的文件句柄。界面上不显示该图片。
	*/
	virtual BOOL ReleasePic( int nRow, int nCol, LPCTSTR strPic ) = 0;

	/**	图片文件信息上报。
	*	子类重载处理。
	*/
	virtual BOOL OnPicInfo( int nTotalPageNum, int nCurPageNum ) = 0;

protected:
	/** 设置图片目录。 */
	void SetPicFolder( LPCTSTR strFolder );

	/** 设置图片方阵大小。 */
	void SetSize( int nRow, int nCol );

	/** 翻页。 */
	void PageNext();
	void PageForward();

	/** 当前页数，从0开始数。 */
	int GetCurPageNum() const;

	/** 获取总页数。
	*	页数从1开始，1张图片也没有也是一页。
	*/
	int GetTotalPageNum();

	/** 获取图片名。 */
	tstring GetPicName( int nRow, int nCol );

	/** 删除。 */
	BOOL DelPic( int nRow, int nCol );

	/** 更新图片显示。 
	*	不更新文件列表。
	*/
	void UpdatePicShow();

	/** 检查更新图片文件。 */
	void CheckUpdatePicFile( );

	/** 重置。 */
	void Reset();

	/** 检查配置是否更新，如果更新，重置 刷新。 */
	void CheckConfigChange();

private:

	int GetPicIndex( int nRow, int nCol );

private:
	/** pic folder. */
	tstring m_strPicFolder;

	/** pics. */
	TFileList m_tPicFileList;

	/** 图片方阵 */
	int m_nPicArrayRow;
	int m_nPicArrayCol;

	/** 当前页。 */
	int m_nCurPageNum;
	

};



