#include "MediaEncode.h"
#include "glog/logging.h"
#include <QDateTime>

extern "C"
{
#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libavcodec/avcodec.h"
#include "ffmpeg/libswscale/swscale.h"
#include "ffmpeg/libswresample/swresample.h"
#include "ffmpeg/libavutil/opt.h"
}

static std::string errStr(int err)
{
    char str[1024] = { 0 };
    av_strerror(err, str, 1024);
    return std::string(str);
}

MediaEncode::MediaEncode()
{
}

MediaEncode::~MediaEncode()
{
}

bool MediaEncode::initVideoCodec(const VidRawParam& raw, const VidEncodeParam& param)
{
    std::lock_guard<std::mutex> lck(m_lock);

    m_vRaw = raw;
    m_vEncode = param;

    // 1.找到视频编码
    LOG(INFO) << "video codec name:" << avcodec_descriptor_get((AVCodecID)param.codecId)->name;
    AVCodec *codec = avcodec_find_encoder((AVCodecID)param.codecId);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_encoder " << avcodec_descriptor_get((AVCodecID)param.codecId)->name << " failed!";
        return false;
    }

    // 2.视频编码器创建
    m_vCtx = avcodec_alloc_context3(codec);
    if (!m_vCtx)
    {
        LOG(ERROR) << "avcodec_alloc_context3 failed";
        return false;
    }

    m_vCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;   // 预览第一帧
    m_vCtx->codec_id = (AVCodecID)param.codecId;    // 编码协议
    m_vCtx->pix_fmt = (AVPixelFormat)param.format;  // 编码格式
    m_vCtx->bit_rate = param.bitrate;               // bitrate, 压缩后每秒多少bit
    m_vCtx->width = param.width;
    m_vCtx->height = param.height;
    m_vCtx->time_base = { 1, param.fps * 1000 };    // 时间基数ms为单位
    m_vCtx->framerate = { param.fps, 1 };
    m_vCtx->gop_size = 100;                         // 画面组I帧间隔
    m_vCtx->max_b_frames = 0;                       // B帧为0
    m_vCtx->thread_count = m_vThdNum;               // 编码线程数
    av_opt_set(m_vCtx->priv_data, "preset", "superfast", 0);

    // 3.像素（尺寸）转换上下文创建 rgb->yuv
    m_vSwsCtx = sws_getCachedContext(m_vSwsCtx,
        m_vRaw.width, m_vRaw.height, (AVPixelFormat)m_vRaw.format,
        m_vCtx->width, m_vCtx->height, m_vCtx->pix_fmt,
        SWS_BICUBIC,
        nullptr, nullptr, nullptr);
    if (!m_vSwsCtx)
    {
        LOG(ERROR) << "sws_getCachedContext failed";
        return false;
    }

    // 4.为AVFrame分配空间
    m_yuv = av_frame_alloc();
    m_yuv->format = m_vCtx->pix_fmt;
    m_yuv->width = m_vCtx->width;
    m_yuv->height = m_vCtx->height;
    m_yuv->pts = 0;
    int ret = av_frame_get_buffer(m_yuv, 0);   // 自动根据cpu字节对齐
    if (ret < 0)
    {
        LOG(ERROR) << "av_frame_get_buffer failed, reason:" << errStr(ret);
        return false;
    }

    return true;
}

bool MediaEncode::deinitVideoCodec()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (m_vCtx)
    {
        avcodec_close(m_vCtx);
        avcodec_free_context(&m_vCtx);
        m_vCtx = nullptr;
        m_vStream = nullptr;
    }

    if (m_vSwsCtx)
    {
        sws_freeContext(m_vSwsCtx);
        m_vSwsCtx = nullptr;
    }

    if (m_yuv)
    {
        av_frame_free(&m_yuv);
        m_yuv = nullptr;
    }

    return true;
}

bool MediaEncode::initAudioCodec(const AudRawParam & raw, const AudEncodeParam & param)
{
    std::lock_guard<std::mutex> lck(m_lock);

    m_aRaw = raw;
    m_aEncode = param;

    // 1.找到音频编码
    LOG(INFO) << "audio codec name:" << avcodec_descriptor_get((AVCodecID)param.codecId)->name;
    AVCodec *codec = avcodec_find_encoder((AVCodecID)param.codecId);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_encoder " << avcodec_descriptor_get((AVCodecID)param.codecId)->name << " failed!";
        return false;
    }

    // 2.音频编码器创建
    m_aCtx = avcodec_alloc_context3(codec);
    if (!m_aCtx)
    {
        LOG(ERROR) << "avcodec_alloc_context3 failed";
        return false;
    }
    m_aCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;   // 预览第一帧
    m_aCtx->codec_id = (AVCodecID)param.codecId;    // 编码协议
    m_aCtx->sample_fmt = (AVSampleFormat)param.fmt; // 编码格式
    m_aCtx->bit_rate = param.bitrate;               // bitrate, 压缩后每秒多少bit
    m_aCtx->sample_rate = param.sampleRate;         // 采样率
    m_aCtx->bits_per_raw_sample = 16;               // 原始采样位数
    m_aCtx->channels = param.channels;
    m_aCtx->channel_layout = av_get_default_channel_layout(param.channels);
    m_aCtx->time_base = { 1, param.sampleRate };    // 时间基数ms为单位
    m_aCtx->framerate = { param.sampleRate, 1 };
    m_aCtx->thread_count = m_aThdNum;               // 编码线程数
    //av_opt_set(m_aCtx->priv_data, "preset", "superfast", 0);

    // 3.音频重采样上下文件创建
    m_aSwrCtx = swr_alloc_set_opts(m_aSwrCtx,
        m_aCtx->channel_layout, m_aCtx->sample_fmt, m_aCtx->sample_rate,	//输出格式
        av_get_default_channel_layout(raw.channels), (AVSampleFormat)raw.fmt, raw.sampleRate,
        0, nullptr);
    int ret = swr_init(m_aSwrCtx);
    if (ret < 0)
    {
        LOG(ERROR) << "swr_init failed, reason:" << errStr(ret);
        return false;
    }

    // 4.为AVFrame分配空间
    m_pcm = av_frame_alloc();
    m_pcm->format = m_aCtx->sample_fmt;
    m_pcm->channels = m_aCtx->channels;
    m_pcm->channel_layout = m_aCtx->channel_layout;
    m_pcm->nb_samples = raw.nbSample;   //一帧音频的样本数量
    ret = av_frame_get_buffer(m_pcm, 0);
    if (ret < 0)
    {
        LOG(ERROR) << "av_frame_get_buffer failed, reason:" << errStr(ret);
        return false;
    }

    return true;
}

bool MediaEncode::deinitAudioCodec()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (m_aCtx)
    {
        avcodec_close(m_aCtx);
        avcodec_free_context(&m_aCtx);
        m_aCtx = nullptr;
        m_aStream = nullptr;
    }

    if (m_aSwrCtx)
    {
        swr_free(&m_aSwrCtx);
        m_aSwrCtx = nullptr;
    }

    if (m_pcm)
    {
        av_frame_free(&m_pcm);
        m_pcm = nullptr;
    }
    
    return true;
}

bool MediaEncode::addVideoStream(AVFormatContext *oc)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!oc)
    {
        LOG(ERROR) << "param is error!";
        return false;
    }

    if (!m_vCtx)
    {
        LOG(ERROR) << "video codecCtx is null!";
        return false;
    }

    // 1.打开编码器
    int ret = avcodec_open2(m_vCtx, m_vCtx->codec, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }

    // 2.添加视频流到输出上下文
    m_vStream = avformat_new_stream(oc, m_vCtx->codec);
    if (!m_vStream)
    {
        LOG(ERROR) << "avformat_new_stream failed!";
        return false;
    }
    m_vStream->id = AVMEDIA_TYPE_VIDEO;
    m_vStream->codecpar->codec_tag = 0;

    ret = avcodec_parameters_from_context(m_vStream->codecpar, m_vCtx);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_parameters_from_context failed, reason:" << errStr(ret);
        return false;
    }

    /// 打印视频流参数配置
    av_dump_format(oc, 0, "video", 1);

    return true;
}

bool MediaEncode::addAudioStream(AVFormatContext * oc)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!oc)
    {
        LOG(ERROR) << "param is error!";
        return false;
    }

    if (!m_aCtx)
    {
        LOG(ERROR) << "audio codecCtx is null!";
        return false;
    }

    // 1.打开编码器
    int ret = avcodec_open2(m_aCtx, m_aCtx->codec, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }

    // 2.添加音频流到输出上下文
    m_aStream = avformat_new_stream(oc, m_aCtx->codec);
    if (!m_aStream)
    {
        LOG(ERROR) << "avformat_new_stream failed!";
        return false;
    }
    m_aStream->id = AVMEDIA_TYPE_AUDIO;
    m_aStream->codecpar->codec_tag = 0;

    ret = avcodec_parameters_from_context(m_aStream->codecpar, m_aCtx);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_parameters_from_context failed, reason:" << errStr(ret);
        return false;
    }

    /// 打印音频流参数配置
    av_dump_format(oc, 0, "audio", 1);

    return true;
}

bool MediaEncode::start()
{
    std::lock_guard<std::mutex> lck(m_lock);

    m_timestamp = QDateTime::currentMSecsSinceEpoch();
    return true;
}

bool MediaEncode::stop()
{
    std::lock_guard<std::mutex> lck(m_lock);

    m_timestamp = 0;
    m_vpts = 0;
    m_apts = 0;
    return true;
}

AVPacket * MediaEncode::encodeVideo(const FrameData * frame)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!frame || !frame->data)
    {
        LOG(ERROR) << "param is error!";
        return nullptr;
    }

    m_vpts = frame->timestamp - m_timestamp;
    if (m_vpts < 0)
    {
        LOG(WARNING) << "abandon! start timestamp:" << m_timestamp << " frame timestamp:" << frame->timestamp;
        return nullptr;
    }

    // 1.rgb->yuv
    const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
    indata[0] = (uint8_t*)frame->data;
    int insize[AV_NUM_DATA_POINTERS] = { 0 };
    insize[0] = m_vRaw.width * m_vRaw.bitsize;

    int h = sws_scale(m_vSwsCtx, indata, insize, 0, m_vRaw.height, m_yuv->data, m_yuv->linesize);
    if (h <= 0)
    {
        LOG(ERROR) << "sws_scale failed";
        return nullptr;
    }

    // 2.转化AVFrame的timebase，开始编码
    static AVRational timebase{ 1, 1000 };
    m_yuv->pts = av_rescale_q(m_vpts, timebase, m_vCtx->time_base);
    LOG(DEBUG) << "video pts:" << m_vpts << ", frame pts:" << m_yuv->pts;

    int ret = avcodec_send_frame(m_vCtx, m_yuv);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_send_frame failed, reason:" << errStr(ret);
        return nullptr;
    }

    // 3.接收编码后的数据并转换AVPacket的timebase
    AVPacket *packet = av_packet_alloc();
    av_init_packet(packet);
    ret = avcodec_receive_packet(m_vCtx, packet);
    if (ret < 0)
    {
        av_packet_free(&packet);
        LOG(ERROR) << "avcodec_receive_packet failed, reason:" << errStr(ret);
        return nullptr;
    }
    packet->stream_index = m_vStream->index;
    av_packet_rescale_ts(packet, m_vCtx->time_base, m_vStream->time_base);  // 转换timebase
    LOG(DEBUG) << "video packet dts:" << packet->dts << ", pts:" << packet->pts;

    return packet;
}

AVPacket * MediaEncode::encodeAudio(const FrameData * frame)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!frame && !frame->data)
    {
        LOG(ERROR) << "param is error!";
        return nullptr;
    }

    m_apts = frame->timestamp - m_timestamp;
    if (m_apts < 0)
    {
        LOG(WARNING) << "abandon! start timestamp:" << m_timestamp << " frame timestamp:" << frame->timestamp;
        return nullptr;
    }

    // 1.pcm->aac
    const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
    indata[0] = (uint8_t*)frame->data;
    int len = swr_convert(m_aSwrCtx, m_pcm->data, m_pcm->nb_samples, indata, m_pcm->nb_samples);
    if (len <= 0)
    {
        LOG(ERROR) << "swr_convert failed!";
        return nullptr;
    }

    // 2.转化AVFrame的timebase，开始编码
    static AVRational timebase{ 1, 1000 };
    m_pcm->pts = av_rescale_q(m_apts, timebase, m_aCtx->time_base);
    LOG(DEBUG) << "audio pts:" << m_apts << ", frame pts:" << m_pcm->pts;

    int ret = avcodec_send_frame(m_aCtx, m_pcm);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_send_frame failed, reason:" << errStr(ret);
        return nullptr;
    }

    // 3.接收编码后的数据并转换AVPacket的timebase
    AVPacket *packet = av_packet_alloc();
    ret = avcodec_receive_packet(m_aCtx, packet);
    if (ret < 0)
    {
        av_packet_free(&packet);
        LOG(ERROR) << "avcodec_receive_packet failed, reason:" << errStr(ret);
        return nullptr;
    }

    packet->stream_index = m_aStream->index;
    av_packet_rescale_ts(packet, m_aCtx->time_base, m_aStream->time_base);  // 转换timebase
    LOG(DEBUG) << "audio packet dts:" << packet->dts << ", pts:" << packet->pts;

    return packet;
}

bool MediaEncode::isAudioPtsEarly()
{
    if (!m_aCtx) return false;
    if (!m_vCtx) return true;

    return m_apts <= m_vpts;
}
