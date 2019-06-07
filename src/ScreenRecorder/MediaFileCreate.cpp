#include "MediaFileCreate.h"
#include "glog/logging.h"
#include "MediaEncode.h"

extern "C"
{
#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libavutil/opt.h"
}

std::string errStr(int err)
{
    char str[1024] = { 0 };
    av_strerror(err, str, 1024);
    return std::string(str);
}

bool MediaFileCreate::open(const char *filename, const char *format_name)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!filename || !format_name)
    {
        LOG(ERROR) << "param is null!";
        return false;
    }

    m_filename = filename;
    int ret = avformat_alloc_output_context2(&m_oc, nullptr, format_name, filename);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_alloc_output_context2 failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "open output file:" << filename;

    // 打开io
    ret = avio_open(&m_oc->pb, filename, AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        LOG(ERROR) << "avio_open failed, reason:" << errStr(ret);
        return false;
    }
    m_encode = new MediaEncode;
    if (!m_encode)
    {
        LOG(ERROR) << "new MediaEncode failed!";
        return false;
    }

    return true;
}

void MediaFileCreate::close()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (m_oc)
    {
        if (m_oc->pb)
        {
            avio_closep(&m_oc->pb); // 关闭IO
        }
        avformat_close_input(&m_oc);
        m_oc = nullptr;
    }
    if (m_encode)
    {
        m_encode->deinitVideoCodec();
        m_encode->deinitAudioCodec();

        delete m_encode;
        m_encode = nullptr;
    }
}

bool MediaFileCreate::setVideoEncode(const VidRawParam & raw, const VidEncodeParam & param)
{
    std::lock_guard<std::mutex> lck(m_lock);
    
    if (!m_encode)
    {
        LOG(ERROR) << "m_encode is null!";
        return false;
    }
    if (!m_encode->initVideoCodec(raw, param))
    {
        LOG(ERROR) << "initVideoCodec failed!";
        return false;
    }
    if (!m_encode->addVideoStream(m_oc))
    {
        LOG(ERROR) << "addVideoStream failed!";
        return false;
    }
    
    return true;
}

bool MediaFileCreate::setAudioEncode(const AudRawParam & raw, const AudEncodeParam & param)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_encode)
    {
        LOG(ERROR) << "m_encode is null!";
        return false;
    }
    if (!m_encode->initAudioCodec(raw, param))
    {
        LOG(ERROR) << "initAudioCodec failed!";
        return false;
    }
    if (!m_encode->addAudioStream(m_oc))
    {
        LOG(ERROR) << "addAudioStream failed!";
        return false;
    }

    return true;
}

AVPacket * MediaFileCreate::encodeToPacket(const FrameData * data)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_encode || !data)
    {
        LOG(ERROR) << "param is null";
        return nullptr;
    }

    if (FrameType_Vid == data->type)
    {
        return m_encode->encodeVideo(data);
    }
    else if (FrameType_Aud == data->type)
    {
        return m_encode->encodeAudio(data);
    }
    else
    {
        return nullptr;
    }
}

bool MediaFileCreate::writeHead()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc)
    {
        LOG(ERROR) << "AVFormatContext is null!";
        return false;
    }

    // 写入封装头
    int ret = avformat_write_header(m_oc, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_write_header failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "write " << m_filename.c_str() << " head success!";
    
    m_encode->start();

    return true;
}

bool MediaFileCreate::writePacket(AVPacket *pkt)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc || !pkt)
    {
        LOG(ERROR) << "param invalid!";
        return false;
    }

    int ret = av_interleaved_write_frame(m_oc, pkt);     // 此函数会释放AVPacket
    if (ret < 0)
    {
        LOG(ERROR) << "av_interleaved_write_frame failed, reason:" << errStr(ret);
        return false;
    }

    return true;
}

bool MediaFileCreate::writeTail()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc)
    {
        LOG(ERROR) << "AVFormatContext is null!";
        return false;
    }

    // 写入尾部信息索引
    int ret = av_write_trailer(m_oc);
    if (ret < 0)
    {
        LOG(ERROR) << "av_write_trailer failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "write " << m_filename.c_str() << " tail success!";
    
    m_encode->stop();

    return true;
}

bool MediaFileCreate::isAudioPtsEarly()
{
    std::lock_guard<std::mutex> lck(m_lock);

    return m_encode ? m_encode->isAudioPtsEarly() : false;
}
