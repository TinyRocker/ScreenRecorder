#pragma once
#include <cstdint>
#include "DXGIManager.h"

// 数值对应ffmpeg version 4.1.3

enum VidPixFmt
{
    VID_PIX_FMT_YUV420P = 0,
    VID_PIX_FMT_RGB24 = 2,
    VID_PIX_FMT_BGR24 = 3,
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
    VID_CAP_MODE_DXGI,
    VID_CAP_MODE_QT,
    VID_CAP_MODE_CNT
};

enum AudCodecID
{
    AUD_CODEC_ID_PCM = 0x10000,
    AUD_CODEC_ID_AAC = 86018,
};

enum AudSampleFmt
{
    AUD_SMP_FMT_S16 = 1,
    AUD_SMP_FMT_FLATP = 8
};

struct VidRawParam
{
    int width;
    int height;
    int fps;
    int bitsize;
    CaptureSource source;
    VidPixFmt format;
    int wid = 0;
    VidCapMode mode;
};

struct VidEncodeParam
{
    int width;
    int height;
    int fps;
    int bitrate;
    VidCodecID codecId;
    VidPixFmt format;
};

struct AudRawParam
{
    int channels;
    int sampleRate;
    int sampleSize;
    int nbSample;
    AudCodecID codecId;
    AudSampleFmt fmt;
};

struct AudEncodeParam
{
    int channels;
    int sampleRate;
    int bitrate;
    AudCodecID codecId;
    AudSampleFmt fmt;
};

enum FrameType
{
    FrameType_Vid = 0,
    FrameType_Aud
};

struct FrameData
{
    char      *data;        // one frame
    FrameType type;
    int64_t   timestamp;    // currentMSecsSinceEpoch
};