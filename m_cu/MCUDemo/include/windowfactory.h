#pragma once

class CUIDialog;

enum EWindowId
{
    WndLogin = 0,   // 登录
    WndWebpage,     // 网页
    WndPicManage,   // 图片管理
    WndPicView,     // 图片查看。
    WndPlayer,      // 播放。
    WndConfig,      // 配置
    WndHelp,        // 帮助
    

    WndUnittestMain,
    WndUnittestUI,
    WndUnittestWebpage,
    WndUnittestRtsp,

    WndAllNum,      // 所有窗口数目。

    WndInvalid = -1,    // 无效窗口id。
};

class CWindowFactory
{
public:
    
public:
    static CWindowFactory *Instance();
    static void Release();

    /** 弹出模态对话框。 */
    int PopoupWnd( EWindowId eWndId );

    /** 显示窗口。 */
    BOOL ShowWindow( EWindowId eShowWndId, EWindowId eShowAfter );

    /** 创建所有窗口。 */
    BOOL CreateAllWnd();

    

    /** 获取窗口。 */
    CUIDialog *GetWnd( EWindowId eWndId );

protected:
    CWindowFactory(void);
    virtual ~CWindowFactory(void);

private:
    /** 创建某窗口。 */
    CUIDialog * CreateWnd( EWindowId eWndId );
private:
    static CWindowFactory *s_instance;

    typedef std::map< EWindowId, CUIDialog * > TWindowsTalbe;
    TWindowsTalbe m_tWndTable;

    
};
