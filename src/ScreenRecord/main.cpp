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
    google::SetLogDestination(google::GLOG_INFO, "./myinfo");

    // ²âÊÔMediaRecord
    QDateTime t = QDateTime::currentDateTime();
    QString filename = t.toString("yyyyMMdd_hhmmss");
    filename = "xcreen_" + filename + ".mp4";
    MediaRecord *record = new MediaRecord();
    record->startRecord(filename.toLocal8Bit());
    
    getchar();

    record->stopRecord();

    QApplication a(argc, argv);
    ScreenRecord w;
    w.show();

    return a.exec();
}
