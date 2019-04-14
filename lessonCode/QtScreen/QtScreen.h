#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtScreen.h"

class QtScreen : public QWidget
{
    Q_OBJECT

public:
    QtScreen(QWidget *parent = Q_NULLPTR);
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    

private:
    Ui::QtScreenClass ui;
};
