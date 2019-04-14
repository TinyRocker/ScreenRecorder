#pragma once
#include "CaptureThread.h"

class AudioCapture : public CaptureThread
{
public:
    AudioCapture();
    virtual ~AudioCapture();

    virtual bool init();
    virtual bool uninit();
private:
    int m_sampleRate = 44100;
    int m_channels = 2;
    int m_sampleSize = 16;
    int m_nbSample = 1024;
};