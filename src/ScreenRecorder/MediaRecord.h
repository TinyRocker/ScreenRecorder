#pragma once
#include <QThread>
#include <mutex>
#include <string>
#include "MediaFormat.h"

class MediaEncode;
class MediaFileCreate;
class VideoCapture;
class AudioCapture;

class MediaRecord : protected QThread
{
public:
    MediaRecord();
    virtual ~MediaRecord();

    bool init(const VidRawParam& vid, const AudRawParam& aud, bool videoIsRec = true, bool audioIsRec = true);
    bool uninit();
    bool startRecord(const VidEncodeParam& vid, const AudEncodeParam& aud);
    bool stopRecord();
    bool startWriteFile(const char *filename);
    bool stopWriteFile();
    bool startWriteRtmp(const char *url);
    bool stopWriteRtmp();
    
protected:
    virtual void run();

private:
    std::mutex m_mutex;
    bool m_init = false;
    bool m_start = false;
    bool m_videoIsRec = false;
    bool m_audioIsRec = false;
    bool m_writeFile = false;
    bool m_writeRtmp = false;
    MediaFileCreate *m_file = nullptr;
    MediaFileCreate *m_rtmp = nullptr;
    VideoCapture    *m_video = nullptr;
    AudioCapture    *m_audio = nullptr;

    std::string     m_filename;
    std::string     m_url;
    VidRawParam     m_vCap;      // 视频输入参数
    AudRawParam     m_aCap;      // 音频输入参数
    VidEncodeParam  m_vOut;      // 视频转码参数
    AudEncodeParam  m_aOut;      // 音频转码参数
};

