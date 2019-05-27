#include "MediaFileCreate.h"
#include "glog/logging.h"

extern "C"
{
#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libavcodec/avcodec.h"
#include "ffmpeg/libswscale/swscale.h"
#include "ffmpeg/libavutil/opt.h"
#include "ffmpeg/libswresample/swresample.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

std::string MediaFileCreate::errStr(int err)
{
    char str[1024] = { 0 };
    av_strerror(err, str, 1024);
    return std::string(str);
}

bool MediaFileCreate::open(const std::string& filename,
    const VidCapParam& vidin, const VidOutParam& vidout,
    const AudCapParam& audrec, const AudOutParam& audswr)
{
    // 清理上一次的设置
    close();

    std::lock_guard<std::mutex> lck(m_lock);

    m_filename = filename;

    // 视频参数设置
    m_inWidth = vidin.width;
    m_inHeight = vidin.height;
    m_outFps = vidin.fps;
    m_bitsize = vidin.bitsize;
    m_outWidth = vidout.width;
    m_outHeight = vidout.height;
    m_vBitrate = vidout.bitrate;
    m_vcodecId = vidout.codecId;
    
    // 音频参数设置
    m_inChannels = audrec.channels;
    m_inSampleRate = audrec.sampleRate;

    m_outChannels = audswr.channels;
    m_outSampleRate = audswr.sampleRate;
    m_aBitrate = audswr.bitrate;
    m_acodecId = audswr.codecId;
 
    int ret = avformat_alloc_output_context2(&m_oc, nullptr, nullptr, filename.c_str());
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_alloc_output_context2 failed, reason:" << errStr(ret);
        return false;
    }
    m_ofmt = m_oc->oformat;
    LOG(INFO) << "open output file:" << m_filename.c_str();

    // 打开io
    ret = avio_open(&m_oc->pb, m_filename.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        LOG(ERROR) << "avio_open failed, reason:" << errStr(ret);
        return false;
    }
    
    m_vpts = 0;
    m_apts = 0;

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

    if (m_vCtx)
    {
        avcodec_close(m_vCtx);
        avcodec_free_context(&m_vCtx);
        m_vCtx = nullptr;
    }

    if (m_aCtx)
    {
        avcodec_close(m_aCtx);
        avcodec_free_context(&m_aCtx);
        m_aCtx = nullptr;
    }

    if (m_vSwsCtx)
    {
        sws_freeContext(m_vSwsCtx);
        m_vSwsCtx = nullptr;
    }

    if (m_aSwrCtx)
    {
        swr_free(&m_aSwrCtx);
        m_aSwrCtx = nullptr;
    }

    if (m_yuv)
    {
        av_frame_free(&m_yuv);
        m_yuv = nullptr;
    }

    if (m_pcm)
    {
        av_frame_free(&m_pcm);
        m_pcm = nullptr;
    }
}

bool MediaFileCreate::addVideoStream()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc) { return false; }

    // 1.找到视频编码
    LOG(INFO) << "video codec name:" << avcodec_descriptor_get((AVCodecID)m_vcodecId)->name;
    AVCodec *codec = avcodec_find_encoder((AVCodecID)m_vcodecId);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_encoder failed!";
        return false;
    }

    // 2.视频编码器创建
    m_vCtx = avcodec_alloc_context3(codec);
    if (!m_vCtx)
    {
        LOG(ERROR) << "avcodec_alloc_context3 failed";
        return false;
    }

    m_vCtx->bit_rate = m_vBitrate;  // bitrate, 压缩后每秒大小
    m_vCtx->width = m_outWidth;
    m_vCtx->height = m_outHeight;
    m_vCtx->time_base = { 1, m_outFps * 1000 };    // 时间基数
    m_vCtx->framerate = { m_outFps, 1 };
    m_vCtx->gop_size = 100;           // 画面组大小，多少帧一个关键帧
    m_vCtx->max_b_frames = 0;         // B帧为0
    m_vCtx->pix_fmt = (AVPixelFormat)m_outPixFmt;
    m_vCtx->codec_id = (AVCodecID)m_vcodecId;
    m_vCtx->thread_count = m_vThdNum;
    av_opt_set(m_vCtx->priv_data, "preset", "superfast", 0);
    m_vCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // 3.打开编码器
    int ret = avcodec_open2(m_vCtx, codec, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }

    // 4.添加视频流到输出上下文
    m_vStream = avformat_new_stream(m_oc, codec);
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
    av_dump_format(m_oc, 0, m_filename.c_str(), 1);

    // 5.像素（尺寸）转换上下文 rgb->yuv
    m_vSwsCtx = sws_getCachedContext(m_vSwsCtx,
        m_inWidth, m_inHeight, (AVPixelFormat)m_inPixFmt,
        m_outWidth, m_outHeight, (AVPixelFormat)m_outPixFmt,
        SWS_BICUBIC,
        nullptr, nullptr, nullptr);
    if (!m_vSwsCtx)
    {
        LOG(ERROR) << "sws_getCachedContext failed";
        return false;
    }

    if (!m_yuv)
    {
        m_yuv = av_frame_alloc();
        m_yuv->format = (AVPixelFormat)m_outPixFmt;
        m_yuv->width = m_outWidth;
        m_yuv->height = m_outHeight;
        m_yuv->pts = 0;
        ret = av_frame_get_buffer(m_yuv, 0);
        if (ret < 0)
        {
            LOG(ERROR) << "av_frame_get_buffer failed, reason:" << errStr(ret);
            return false;
        }
    }

    return true;
}

bool MediaFileCreate::addAudioStream()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc) return false;
    LOG(INFO) << "audio codec name:" << avcodec_descriptor_get((AVCodecID)m_vcodecId)->name;

    // 1.找到音频编码器
    AVCodec * codec = avcodec_find_encoder((AVCodecID)m_acodecId);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_encoder failed, AVCodecID:" << m_acodecId;
        return false;
    }

    // 2.创建音频编码器
    m_aCtx = avcodec_alloc_context3(codec);
    if (!m_aCtx)
    {
        LOG(ERROR) << "avcodec_alloc_context3 failed";
        return false;
    }
    m_aCtx->bit_rate = m_aBitrate;
    m_aCtx->sample_rate = m_outSampleRate;
    m_aCtx->bits_per_raw_sample = 16;
    m_aCtx->sample_fmt = (AVSampleFormat)m_outSampleFmt;
    m_aCtx->channels = m_outChannels;
    m_aCtx->channel_layout = av_get_default_channel_layout(m_outChannels);
    m_aCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    m_aCtx->time_base = { 1, m_outSampleRate };    // 时间基数
    m_aCtx->framerate = { m_outSampleRate, 1 };

    // 3.打开音频编码器
    int ret = avcodec_open2(m_aCtx, codec, nullptr);
    if (ret != 0)
    {
        avcodec_free_context(&m_aCtx);
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "avcodec_open2 " << m_acodecId << " success!";

    m_aStream = avformat_new_stream(m_oc, codec);
    if (!m_aStream)
    {
        LOG(ERROR) << "avformat_new_stream failed!";
        return false;
    }
    m_aStream->codecpar->codec_tag = 0;
    avcodec_parameters_from_context(m_aStream->codecpar, m_aCtx);

    av_dump_format(m_oc, 0, m_filename.c_str(), 1);

    // 4.音频重采样上下文件创建
    m_aSwrCtx = swr_alloc_set_opts(m_aSwrCtx,
        m_aCtx->channel_layout, m_aCtx->sample_fmt, m_aCtx->sample_rate,	//输出格式
        av_get_default_channel_layout(m_inChannels), (AVSampleFormat)m_inSampleFmt, m_inSampleRate,
        0, nullptr);
    ret = swr_init(m_aSwrCtx);
    if (ret < 0)
    {
        LOG(ERROR) << "swr_init failed, reason:" << errStr(ret);
        return false;
    }

    if (!m_pcm)
    {
        m_pcm = av_frame_alloc();
        m_pcm->format = m_aCtx->sample_fmt;
        m_pcm->channels = m_aCtx->channels;
        m_pcm->channel_layout = m_aCtx->channel_layout;
        m_pcm->nb_samples = m_nbSample; //一帧音频的样本数量
        ret = av_frame_get_buffer(m_pcm, 0);
        if (ret < 0)
        {
            LOG(ERROR) << "av_frame_get_buffer failed, reason:" << errStr(ret);
            return false;
        }
    }
    return true;
}

void * MediaFileCreate::encodeVideo(const FrameData * frame)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc || !frame || !frame->data) return nullptr;
    m_vpts = frame->timestamp - m_timestamp;
    if (m_vpts < 0)
    {
        LOG(WARNING) << "abandon! start timestamp:" << m_timestamp << "frame timestamp:" << frame->timestamp;
        return nullptr;
    }

    const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
    indata[0] = (uint8_t*)frame->data;
    int insize[AV_NUM_DATA_POINTERS] = { 0 };
    insize[0] = m_inWidth * m_bitsize;  // BRGA, a 为透明通道

    // rgb->yuv
    int h = sws_scale(m_vSwsCtx, indata, insize, 0, m_inHeight, m_yuv->data, m_yuv->linesize);
    if (h <= 0)
    {
        LOG(ERROR) << "sws_scale failed";
        return nullptr;
    }

    m_yuv->pts = av_rescale_q(m_vpts, AVRational{1, 1000}, m_vCtx->time_base);
    LOG(DEBUG) << "video pts:" << m_vpts << ", frame pts:" << m_yuv->pts;

    int ret = avcodec_send_frame(m_vCtx, m_yuv);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_send_frame failed, reason:" << errStr(ret);
        return nullptr;
    }

    // 接收编码后的数据
    AVPacket *packet = av_packet_alloc();
    av_init_packet(packet);
    ret = avcodec_receive_packet(m_vCtx, packet);
    if (ret < 0 || packet->size <= 0)
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

void * MediaFileCreate::encodeAudio(const FrameData * frame)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc || !frame && !frame->data) return nullptr;
    m_apts = frame->timestamp - m_timestamp;
    if (m_apts < 0)
    {
        LOG(WARNING) << "abandon! start timestamp:" << m_timestamp << "frame timestamp:" << frame->timestamp;
        return nullptr;
    }

    const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
    indata[0] = (uint8_t*)frame->data;
    int len = swr_convert(m_aSwrCtx, m_pcm->data, m_pcm->nb_samples, indata, m_pcm->nb_samples);
    
    m_pcm->pts = av_rescale_q(m_apts, AVRational{ 1, 1000 }, m_aCtx->time_base);
    LOG(DEBUG) << "audio pts:" << m_apts << ", frame pts:" << m_pcm->pts;

    // 2.音频编码
    int ret = avcodec_send_frame(m_aCtx, m_pcm);
    if (ret != 0)
    {
        LOG(ERROR) << "avcodec_send_frame failed, reason:" << errStr(ret);
        return nullptr;
    }

    // 接收编码后的数据
    AVPacket *packet = av_packet_alloc();
    ret = avcodec_receive_packet(m_aCtx, packet);
    if (ret != 0)
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

bool MediaFileCreate::writeHead()
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc) return false;

    m_timestamp = QDateTime::currentMSecsSinceEpoch();

    // 写入封装头
    int ret = avformat_write_header(m_oc, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_write_header failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "write " << m_filename.c_str() << "head success!";

    return true;
}

bool MediaFileCreate::writeFrame(void * packet)
{
    std::lock_guard<std::mutex> lck(m_lock);

    if (!m_oc || !packet || ((AVPacket*)packet)->size <= 0)
    {
        LOG(ERROR) << "param invalid!";
        return false;
    }
    int ret = av_interleaved_write_frame(m_oc, (AVPacket*)packet);     // 此函数会释放AVPacket
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

    if (!m_oc) return false;

    // 写入尾部信息索引
    int ret = av_write_trailer(m_oc);
    if (ret != 0)
    {
        LOG(ERROR) << "av_write_trailer failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "write " << m_filename.c_str() << "tail success!";

    return true;
}

bool MediaFileCreate::isAudioPtsEarly()
{
    if (!m_aCtx) return false;
    if (!m_vCtx) return true;
    
    return m_apts <= m_vpts;
}