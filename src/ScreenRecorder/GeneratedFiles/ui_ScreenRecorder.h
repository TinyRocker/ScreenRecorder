/********************************************************************************
** Form generated from reading UI file 'ScreenRecorder.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENRECORDER_H
#define UI_SCREENRECORDER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenRecorderClass
{
public:
    QWidget *background;
    QPushButton *miniButton;
    QPushButton *closeButton;
    QWidget *context;
    QPushButton *record;
    QLineEdit *lineEdit_fileSavePath;
    QCheckBox *checkAudio;
    QCheckBox *checkVideo;
    QLabel *label_fileSavePath;
    QLineEdit *lineEdit_width;
    QLineEdit *lineEdit_height;
    QLineEdit *lineEdit_fps;
    QLabel *label_fps;
    QLabel *label_width;
    QLabel *label_height;
    QLineEdit *lineEdit_channels;
    QLineEdit *lineEdit_samplerate;
    QLabel *label_channels;
    QLabel *label_samplerate;
    QLabel *label_recordingVideo;
    QLabel *label_recordingAudio;
    QLabel *label_time;
    QRadioButton *radioButton_dx9;
    QRadioButton *radioButton_qt;
    QLabel *label_videoMode;
    QComboBox *comboBox_logLevel;
    QLineEdit *lineEdit_logSavePath;
    QLabel *label_logSavePath;
    QLabel *label_fileSavePrefix;
    QLabel *label_logSavePrefix;
    QLineEdit *lineEdit_fileSavePrefix;
    QLineEdit *lineEdit_logSavePrefix;
    QLabel *label_logLevel;
    QLabel *title;
    QLabel *logo;

    void setupUi(QWidget *ScreenRecorderClass)
    {
        if (ScreenRecorderClass->objectName().isEmpty())
            ScreenRecorderClass->setObjectName(QString::fromUtf8("ScreenRecorderClass"));
        ScreenRecorderClass->resize(600, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ScreenRecorder/Resources/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ScreenRecorderClass->setWindowIcon(icon);
        ScreenRecorderClass->setStyleSheet(QString::fromUtf8("#background\n"
"{\n"
"background-color: rgb(80, 80, 80);\n"
"border-radius:15px\n"
"}\n"
"#context\n"
"{\n"
"background-color: rgb(100, 100, 100);\n"
"}\n"
"#title\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"	font: 11pt \"Arial\";\n"
"}"));
        background = new QWidget(ScreenRecorderClass);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 600, 300));
        background->setStyleSheet(QString::fromUtf8(""));
        miniButton = new QPushButton(background);
        miniButton->setObjectName(QString::fromUtf8("miniButton"));
        miniButton->setGeometry(QRect(525, 10, 24, 24));
        miniButton->setStyleSheet(QString::fromUtf8("QPushButton:pressed\n"
"{\n"
"background-color: rgba(255, 255, 255, 0);\n"
"}\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ScreenRecorder/Resources/mini_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        miniButton->setIcon(icon1);
        miniButton->setIconSize(QSize(24, 24));
        miniButton->setFlat(true);
        closeButton = new QPushButton(background);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(560, 10, 24, 24));
        closeButton->setStyleSheet(QString::fromUtf8("QPushButton:pressed\n"
"{\n"
"background-color: rgba(255, 255, 255, 0);\n"
"}\n"
""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ScreenRecorder/Resources/close_24px.ico"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon2);
        closeButton->setIconSize(QSize(24, 24));
        closeButton->setFlat(true);
        context = new QWidget(background);
        context->setObjectName(QString::fromUtf8("context"));
        context->setGeometry(QRect(0, 40, 600, 260));
        context->setStyleSheet(QString::fromUtf8("#label_fileSavePath\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_fileSavePath\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_logSavePath\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_logSavePath\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_fileSavePrefix\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_logSavePrefix\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_fileSavePrefix\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_logSavePrefix\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_logLevel\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#checkAudio\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#checkVideo\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_channels\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"\n"
"#label_fps\n"
"{\n"
"color: rgb(255,"
                        " 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_height\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_samplerate\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_setAudio\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_setVideo\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_width\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#radioButton_dx9\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#radioButton_qt\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#label_videoMode\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_width\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_height\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_fps\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_channels\n"
"{\n"
"font: 11pt \"Arial\";\n"
"}\n"
"#lineEdit_samplerate\n"
"{\n"
"f"
                        "ont: 11pt \"Arial\";\n"
"}"));
        record = new QPushButton(context);
        record->setObjectName(QString::fromUtf8("record"));
        record->setGeometry(QRect(390, 130, 72, 72));
        record->setStyleSheet(QString::fromUtf8("QPushButton:!hover\n"
"{\n"
"border-image: url(:/ScreenRecorder/Resources/start_72px.ico);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"border-image: url(:/ScreenRecorder/Resources/start_72px.ico);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"background-color: rgba(255, 255, 255, 0);\n"
"}\n"
""));
        record->setIconSize(QSize(72, 72));
        record->setAutoDefault(false);
        record->setFlat(true);
        lineEdit_fileSavePath = new QLineEdit(context);
        lineEdit_fileSavePath->setObjectName(QString::fromUtf8("lineEdit_fileSavePath"));
        lineEdit_fileSavePath->setGeometry(QRect(120, 10, 261, 30));
        lineEdit_fileSavePath->setStyleSheet(QString::fromUtf8(""));
        checkAudio = new QCheckBox(context);
        checkAudio->setObjectName(QString::fromUtf8("checkAudio"));
        checkAudio->setGeometry(QRect(10, 190, 80, 20));
        checkAudio->setStyleSheet(QString::fromUtf8(""));
        checkAudio->setChecked(true);
        checkVideo = new QCheckBox(context);
        checkVideo->setObjectName(QString::fromUtf8("checkVideo"));
        checkVideo->setGeometry(QRect(10, 90, 80, 20));
        checkVideo->setStyleSheet(QString::fromUtf8(""));
        checkVideo->setChecked(true);
        label_fileSavePath = new QLabel(context);
        label_fileSavePath->setObjectName(QString::fromUtf8("label_fileSavePath"));
        label_fileSavePath->setGeometry(QRect(10, 10, 100, 30));
        label_fileSavePath->setStyleSheet(QString::fromUtf8(""));
        lineEdit_width = new QLineEdit(context);
        lineEdit_width->setObjectName(QString::fromUtf8("lineEdit_width"));
        lineEdit_width->setGeometry(QRect(60, 115, 40, 20));
        lineEdit_width->setStyleSheet(QString::fromUtf8(""));
        lineEdit_height = new QLineEdit(context);
        lineEdit_height->setObjectName(QString::fromUtf8("lineEdit_height"));
        lineEdit_height->setGeometry(QRect(180, 115, 40, 20));
        lineEdit_height->setStyleSheet(QString::fromUtf8(""));
        lineEdit_fps = new QLineEdit(context);
        lineEdit_fps->setObjectName(QString::fromUtf8("lineEdit_fps"));
        lineEdit_fps->setGeometry(QRect(280, 115, 40, 20));
        lineEdit_fps->setStyleSheet(QString::fromUtf8(""));
        label_fps = new QLabel(context);
        label_fps->setObjectName(QString::fromUtf8("label_fps"));
        label_fps->setGeometry(QRect(249, 115, 40, 20));
        label_width = new QLabel(context);
        label_width->setObjectName(QString::fromUtf8("label_width"));
        label_width->setGeometry(QRect(10, 115, 40, 20));
        label_height = new QLabel(context);
        label_height->setObjectName(QString::fromUtf8("label_height"));
        label_height->setGeometry(QRect(120, 115, 40, 20));
        lineEdit_channels = new QLineEdit(context);
        lineEdit_channels->setObjectName(QString::fromUtf8("lineEdit_channels"));
        lineEdit_channels->setGeometry(QRect(80, 220, 40, 20));
        lineEdit_channels->setStyleSheet(QString::fromUtf8(""));
        lineEdit_samplerate = new QLineEdit(context);
        lineEdit_samplerate->setObjectName(QString::fromUtf8("lineEdit_samplerate"));
        lineEdit_samplerate->setGeometry(QRect(220, 220, 60, 20));
        lineEdit_samplerate->setStyleSheet(QString::fromUtf8(""));
        label_channels = new QLabel(context);
        label_channels->setObjectName(QString::fromUtf8("label_channels"));
        label_channels->setGeometry(QRect(10, 220, 60, 20));
        label_samplerate = new QLabel(context);
        label_samplerate->setObjectName(QString::fromUtf8("label_samplerate"));
        label_samplerate->setGeometry(QRect(130, 220, 81, 20));
        label_recordingVideo = new QLabel(context);
        label_recordingVideo->setObjectName(QString::fromUtf8("label_recordingVideo"));
        label_recordingVideo->setGeometry(QRect(340, 215, 32, 32));
        label_recordingVideo->setScaledContents(false);
        label_recordingAudio = new QLabel(context);
        label_recordingAudio->setObjectName(QString::fromUtf8("label_recordingAudio"));
        label_recordingAudio->setGeometry(QRect(380, 215, 32, 32));
        label_time = new QLabel(context);
        label_time->setObjectName(QString::fromUtf8("label_time"));
        label_time->setGeometry(QRect(440, 215, 130, 32));
        label_time->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 24pt \"Arial\";"));
        radioButton_dx9 = new QRadioButton(context);
        radioButton_dx9->setObjectName(QString::fromUtf8("radioButton_dx9"));
        radioButton_dx9->setGeometry(QRect(120, 145, 80, 20));
        radioButton_dx9->setChecked(true);
        radioButton_qt = new QRadioButton(context);
        radioButton_qt->setObjectName(QString::fromUtf8("radioButton_qt"));
        radioButton_qt->setGeometry(QRect(210, 145, 50, 20));
        radioButton_qt->setChecked(false);
        label_videoMode = new QLabel(context);
        label_videoMode->setObjectName(QString::fromUtf8("label_videoMode"));
        label_videoMode->setGeometry(QRect(10, 145, 90, 20));
        label_videoMode->setStyleSheet(QString::fromUtf8(""));
        comboBox_logLevel = new QComboBox(context);
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->setObjectName(QString::fromUtf8("comboBox_logLevel"));
        comboBox_logLevel->setEnabled(true);
        comboBox_logLevel->setGeometry(QRect(460, 90, 71, 30));
        comboBox_logLevel->setMinimumSize(QSize(0, 20));
        comboBox_logLevel->setEditable(false);
        comboBox_logLevel->setModelColumn(0);
        lineEdit_logSavePath = new QLineEdit(context);
        lineEdit_logSavePath->setObjectName(QString::fromUtf8("lineEdit_logSavePath"));
        lineEdit_logSavePath->setGeometry(QRect(120, 50, 261, 30));
        lineEdit_logSavePath->setStyleSheet(QString::fromUtf8(""));
        label_logSavePath = new QLabel(context);
        label_logSavePath->setObjectName(QString::fromUtf8("label_logSavePath"));
        label_logSavePath->setGeometry(QRect(10, 50, 100, 30));
        label_logSavePath->setStyleSheet(QString::fromUtf8(""));
        label_fileSavePrefix = new QLabel(context);
        label_fileSavePrefix->setObjectName(QString::fromUtf8("label_fileSavePrefix"));
        label_fileSavePrefix->setGeometry(QRect(390, 10, 60, 30));
        label_fileSavePrefix->setStyleSheet(QString::fromUtf8(""));
        label_logSavePrefix = new QLabel(context);
        label_logSavePrefix->setObjectName(QString::fromUtf8("label_logSavePrefix"));
        label_logSavePrefix->setGeometry(QRect(390, 50, 60, 30));
        label_logSavePrefix->setStyleSheet(QString::fromUtf8(""));
        lineEdit_fileSavePrefix = new QLineEdit(context);
        lineEdit_fileSavePrefix->setObjectName(QString::fromUtf8("lineEdit_fileSavePrefix"));
        lineEdit_fileSavePrefix->setGeometry(QRect(459, 10, 121, 30));
        lineEdit_fileSavePrefix->setStyleSheet(QString::fromUtf8(""));
        lineEdit_logSavePrefix = new QLineEdit(context);
        lineEdit_logSavePrefix->setObjectName(QString::fromUtf8("lineEdit_logSavePrefix"));
        lineEdit_logSavePrefix->setGeometry(QRect(459, 50, 121, 30));
        lineEdit_logSavePrefix->setStyleSheet(QString::fromUtf8(""));
        label_logLevel = new QLabel(context);
        label_logLevel->setObjectName(QString::fromUtf8("label_logLevel"));
        label_logLevel->setGeometry(QRect(390, 90, 60, 30));
        label_logLevel->setStyleSheet(QString::fromUtf8(""));
        label_recordingAudio->raise();
        label_recordingVideo->raise();
        record->raise();
        lineEdit_fileSavePath->raise();
        checkAudio->raise();
        checkVideo->raise();
        label_fileSavePath->raise();
        lineEdit_width->raise();
        lineEdit_height->raise();
        lineEdit_fps->raise();
        label_fps->raise();
        label_width->raise();
        label_height->raise();
        lineEdit_channels->raise();
        lineEdit_samplerate->raise();
        label_channels->raise();
        label_samplerate->raise();
        label_time->raise();
        radioButton_dx9->raise();
        radioButton_qt->raise();
        label_videoMode->raise();
        comboBox_logLevel->raise();
        lineEdit_logSavePath->raise();
        label_logSavePath->raise();
        label_fileSavePrefix->raise();
        label_logSavePrefix->raise();
        lineEdit_fileSavePrefix->raise();
        lineEdit_logSavePrefix->raise();
        label_logLevel->raise();
        title = new QLabel(background);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(50, 10, 141, 21));
        logo = new QLabel(background);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(8, 8, 28, 28));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/ScreenRecorder/Resources/logo.ico")));
        logo->setScaledContents(true);

        retranslateUi(ScreenRecorderClass);
        QObject::connect(closeButton, SIGNAL(clicked()), ScreenRecorderClass, SLOT(close()));
        QObject::connect(miniButton, SIGNAL(clicked()), ScreenRecorderClass, SLOT(showMinimized()));
        QObject::connect(record, SIGNAL(clicked()), ScreenRecorderClass, SLOT(record()));
        QObject::connect(checkVideo, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkVideoIsRecord()));
        QObject::connect(checkAudio, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkAudioIsRecord()));
        QObject::connect(comboBox_logLevel, SIGNAL(currentTextChanged(QString)), ScreenRecorderClass, SLOT(setLogLevel()));

        record->setDefault(false);
        comboBox_logLevel->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(ScreenRecorderClass);
    } // setupUi

    void retranslateUi(QWidget *ScreenRecorderClass)
    {
        ScreenRecorderClass->setWindowTitle(QApplication::translate("ScreenRecorderClass", "ScreenRecorder", nullptr));
        miniButton->setText(QString());
        closeButton->setText(QString());
        record->setText(QString());
        lineEdit_fileSavePath->setText(QApplication::translate("ScreenRecorderClass", "./video/", nullptr));
        checkAudio->setText(QApplication::translate("ScreenRecorderClass", "\345\275\225\345\210\266\351\237\263\351\242\221", nullptr));
        checkVideo->setText(QApplication::translate("ScreenRecorderClass", "\345\275\225\345\210\266\350\247\206\351\242\221", nullptr));
        label_fileSavePath->setText(QApplication::translate("ScreenRecorderClass", "\346\226\207\344\273\266\344\277\235\345\255\230\350\267\257\345\276\204\357\274\232", nullptr));
        lineEdit_width->setText(QApplication::translate("ScreenRecorderClass", "1920", nullptr));
        lineEdit_height->setText(QApplication::translate("ScreenRecorderClass", "1080", nullptr));
        lineEdit_fps->setText(QApplication::translate("ScreenRecorderClass", "25", nullptr));
        label_fps->setText(QApplication::translate("ScreenRecorderClass", "fps", nullptr));
        label_width->setText(QApplication::translate("ScreenRecorderClass", "width", nullptr));
        label_height->setText(QApplication::translate("ScreenRecorderClass", "height", nullptr));
        lineEdit_channels->setText(QApplication::translate("ScreenRecorderClass", "2", nullptr));
        lineEdit_samplerate->setText(QApplication::translate("ScreenRecorderClass", "44100", nullptr));
        label_channels->setText(QApplication::translate("ScreenRecorderClass", "channels", nullptr));
        label_samplerate->setText(QApplication::translate("ScreenRecorderClass", "samplerate", nullptr));
        label_recordingVideo->setText(QString());
        label_recordingAudio->setText(QString());
        label_time->setText(QApplication::translate("ScreenRecorderClass", "00:00:00", nullptr));
        radioButton_dx9->setText(QApplication::translate("ScreenRecorderClass", "Directx9", nullptr));
        radioButton_qt->setText(QApplication::translate("ScreenRecorderClass", "Qt", nullptr));
        label_videoMode->setText(QApplication::translate("ScreenRecorderClass", "\350\247\206\351\242\221\345\275\225\345\210\266\346\250\241\345\274\217", nullptr));
        comboBox_logLevel->setItemText(0, QApplication::translate("ScreenRecorderClass", "DETAIL", nullptr));
        comboBox_logLevel->setItemText(1, QApplication::translate("ScreenRecorderClass", "DEBUG", nullptr));
        comboBox_logLevel->setItemText(2, QApplication::translate("ScreenRecorderClass", "INFO", nullptr));
        comboBox_logLevel->setItemText(3, QApplication::translate("ScreenRecorderClass", "WARNING", nullptr));
        comboBox_logLevel->setItemText(4, QApplication::translate("ScreenRecorderClass", "ERROR", nullptr));
        comboBox_logLevel->setItemText(5, QApplication::translate("ScreenRecorderClass", "FATEL", nullptr));

        lineEdit_logSavePath->setText(QApplication::translate("ScreenRecorderClass", "./log/", nullptr));
        label_logSavePath->setText(QApplication::translate("ScreenRecorderClass", "\346\227\245\345\277\227\344\277\235\345\255\230\350\267\257\345\276\204\357\274\232", nullptr));
        label_fileSavePrefix->setText(QApplication::translate("ScreenRecorderClass", "\346\226\207\344\273\266\345\211\215\347\274\200\357\274\232", nullptr));
        label_logSavePrefix->setText(QApplication::translate("ScreenRecorderClass", "\346\227\245\345\277\227\345\211\215\347\274\200\357\274\232", nullptr));
        lineEdit_fileSavePrefix->setText(QApplication::translate("ScreenRecorderClass", "ScreenRecord_", nullptr));
        lineEdit_logSavePrefix->setText(QApplication::translate("ScreenRecorderClass", "LOG_", nullptr));
        label_logLevel->setText(QApplication::translate("ScreenRecorderClass", "\346\227\245\345\277\227\347\272\247\345\210\253\357\274\232", nullptr));
        title->setText(QApplication::translate("ScreenRecorderClass", "ScreenRecoder", nullptr));
        logo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ScreenRecorderClass: public Ui_ScreenRecorderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENRECORDER_H
