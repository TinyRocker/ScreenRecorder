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

    bool init(const VidRecordParam& vid, const AudRecordParam& aud);
    bool uninit();
    bool startRecord(const std::string& filename, const VidSwsParam& vid, const AudSwrParam& aud);
    bool stopRecord();
    
protected:
    virtual void run();

private:
    bool m_init = false;
    bool m_start = false;
    std::mutex m_stateLock;
    MediaFileCreate *m_file = nullptr;
    VideoCapture    *m_video = nullptr;
    AudioCapture    *m_audio = nullptr;

    std::string m_filename;
    VidRecordParam m_vRec;      // 视频输入参数
    AudRecordParam m_aRec;      // 音频输入参数
    VidSwsParam    m_vSws;      // 视频转码参数
    AudSwrParam    m_aSwr;      // 音频转码参数
};

