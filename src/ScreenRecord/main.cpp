#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QScreen>
#include <QtWidgets/QWidget>
#include <QPainter>
#include <thread>
#include "glog/logging.h"

#include "ScreenRecord.h"

#include "MediaRecord.h"

#include "MemoryPool.h"
#include "VideoCapture.h"

//#include <d3d9.h>

#ifdef _DEBUG
#pragma comment(lib, "glogd") 
#else
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    google::InitGoogleLogging((const char*)argv[0]); // 初始化glog
    google::SetLogDestination(google::GLOG_INFO, "./log/myinfo");
    FLAGS_logbufsecs = 0;

    QScreen *m_screen = QGuiApplication::primaryScreen();
    int width = m_screen->size().width();
    int height = m_screen->size().height();

    // 测试video线程类
    ScreenRecord screen;
    screen.show();

#if 0

    int vbitrate = 5 * 1024 * 1000; // 5 Mbps
    int abitrate = 2 * 128 * 1000;   // 2 Mbps

    VidRecordParam vidrec = { width, height, 20, 4, VID_CAP_MODE_DIRECTX };
    AudRecordParam audrec = { 2, 48000, 16 };

    VidSwsParam vidsws = { 1920, 1080, vbitrate, VID_CODEC_ID_H264 };
    AudSwrParam audswr = { 2, 48000 , abitrate, AUD_CODEC_ID_AAC };

    // 测试MediaRecord
    MediaRecord *record = new MediaRecord();
    if (!record->init(vidrec, audrec))
    {
        LOG(ERROR) << "MediaRecord init failed";
        return -1;
    }

    QDateTime t = QDateTime::currentDateTime();

    QString filename = t.toString("yyyyMMdd_hhmmss");
    filename = "./video/xcreen_" +  filename + ".mp4";

    record->startRecord(filename.toLocal8Bit().toStdString(), vidsws, audswr);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    //getchar();

    record->stopRecord();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    system("pause");

    //int bitrate = 5 * 1024 * 1000; // 5 Mbps
    //VidRecParam vidParam[5] =
    //{
    //    { 2560, 1440, 10, bitrate, VID_CODEC_ID_H264 },
    //    { 1280, 600, 5, bitrate, VID_CODEC_ID_H264 },
    //    { 800, 600, 60, bitrate, VID_CODEC_ID_H264 },
    //    { 1920, 1080, 25, bitrate, VID_CODEC_ID_H264 },
    //    { 1280, 720, 30, bitrate, VID_CODEC_ID_H264 },
    //};

    //for (size_t i = 0; i < 5; i++)
    //{
    //    QDateTime t = QDateTime::currentDateTime();
    //    QString filename = t.toString("yyyyMMdd_hhmmss");
    //    //QString fmt = vidParam[i].width + 'x' + vidParam[i].height + '_' + vidParam[i].fps;
    //    filename = "xcreen_" +  filename + ".mp4";

    //    record->startRecord(filename.toLocal8Bit(), vidParam[i]);
    //    _sleep(5000);

    //    record->stopRecord();

    //    _sleep(5000);
    //}
#endif
    

    //w.show();

    return a.exec();
}
