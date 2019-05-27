#include "ScreenRecorder.h"
#include "MediaRecord.h"
#include "glog/logging.h"
#include <QScreen>
#include <QDesktopWidget> 

ScreenRecorder::ScreenRecorder(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    m_mediaRecord = new MediaRecord();
    
    ui.label_time->clear();
    ui.label_recordingVideo->hide();
    ui.label_recordingAudio->hide();
    ui.label_recordingVideo->setStyleSheet("border-image:url(:/ScreenRecorder/Resources/video_recorder_32px.ico)");
    ui.label_recordingAudio->setStyleSheet("border-image:url(:/ScreenRecorder/Resources/audio_recorder_32px.ico)");
    
    startTimer(500);
}

ScreenRecorder::~ScreenRecorder()
{
    delete m_mediaRecord;
}

void ScreenRecorder::timerEvent(QTimerEvent * event)
{
    if (!m_record)
    {
        return;
    }
    int es = m_time.elapsed() / 1000;
    int h = es / 3600;
    int m = (es % 3600) / 60;
    int s = es % 60;
    char buf[32] = { 0 };

    sprintf(buf, "%02d:%02d:%02d", h, m, s);
    ui.label_time->setText(buf);
}

void ScreenRecorder::setVideoIconDisabled(bool disable)
{
    if (disable)
    {
        ui.lineEdit_width->setDisabled(true);
        ui.lineEdit_height->setDisabled(true);
        ui.lineEdit_fps->setDisabled(true);
        ui.radioButton_dx9->setDisabled(true);
        ui.radioButton_qt->setDisabled(true);
    }
    else
    {
        ui.lineEdit_width->setEnabled(true);
        ui.lineEdit_height->setEnabled(true);
        ui.lineEdit_fps->setEnabled(true);
        ui.radioButton_dx9->setEnabled(true);
        ui.radioButton_qt->setEnabled(true);
    }
}

void ScreenRecorder::setAudioIconDisabled(bool disable)
{
    if (disable)
    {
        ui.lineEdit_channels->setDisabled(true);
        ui.lineEdit_samplerate->setDisabled(true);
    }
    else
    {
        ui.lineEdit_channels->setEnabled(true);
        ui.lineEdit_samplerate->setEnabled(true);
    }
}

void ScreenRecorder::setFileIconDisabled(bool disable)
{
    if (disable)
    {
        ui.lineEdit_fileSavePath->setDisabled(true);
        ui.lineEdit_fileSavePrefix->setDisabled(true);
    }
    else
    {
        ui.lineEdit_fileSavePath->setEnabled(true);
        ui.lineEdit_fileSavePrefix->setEnabled(true);
    }
}

void ScreenRecorder::setLogIconDisabled(bool disable)
{
    if (disable)
    {
        ui.lineEdit_logSavePath->setDisabled(true);
        ui.lineEdit_logSavePrefix->setDisabled(true);
    }
    else
    {
        ui.lineEdit_logSavePath->setEnabled(true);
        ui.lineEdit_logSavePrefix->setEnabled(true);
    }
}

void ScreenRecorder::checkVideoIsRecord()
{
    if (!ui.checkVideo->isChecked())
    {
        setVideoIconDisabled(true);
    }
    else
    {
        setVideoIconDisabled(false);
    }
}

void ScreenRecorder::checkAudioIsRecord()
{
    if (!ui.checkAudio->isChecked())
    {
        setAudioIconDisabled(true);
    }
    else
    {
        setAudioIconDisabled(false);
    }
}

void ScreenRecorder::record()
{
    if (!m_record)
    {
        // 开始录制
        QString filefullname = ui.lineEdit_fileSavePath->text();
        filefullname += ui.lineEdit_fileSavePrefix->text();
        filefullname += QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        filefullname += ".mp4";

        QString logPrefix = ui.lineEdit_logSavePath->text();
        logPrefix += ui.lineEdit_logSavePrefix->text();
        logPrefix += ui.comboBox_logLevel->currentText();

        // 开始日志记录
        google::InitGoogleLogging("ScreenRecorder"); // 初始化glog
        google::SetLogDestination(google::GLOG_DETAIL, logPrefix.toLocal8Bit());
        FLAGS_minloglevel = ui.comboBox_logLevel->currentIndex();
        FLAGS_logbufsecs = 0;

        int out_width = ui.lineEdit_width->text().toInt();
        int out_height = ui.lineEdit_height->text().toInt();
        int out_fps = ui.lineEdit_fps->text().toInt();
        int out_channels = ui.lineEdit_channels->text().toInt();
        int out_samplerate = ui.lineEdit_samplerate->text().toInt();

        bool audioIsRec = ui.checkAudio->isChecked();
        bool videoIsRec = ui.checkVideo->isChecked();
        if (!audioIsRec && !videoIsRec)
        {
            return;
        }

        VidCapMode mode = VID_CAP_MODE_DX9;
        if (ui.radioButton_dx9->isChecked())
        {
            mode = VID_CAP_MODE_DX9;
        }
        else if (ui.radioButton_qt->isChecked())
        {
            mode = VID_CAP_MODE_QT;
        }

        int width = QGuiApplication::primaryScreen()->size().width();
        int height = QGuiApplication::primaryScreen()->size().height();
        int vbitrate = 5 * 1024 * 1000;  // 5 Mbps
        int abitrate = 2 * 512 * 1000;   // 1 Mbps
        WId wid = QApplication::desktop()->winId();
        VidCapParam vidrec = { width, height, out_fps, 4, mode, wid };
        AudCapParam audrec = { out_channels, out_samplerate, 16, 1024 };

        VidOutParam vidsws = { out_width, out_height, vbitrate, VID_CODEC_ID_H264 };
        AudOutParam audswr = { out_channels, out_samplerate , abitrate, AUD_CODEC_ID_AAC };

        if (false == m_mediaRecord->init(vidrec, audrec, videoIsRec, audioIsRec))
        {
            LOG(ERROR) << "m_mediaRecord init failed!";
            return;
        }

        if (false == m_mediaRecord->startRecord(filefullname.toLocal8Bit().toStdString(), vidsws, audswr))
        {
            LOG(ERROR) << "m_mediaRecord startRecord failed!";
            return;
        }

        m_record = true;
        m_time.restart();

        ui.record->setStyleSheet("border-image:url(:/ScreenRecorder/Resources/stop_72px.ico)");
        if (videoIsRec)
        {
            ui.label_recordingVideo->show();
        }
        if (audioIsRec)
        {
            ui.label_recordingAudio->show();
        }
        ui.checkVideo->setDisabled(true);
        ui.checkAudio->setDisabled(true);
        setVideoIconDisabled(true);
        setAudioIconDisabled(true);
        setFileIconDisabled(true);
        setLogIconDisabled(true);
    }
    else
    {
        // 停止录制 TODO
        m_mediaRecord->stopRecord();
 
        m_record = false;
        ui.record->setStyleSheet("border-image:url(:/ScreenRecorder/Resources/start_72px.ico)");
        ui.label_recordingVideo->hide();
        ui.label_recordingAudio->hide();
        ui.checkVideo->setEnabled(true);
        ui.checkAudio->setEnabled(true);
        setVideoIconDisabled(false);
        setAudioIconDisabled(false);
        setFileIconDisabled(false);
        setLogIconDisabled(false);

        // 停止日志记录
        google::ShutdownGoogleLogging();
    }
}

void ScreenRecorder::setLogLevel()
{
    FLAGS_minloglevel = ui.comboBox_logLevel->currentIndex();
}
