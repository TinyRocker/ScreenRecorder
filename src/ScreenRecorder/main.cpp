#include "ScreenRecorder.h"
#include "glog/logging.h"

#ifdef _DEBUG
#pragma comment(lib, "glogd") 
#else
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenRecorder screen;
    screen.show();

    return a.exec();
}