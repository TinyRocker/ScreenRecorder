#pragma once

#include <QtWidgets/QWidget>
#include <QTime>
#include "ui_ScreenRecorder.h"

class MediaRecord;

class ScreenRecorder : public QWidget
{
    Q_OBJECT

public:
    ScreenRecorder(QWidget *parent = Q_NULLPTR);
    ~ScreenRecorder();
protected:
    virtual void timerEvent(QTimerEvent *event);
private:
    void setVideoIconDisabled(bool);
    void setAudioIconDisabled(bool);
    void setFileIconDisabled(bool);
    void setLogIconDisabled(bool);
public slots:
    void checkVideoIsRecord();
    void checkAudioIsRecord();
    void record();
    void setLogLevel();
private:
    Ui::ScreenRecorderClass ui;
    bool         m_record = false;
    QTime        m_time;
    MediaRecord  *m_mediaRecord = nullptr;
};
