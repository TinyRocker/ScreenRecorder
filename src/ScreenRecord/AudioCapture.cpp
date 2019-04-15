#include "AudioCapture.h"
#include "glog/logging.h"
#include <QAudioInput>

#include <iostream>

AudioCapture::AudioCapture()
{
}


AudioCapture::~AudioCapture()
{
}

bool AudioCapture::init()
{
    if (m_init)
    {
        LOG(WARNING) << "VideoCapture is already inited";
        return true;
    }
    m_mutex.lock();
    m_audFmt = new QAudioFormat;
    m_audFmt->setSampleRate(m_sampleRate);
    m_audFmt->setChannelCount(m_channels);
    m_audFmt->setSampleSize(m_sampleSize);
    m_audFmt->setSampleType(QAudioFormat::UnSignedInt);
    m_audFmt->setByteOrder(QAudioFormat::LittleEndian);
    m_audFmt->setCodec("audio/pcm");
    m_input = new QAudioInput(*m_audFmt);
    m_io = m_input->start();
    m_init = true;
    m_mutex.unlock();

    return true;
}

bool AudioCapture::uninit()
{
    m_mutex.lock();
    if (m_io)
    {
        m_io->close();
    }

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
    
    m_init = false;
    m_mutex.unlock();

    return true;
}

void AudioCapture::run()
{
    int size = m_nbSample * m_channels * m_sampleSize / 8;
    char *data = nullptr;
    while (m_start)
    {
        //std::cout << 'a1';
        m_mutex.lock();
        if (m_datas.size() >= m_cacheSize)
        {
            m_mutex.unlock();
            LOG(WARNING) << "cache buff full, wait 1ms, cache size:" << m_cacheSize;
            msleep(1); // 缓存区已满，等待1ms
            continue;
        }
        data = new char[size];
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
        m_datas.push_back(data);
        m_mutex.unlock();
        //std::cout << 'a2';
    }
}