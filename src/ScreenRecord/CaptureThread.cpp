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
        LOG(WARNING) << "CaptureThread not init";
        return false;
    }

    m_mutex.lock();
    m_start = true;
    m_mutex.unlock();

    LOG(INFO) << "startCapture";
    start();    // Æô¶¯qtÏß³Ì

    return true;
}

bool CaptureThread::stopCapture()
{
    if (!m_init)
    {
        LOG(WARNING) << "CaptureThread not init";
        return false;
    }
    LOG(INFO) << "stopCapture";
    m_mutex.lock();
    m_start = false;
    m_mutex.unlock();

    terminate();
    wait();
    
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