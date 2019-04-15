#pragma once
#include <QThread>
#include <list>
#include <mutex>

class CaptureThread : protected QThread
{
public:
    virtual ~CaptureThread();

    virtual bool init() = 0;
    virtual bool uninit() = 0;

    virtual bool startCapture();
    virtual bool stopCapture();
    virtual void *getData();    //线程安全,返回的空间由用户释放
    void cacheSize(int size) { size > 0 ? m_cacheSize = size : m_cacheSize; }
protected:
    CaptureThread();
    virtual void run() = 0;

protected:
    bool m_init = false;
    bool m_start = false;
    std::mutex m_mutex;
    std::list<void*> m_datas;
    int m_cacheSize = 10;
};

