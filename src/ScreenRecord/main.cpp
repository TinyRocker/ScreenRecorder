#include "ScreenRecord.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include "glog/logging.h"

#include "MediaRecord.h"

#ifdef _DEBUG
#pragma comment(lib, "glogd") 
#else
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

int main(int argc, char *argv[])
{
    google::InitGoogleLogging((const char*)argv[0]); // ³õÊ¼»¯glog
    google::SetLogDestination(google::GLOG_INFO, "./log/myinfo");
    FLAGS_logbufsecs = 0;
    // ²âÊÔMediaRecord
    MediaRecord *record = new MediaRecord();
    if (!record->init())
    {
        LOG(ERROR) << "MediaRecord init failed";
        return -1;
    }
    int vbitrate = 5 * 1024 * 1000; // 5 Mbps
    int abitrate = 2 * 128 * 1000;   // 5 Mbps

    VidRecParam vidParam = { 1920, 1080, 10, vbitrate, VID_CODEC_ID_H264 };
    AudRecParam audParam = { 2, 48000, abitrate, AUD_CODEC_ID_AAC };
    QDateTime t = QDateTime::currentDateTime();

    QString filename = t.toString("yyyyMMdd_hhmmss");
    //QString fmt = vidParam[i].width + 'x' + vidParam[i].height + '_' + vidParam[i].fps;
    filename = "./video/xcreen_" +  filename + ".mp4";

    record->startRecord(filename.toLocal8Bit(), vidParam, audParam);
    _sleep(20000);

    record->stopRecord();

    _sleep(2000);

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

    QApplication a(argc, argv);
    ScreenRecord w;
    //w.show();

    return a.exec();
}
