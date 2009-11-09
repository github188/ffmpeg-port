#pragma once

class CMediaPlayer
{
public:
	CMediaPlayer(void);
	virtual ~CMediaPlayer(void);

	/** ����RTSP���ӡ� */
	bool PlayRtsp( LPCTSTR strUrl );

	/** �����ļ��� */
	bool PlayFile( LPCTSTR strFilePath );

	/** ֹͣ���š�*/
	bool StopPlay();

	/** ���ò��Ŵ��ڡ� */
	void SetPlayWnd( HWND hWnd );
};
