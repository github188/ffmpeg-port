#pragma once

class CMediaPlayer
{
public:
	CMediaPlayer(void);
	virtual ~CMediaPlayer(void);

	/** 播放RTSP链接。 */
	bool PlayRtsp( LPCTSTR strUrl );

	/** 播放文件。 */
	bool PlayFile( LPCTSTR strFilePath );

	/** 停止播放。*/
	bool StopPlay();

	/** 设置播放窗口。 */
	void SetPlayWnd( HWND hWnd );
};
