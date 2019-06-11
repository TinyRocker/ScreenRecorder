#pragma once
#include <mutex>
#include "MediaFormat.h"

class AVFrame;
class AVPacket;
class AVCodecContext;
class AVFormatContext;
class AVStream;
class SwsContext;
class SwrContext;

class MediaEncode
{
public:
    MediaEncode();
    virtual ~MediaEncode();

    bool initVideoCodec(const VidRawParam& raw, const VidEncodeParam& param);
    bool deinitVideoCodec();
    bool initAudioCodec(const AudRawParam& raw, const AudEncodeParam& param);
    bool deinitAudioCodec();
    bool addVideoStream(AVFormatContext *oc);
    bool addAudioStream(AVFormatContext *oc);
    bool start();
    bool stop();
    AVPacket* encodeVideo(const FrameData *frame);
    AVPacket* encodeAudio(const FrameData *frame);
    bool isAudioPtsEarly();

    AVCodecContext *videoCodecContext() const { return m_vCtx; }
    AVCodecContext *audioCodecContext() const { return m_aCtx; }
    AVStream *videoStream() const { return m_vStream; }
    AVStream *audioStream() const { return m_aStream; }
    int64_t videoPts() const { return m_vpts; }
    int64_t audioPts() const { return m_apts; }
    
private:
    std::mutex  m_lock;
    AVCodecContext  *m_vCtx = nullptr;      // 视频编码器上下文
    AVCodecContext  *m_aCtx = nullptr;      // 音频编码器上下文
    AVStream        *m_vStream = nullptr;   // 视频流
    AVStream        *m_aStream = nullptr;   // 音频流
    SwsContext      *m_vSwsCtx = nullptr;   // 视频像素转换上下文
    SwrContext      *m_aSwrCtx = nullptr;   // 音频重采样上下文
    AVFrame         *m_yuv = nullptr;       // 视频重采样输出yuv frame
    AVFrame         *m_pcm = nullptr;       // 音频重采样输出pcm

    VidRawParam     m_vRaw;     // 视频输入参数
    VidEncodeParam  m_vEncode;  // 视频输出参数
    AudRawParam     m_aRaw;     // 音频输出参数
    AudEncodeParam  m_aEncode;  // 音频输出参数

    int64_t m_timestamp = 0;    // 开始编码时的时间戳
    int64_t m_vpts = 0;         // 视频的pts
    int64_t m_apts = 0;         // 音频的pts
    int     m_vThdNum = std::thread::hardware_concurrency();      // 视频编码线程数
    int     m_aThdNum = 2;      // 音频编码线程数
};