#include "ScreenRecord.h"
#include <QtWidgets/QApplication>

#include "glog/logging.h"

#ifdef _DEBUG
#pragma comment(lib, "glogd") 
#else
#pragma comment(lib, "glog.lib")
#endif // _DEBUG



int main(int argc, char *argv[])
{
    google::InitGoogleLogging((const char*)argv[0]); // ≥ı ºªØglog
    google::SetLogDestination(google::GLOG_INFO, "./myinfo");

    QApplication a(argc, argv);
    ScreenRecord w;
    w.show();

    return a.exec();
}
