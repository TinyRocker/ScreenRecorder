#pragma once
#include "CaptureThread.h"

class QAudioFormat;
class QAudioInput;
class QIODevice;

class AudioCapture : public CaptureThread
{
public:
    AudioCapture();
    virtual ~AudioCapture();

    virtual bool init();
    virtual bool uninit();

    int channels() const { return m_channels; }
    int sampleRate() const { return m_sampleRate; }
protected:
    virtual void run();
private:
    QAudioFormat *m_audFmt = nullptr;;
    QAudioInput  *m_input = nullptr;
    QIODevice    *m_io = nullptr;

    int m_sampleRate = 48000;
    int m_channels = 2;
    int m_sampleSize = 16;
    int m_nbSample = 1024;
};