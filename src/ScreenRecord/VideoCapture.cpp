#include "VideoCapture.h"
#include "MemoryPool.h"
#include "MediaFormat.h"
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

bool VideoCapture::init(const VidRecordParam& param)
{
    if (m_init)
    {
        LOG(WARNING) << "VideoCapture is already inited";
        return false;
    }

    m_width = param.width;
    m_height = param.height;
    m_fps = param.fps;
    m_bitsize = param.bitsize;
    m_mode = param.mode;
    
    if (VID_CAP_MODE_DIRECTX == m_mode)
    {
        // 1.创建directx3d对象
        m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (!m_d3d)
        {
            LOG(ERROR) << "Direct3DCreate9 failed!";
            return false;
        }

        //int width = GetSystemMetrics(SM_CXSCREEN);
        //int height = GetSystemMetrics(SM_CYSCREEN);
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
    m_mempool = new MemoryPool(m_cacheSize, m_width * m_height * m_bitsize);
    m_init = true;

    return true;
}

bool VideoCapture::uninit()
{
    //Direct3D 应该被析构掉，未找到对应函数
    delete m_rect;
    m_rect = nullptr;
    delete m_mempool;
    m_mempool = nullptr;
    m_init = false;

    return true;
}

bool VideoCapture::startCapture()
{
    std::lock_guard<std::mutex> lock(m_oper_lock);
    if (!m_init)
    {
        LOG(WARNING) << "VideoCapture Thread not init";
        return false;
    }

    m_start = true;
    start();    // 启动qt线程

    LOG(INFO) << "start VideoCapture Thread";
    return true;
}

bool VideoCapture::stopCapture()
{
    std::lock_guard<std::mutex> lock(m_oper_lock);
    if (!m_init)
    {
        LOG(WARNING) << "VideoCapture Thread not init";
        return false;
    }

    m_start = false;
    terminate();          // 结束qt线程
    wait();

    m_mempool->clean();   // 结束获取后及时清理数据

    LOG(INFO) << "stop VideoCapture Thread";
    return true;
}

void * VideoCapture::getData()
{
    m_data_lock.lock();
    void *data = nullptr;
    if (!m_datas.empty())
    {
        data = m_datas.front();
        m_datas.pop();
    }
    m_data_lock.unlock();

    return data;
}

void VideoCapture::freeData(void * p)
{
    m_mempool->freeMemory(p);
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
        std::lock_guard<std::mutex> lock(m_oper_lock);
        if (!m_start)   // double check
        {
            LOG(INFO) << "stop video capture";
            break;
        }

        data = m_mempool->allocMemory();
        if (!data)
        {
            LOG(WARNING) << "cache buff full, wait 50ms, cache size:" << m_cacheSize;
            msleep(50); // 缓存区已满，等待50ms
            continue;
        }
        if (!captureData(data)) // 获取一帧数据
        {
            m_mempool->freeMemory(data);
            data = nullptr;
            LOG(WARNING) << "video captureData failed";
            continue;
        }

        // 写入缓冲队列
        m_data_lock.lock();
        m_datas.push(data);
        m_data_lock.unlock();

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
    memcpy(data, m_rect->pBits, m_width * m_height * m_bitsize);
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
