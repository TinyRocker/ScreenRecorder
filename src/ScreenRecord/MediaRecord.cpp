#include "MediaRecord.h"
#include "MediaFileCreate.h"
#include "VideoCapture.h"
#include "AudioCapture.h"
#include "glog/logging.h"

#include <iostream>

MediaRecord::MediaRecord()
{
    m_file = new MediaFileCreate();
    m_video = new VideoCapture();
    m_audio = new AudioCapture();
}

MediaRecord::~MediaRecord()
{
    delete m_file;
    delete m_video;
    delete m_audio;
}

bool MediaRecord::init(const VidRecordParam& vid, const AudRecordParam& aud)
{
    m_vRec = vid;
    m_aRec = aud;

    if (m_init)
    {
        LOG(WARNING) << "MediaRecord is already init!";
        return false;
    }

    if (!m_video->init(vid))
    {
        LOG(ERROR) << "video capture thread init failed!";
        return false;
    }
    if (!m_audio->init(aud))
    {
        LOG(ERROR) << "audio capture thread init failed!";
        return false;
    }

    m_init = true;

    return true;
}

bool MediaRecord::uninit()
{
    if (!m_video->uninit())
    {
        LOG(ERROR) << "video capture thread uninit failed!";
        return false;
    }
    if (!m_audio->uninit())
    {
        LOG(ERROR) << "audio capture thread uninit failed!";
        return false;
    }
    m_init = false;

    return true;
}

bool MediaRecord::startRecord(const std::string& filename, const VidSwsParam& vid, const AudSwrParam& aud)
{
    std::lock_guard<std::mutex> lck(m_stateLock);
    if (!m_init)
    {
        LOG(ERROR) << "MediaRecord is not init!";
        return false;
    }

    m_filename = filename;
    m_vSws = vid;
    m_aSwr = aud;

    if (!m_file->open(filename, m_vRec, m_vSws, m_aRec, m_aSwr))
    {
        LOG(ERROR) << "MediaFileCreate create file:" << filename << "failed!";
        return false;
    }

    m_file->addVideoStream();
    m_file->addAudioStream();
    m_file->writeHead();        // 写入MP4文件头
    
    m_video->startCapture();    // 开始捕获视频
    m_audio->startCapture();    // 开始捕获音频

    start();                    // 开启QT线程，写入文件
    m_start = true;

    return true;
}

bool MediaRecord::stopRecord()
{
    std::lock_guard<std::mutex> lck(m_stateLock);

    m_file->writeTail();        // 写入文件尾
    m_file->close();            // 关闭文件

    m_video->stopCapture();     // 停止捕获视频
    m_audio->stopCapture();     // 停止捕获音频

    terminate();                // 结束QT线程
    wait();
    m_start = false;

    return true;
}

#include <QTime>
void MediaRecord::run()
{
    uint8_t *rgb = nullptr;
    uint8_t *pcm = nullptr;
    void *packet = nullptr;
    QTime time;

    while (m_start)
    {
#if 0
        time.restart();
        std::lock_guard<std::mutex> lck(m_stateLock);
        if (!m_start)   // double check
        {
            LOG(INFO) << "stop media record, file:" << m_filename.c_str();
            break;
        }

        rgb = (uint8_t *)m_video->getData();    // 从缓存队列获取一帧RGB数据
        if (rgb)
        {
            
            packet = m_file->encodeVideo(rgb);  // 编码为YUV420P
            if (packet)
            {
                m_file->writeFrame(packet); // 写入文件
                std::cout << "write video one frame! use time:" << time.restart() << std::endl;
            }
            else
            {
                LOG(ERROR) << "encode video failed, filename:" << m_filename.c_str();
            }
            m_video->freeData(rgb);
        }
        msleep(1);
#endif
#if 1
        time.restart();
        //if (m_file->isVideoFront())
        {
            rgb = (uint8_t *)m_video->getData();    // 从缓存队列获取一帧RGB数据
            if (rgb)
            {

                packet = m_file->encodeVideo(rgb);  // 编码为YUV420P
                if (packet)
                {
                    m_file->writeFrame(packet); // 写入文件
                    std::cout << "write video one frame! use time:" << time.restart() << std::endl;
                }
                else
                {
                    LOG(ERROR) << "encode video failed, filename:" << m_filename.c_str();
                }
                m_video->freeData(rgb);
            }
        }
        //else
        {
            pcm = (uint8_t *)m_audio->getData();
            if (pcm)
            {
                packet = m_file->encodeAudio(pcm);
                if (packet)
                {
                    m_file->writeFrame(packet); // 写入文件
                    std::cout << "write audio one frame! use time:" << time.restart() << std::endl;
                }
                else
                {
                    LOG(ERROR) << "encode audio failed, filename:" << m_filename.c_str();
                }
                m_audio->freeData(pcm);
            }
        }
#endif
        msleep(1);  // 等1ms再写入，避免cpu占用率过高
    }
}
