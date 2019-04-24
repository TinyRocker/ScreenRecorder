#include "AudioCapture.h"
#include "MemoryPool.h"
#include "glog/logging.h"
#include <QAudioInput>
#include <QTime>

#include <iostream>

AudioCapture::AudioCapture()
{
}

AudioCapture::~AudioCapture()
{
}

bool AudioCapture::init(const AudRecordParam& param)
{
    if (m_init)
    {
        LOG(WARNING) << "VideoCapture is already inited";
        return false;
    }

    m_channels = param.channels;
    m_sampleRate = param.sampleRate;
    m_sampleSize = param.sampleSize;

    m_audFmt = new QAudioFormat;
    m_audFmt->setChannelCount(m_channels);
    m_audFmt->setSampleRate(m_sampleRate);
    m_audFmt->setSampleSize(m_sampleSize);
    m_audFmt->setSampleType(QAudioFormat::UnSignedInt);
    m_audFmt->setByteOrder(QAudioFormat::LittleEndian);
    m_audFmt->setCodec("audio/pcm");
    m_input = new QAudioInput(*m_audFmt);

    m_mempool = new MemoryPool(m_cacheSize, m_nbSample * m_channels * m_sampleSize / 8);

    m_init = true;

    return true;
}

bool AudioCapture::uninit()
{
    if (m_input)
    {
        m_input->stop();
        delete m_input;
        m_input = nullptr;
    }

    if (m_audFmt)
    {
        delete m_audFmt;
        m_audFmt = nullptr;
    }
    
    if (m_mempool)
    {
        delete m_mempool;
        m_mempool = nullptr;
    }

    m_init = false;

    return true;
}

bool AudioCapture::startCapture()
{
    std::lock_guard<std::mutex> lck(m_statLock);
    if (!m_init)
    {
        LOG(WARNING) << "AudioCapture Thread not init";
        return false;
    }
    m_io = m_input->start();
    m_start = true;
    start();    // 启动qt线程

    LOG(INFO) << "start AudioCapture Thread";
    return true;
}

bool AudioCapture::stopCapture()
{
    std::lock_guard<std::mutex> lck(m_statLock);
    if (!m_init)
    {
        LOG(WARNING) << "AudioCapture Thread not init";
        return false;
    }

    m_start = false;
    terminate();        // 结束qt线程
    wait();

    m_io->close();
    m_mempool->clean();   // 结束获取后及时清理数据

    LOG(INFO) << "stop AudioCapture Thread";
    return true;
}

void * AudioCapture::getData()
{
    m_dataLock.lock();
    void *data = nullptr;
    if (!m_datas.empty())
    {
        data = m_datas.front();
        m_datas.pop();
    }
    m_dataLock.unlock();
    return data;
}

void AudioCapture::freeData(void * p)
{
    m_mempool->freeMemory(p);
}

void AudioCapture::run()
{
    int size = m_nbSample * m_channels * m_sampleSize / 8;
    char *data = nullptr;
    QTime time;

    while (m_start)
    {
        time.restart();
        std::lock_guard<std::mutex> lck(m_statLock);
        if (!m_start)   // double check
        {
            LOG(INFO) << "stop audio capture";
            break;
        }

        data = (char *)m_mempool->allocMemory();
        if (!data)
        {
            LOG(WARNING) << "cache buff full, wait 5ms, cache size:" << m_cacheSize;
            msleep(5); // 缓存区已满，等待5ms
            continue;
        }

        int readedSize = 0;     // 已读size
        while (readedSize < size)
        {
            int br = m_input->bytesReady();
            if (br < m_nbSample)
            {
                msleep(1);  // 若声卡准备的数据不足，等待1ms
                continue;
            }
            int s = m_nbSample;
            // 最后一次
            if (size - readedSize < s)
            {
                s = size - readedSize;
            }
            int len = m_io->read(data + readedSize, s);
            readedSize += len;
        }

        m_dataLock.lock();
        m_datas.push(data);
        m_dataLock.unlock();
        
        std::cout << "capture one frame audio use time:" << time.restart() << std::endl;
        //msleep(1);  // 线程休眠1ms
    }
}