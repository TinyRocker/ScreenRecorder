#pragma once
#include "MediaFormat.h"
#include "Capture.h"

class ScreenImageCapture;

class VideoCapture : public Capture
{
public:
    VideoCapture();
    virtual ~VideoCapture();

    bool init(const VidRawParam& param);
    bool uninit();

    int width() const { return m_width; }
    int height() const { return m_height; }
    int bitsize() const { return m_bitsize; }
    int fps() const { return m_fps; }
protected:
    virtual bool captureData(char *p);
    virtual int syncTimeMs(int use_time);

private:
    ScreenImageCapture *m_screenCapture = nullptr;

    int m_width = 1920;
    int m_height = 1080;
    int m_bitsize = 4;      // bgra
    int m_fps = 25;
};