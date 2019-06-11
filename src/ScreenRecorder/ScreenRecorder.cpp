#include "ScreenRecorder.h"
#include "MediaRecord.h"
#include "glog/logging.h"
#include <QScreen>
#include <QDesktopWidget> 
#include <QMouseEvent>
#include <QMessageBox>

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
    setVideoConfig();
    ui.checkRtmp->setChecked(false);
    setRtmpIconDisabled(true);

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
    static char buf[32] = { 0 };

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

void ScreenRecorder::setVideoIconDisabled(bool stat)
{
    ui.lineEdit_width->setDisabled(stat);
    ui.lineEdit_height->setDisabled(stat);
    ui.lineEdit_fps->setDisabled(stat);
    ui.comboBox_captureMode->setDisabled(stat);
    ui.comboBox_screen->setDisabled(stat);
    ui.comboBox_bitrate_video->setDisabled(stat);
}

void ScreenRecorder::setAudioIconDisabled(bool stat)
{
    ui.lineEdit_channels->setDisabled(stat);
    ui.lineEdit_samplerate->setDisabled(stat);
    ui.comboBox_bitrate_audio->setDisabled(stat);
}

void ScreenRecorder::setFileIconDisabled(bool stat)
{
    ui.lineEdit_fileSavePath->setDisabled(stat);
    ui.lineEdit_fileSavePrefix->setDisabled(stat);
}

void ScreenRecorder::setLogIconDisabled(bool stat)
{
    ui.lineEdit_logSavePath->setDisabled(stat);
}

void ScreenRecorder::setRtmpIconDisabled(bool stat)
{
    ui.lineEdit_rtmpPath->setDisabled(stat);
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

void ScreenRecorder::checkLogIsRecord()
{
    if (!ui.checkLog->isChecked())
    {
        setLogIconDisabled(true);
    }
    else
    {
        setLogIconDisabled(false);
    }
}

void ScreenRecorder::checkFileIsRecord()
{
    if (!ui.checkFile->isChecked())
    {
        setFileIconDisabled(true);
    }
    else
    {
        setFileIconDisabled(false);
    }
}

void ScreenRecorder::checkRtmpIsRecord()
{
    if (!ui.checkRtmp->isChecked())
    {
        setRtmpIconDisabled(true);
    }
    else
    {
        setRtmpIconDisabled(false);
    }
}

bool ScreenRecorder::startRecord()
{
    // 开始日志记录
    if (ui.checkLog->isChecked())
    {
        QString logPrefix = ui.lineEdit_logSavePath->text();
        logPrefix += ui.comboBox_logLevel->currentText();
        google::InitGoogleLogging("ScreenRecorder"); // 初始化glog
        google::SetLogDestination(google::GLOG_DETAIL, logPrefix.toLocal8Bit());
        FLAGS_minloglevel = ui.comboBox_logLevel->currentIndex();
        FLAGS_logbufsecs = 0;
    }

    int out_width = ui.lineEdit_width->text().toInt();
    int out_height = ui.lineEdit_height->text().toInt();
    int out_fps = ui.lineEdit_fps->text().toInt();
    int out_channels = ui.lineEdit_channels->text().toInt();
    int out_samplerate = ui.lineEdit_samplerate->text().toInt();
    bool audioIsRec = ui.checkAudio->isChecked();
    bool videoIsRec = ui.checkVideo->isChecked();
    if (!audioIsRec && !videoIsRec)
    {
        return false;
    }

    VidCapMode mode = VID_CAP_MODE_DXGI;
    if (ui.comboBox_captureMode->currentText() == "DXGI") mode = VID_CAP_MODE_DXGI;
    else if (ui.comboBox_captureMode->currentText() == "DX9") mode = VID_CAP_MODE_DX9;
    else if (ui.comboBox_captureMode->currentText() == "QT") mode = VID_CAP_MODE_QT;

    CaptureSource screen;
    WId wid = 0;
    int width;
    int height;
    if (ui.comboBox_screen->currentText() == "Desktop")
    {
        screen = CSDesktop;
        wid = QApplication::desktop()->winId();
        width = QApplication::desktop()->width();
        height = QApplication::desktop()->height();
    }
    if (ui.comboBox_screen->currentText() == "Monitor1")
    {
        screen = CSMonitor1;
        wid = QApplication::desktop()->screen(0)->winId();
        width = QApplication::desktop()->screenGeometry(0).width();
        height = QApplication::desktop()->screenGeometry(0).height();
    }
    if (ui.comboBox_screen->currentText() == "Monitor2")
    {
        screen = CSMonitor2;
        wid = QApplication::desktop()->screen(1)->winId();
        width = QApplication::desktop()->screenGeometry(1).width();
        height = QApplication::desktop()->screenGeometry(1).height();
    }

    int vbitrate = ui.comboBox_bitrate_video->currentText().toInt() * 1000;
    int abitrate = ui.comboBox_bitrate_audio->currentText().toInt() * 1000;

    VidRawParam vidrec = { width, height, out_fps, 4, screen, VID_PIX_FMT_BGRA, wid, mode };
    AudRawParam audrec = { out_channels, out_samplerate, 16, 1024, AUD_CODEC_ID_PCM, AUD_SMP_FMT_S16 };

    VidEncodeParam vidsws = { out_width, out_height, out_fps, vbitrate, VID_CODEC_ID_H264, VID_PIX_FMT_YUV420P };
    AudEncodeParam audswr = { out_channels, out_samplerate , abitrate, AUD_CODEC_ID_AAC, AUD_SMP_FMT_FLATP };

    if (false == m_mediaRecord->init(vidrec, audrec, videoIsRec, audioIsRec))
    {
        LOG(ERROR) << "m_mediaRecord init failed!";
        return false;
    }

    if (false == m_mediaRecord->startRecord(vidsws, audswr))
    {
        LOG(ERROR) << "m_mediaRecord startRecord failed!";
        return false;
    }

    // 开始录制到文件
    if (ui.checkFile->isChecked())
    {
        QString filefullname = ui.lineEdit_fileSavePath->text();
        filefullname += ui.lineEdit_fileSavePrefix->text();
        filefullname += QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        filefullname += ".mp4";
        if (!m_mediaRecord->startWriteFile(filefullname.toLocal8Bit().toStdString().c_str()))
        {
            LOG(ERROR) << "startWriteFile failed!";
            return false;
        }
    }

    // 开始推流
    if (ui.checkRtmp->isChecked())
    {
        QString rtmpurl = ui.lineEdit_rtmpPath->text();
        if (!m_mediaRecord->startWriteRtmp(rtmpurl.toStdString().c_str()))
        {
            LOG(ERROR) << "startWriteRtmp failed!";
            return false;
        }
    }
    return true;
}

bool ScreenRecorder::stopRecord()
{
    // 停止录制
    m_mediaRecord->stopRecord();
    if (ui.checkFile->isChecked()) m_mediaRecord->stopWriteFile();
    if (ui.checkRtmp->isChecked()) m_mediaRecord->stopWriteRtmp();
    m_mediaRecord->uninit();
    
    if (ui.checkLog->isChecked())
    {
        // 停止日志记录
        google::ShutdownGoogleLogging();
    }
    return true;
}

void ScreenRecorder::setIconStatus(bool recording)
{
    // 修改ui图标
    if (recording)
    {
        ui.record->setStyleSheet("border-image:url(:/ScreenRecorder/Resources/stop_72px.ico)");
        if (ui.checkVideo->isChecked()) ui.label_recordingVideo->show();
        if (ui.checkAudio->isChecked()) ui.label_recordingAudio->show();
    }
    else
    {
        ui.record->setStyleSheet("border-image:url(:/ScreenRecorder/Resources/start_72px.ico)");
        ui.label_recordingVideo->hide();
        ui.label_recordingAudio->hide();
    }
    
    ui.checkVideo->setDisabled(recording);
    ui.checkAudio->setDisabled(recording);
    ui.checkLog->setDisabled(recording);
    ui.checkFile->setDisabled(recording);
    ui.checkRtmp->setDisabled(recording);

    setVideoIconDisabled(recording);
    setAudioIconDisabled(recording);
    setFileIconDisabled(recording);
    setLogIconDisabled(recording);
    setRtmpIconDisabled(recording);
}

void ScreenRecorder::record()
{
    if (!m_record)
    {
        if (!startRecord())
        {
            stopRecord();
            QMessageBox::about(this, "error", "startRecord failed!");
            return;
        }
        m_record = true;
        m_time.restart();
    }
    else
    {
        if (!stopRecord())
        {
            QMessageBox::about(this, "error", "stopRecord failed!");
            return;
        }
        m_record = false;
    }
    setIconStatus(m_record);
}

bool ScreenRecorder::close()
{
    if (m_record)
    {
        record();
    }
    return QWidget::close();
}

void ScreenRecorder::setLogLevel()
{
    FLAGS_minloglevel = ui.comboBox_logLevel->currentIndex();
}

void ScreenRecorder::setVideoConfig()
{
    int width = 0;
    int height = 0;
    if (ui.comboBox_screen->currentText() == "Desktop")
    {
        width = QApplication::desktop()->width();
        height = QApplication::desktop()->height();
    }
    if (ui.comboBox_screen->currentText() == "Monitor1")
    {
        width = QApplication::desktop()->screenGeometry(0).width();
        height = QApplication::desktop()->screenGeometry(0).height();
    }
    if (ui.comboBox_screen->currentText() == "Monitor2")
    {
        width = QApplication::desktop()->screenGeometry(1).width();
        height = QApplication::desktop()->screenGeometry(1).height();
    }
    ui.lineEdit_width->setText(QString::number(width));
    ui.lineEdit_height->setText(QString::number(height));
}