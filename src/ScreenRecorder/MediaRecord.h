#pragma once
#include <QThread>
#include <mutex>
#include <string>
#include "MediaFormat.h"

class MediaFileCreate;
class VideoCapture;
class AudioCapture;

class MediaRecord : protected QThread
{
public:
    MediaRecord();
    virtual ~MediaRecord();

    bool init(const VidCapParam& vid, const AudCapParam& aud, bool videoIsRec = true, bool audioIsRec = true);
    bool uninit();
    bool startRecord(const std::string& filename, const VidOutParam& vid, const AudOutParam& aud);
    bool stopRecord();
    
protected:
    virtual void run();

private:
    bool m_init = false;
    bool m_start = false;
    bool m_videoIsRec = false;
    bool m_audioIsRec = false;
    std::mutex m_mutex;
    MediaFileCreate *m_file = nullptr;
    VideoCapture    *m_video = nullptr;
    AudioCapture    *m_audio = nullptr;

    std::string    m_filename;
    VidCapParam    m_vCap;      // 视频输入参数
    AudCapParam    m_aCap;      // 音频输入参数
    VidOutParam    m_vOut;      // 视频转码参数
    AudOutParam    m_aOut;      // 音频转码参数
};

