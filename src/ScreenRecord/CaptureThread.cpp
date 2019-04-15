#include "CaptureThread.h"

#include <QTime>
#include <iostream>

#include "glog/logging.h"

using namespace std;

CaptureThread::CaptureThread()
{
}

CaptureThread::~CaptureThread()
{
}

bool CaptureThread::startCapture()
{
    if (!m_init)
    {
        LOG(WARNING) << "Capture Thread not init";
        return false;
    }

    m_mutex.lock();
    m_start = true;
    start();    // 启动qt线程
    m_mutex.unlock();

    LOG(INFO) << "start Capture Thread";
    return true;
}

bool CaptureThread::stopCapture()
{
    if (!m_init)
    {
        LOG(WARNING) << "Capture Thread not init";
        return false;
    }
    
    m_mutex.lock();
    m_start = false;
    terminate();    // 结束qt线程
    wait();
    
    // 清理缓存队列
    while (!m_datas.empty())
    {
        void *p = m_datas.front();
        m_datas.pop_front();
        delete p;
    }
    m_mutex.unlock();

    LOG(INFO) << "stop Capture Thread";
    return true;
}

void * CaptureThread::getData()
{
    void *data = nullptr;
    m_mutex.lock();
    if (!m_datas.empty())
    {
        data = m_datas.front();
        m_datas.pop_front();
    }
    m_mutex.unlock();

    return data;
}