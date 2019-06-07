#pragma once
#include <string>
#include <mutex>
#include "MediaFormat.h"

class AVFormatContext;
class AVPacket;
class MediaEncode;

class MediaFileCreate
{
public:
    MediaFileCreate() {};
    virtual ~MediaFileCreate() {};
    
    bool open(const char *filename, const char *format_name);
    void close();
    bool setVideoEncode(const VidRawParam& raw, const VidEncodeParam& param);
    bool setAudioEncode(const AudRawParam& raw, const AudEncodeParam& param);
    AVPacket * encodeToPacket(const FrameData *data);
    bool writeHead();
    bool writePacket(AVPacket *pkt);
    bool writeTail();
    bool isAudioPtsEarly();
protected:
    std::mutex  m_lock;
    std::string m_filename;
    AVFormatContext *m_oc = nullptr;        // 封装输出上下文
    MediaEncode     *m_encode = nullptr;    // 编码器
};

