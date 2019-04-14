#include "ScreenRecord.h"

#include "VideoCapture.h"

#include <QPainter>
#include <QTime>

static VideoCapture *ct = nullptr;

ScreenRecord::ScreenRecord(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    ct = new VideoCapture();
    //ct->init(VideoCapture::VID_CAP_MODE_DIRECTX);
    ct->init(VideoCapture::VID_CAP_MODE_DIRECTX);
    ct->startCapture();

    // 测试代码
    startTimer(100);
}

// qt定时器是信号槽实现，不能做高复杂时间度的操作
void ScreenRecord::timerEvent(QTimerEvent *event)
{
    update();
}

void ScreenRecord::paintEvent(QPaintEvent *event)
{
    uchar *data = (uchar*)ct->getData();
    if (!data) return;
    QImage image(data, ct->width(), ct->height(), QImage::Format_ARGB32);
    
    // 绘制截屏
    QPainter p;
    p.begin(this);
    p.drawImage(QPoint(), image);
    p.end();
    delete data;
}