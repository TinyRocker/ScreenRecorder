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
    char str[512] = { 0 };
    av_strerror(err, str, 512);
    return std::string(str);
}

bool MediaFileCreate::open(const char * file, const VidSwsParam& vid, const AudSwrParam& aud)
{
    close();

    std::lock_guard<std::mutex> lck(m_mutex);

    m_inWidth = vid.inwidth;
    m_inHeight = vid.inheight;
    m_bitsize = vid.inbitsize;
    m_outWidth = vid.recParm.width;
    m_outHeight = vid.recParm.height;
    m_outFps = vid.recParm.fps;
    m_vBitrate = vid.recParm.bitrate;
    m_vcodecId = vid.recParm.codecId;

    m_inChannels = aud.channels;
    m_inSampleRate = aud.sampleRate;
    m_outChannels = aud.recParm.channels;
    m_outSampleRate = aud.recParm.sampleRate;
    m_aBitrate = aud.recParm.bitrate;
    m_acodecId = aud.recParm.codecId;

    LOG(INFO) << "open:" << file;
    m_filename = file;
    
    int ret = avformat_alloc_output_context2(&m_ic, nullptr, nullptr, file);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_alloc_output_context2 failed, reason:" << errStr(ret);
        return false;
    }
    return true;
}

void MediaFileCreate::close()
{
    LOG(INFO) << "close:" << m_filename.c_str();
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_ic)
    {
        avformat_close_input(&m_ic);
        m_ic = nullptr;
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
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!m_ic) { return false; }

    // 1.找到视频编码
    AVCodec *codec = avcodec_find_encoder((AVCodecID)m_vcodecId);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_encoder failed, AVCodecID:" << m_vcodecId;
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
    m_vCtx->time_base = { 1, m_outFps };    // 时间基数
    m_vCtx->framerate = { m_outFps, 1 };
    m_vCtx->gop_size = 100;          // 画面组大小，多少帧一个关键帧
    m_vCtx->max_b_frames = 0;        // B帧为0
    m_vCtx->pix_fmt = (AVPixelFormat)m_outPixFmt;
    m_vCtx->codec_id = (AVCodecID)m_vcodecId;
    m_vCtx->thread_count = m_vThdNum;
    av_opt_set(m_vCtx->priv_data, "preset", "superfast", 0);
    m_vCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // 3.打开编码器
    int ret = avcodec_open2(m_vCtx, codec, nullptr);
    if (ret < 0)
    {
        avcodec_free_context(&m_vCtx);
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "avcodec_open2 " << m_vcodecId << " success!";

    // 4.添加视频流到输出上下文
    m_vStream = avformat_new_stream(m_ic, codec);
    if (!m_vStream)
    {
        LOG(ERROR) << "avformat_new_stream failed";
        return false;
    }
    
    m_vStream->id = AVMEDIA_TYPE_VIDEO;
    m_vStream->codecpar->codec_tag = 0;
    avcodec_parameters_from_context(m_vStream->codecpar, m_vCtx);

    av_dump_format(m_ic, 0, m_filename.c_str(), 1);

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
        ret = av_frame_get_buffer(m_yuv, 32);
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
    std::lock_guard<std::mutex> lck(m_mutex);
    if (!m_ic) return false;

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
    m_aCtx->sample_fmt = (AVSampleFormat)m_outSampleFmt;
    m_aCtx->channels = m_outChannels;
    m_aCtx->channel_layout = av_get_default_channel_layout(m_outChannels);
    m_aCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    m_vCtx->time_base = { 1, m_outSampleRate };    // 时间基数
    m_vCtx->framerate = { m_outSampleRate, 1 };

    // 3.打开音频编码器
    int ret = avcodec_open2(m_aCtx, codec, nullptr);
    if (ret != 0)
    {
        avcodec_free_context(&m_aCtx);
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "avcodec_open2 " << m_acodecId << " success!";

    m_aStream = avformat_new_stream(m_ic, codec);
    if (!m_aStream)
    {
        LOG(ERROR) << "avformat_new_stream failed!";
        return false;
    }
    m_aStream->codecpar->codec_tag = 0;
    avcodec_parameters_from_context(m_aStream->codecpar, m_aCtx);

    av_dump_format(m_ic, 0, m_filename.c_str(), 1);

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

    return false;
}

void * MediaFileCreate::encodeVideo(const uint8_t * rgb)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (!m_ic || !rgb) return nullptr;
    const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
    indata[0] = rgb;
    int insize[AV_NUM_DATA_POINTERS] = { 0 };
    insize[0] = m_inWidth * m_bitsize;  // BRGA, a 为透明通道

    // rgb->yuv
    int h = sws_scale(m_vSwsCtx, indata, insize, 0, m_inHeight, m_yuv->data, m_yuv->linesize);
    if (h <= 0)
    {
        LOG(ERROR) << "sws_scale failed";
        return nullptr;
    }

    // encode
    m_yuv->pts = m_vpts++;
    int ret = avcodec_send_frame(m_vCtx, m_yuv);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_send_frame failed, reason:" << errStr(ret);
        return nullptr;
    }

    // 接收编码后的数据
    AVPacket *packet = av_packet_alloc();
    ret = avcodec_receive_packet(m_vCtx, packet);
    if (ret < 0 || packet->size <= 0)
    {
        av_packet_free(&packet);
        LOG(ERROR) << "avcodec_receive_packet failed, reason:" << errStr(ret);
        return nullptr;
    }

    av_packet_rescale_ts(packet, m_vCtx->time_base, m_vStream->time_base);  // 转换timebase
    packet->stream_index = m_vStream->index;

    return packet;
}

void * MediaFileCreate::encodeAudio(const uint8_t * pcm)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (!m_ic || !pcm) return nullptr;

    const uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
    indata[0] = pcm;
    int len = swr_convert(m_aSwrCtx, m_pcm->data, m_pcm->nb_samples, indata, m_pcm->nb_samples);
    
    // 2.音频编码
    int ret = avcodec_send_frame(m_aCtx, m_pcm);
    if (ret != 0)
    {
        LOG(ERROR) << "avcodec_send_frame failed, reason:" << errStr(ret);
        return nullptr;
    }

    // 接收编码后的数据
    AVPacket *packet = av_packet_alloc();
    //av_init_packet(pkt);
    ret = avcodec_receive_packet(m_aCtx, packet);
    if (ret != 0)
    {
        av_packet_free(&packet);
        LOG(ERROR) << "avcodec_receive_packet failed, reason:" << errStr(ret);
        return nullptr;
    }
    packet->stream_index = m_aStream->index;
    packet->pts = m_apts;
    m_apts += av_rescale_q(m_pcm->nb_samples, { 1, m_aCtx->sample_rate }, m_aCtx->time_base);

    //av_packet_rescale_ts(packet, m_aCtx->time_base, m_aStream->time_base);  // 转换timebase

    return packet;
}

bool MediaFileCreate::writeHead()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (!m_ic) return false;

    // 打开io
    int ret = avio_open(&m_ic->pb, m_filename.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0)
    {
        LOG(ERROR) << "avio_open failed, reason:" << errStr(ret);
        return false;
    }

    // 写入封装头
    ret = avformat_write_header(m_ic, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avformat_write_header failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "write " << m_filename << " success!";

    return true;
}

bool MediaFileCreate::writeFrame(void * packet)
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!m_ic || !packet || ((AVPacket*)packet)->size <= 0)
    {
        LOG(ERROR) << "param invalid!";
        return false;
    }
    int ret = av_interleaved_write_frame(m_ic, (AVPacket*)packet);     // 此函数会释放AVPacket
    if (ret < 0)
    {
        LOG(ERROR) << "av_interleaved_write_frame failed, reason:" << errStr(ret);
        return false;
    }
    return true;
}

bool MediaFileCreate::writeTail()
{
    std::lock_guard<std::mutex> lck(m_mutex);

    if (!m_ic || !m_ic->pb)
    {
        return false;
    }

    // 写入尾部信息索引
    int ret = av_write_trailer(m_ic);
    if (ret != 0)
    {
        LOG(ERROR) << "av_write_trailer failed, reason:" << errStr(ret);
        return false;
    }
    ret = avio_closep(&m_ic->pb);
    if (ret < 0)
    {
        LOG(ERROR) << "avio_close failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "write tail success!";

    return true;
}

bool MediaFileCreate::isVideoFront()
{
    if (!m_ic || !m_aCtx || !m_vCtx) return false;
    int ret = av_compare_ts(m_vpts, m_vCtx->time_base, m_apts, m_aCtx->time_base);
    if (ret <= 0)
    {
        return true;
    }
    return false;
}