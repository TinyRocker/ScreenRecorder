/********************************************************************************
** Form generated from reading UI file 'QtScreen.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSCREEN_H
#define UI_QTSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtScreenClass
{
public:

    void setupUi(QWidget *QtScreenClass)
    {
        if (QtScreenClass->objectName().isEmpty())
            QtScreenClass->setObjectName(QString::fromUtf8("QtScreenClass"));
        QtScreenClass->resize(600, 400);

        retranslateUi(QtScreenClass);

        QMetaObject::connectSlotsByName(QtScreenClass);
    } // setupUi

    void retranslateUi(QWidget *QtScreenClass)
    {
        QtScreenClass->setWindowTitle(QApplication::translate("QtScreenClass", "QtScreen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtScreenClass: public Ui_QtScreenClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSCREEN_H
