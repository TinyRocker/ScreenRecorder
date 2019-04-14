#pragma once
#include <QThread>
#include <mutex>

class MediaFileCreate;
class VideoCapture;

class MediaRecord : protected QThread
{
public:
    MediaRecord();
    virtual ~MediaRecord();

    bool startRecord(const char *filename);
    bool stopRecord();
    
protected:
    virtual void run();

private:
    bool m_start = false;
    std::mutex m_mutex;
    MediaFileCreate *m_file = nullptr;
    VideoCapture    *m_video = nullptr;

    int m_width = 1920;
    int m_height = 1080;
    int m_fps = 20;
};

