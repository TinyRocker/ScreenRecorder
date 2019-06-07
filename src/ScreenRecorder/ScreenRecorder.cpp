#include "ScreenRecorder.h"
#include "MediaRecord.h"
#include "glog/logging.h"
#include <QScreen>
#include <QDesktopWidget> 
#include <QMouseEvent>

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

void ScreenRecorder::mousePressEvent(QMouseEvent * event)
{
    // globalPos()获取鼠标在根窗口的位置
    // frameGeometry().topLeft()获取程序主左上角在根窗口的位置

    m_move = false;
    if (event->button() == Qt::LeftButton)
    {
        m_dragPoint = event->globalPos() - frameGeometry().topLeft();
        event->accept();  // 鼠标事件被系统接收
        if (ui.title->geometry().contains(m_dragPoint))
        {
            m_move = true;
        }
    }
}

void ScreenRecorder::mouseReleaseEvent(QMouseEvent * event)
{
    m_move = false;
    event->accept();
}

void ScreenRecorder::mouseMoveEvent(QMouseEvent * event)
{
    if (m_move && event->buttons() == Qt::LeftButton) // 当满足鼠标左键被点击时
    {
        move(event->globalPos() - m_dragPoint);     // 移动窗口
        event->accept();
    }
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
        ui.comboBox_bitrate_video->setDisabled(true);
    }
    else
    {
        ui.lineEdit_width->setEnabled(true);
        ui.lineEdit_height->setEnabled(true);
        ui.lineEdit_fps->setEnabled(true);
        ui.radioButton_dx9->setEnabled(true);
        ui.radioButton_qt->setEnabled(true);
        ui.comboBox_bitrate_video->setEnabled(true);
    }
}

void ScreenRecorder::setAudioIconDisabled(bool disable)
{
    if (disable)
    {
        ui.lineEdit_channels->setDisabled(true);
        ui.lineEdit_samplerate->setDisabled(true);
        ui.comboBox_bitrate_audio->setDisabled(true);
    }
    else
    {
        ui.lineEdit_channels->setEnabled(true);
        ui.lineEdit_samplerate->setEnabled(true);
        ui.comboBox_bitrate_audio->setEnabled(true);
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
        
        int vbitrate = ui.comboBox_bitrate_video->currentText().toInt() * 1000;
        int abitrate = ui.comboBox_bitrate_audio->currentText().toInt() * 1000;

        WId wid = QApplication::desktop()->winId();
        VidRawParam vidrec = { width, height, out_fps, 4, VID_PIX_FMT_BGRA, wid, mode };
        AudRawParam audrec = { out_channels, out_samplerate, 16, 1024, AUD_CODEC_ID_PCM, AUD_SMP_FMT_S16 };

        VidEncodeParam vidsws = { out_width, out_height, out_fps, vbitrate, VID_CODEC_ID_H264, VID_PIX_FMT_YUV420P };
        AudEncodeParam audswr = { out_channels, out_samplerate , abitrate, AUD_CODEC_ID_AAC, AUD_SMP_FMT_FLATP };

        if (false == m_mediaRecord->init(vidrec, audrec, videoIsRec, audioIsRec))
        {
            LOG(ERROR) << "m_mediaRecord init failed!";
            return;
        }

        if (false == m_mediaRecord->startRecord(vidsws, audswr))
        {
            LOG(ERROR) << "m_mediaRecord startRecord failed!";
            return;
        }
        
        m_record = true;
        m_time.restart();

        m_mediaRecord->startWriteFile(filefullname.toLocal8Bit().toStdString().c_str());
        m_mediaRecord->startWriteRtmp("rtmp://192.168.1.111/live");

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
        // 停止录制
        m_mediaRecord->stopRecord();
        m_mediaRecord->stopWriteFile();
        m_mediaRecord->stopWriteRtmp();
        m_mediaRecord->uninit();

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

bool ScreenRecorder::close()
{
    if (m_record)
    {
        record();
    }
    return QWidget::close();
}
