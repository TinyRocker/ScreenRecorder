#pragma once
#include <cstdint>

// 数值对应ffmpeg version 4.0.2

enum VidPixFmt
{
    VID_PIX_FMT_YUV420P = 0,
    VID_PIX_FMT_BGRA = 28
};

enum VidCodecID
{
    VID_CODEC_ID_H264 = 27,
    VID_CODEC_ID_H265 = 173
};

enum VidCapMode
{
    VID_CAP_MODE_NONE = 0,
    VID_CAP_MODE_DX9,
    VID_CAP_MODE_QT,
    VID_CAP_MODE_CNT
};

enum AudCodecID
{
    AUD_CODEC_ID_AAC = 86018,
};

enum AudSampleFmt
{
    AUD_SMP_FMT_S16 = 1,
    AUD_SMP_FMT_FLATP = 8
};

struct VidCapParam
{
    int width;
    int height;
    int fps;
    int bitsize;
    VidCapMode mode;
    int wid = 0;
    VidPixFmt format = VID_PIX_FMT_BGRA;
};

struct VidOutParam
{
    int width;
    int height;
    int bitrate;
    VidCodecID codecId;
};

struct AudCapParam
{
    int channels;
    int sampleRate;
    int sampleSize;
    int nbSample;
    AudCodecID codecId;
    AudSampleFmt fmt;
};

struct AudOutParam
{
    int channels;
    int sampleRate;
    int bitrate;
    AudCodecID codecId;
    AudSampleFmt fmt;
};

struct FrameData
{
    char *data;             // one frame
    int64_t  timestamp;     // currentMSecsSinceEpoch
};