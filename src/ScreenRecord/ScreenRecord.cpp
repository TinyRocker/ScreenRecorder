#include "ScreenRecord.h"

#include "VideoCapture.h"

#include <QPainter>
#include <QTime>

static VideoCapture *ct = nullptr;
static uchar *videodata = nullptr;

ScreenRecord::ScreenRecord(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    int width = 2560;
    int height = 1440;
    int vbitrate = 5 * 1024 * 1000; // 5 Mbps
    int abitrate = 2 * 128 * 1000;   // 2 Mbps

    VidRecordParam vidrec = { width, height, 20, 4, VID_CAP_MODE_DIRECTX };
    AudRecordParam audrec = { 2, 48000, 16 };

    VidSwsParam vidsws = { 1920, 1080, vbitrate, VID_CODEC_ID_H264 };
    AudSwrParam audswr = { 2, 48000 , abitrate, AUD_CODEC_ID_AAC };


    ct = new VideoCapture();
    ct->init(vidrec);
    ct->startCapture();

    // 测试代码
    startTimer(30);
}

// qt定时器是信号槽实现，不能做高复杂时间度的操作
void ScreenRecord::timerEvent(QTimerEvent *event)
{
    videodata = (uchar*)ct->getData();
    if (videodata)
    {
        update();
    }
}

void ScreenRecord::paintEvent(QPaintEvent *event)
{
    //uchar *data = (uchar*)ct->getData();
    //if (!data) return;
    QImage image(videodata, ct->width(), ct->height(), QImage::Format_ARGB32);

    QPainter p;
    p.begin(this);
    p.drawImage(QPoint(), image);
    p.end();
    ct->freeData(videodata);
}