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

bool MediaFileCreate::open(const char * file)
{
    close();
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

bool MediaFileCreate::addVideoStream(VidCodecID codecId, int bitsize)
{
    if (!m_ic) { return false; }
    m_bitsize = bitsize;

    // 1.视频编码器创建
    AVCodec *codec = avcodec_find_encoder((AVCodecID)codecId);
    if (!codec)
    {
        LOG(ERROR) << "avcodec_find_encoder failed, AVCodecID:" << codecId;
        return false;
    }

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
    m_vCtx->codec_id = (AVCodecID)codecId;
    m_vCtx->thread_count = m_vThdNum;
    av_opt_set(m_vCtx->priv_data, "preset", "superfast", 0);
    m_vCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // 2.打开编码器
    int ret = avcodec_open2(m_vCtx, codec, nullptr);
    if (ret < 0)
    {
        LOG(ERROR) << "avcodec_open2 failed, reason:" << errStr(ret);
        return false;
    }
    LOG(INFO) << "avcodec_open2 " << codecId << " success!";

    // 3.添加视频流到输出上下文
    //m_vStream = avformat_new_stream(m_ic, codec);
    m_vStream = avformat_new_stream(m_ic, nullptr);
    if (!m_vStream)
    {
        LOG(ERROR) << "avformat_new_stream failed";
        return false;
    }

    m_vStream->id = AVMEDIA_TYPE_VIDEO;
    m_vStream->codecpar->codec_tag = 0;
    avcodec_parameters_from_context(m_vStream->codecpar, m_vCtx);

    av_dump_format(m_ic, 0, m_filename.c_str(), 1);

    // 4.像素（尺寸）转换上下文 rgb->yuv
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
    LOG(INFO) << "video AVFrame create success!";

    return true;
}

//AVPacket * MediaFileCreate::encodeVideo(const uint8_t * rgb)
void * MediaFileCreate::encodeVideo(const uint8_t * rgb)
{
    if (!m_ic) return nullptr;
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
    av_init_packet(packet);
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

bool MediaFileCreate::writeHead()
{
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
    if (!m_ic || !packet || ((AVPacket*)packet)->size <= 0) return false;
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
    if (!m_ic || !m_ic->pb) return false;

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