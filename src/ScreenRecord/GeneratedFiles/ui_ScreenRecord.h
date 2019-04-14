/********************************************************************************
** Form generated from reading UI file 'ScreenRecord.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENRECORD_H
#define UI_SCREENRECORD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenRecordClass
{
public:

    void setupUi(QWidget *ScreenRecordClass)
    {
        if (ScreenRecordClass->objectName().isEmpty())
            ScreenRecordClass->setObjectName(QString::fromUtf8("ScreenRecordClass"));
        ScreenRecordClass->resize(600, 400);

        retranslateUi(ScreenRecordClass);

        QMetaObject::connectSlotsByName(ScreenRecordClass);
    } // setupUi

    void retranslateUi(QWidget *ScreenRecordClass)
    {
        ScreenRecordClass->setWindowTitle(QApplication::translate("ScreenRecordClass", "ScreenRecord", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenRecordClass: public Ui_ScreenRecordClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENRECORD_H
