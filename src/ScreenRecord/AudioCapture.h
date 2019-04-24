#pragma once
#include "MediaFormat.h"
#include <QThread>
#include <queue>

class QAudioFormat;
class QAudioInput;
class QIODevice;
class MemoryPool;

class AudioCapture : protected QThread
{
public:
    AudioCapture();
    virtual ~AudioCapture();

    virtual bool init(const AudRecordParam& param);
    virtual bool uninit();
    bool startCapture();
    bool stopCapture();
    void *getData();    //线程安全
    void freeData(void *p);
    int channels() const { return m_channels; }
    int sampleRate() const { return m_sampleRate; }
protected:
    virtual void run();
private:
    bool m_init = false;
    bool m_start = false;
    std::mutex m_statLock;     // 状态锁
    std::mutex m_dataLock;     // 数据锁
    MemoryPool *m_mempool = nullptr;
    std::queue<void*> m_datas;
    int m_cacheSize = 10;

    QAudioFormat *m_audFmt = nullptr;;
    QAudioInput  *m_input = nullptr;
    QIODevice    *m_io = nullptr;

    int m_sampleRate = 48000;
    int m_channels = 2;
    int m_sampleSize = 16;
    int m_nbSample = 1024;
};