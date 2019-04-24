#pragma once
#include <string>
#include <mutex>
#include "MediaFormat.h"

class AVFormatContext;
class AVCodecContext;
class AVStream;
class SwsContext;
class SwrContext;
class AVFrame;
class AVPacket;

class MediaFileCreate
{
public:
    MediaFileCreate() {};
    virtual ~MediaFileCreate() {};
    
    bool open(const std::string& filename,
        const VidRecordParam& vidrec, const VidSwsParam& vidsws,
        const AudRecordParam& audrec, const AudSwrParam& audswr);
    void close();
    bool addVideoStream();
    bool addAudioStream();
    void* encodeVideo(const uint8_t *rgb);
    void* encodeAudio(const uint8_t *pcm);
    bool writeHead();
    bool writeFrame(void *packet);
    bool writeTail();
    bool isVideoFront();

    void inWidth(int width) { m_inWidth = width; }
    void inHeight(int height) { m_inHeight = height; }
    void outWidth(int width) { m_outWidth = width; }
    void outHeight(int height) { m_outHeight = height; }
    void outFps(int fps) { m_outFps = fps; }

private:
    std::string errStr(int err);
protected:
    std::string m_filename;

    AVFormatContext *m_ic = nullptr;        // 封装mp4输出上下文
    AVCodecContext  *m_vCtx = nullptr;      // 视频编码器上下文
    AVCodecContext  *m_aCtx = nullptr;      // 音频编码器上下文
    AVStream        *m_vStream = nullptr;   // 视频流
    AVStream        *m_aStream = nullptr;   // 音频流
    SwsContext      *m_vSwsCtx = nullptr;   // 视频像素转换上下文
    SwrContext      *m_aSwrCtx = nullptr;   // 音频重采样上下文
    AVFrame         *m_yuv = nullptr;       // 视频重采样输出yuv frame
    AVFrame         *m_pcm = nullptr;       // 音频重采样输出pcm
    uint64_t m_vpts = 0;                    // 视频的pts
    uint64_t m_apts = 0;                    // 音频的pts

    int m_vThdNum = 1;    // 视频编码线程数
    int m_aThdNum = 1;    // 音频编码线程数

    // 视频编码输入参数
    int m_inWidth = 1920;
    int m_inHeight = 1080;
    int m_inPixFmt = VID_PIX_FMT_BGRA;    // AV_PIX_FMT_BGRA
    int m_bitsize = 4;

    // 视频编码输出参数
    int m_vBitrate = 10 * 1024 * 1000; // 10 Mbps
    int m_outWidth = 1920;
    int m_outHeight = 1080;
    int m_outPixFmt = VID_PIX_FMT_YUV420P;    // AV_PIX_FMT_YUV420P
    int m_outFps = 25;
    VidCodecID m_vcodecId = VID_CODEC_ID_H264;

    // 音频输入参数
    int m_inChannels = 2;                           // 输入通道
    int m_inSampleRate = 44100;                     // 采样率
    AudSampleFmt m_inSampleFmt = AUD_SMP_FMT_S16;   // 采样格式
    int m_nbSample = 1024;    // 输入输出通道的每帧数据每通道样本数量 

    // 音频输出参数
    int m_aBitrate = 2 * 64 * 1000; // 128 Kbps
    int m_outChannels = 2;
    int m_outSampleRate = 44100;                      // 采样率
    AudSampleFmt m_outSampleFmt = AUD_SMP_FMT_FLATP;  // 采样格式
    AudCodecID m_acodecId = AUD_CODEC_ID_AAC;   // AV_CODEC_ID_AAC

};

