#pragma once
#include <cstdint>
#include <string>

class AVPacket;
enum XSampleFmt
{
    X_S16 = 1,
    X_FLATP = 8
};

class XVideoWriter
{
public:
    virtual ~XVideoWriter();
    static XVideoWriter *getInstance(uint16_t index = 0);
    std::string filename() const { return m_filename; }
    int inWidth() const { return m_inWidth; }
    int inHeight() const { return m_inHeight; }
    int nbSample() const { return m_nbSample; }
    int inChannel() const { return m_inChannel; }
    virtual bool open(const char* file) = 0;
    virtual void close() = 0;
    virtual bool addVideoStream() = 0;
    virtual bool addAudioStream() = 0;
    virtual AVPacket* encodeVideo(const uint8_t *rgb) = 0;
    virtual AVPacket* encodeAudio(const uint8_t *pcm) = 0;
    virtual bool writeHead() = 0;
    virtual bool writeFrame(AVPacket *packet) = 0;
    virtual bool writeTail() = 0;
    virtual bool isVideoFront() = 0;
protected:
    XVideoWriter();
    std::string m_filename;
    
    // 音频输入参数
    int m_inWidth = 1280;
    int m_inHeight = 720;
    int m_inPixFmt = 28;    // AV_PIX_FMT_BGRA
    
    // 音频输入参数
    int m_inChannel = 6;
    int m_inSampleRate = 48000;         // 采样率
    XSampleFmt m_inSampleFmt = X_S16;   // 采样格式
    int m_nbSample = 1024;    // 输入输出通道的每帧数据每通道样本数量 

    // 视频输出参数
    int m_vBitrate = 1 * 1024 * 1000; // 2 Mbps
    int m_outWidth = 1280;
    int m_outHeight = 720;
    int m_outPixFmt = 0;    // AV_PIX_FMT_YUV420P
    int m_outFps = 25;

    // 音频输出参数
    int m_aBitrate = 2 * 64 * 1000; // 128 Kbps
    int m_outChannel = 2;
    int m_outSampleRate = 48000;            // 采样率
    XSampleFmt m_outSampleFmt = X_FLATP;    // 采样格式
};

