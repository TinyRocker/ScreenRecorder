#include "MediaRecord.h"
#include "MediaFileCreate.h"
#include "VideoCapture.h"
#include "glog/logging.h"

#include <iostream>

MediaRecord::MediaRecord()
{
    m_file = new MediaFileCreate();
    m_video = new VideoCapture(VideoCapture::VID_CAP_MODE_DIRECTX, m_fps);
}

MediaRecord::~MediaRecord()
{
    delete m_file;
    delete m_video;
}

bool MediaRecord::startRecord(const char * filename)
{
    if (!m_video->init())
    {
        LOG(ERROR) << "video capture thread init failed!";
        return false;
    }

    if (!m_file->open(filename))
    {
        LOG(ERROR) << "MediaFileCreate create file:" << filename << "failed!";
        return false;
    }
    
    m_mutex.lock();

    m_file->addVideoStream(MediaFileCreate::VID_CODEC_ID_H264, 4);
    m_file->writeHead();
    m_video->startCapture();
    m_mutex.unlock();

    start();
    return true;
}

bool MediaRecord::stopRecord()
{
    m_mutex.lock();
    m_video->stopCapture();
    m_file->writeTail();
    m_file->close();
    m_mutex.unlock();

    terminate();
    wait();

    return true;
}

void MediaRecord::run()
{
    while (!m_start)
    {
        m_mutex.lock();

        uint8_t *rgb = (uint8_t *)m_video->getData();
        if (rgb)
        {
            
            void *packet = m_file->encodeVideo(rgb);
            m_file->writeFrame(packet);
            delete rgb;
            std::cout << "v";
        }

        m_mutex.unlock();

        msleep(5);
    }
}
