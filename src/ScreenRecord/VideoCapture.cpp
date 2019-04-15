#include "VideoCapture.h"
#include "glog/logging.h"

//#include <QApplication>
//#include <QGuiApplication> 
//#include <QDesktopWidget> 
//#include <QScreen>
#include <QTime>

#include <iostream>

#pragma comment(lib, "d3d9.lib")

VideoCapture::VideoCapture()
{
}

VideoCapture::~VideoCapture()
{
}

bool VideoCapture::init()
{
    if (m_init)
    {
        LOG(WARNING) << "VideoCapture is already inited";
        return true;
    }

    if (VID_CAP_MODE_DIRECTX == m_mode)
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
        m_width = GetSystemMetrics(SM_CXSCREEN);
        m_height = GetSystemMetrics(SM_CYSCREEN);
        m_device->CreateOffscreenPlainSurface(m_width, m_height,
            D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_surface, nullptr);
        if (!m_surface)
        {
            LOG(ERROR) << "CreateOffscreenPlainSurface failed";
            return false;
        }

        // 4.初始化抓屏数据
        m_rect = new D3DLOCKED_RECT;
        ZeroMemory(m_rect, sizeof(*m_rect));
    }
    else if (VID_CAP_MODE_QT == m_mode)
    {
        //m_screen = QGuiApplication::primaryScreen();
        //m_width = m_screen->size().width();
        //m_height = m_screen->size().height();
    }
    else
    {
        return false;
    }

    m_mutex.lock();
    m_init = true;
    m_mutex.unlock();

    return true;
}

bool VideoCapture::uninit()
{
    //Direct3D 应该被析构掉，未找到对应函数
    m_mutex.lock();
    m_init = false;
    delete m_rect;
    m_rect = nullptr;
    m_mutex.unlock();

    return true;
}

void VideoCapture::run()
{
    QTime time;
    int ms_wait = 0;
    int fps_base = 1000 / m_fps;
    int use_time = 0;
    void *data = nullptr;

    while (m_start)
    {
        time.restart();
        m_mutex.lock();
        if (m_datas.size() >= m_cacheSize)
        {
            m_mutex.unlock();
            LOG(WARNING) << "cache buff full, wait 10ms, cache size:" << m_cacheSize;
            msleep(10); // 缓存区已满，等待10ms
            continue;
        }

        // 获取一帧数据
        data = new char[m_width * m_height * m_bitSize];
        if (!captureData(data))
        {
            m_mutex.unlock();
            delete data;
            LOG(WARNING) << "video captureData failed";
            continue;
        }

        // 写入缓冲队列
        m_datas.push_back(data);
        m_mutex.unlock();

        // 若抓取的帧率快于FPS,同步抓取的帧率
        use_time = time.restart();
        ms_wait = fps_base - use_time;
        if (ms_wait > 0)
        {
            msleep(ms_wait);
        }
        //std::cout << "fps_base:" << fps_base << " use:" << use_time << " ms_wait:" << ms_wait << std::endl;
    }
}

bool VideoCapture::captureData(void *data)
{
    if (!data)
    {
        LOG(ERROR) << "input data is null!";
        return false;
    }

    if (VID_CAP_MODE_DIRECTX == m_mode)
    {
        return captureWithDirectX(data);
    }
    else if (VID_CAP_MODE_QT == m_mode)
    {
        return captureWithQt(data);
    }

    return nullptr;
}

bool VideoCapture::captureWithDirectX(void *data)
{
    // 抓屏
    m_device->GetFrontBufferData(0, m_surface);

    // 取出数据
    if (m_surface->LockRect(m_rect, nullptr, 0) != S_OK)
    {
        m_surface->UnlockRect();
        LOG(ERROR) << "directx surface LockRect failed";
        return false;
    }
    memcpy(data, m_rect->pBits, m_width * m_height * m_bitSize);
    m_surface->UnlockRect();

    return true;
}

bool VideoCapture::captureWithQt(void *data)
{
    //winId相当于windows编程的WHND
    //QPixmap pixmap = m_screen->grabWindow(QApplication::desktop()->winId());
    //m_screen->grabWindow(QApplication::desktop()->winId());
    //memcpy(data, &pixmap.data_ptr(), m_width * m_height * m_bitSize);
    return false;
}
