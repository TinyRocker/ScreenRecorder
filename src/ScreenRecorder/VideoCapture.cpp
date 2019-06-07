#include "VideoCapture.h"
#include "MemoryPool.h"
#include "MediaFormat.h"
#include "ScreenImageCapture.h"
#include "glog/logging.h"

VideoCapture::VideoCapture()
{
    std::lock_guard<std::mutex> lck(m_oper_lck);

    m_screenCapture = new ScreenImageCapture;
}

VideoCapture::~VideoCapture()
{
    std::lock_guard<std::mutex> lck(m_oper_lck);

    delete m_screenCapture;
}

bool VideoCapture::init(const VidRawParam& param)
{
    std::lock_guard<std::mutex> lck(m_oper_lck);

    if (m_init)
    {
        LOG(WARNING) << "VideoCapture is already inited";
        return false;
    }

    m_width = param.width;
    m_height = param.height;
    m_fps = param.fps;
    m_bitsize = param.bitsize;
    
    if (false == m_screenCapture->init(m_width, m_height, param.mode, param.wid))
    {
        LOG(ERROR) << "screen capture init failed!";
        return false;
    }
    m_cacheSize = 5;

    m_type = FrameType_Vid;

    m_data_lck.lock();
    m_mempool = new MemoryPool(m_cacheSize, m_width * m_height * m_bitsize);
    m_data_lck.unlock();

    m_init = true;

    return true;
}

bool VideoCapture::uninit()
{
    std::lock_guard<std::mutex> lck(m_oper_lck);

    m_data_lck.lock();
    if (m_mempool)
    {
        m_mempool->clean();
        delete m_mempool;
        m_mempool = nullptr;
    }
    m_data_lck.unlock();

    m_init = false;

    return true;
}

bool VideoCapture::captureData(char * p)
{
    return m_screenCapture->captureScreen(p);
}

int VideoCapture::syncTimeMs(int use_time)
{
    return 1000 / m_fps - use_time;
}