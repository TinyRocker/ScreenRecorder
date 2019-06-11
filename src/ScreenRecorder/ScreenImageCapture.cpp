#include "ScreenImageCapture.h"
#include "DXGIManager.h"
#include "glog/logging.h"

#include <QScreen>
#include <QApplication>
#include <QGuiApplication> 
#include <QDesktopWidget> 

#pragma comment(lib, "d3d9.lib")

ScreenImageCapture::ScreenImageCapture()
{
    m_dxgi = new DXGIManager;
    m_rcDim = new RECT;
    CoInitialize(NULL);
    CComPtr<IWICImagingFactory> spWICFactory = NULL;
    HRESULT hr = spWICFactory.CoCreateInstance(CLSID_WICImagingFactory);
    if (FAILED(hr))
    {
        LOG(ERROR) << "spWICFactory.CoCreateInstance failed! hr=" << hr;
    }
}

ScreenImageCapture::~ScreenImageCapture()
{
    delete m_dxgi;
    delete m_rcDim;
}

bool ScreenImageCapture::init(int width, int height, int screen, VidCapMode mode, int wid)
{
    std::lock_guard<std::mutex> lck(m_lock);

    m_width = width;
    m_height = height;
    m_mode = mode;
    m_wid = wid;

    LOG(INFO) << "init width:" << m_width << ", height:" << m_height << "screen:" << screen << ", mode:" << m_mode;

    if (VID_CAP_MODE_DX9 == m_mode)
    {
        // 1.创建directx3d对象
        m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (!m_d3d)
        {
            LOG(ERROR) << "Direct3DCreate9 failed!";
            return false;
        }
        
        // 2.创建显卡的设备对象
        D3DPRESENT_PARAMETERS pa;
        ZeroMemory(&pa, sizeof(pa));
        pa.Windowed = true;
        pa.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
        pa.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pa.hDeviceWindow = GetDesktopWindow();
        m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0,
            D3DCREATE_HARDWARE_VERTEXPROCESSING, &pa, &m_device);
        if (!m_device)
        {
            LOG(ERROR) << "CreateDevice failed";
            return false;
        }

        // 3.创建离屏表面
        //int width = GetSystemMetrics(SM_CXSCREEN);
        //int height = GetSystemMetrics(SM_CYSCREEN);
        m_device->CreateOffscreenPlainSurface(m_width, m_height,
            D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_surface, nullptr);
        if (!m_surface)
        {
            LOG(ERROR) << "CreateOffscreenPlainSurface failed";
            return false;
        }

        // 4.初始化抓屏数据
        m_rect = (D3DLOCKED_RECT*) new _D3DLOCKED_RECT;
        ZeroMemory(m_rect, sizeof(*m_rect));
    }
    else if (VID_CAP_MODE_DXGI == m_mode)
    {
        m_dxgi->SetCaptureSource((CaptureSource)screen);
        m_dxgi->GetOutputRect(*m_rcDim);
    }
    else if (VID_CAP_MODE_QT == m_mode)
    {
        m_screen = QGuiApplication::primaryScreen();
        //if (CSDesktop == (CaptureSource)screen) m_screen = (QScreen*)QApplication::desktop()->screen();
        //else if (CSMonitor1 == (CaptureSource)screen) m_screen = (QScreen*)QApplication::desktop()->screen(0);
        //else if (CSMonitor1 == (CaptureSource)screen) m_screen = (QScreen*)QApplication::desktop()->screen(1);

        if (!m_screen) return false;
        LOG(INFO) << "width:" << m_width << " height:" << m_height;
    }
    else
    {
        LOG(WARNING) << "not support!";
        return false;
    }

    m_init = true;

    return true;
}

bool ScreenImageCapture::deinit()
{
    std::lock_guard<std::mutex> lck(m_lock);

    LOG(INFO) << "deinit width:" << m_width << ", height:" << m_height << ", mode:" << m_mode;
    delete m_rect;
    m_rect = nullptr;
    m_screen = nullptr;
    m_wid = 0;
    m_init = false;

    //Direct3D 应该被析构掉，未找到对应函数

    return true;
}

bool ScreenImageCapture::captureScreen(char * data)
{
    if (!data)
    {
        LOG(ERROR) << "param is null!";
        return false;
    }

    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_init)
    {
        LOG(ERROR) << "not init!";
        return false;
    }

    if (VID_CAP_MODE_DX9 == m_mode)
    {
        return captureScreenWithDx9(data);
    }
    else if (VID_CAP_MODE_QT == m_mode)
    {
        return captureScreenWithQt(data);
    }
    else if (VID_CAP_MODE_DXGI == m_mode)
    {
        return captureScreenWithDxgi(data);
    }
    else
    {
        LOG(ERROR) << "not support mode:" << m_mode;
        return false;
    }
}

bool ScreenImageCapture::captureScreenWithDx9(char * data)
{
    // 抓屏
    m_device->GetFrontBufferData(0, m_surface);

    // 取出数据
    if (m_surface->LockRect(m_rect, nullptr, 0) != S_OK)
    {
        m_surface->UnlockRect();
        LOG(ERROR) << "directx surface LockRect failed!";
        return false;
    }
    memcpy(data, m_rect->pBits, m_width * m_height * 4);    // bgra is 4 bytes
    m_surface->UnlockRect();

    return true;
}

bool ScreenImageCapture::captureScreenWithQt(char * data)
{
    // 截取全屏
    if (!m_screen)
    {
        return false;
    }

    //winId相当于windows编程的WHND
    QPixmap pixmap = m_screen->grabWindow(m_wid);
    memcpy(data, pixmap.toImage().bits(), m_width * m_height * 4);

    return true;
}

bool ScreenImageCapture::captureScreenWithDxgi(char * data)
{
    static HRESULT hr;
    hr = m_dxgi->GetOutputBits((BYTE*)data, *m_rcDim);
    if (FAILED(hr))
    {
        LOG(DETAIL) << "GetOutputBits failed with hr" << hr;
        return false;
    }
    return true;
}