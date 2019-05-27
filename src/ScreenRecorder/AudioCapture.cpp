#include "AudioCapture.h"
#include "MemoryPool.h"
#include "glog/logging.h"
#include <QAudioInput>

AudioCapture::AudioCapture()
{
}

AudioCapture::~AudioCapture()
{
}

bool AudioCapture::init(const AudCapParam& param)
{
    std::lock_guard<std::mutex> lck(m_oper_lck);

    if (m_init)
    {
        LOG(WARNING) << "AudioCapture is already inited";
        return false;
    }

    m_channels = param.channels;
    m_sampleRate = param.sampleRate;
    m_sampleSize = param.sampleSize;
    m_nbSample = param.nbSample;

    m_audFmt = new QAudioFormat;
    m_audFmt->setChannelCount(m_channels);
    m_audFmt->setSampleRate(m_sampleRate);
    m_audFmt->setSampleSize(m_sampleSize);
    m_audFmt->setSampleType(QAudioFormat::UnSignedInt);
    m_audFmt->setByteOrder(QAudioFormat::LittleEndian);
    m_audFmt->setCodec("audio/pcm");
    m_input = new QAudioInput(*m_audFmt);

    m_size = m_nbSample * m_channels * m_sampleSize / 8;
    m_cacheSize = 50;
    
    m_data_lck.lock();
    m_mempool = new MemoryPool(m_cacheSize, m_nbSample * m_channels * m_sampleSize / 8);
    m_data_lck.unlock();

    m_init = true;

    return true;
}

bool AudioCapture::uninit()
{
    std::lock_guard<std::mutex> lck(m_oper_lck);

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
    
    m_data_lck.lock();
    if (m_mempool)
    {
        delete m_mempool;
        m_mempool = nullptr;
    }
    m_data_lck.unlock();

    m_init = false;

    return true;
}

bool AudioCapture::startCapture()
{
    bool ret = Capture::startCapture();
    if (!ret)
    {
        return false;
    }

    m_oper_lck.lock();
    if (!m_init)
    {
        m_oper_lck.unlock();
        LOG(ERROR) << "AudioCapture Thread not init!";
        return false;
    }
    m_io = m_input->start();    // 启动声卡io
    m_oper_lck.unlock();

    return true;
}

bool AudioCapture::stopCapture()
{
    Capture::stopCapture();
 
    m_oper_lck.lock();
    if (m_input)
    {
        m_input->stop();
    }
    m_io = nullptr;
    m_oper_lck.unlock();

    LOG(INFO) << "stop AudioCapture Thread";
    return true;
}

bool AudioCapture::captureData(char * data)
{
    int readedSize = 0;     // 已读size
    while (readedSize < m_size)
    {
        int br = m_input->bytesReady();
        if (br < m_nbSample)
        {
            msleep(1);  // 若声卡准备的数据不足，等待1ms
            continue;
        }
        int s = m_nbSample;

        // 最后一次
        if (m_size - readedSize < s)
        {
            s = m_size - readedSize;
        }
        int len = m_io->read(data + readedSize, s);
        readedSize += len;
    }
    return true;
}

int AudioCapture::syncTimeMs(int use_time)
{
    return 1;
}