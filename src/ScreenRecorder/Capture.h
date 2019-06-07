#pragma once
#include <QThread>
#include <mutex>
#include <queue>
#include "MediaFormat.h"

class MemoryPool;

class Capture : protected QThread
{
public:
    virtual ~Capture();

    virtual bool startCapture();
    virtual bool stopCapture();
    FrameData *getData();
    //void popData();
    void freeData(FrameData *p);
    void clean();
protected:
    Capture();
    void pushData(FrameData *p);
    virtual void run();
    virtual bool captureData(char *p) = 0;
    virtual int syncTimeMs(int use_time) = 0;
    
    // 多线程安全
    std::mutex  m_data_lck;              // 数据锁
    std::mutex  m_oper_lck;              // 操作锁
    bool        m_init = false;
    bool        m_start = false;
    MemoryPool  *m_mempool = nullptr;    // 内存池句柄
    std::queue<FrameData*> m_frames;     // 内存队列
    FrameType   m_type;
    int         m_cacheSize = 10;
};