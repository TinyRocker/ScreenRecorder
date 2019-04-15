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

    bool init();
    bool uninit();
    bool startRecord(const char *filename, const VidRecParam& vidRecParam, const AudRecParam& audRecParam);
    bool stopRecord();
    
protected:
    virtual void run();

private:
    bool m_init = false;
    bool m_start = false;
    std::mutex m_mutex;
    MediaFileCreate *m_file = nullptr;
    VideoCapture    *m_video = nullptr;
    AudioCapture    *m_audio = nullptr;

    std::string m_filename;
    VidRecParam m_vidParam;
};

