#pragma once
#include "MediaFormat.h"
#include "Capture.h"

class QAudioFormat;
class QAudioInput;
class QIODevice;

class AudioCapture : public Capture
{
public:
    AudioCapture();
    virtual ~AudioCapture();

    virtual bool init(const AudRawParam& param);
    virtual bool uninit();
    bool startCapture();
    bool stopCapture();

    int channels() const { return m_channels; }
    int sampleRate() const { return m_sampleRate; }
protected:
    virtual bool captureData(char *p);
    virtual int syncTimeMs(int use_time);
private:
    QAudioFormat *m_audFmt = nullptr;;
    QAudioInput  *m_input = nullptr;
    QIODevice    *m_io = nullptr;

    int m_sampleRate = 48000;
    int m_channels = 2;
    int m_sampleSize = 16;
    int m_nbSample = 1024;
    int m_size;
};