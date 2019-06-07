#include "Capture.h"
#include "MemoryPool.h"
#include "MediaFormat.h"
#include "glog/logging.h"
#include <QTime>

Capture::Capture()
{
}

Capture::~Capture()
{
    m_oper_lck.lock();
    if (m_mempool)
    {
        delete m_mempool;
    }
    m_oper_lck.unlock();
}

bool Capture::startCapture()
{
    m_oper_lck.lock();
    if (!m_init)
    {
        LOG(ERROR) << "Capture Thread not init!";
        return false;
    }

    m_start = true;
    m_oper_lck.unlock();

    LOG(INFO) << "start Capture Thread";
    start();    // 启动qt线程

    return true;
}

bool Capture::stopCapture()
{
    m_oper_lck.lock();
    if (!m_init)
    {
        LOG(WARNING) << "Capture Thread not init";
        return false;
    }
    clean();    // 清理数据 
    m_start = false;
    m_oper_lck.unlock();

    LOG(INFO) << "stop Capture Thread";
    wait();             // 等待qt线程退出

    return true;
}

void Capture::pushData(FrameData * p)
{
    if (!p || !p->data) { return; }
    std::lock_guard<std::mutex> lck(m_data_lck);

    m_frames.push(p);
}

FrameData * Capture::getData()
{
    std::lock_guard<std::mutex> lck(m_data_lck);

    FrameData *data = nullptr;
    if (!m_frames.empty())
    {
        data = m_frames.front();
        m_frames.pop();
    }

    return data;
}

//void Capture::popData()
//{
//    std::lock_guard<std::mutex> lck(m_data_lck);
//
//    m_frames.pop();
//}

void Capture::freeData(FrameData * p)
{
    std::lock_guard<std::mutex> lck(m_data_lck);

    if (m_mempool)
    {
        m_mempool->freeMemory(p);
    }
}

void Capture::clean()
{
    std::lock_guard<std::mutex> lck(m_data_lck);

    while (!m_frames.empty())
    {
        FrameData *p = m_frames.front();
        if (m_mempool)
        {
            m_mempool->freeMemory(p);
        }
        m_frames.pop();
    }
}

void Capture::run()
{
    QTime time;
    int wait_time = 0;
    int use_time = 0;

    while (m_start)
    {
        time.restart();     // 定时器清0，计算截取一帧所需时间
        m_oper_lck.lock();
        if (!m_start)   // double check
        {
            m_oper_lck.unlock();
            break;
        }

        FrameData *frame = m_mempool->allocMemory();
        if (!frame || !frame->data)
        {
            m_oper_lck.unlock();
            LOG(DETAIL) << "cache buff full, wait 1ms, cache size:" << m_cacheSize;
            msleep(1);   // 缓存区已满，等待10ms
            continue;
        }

        if (!captureData(frame->data)) // 获取一帧数据
        {
            m_mempool->freeMemory(frame);
            frame = nullptr;
            m_oper_lck.unlock();
            LOG(ERROR) << "captureData failed!";
            msleep(1);
            continue;
        }

        frame->type = m_type;
        frame->timestamp = QDateTime::currentMSecsSinceEpoch(); // 记录时间戳
        pushData(frame);                                        // 写入缓冲队列

        // 若抓取的帧率快于FPS,同步抓取的帧率
        use_time = time.restart();
        wait_time = syncTimeMs(use_time);
        wait_time = wait_time > 0 ? wait_time : 1;

        m_oper_lck.unlock();

        LOG(DEBUG) << "use_time:" << use_time << ", wait_time:" << wait_time;
        msleep(wait_time);
    }
}