#pragma once

class CUIDialog;

enum EWindowId
{
    WndLogin = 0,   // ��¼
    WndWebpage,     // ��ҳ
    WndPicManage,   // ͼƬ����
    WndPicView,     // ͼƬ�鿴��
    WndPlayer,      // ���š�
    WndConfig,      // ����
    WndHelp,        // ����
    

    WndUnittestMain,
    WndUnittestUI,
    WndUnittestWebpage,
    WndUnittestRtsp,

    WndAllNum,      // ���д�����Ŀ��

    WndInvalid = -1,    // ��Ч����id��
};

class CWindowFactory
{
public:
    
public:
    static CWindowFactory *Instance();
    static void Release();

    /** ����ģ̬�Ի��� */
    int PopoupWnd( EWindowId eWndId );

    /** ��ʾ���ڡ� */
    BOOL ShowWindow( EWindowId eShowWndId, EWindowId eShowAfter );

    /** �������д��ڡ� */
    BOOL CreateAllWnd();

    

    /** ��ȡ���ڡ� */
    CUIDialog *GetWnd( EWindowId eWndId );

protected:
    CWindowFactory(void);
    virtual ~CWindowFactory(void);

private:
    /** ����ĳ���ڡ� */
    CUIDialog * CreateWnd( EWindowId eWndId );
private:
    static CWindowFactory *s_instance;

    typedef std::map< EWindowId, CUIDialog * > TWindowsTalbe;
    TWindowsTalbe m_tWndTable;

    
};
