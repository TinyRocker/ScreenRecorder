#pragma once
#include "MediaFormat.h"
#include <mutex>
#include <d3d9.h>

class QScreen;

class ScreenImageCapture
{
public:
    ScreenImageCapture();
    virtual ~ScreenImageCapture();
    bool init(int width, int height, VidCapMode mode = VID_CAP_MODE_DX9, int wid = 0);
    bool deinit();
    bool captureScreen(char *data);
protected:
    virtual bool captureScreenWithDx9(char *data);
    virtual bool captureScreenWithQt(char *data);
private:
    std::mutex  m_lock;     // 线程安全
    VidCapMode  m_mode;     // 抓取屏幕模式
    bool        m_init = false;

    // DirectX 9
    IDirect3D9          *m_d3d = nullptr;       // directx3d 对象
    IDirect3DDevice9    *m_device = nullptr;    // 显卡设备对象
    IDirect3DSurface9   *m_surface = nullptr;   // 离屏表面
    D3DLOCKED_RECT      *m_rect = nullptr;      // 抓屏数据

    // QT
    QScreen *m_screen = nullptr;
    int     m_wid = 0;

    int         m_width;
    int         m_height;
};

