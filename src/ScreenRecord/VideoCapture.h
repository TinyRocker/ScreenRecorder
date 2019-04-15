#pragma once
#include "CaptureThread.h"
#include "MediaFormat.h"
#include <d3d9.h>

class QScreen;
class QPixmap;

class VideoCapture : public CaptureThread
{
public:
    VideoCapture();
    virtual ~VideoCapture();

    virtual bool init();
    virtual bool uninit();

    int width() const { return m_width; }
    int height() const { return m_height; }
    int bitsize() const { return m_bitSize; }
    int fps() const { return m_fps; }
    void fps(int fps) { m_fps = fps; }
    void mode(VidCapMode mode) { m_mode = mode; }

protected:
    virtual void run();
    virtual bool captureData(void *data);
    bool captureWithDirectX(void *data);
    bool captureWithQt(void *data);

private:
    
    VidCapMode          m_mode = VID_CAP_MODE_DIRECTX;
    
    // DirectX 9
    IDirect3D9          *m_d3d = nullptr;       // directx3d 对象
    IDirect3DDevice9    *m_device = nullptr;    // 显卡设备对象
    IDirect3DSurface9   *m_surface = nullptr;   // 离屏表面
    D3DLOCKED_RECT      *m_rect = nullptr;      // 抓屏数据

    // QT
    QScreen *m_screen = nullptr;
    QPixmap *m_pixmap = nullptr;

    int m_width = 2560;
    int m_height = 1440;
    int m_bitSize = 4;      // bgra
    int m_fps = 25;
};