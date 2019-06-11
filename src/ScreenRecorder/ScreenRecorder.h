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
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
private:
    void setVideoIconDisabled(bool);
    void setAudioIconDisabled(bool);
    void setFileIconDisabled(bool);
    void setLogIconDisabled(bool);
    void setRtmpIconDisabled(bool);
    bool startRecord();
    bool stopRecord();
    void setIconStatus(bool recording);
public slots:
    void checkVideoIsRecord();
    void checkAudioIsRecord();
    void checkLogIsRecord();
    void checkFileIsRecord();
    void checkRtmpIsRecord();
    void record();
    bool close();
    void setLogLevel();
    void setVideoConfig();
private:
    Ui::ScreenRecorderClass ui;
    bool         m_record = false;
    bool         m_move = false;
    QTime        m_time;
    QPoint       m_dragPoint;
    MediaRecord  *m_mediaRecord = nullptr;
};
