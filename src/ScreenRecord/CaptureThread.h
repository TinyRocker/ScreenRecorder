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
    virtual void *getData();
protected:
    CaptureThread();

    virtual void run() = 0;
    virtual bool captureData(void *data) = 0;

protected:
    bool m_init = false;
    bool m_start = false;
    std::mutex m_mutex;
    std::list<void*> m_datas;
    int m_cacheSize = 10;
};

