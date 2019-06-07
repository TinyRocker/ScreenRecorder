#include "ScreenRecorder.h"
#include "glog/logging.h"

extern "C"
{
#include "ffmpeg/libavformat/avformat.h"
}

#ifdef _DEBUG
#pragma comment(lib, "glogd") 
#else
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")

int main(int argc, char *argv[])
{
    //注册所有网络协议
    //avformat_network_init();

    QApplication a(argc, argv);
    ScreenRecorder screen;
    screen.show();

    return a.exec();
}