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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenRecorderClass
{
public:
    QWidget *background;
    QWidget *title;
    QLabel *logo;
    QLabel *label_title;
    QPushButton *miniButton;
    QPushButton *closeButton;
    QPushButton *record;
    QLabel *label_time;
    QLabel *label_recordingAudio;
    QLabel *label_recordingVideo;
    QWidget *context;
    QLineEdit *lineEdit_fileSavePath;
    QCheckBox *checkAudio;
    QCheckBox *checkVideo;
    QLabel *label_fileSavePath;
    QLineEdit *lineEdit_width;
    QLineEdit *lineEdit_height;
    QLineEdit *lineEdit_fps;
    QLabel *label_fps;
    QLabel *label_width;
    QLineEdit *lineEdit_channels;
    QLineEdit *lineEdit_samplerate;
    QLabel *label_channels;
    QLabel *label_samplerate;
    QLabel *label_captureMode;
    QComboBox *comboBox_logLevel;
    QLineEdit *lineEdit_logSavePath;
    QLabel *label_logSavePath;
    QLabel *label_fileSavePrefix;
    QLineEdit *lineEdit_fileSavePrefix;
    QLabel *label_logLevel;
    QComboBox *comboBox_bitrate_video;
    QComboBox *comboBox_bitrate_audio;
    QLabel *label_bitrate_video;
    QLabel *label_bitrate_audio;
    QLabel *label_videoKbps;
    QLabel *label_audioKbps;
    QComboBox *comboBox_captureMode;
    QCheckBox *checkFile;
    QCheckBox *checkLog;
    QCheckBox *checkRtmp;
    QLineEdit *lineEdit_rtmpPath;
    QLabel *label_rtmpPath;
    QComboBox *comboBox_screen;

    void setupUi(QWidget *ScreenRecorderClass)
    {
        if (ScreenRecorderClass->objectName().isEmpty())
            ScreenRecorderClass->setObjectName(QString::fromUtf8("ScreenRecorderClass"));
        ScreenRecorderClass->resize(600, 200);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ScreenRecorder/Resources/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ScreenRecorderClass->setWindowIcon(icon);
        ScreenRecorderClass->setStyleSheet(QString::fromUtf8("#title\n"
"{\n"
"background-color: rgb(70, 70, 70);\n"
"}\n"
"#context\n"
"{\n"
"background-color: rgb(100, 100, 100);\n"
"}"));
        background = new QWidget(ScreenRecorderClass);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 600, 200));
        background->setStyleSheet(QString::fromUtf8(""));
        title = new QWidget(background);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(0, 0, 600, 40));
        title->setStyleSheet(QString::fromUtf8("#label_title\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"	font: 11pt \"Arial\";\n"
"}"));
        logo = new QLabel(title);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(10, 6, 28, 28));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/ScreenRecorder/Resources/logo.ico")));
        logo->setScaledContents(true);
        label_title = new QLabel(title);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(50, 10, 110, 20));
        miniButton = new QPushButton(title);
        miniButton->setObjectName(QString::fromUtf8("miniButton"));
        miniButton->setGeometry(QRect(530, 8, 24, 24));
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
        closeButton = new QPushButton(title);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(560, 8, 24, 24));
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
        record = new QPushButton(title);
        record->setObjectName(QString::fromUtf8("record"));
        record->setGeometry(QRect(320, 4, 32, 32));
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
        label_time = new QLabel(title);
        label_time->setObjectName(QString::fromUtf8("label_time"));
        label_time->setGeometry(QRect(370, 10, 110, 24));
        label_time->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 20pt \"Arial\";"));
        label_recordingAudio = new QLabel(title);
        label_recordingAudio->setObjectName(QString::fromUtf8("label_recordingAudio"));
        label_recordingAudio->setGeometry(QRect(265, 8, 24, 24));
        label_recordingVideo = new QLabel(title);
        label_recordingVideo->setObjectName(QString::fromUtf8("label_recordingVideo"));
        label_recordingVideo->setGeometry(QRect(230, 8, 24, 24));
        label_recordingVideo->setScaledContents(false);
        context = new QWidget(background);
        context->setObjectName(QString::fromUtf8("context"));
        context->setGeometry(QRect(0, 40, 600, 200));
        context->setStyleSheet(QString::fromUtf8("#label_fileSavePath\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#lineEdit_fileSavePath\n"
"{\n"
"}\n"
"#label_logSavePath\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#lineEdit_logSavePath\n"
"{\n"
"}\n"
"#label_fileSavePrefix\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_logSavePrefix\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#lineEdit_fileSavePrefix\n"
"{\n"
"}\n"
"#lineEdit_logSavePrefix\n"
"{\n"
"}\n"
"#label_logLevel\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#checkAudio\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#checkVideo\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_channels\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"#label_fps\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_height\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_samplerate\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_setAudio\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_setVideo\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_width\n"
"{\n"
"color: rgb(255,"
                        " 255, 255);\n"
"}\n"
"#radioButton_dx9\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#radioButton_qt\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_videoMode\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#lineEdit_width\n"
"{\n"
"}\n"
"#lineEdit_height\n"
"{\n"
"}\n"
"#lineEdit_fps\n"
"{\n"
"}\n"
"#lineEdit_channels\n"
"{\n"
"}\n"
"#lineEdit_samplerate\n"
"{\n"
"}\n"
"#label_bitrate_video\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_bitrate_audio\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#checkLog\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#checkFile\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#checkRtmp\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_captureMode\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}\n"
"#label_rtmpPath\n"
"{\n"
"color: rgb(255, 255, 255);\n"
"}"));
        lineEdit_fileSavePath = new QLineEdit(context);
        lineEdit_fileSavePath->setObjectName(QString::fromUtf8("lineEdit_fileSavePath"));
        lineEdit_fileSavePath->setGeometry(QRect(180, 100, 200, 20));
        lineEdit_fileSavePath->setStyleSheet(QString::fromUtf8(""));
        checkAudio = new QCheckBox(context);
        checkAudio->setObjectName(QString::fromUtf8("checkAudio"));
        checkAudio->setGeometry(QRect(10, 40, 70, 20));
        checkAudio->setStyleSheet(QString::fromUtf8(""));
        checkAudio->setChecked(true);
        checkVideo = new QCheckBox(context);
        checkVideo->setObjectName(QString::fromUtf8("checkVideo"));
        checkVideo->setGeometry(QRect(10, 10, 70, 20));
        checkVideo->setStyleSheet(QString::fromUtf8(""));
        checkVideo->setChecked(true);
        label_fileSavePath = new QLabel(context);
        label_fileSavePath->setObjectName(QString::fromUtf8("label_fileSavePath"));
        label_fileSavePath->setGeometry(QRect(95, 100, 80, 20));
        label_fileSavePath->setStyleSheet(QString::fromUtf8(""));
        lineEdit_width = new QLineEdit(context);
        lineEdit_width->setObjectName(QString::fromUtf8("lineEdit_width"));
        lineEdit_width->setGeometry(QRect(140, 10, 30, 20));
        lineEdit_width->setStyleSheet(QString::fromUtf8(""));
        lineEdit_height = new QLineEdit(context);
        lineEdit_height->setObjectName(QString::fromUtf8("lineEdit_height"));
        lineEdit_height->setGeometry(QRect(175, 10, 30, 20));
        lineEdit_height->setStyleSheet(QString::fromUtf8(""));
        lineEdit_fps = new QLineEdit(context);
        lineEdit_fps->setObjectName(QString::fromUtf8("lineEdit_fps"));
        lineEdit_fps->setGeometry(QRect(260, 10, 40, 20));
        lineEdit_fps->setStyleSheet(QString::fromUtf8(""));
        label_fps = new QLabel(context);
        label_fps->setObjectName(QString::fromUtf8("label_fps"));
        label_fps->setGeometry(QRect(220, 10, 35, 20));
        label_width = new QLabel(context);
        label_width->setObjectName(QString::fromUtf8("label_width"));
        label_width->setGeometry(QRect(95, 10, 40, 20));
        lineEdit_channels = new QLineEdit(context);
        lineEdit_channels->setObjectName(QString::fromUtf8("lineEdit_channels"));
        lineEdit_channels->setGeometry(QRect(140, 40, 65, 20));
        lineEdit_channels->setStyleSheet(QString::fromUtf8(""));
        lineEdit_samplerate = new QLineEdit(context);
        lineEdit_samplerate->setObjectName(QString::fromUtf8("lineEdit_samplerate"));
        lineEdit_samplerate->setGeometry(QRect(260, 40, 40, 20));
        lineEdit_samplerate->setStyleSheet(QString::fromUtf8(""));
        label_channels = new QLabel(context);
        label_channels->setObjectName(QString::fromUtf8("label_channels"));
        label_channels->setGeometry(QRect(95, 40, 40, 20));
        label_samplerate = new QLabel(context);
        label_samplerate->setObjectName(QString::fromUtf8("label_samplerate"));
        label_samplerate->setGeometry(QRect(220, 40, 35, 20));
        label_captureMode = new QLabel(context);
        label_captureMode->setObjectName(QString::fromUtf8("label_captureMode"));
        label_captureMode->setGeometry(QRect(430, 10, 31, 20));
        label_captureMode->setStyleSheet(QString::fromUtf8(""));
        comboBox_logLevel = new QComboBox(context);
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->addItem(QString());
        comboBox_logLevel->setObjectName(QString::fromUtf8("comboBox_logLevel"));
        comboBox_logLevel->setEnabled(true);
        comboBox_logLevel->setGeometry(QRect(450, 70, 61, 20));
        comboBox_logLevel->setMinimumSize(QSize(0, 20));
        comboBox_logLevel->setEditable(false);
        comboBox_logLevel->setModelColumn(0);
        lineEdit_logSavePath = new QLineEdit(context);
        lineEdit_logSavePath->setObjectName(QString::fromUtf8("lineEdit_logSavePath"));
        lineEdit_logSavePath->setGeometry(QRect(180, 70, 200, 20));
        lineEdit_logSavePath->setStyleSheet(QString::fromUtf8(""));
        label_logSavePath = new QLabel(context);
        label_logSavePath->setObjectName(QString::fromUtf8("label_logSavePath"));
        label_logSavePath->setGeometry(QRect(95, 70, 80, 20));
        label_logSavePath->setStyleSheet(QString::fromUtf8(""));
        label_fileSavePrefix = new QLabel(context);
        label_fileSavePrefix->setObjectName(QString::fromUtf8("label_fileSavePrefix"));
        label_fileSavePrefix->setGeometry(QRect(390, 100, 60, 20));
        label_fileSavePrefix->setStyleSheet(QString::fromUtf8(""));
        lineEdit_fileSavePrefix = new QLineEdit(context);
        lineEdit_fileSavePrefix->setObjectName(QString::fromUtf8("lineEdit_fileSavePrefix"));
        lineEdit_fileSavePrefix->setGeometry(QRect(450, 100, 121, 20));
        lineEdit_fileSavePrefix->setStyleSheet(QString::fromUtf8(""));
        label_logLevel = new QLabel(context);
        label_logLevel->setObjectName(QString::fromUtf8("label_logLevel"));
        label_logLevel->setGeometry(QRect(390, 70, 60, 20));
        label_logLevel->setStyleSheet(QString::fromUtf8(""));
        comboBox_bitrate_video = new QComboBox(context);
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->addItem(QString());
        comboBox_bitrate_video->setObjectName(QString::fromUtf8("comboBox_bitrate_video"));
        comboBox_bitrate_video->setEnabled(true);
        comboBox_bitrate_video->setGeometry(QRect(340, 10, 50, 20));
        comboBox_bitrate_video->setMinimumSize(QSize(0, 20));
        comboBox_bitrate_video->setEditable(false);
        comboBox_bitrate_video->setModelColumn(0);
        comboBox_bitrate_audio = new QComboBox(context);
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->addItem(QString());
        comboBox_bitrate_audio->setObjectName(QString::fromUtf8("comboBox_bitrate_audio"));
        comboBox_bitrate_audio->setEnabled(true);
        comboBox_bitrate_audio->setGeometry(QRect(340, 40, 50, 20));
        comboBox_bitrate_audio->setMinimumSize(QSize(0, 20));
        comboBox_bitrate_audio->setEditable(false);
        comboBox_bitrate_audio->setModelColumn(0);
        label_bitrate_video = new QLabel(context);
        label_bitrate_video->setObjectName(QString::fromUtf8("label_bitrate_video"));
        label_bitrate_video->setGeometry(QRect(310, 10, 30, 20));
        label_bitrate_video->setStyleSheet(QString::fromUtf8(""));
        label_bitrate_audio = new QLabel(context);
        label_bitrate_audio->setObjectName(QString::fromUtf8("label_bitrate_audio"));
        label_bitrate_audio->setGeometry(QRect(310, 40, 30, 20));
        label_bitrate_audio->setStyleSheet(QString::fromUtf8(""));
        label_videoKbps = new QLabel(context);
        label_videoKbps->setObjectName(QString::fromUtf8("label_videoKbps"));
        label_videoKbps->setGeometry(QRect(400, 10, 30, 20));
        label_videoKbps->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_audioKbps = new QLabel(context);
        label_audioKbps->setObjectName(QString::fromUtf8("label_audioKbps"));
        label_audioKbps->setGeometry(QRect(400, 40, 30, 20));
        label_audioKbps->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        comboBox_captureMode = new QComboBox(context);
        comboBox_captureMode->addItem(QString());
        comboBox_captureMode->addItem(QString());
        comboBox_captureMode->addItem(QString());
        comboBox_captureMode->setObjectName(QString::fromUtf8("comboBox_captureMode"));
        comboBox_captureMode->setEnabled(true);
        comboBox_captureMode->setGeometry(QRect(460, 10, 51, 20));
        comboBox_captureMode->setMinimumSize(QSize(0, 20));
        comboBox_captureMode->setEditable(false);
        comboBox_captureMode->setModelColumn(0);
        checkFile = new QCheckBox(context);
        checkFile->setObjectName(QString::fromUtf8("checkFile"));
        checkFile->setGeometry(QRect(9, 100, 70, 20));
        checkFile->setStyleSheet(QString::fromUtf8(""));
        checkFile->setChecked(true);
        checkLog = new QCheckBox(context);
        checkLog->setObjectName(QString::fromUtf8("checkLog"));
        checkLog->setGeometry(QRect(10, 70, 70, 20));
        checkLog->setStyleSheet(QString::fromUtf8(""));
        checkLog->setChecked(true);
        checkRtmp = new QCheckBox(context);
        checkRtmp->setObjectName(QString::fromUtf8("checkRtmp"));
        checkRtmp->setGeometry(QRect(10, 130, 70, 20));
        checkRtmp->setStyleSheet(QString::fromUtf8(""));
        checkRtmp->setChecked(false);
        lineEdit_rtmpPath = new QLineEdit(context);
        lineEdit_rtmpPath->setObjectName(QString::fromUtf8("lineEdit_rtmpPath"));
        lineEdit_rtmpPath->setGeometry(QRect(180, 130, 391, 20));
        lineEdit_rtmpPath->setStyleSheet(QString::fromUtf8(""));
        label_rtmpPath = new QLabel(context);
        label_rtmpPath->setObjectName(QString::fromUtf8("label_rtmpPath"));
        label_rtmpPath->setGeometry(QRect(95, 130, 71, 20));
        label_rtmpPath->setStyleSheet(QString::fromUtf8(""));
        comboBox_screen = new QComboBox(context);
        comboBox_screen->addItem(QString());
        comboBox_screen->addItem(QString());
        comboBox_screen->addItem(QString());
        comboBox_screen->setObjectName(QString::fromUtf8("comboBox_screen"));
        comboBox_screen->setEnabled(true);
        comboBox_screen->setGeometry(QRect(520, 10, 71, 20));
        comboBox_screen->setMinimumSize(QSize(0, 20));
        comboBox_screen->setEditable(false);
        comboBox_screen->setModelColumn(0);
        context->raise();
        title->raise();

        retranslateUi(ScreenRecorderClass);
        QObject::connect(checkVideo, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkVideoIsRecord()));
        QObject::connect(miniButton, SIGNAL(clicked()), ScreenRecorderClass, SLOT(showMinimized()));
        QObject::connect(closeButton, SIGNAL(clicked()), ScreenRecorderClass, SLOT(close()));
        QObject::connect(record, SIGNAL(clicked()), ScreenRecorderClass, SLOT(record()));
        QObject::connect(comboBox_logLevel, SIGNAL(currentTextChanged(QString)), ScreenRecorderClass, SLOT(setLogLevel()));
        QObject::connect(checkAudio, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkAudioIsRecord()));
        QObject::connect(checkLog, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkLogIsRecord()));
        QObject::connect(checkFile, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkFileIsRecord()));
        QObject::connect(checkRtmp, SIGNAL(clicked()), ScreenRecorderClass, SLOT(checkRtmpIsRecord()));
        QObject::connect(comboBox_screen, SIGNAL(currentTextChanged(QString)), ScreenRecorderClass, SLOT(setVideoConfig()));

        record->setDefault(false);
        comboBox_logLevel->setCurrentIndex(2);
        comboBox_bitrate_video->setCurrentIndex(6);
        comboBox_bitrate_audio->setCurrentIndex(3);
        comboBox_captureMode->setCurrentIndex(0);
        comboBox_screen->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ScreenRecorderClass);
    } // setupUi

    void retranslateUi(QWidget *ScreenRecorderClass)
    {
        ScreenRecorderClass->setWindowTitle(QApplication::translate("ScreenRecorderClass", "ScreenRecorder", nullptr));
        logo->setText(QString());
        label_title->setText(QApplication::translate("ScreenRecorderClass", "ScreenRecoder", nullptr));
        miniButton->setText(QString());
        closeButton->setText(QString());
        record->setText(QString());
        label_time->setText(QApplication::translate("ScreenRecorderClass", "00:00:00", nullptr));
        label_recordingAudio->setText(QString());
        label_recordingVideo->setText(QString());
        lineEdit_fileSavePath->setText(QApplication::translate("ScreenRecorderClass", "./video/", nullptr));
        checkAudio->setText(QApplication::translate("ScreenRecorderClass", "\345\275\225\345\210\266\351\237\263\351\242\221", nullptr));
        checkVideo->setText(QApplication::translate("ScreenRecorderClass", "\345\275\225\345\210\266\350\247\206\351\242\221", nullptr));
        label_fileSavePath->setText(QApplication::translate("ScreenRecorderClass", "\346\226\207\344\273\266\344\277\235\345\255\230\350\267\257\345\276\204\357\274\232", nullptr));
        lineEdit_width->setText(QApplication::translate("ScreenRecorderClass", "1920", nullptr));
        lineEdit_height->setText(QApplication::translate("ScreenRecorderClass", "1080", nullptr));
        lineEdit_fps->setText(QApplication::translate("ScreenRecorderClass", "30", nullptr));
        label_fps->setText(QApplication::translate("ScreenRecorderClass", "\345\270\247\347\216\207", nullptr));
        label_width->setText(QApplication::translate("ScreenRecorderClass", "\345\210\206\350\276\250\347\216\207", nullptr));
        lineEdit_channels->setText(QApplication::translate("ScreenRecorderClass", "2", nullptr));
        lineEdit_samplerate->setText(QApplication::translate("ScreenRecorderClass", "44100", nullptr));
        label_channels->setText(QApplication::translate("ScreenRecorderClass", "\351\200\232\351\201\223\346\225\260", nullptr));
        label_samplerate->setText(QApplication::translate("ScreenRecorderClass", "\351\207\207\346\240\267\347\216\207", nullptr));
        label_captureMode->setText(QApplication::translate("ScreenRecorderClass", "\346\250\241\345\274\217", nullptr));
        comboBox_logLevel->setItemText(0, QApplication::translate("ScreenRecorderClass", "DETAIL", nullptr));
        comboBox_logLevel->setItemText(1, QApplication::translate("ScreenRecorderClass", "DEBUG", nullptr));
        comboBox_logLevel->setItemText(2, QApplication::translate("ScreenRecorderClass", "INFO", nullptr));
        comboBox_logLevel->setItemText(3, QApplication::translate("ScreenRecorderClass", "WARNING", nullptr));
        comboBox_logLevel->setItemText(4, QApplication::translate("ScreenRecorderClass", "ERROR", nullptr));
        comboBox_logLevel->setItemText(5, QApplication::translate("ScreenRecorderClass", "FATEL", nullptr));

        lineEdit_logSavePath->setText(QApplication::translate("ScreenRecorderClass", "./log/", nullptr));
        label_logSavePath->setText(QApplication::translate("ScreenRecorderClass", "\346\227\245\345\277\227\344\277\235\345\255\230\350\267\257\345\276\204\357\274\232", nullptr));
        label_fileSavePrefix->setText(QApplication::translate("ScreenRecorderClass", "\346\226\207\344\273\266\345\211\215\347\274\200\357\274\232", nullptr));
        lineEdit_fileSavePrefix->setText(QApplication::translate("ScreenRecorderClass", "ScreenRecord_", nullptr));
        label_logLevel->setText(QApplication::translate("ScreenRecorderClass", "\346\227\245\345\277\227\347\272\247\345\210\253\357\274\232", nullptr));
        comboBox_bitrate_video->setItemText(0, QApplication::translate("ScreenRecorderClass", "256", nullptr));
        comboBox_bitrate_video->setItemText(1, QApplication::translate("ScreenRecorderClass", "384", nullptr));
        comboBox_bitrate_video->setItemText(2, QApplication::translate("ScreenRecorderClass", "512", nullptr));
        comboBox_bitrate_video->setItemText(3, QApplication::translate("ScreenRecorderClass", "768", nullptr));
        comboBox_bitrate_video->setItemText(4, QApplication::translate("ScreenRecorderClass", "1024", nullptr));
        comboBox_bitrate_video->setItemText(5, QApplication::translate("ScreenRecorderClass", "1536", nullptr));
        comboBox_bitrate_video->setItemText(6, QApplication::translate("ScreenRecorderClass", "2048", nullptr));
        comboBox_bitrate_video->setItemText(7, QApplication::translate("ScreenRecorderClass", "4096", nullptr));
        comboBox_bitrate_video->setItemText(8, QApplication::translate("ScreenRecorderClass", "6140", nullptr));
        comboBox_bitrate_video->setItemText(9, QApplication::translate("ScreenRecorderClass", "8184", nullptr));

        comboBox_bitrate_video->setCurrentText(QApplication::translate("ScreenRecorderClass", "2048", nullptr));
        comboBox_bitrate_audio->setItemText(0, QApplication::translate("ScreenRecorderClass", "32", nullptr));
        comboBox_bitrate_audio->setItemText(1, QApplication::translate("ScreenRecorderClass", "64", nullptr));
        comboBox_bitrate_audio->setItemText(2, QApplication::translate("ScreenRecorderClass", "96", nullptr));
        comboBox_bitrate_audio->setItemText(3, QApplication::translate("ScreenRecorderClass", "128", nullptr));
        comboBox_bitrate_audio->setItemText(4, QApplication::translate("ScreenRecorderClass", "192", nullptr));
        comboBox_bitrate_audio->setItemText(5, QApplication::translate("ScreenRecorderClass", "256", nullptr));
        comboBox_bitrate_audio->setItemText(6, QApplication::translate("ScreenRecorderClass", "320", nullptr));
        comboBox_bitrate_audio->setItemText(7, QApplication::translate("ScreenRecorderClass", "384", nullptr));
        comboBox_bitrate_audio->setItemText(8, QApplication::translate("ScreenRecorderClass", "448", nullptr));
        comboBox_bitrate_audio->setItemText(9, QApplication::translate("ScreenRecorderClass", "512", nullptr));

        comboBox_bitrate_audio->setCurrentText(QApplication::translate("ScreenRecorderClass", "128", nullptr));
        label_bitrate_video->setText(QApplication::translate("ScreenRecorderClass", "\347\240\201\347\216\207", nullptr));
        label_bitrate_audio->setText(QApplication::translate("ScreenRecorderClass", "\347\240\201\347\216\207", nullptr));
        label_videoKbps->setText(QApplication::translate("ScreenRecorderClass", "Kbps", nullptr));
        label_audioKbps->setText(QApplication::translate("ScreenRecorderClass", "Kbps", nullptr));
        comboBox_captureMode->setItemText(0, QApplication::translate("ScreenRecorderClass", "DXGI", nullptr));
        comboBox_captureMode->setItemText(1, QApplication::translate("ScreenRecorderClass", "DX9", nullptr));
        comboBox_captureMode->setItemText(2, QApplication::translate("ScreenRecorderClass", "QT", nullptr));

        checkFile->setText(QApplication::translate("ScreenRecorderClass", "\344\277\235\345\255\230\346\226\207\344\273\266", nullptr));
        checkLog->setText(QApplication::translate("ScreenRecorderClass", "\350\256\260\345\275\225\346\227\245\345\277\227", nullptr));
        checkRtmp->setText(QApplication::translate("ScreenRecorderClass", "\346\216\250\346\265\201", nullptr));
        lineEdit_rtmpPath->setText(QApplication::translate("ScreenRecorderClass", "rtmp://192.168.1.111/live", nullptr));
        label_rtmpPath->setText(QApplication::translate("ScreenRecorderClass", "\346\265\201\345\234\260\345\235\200\357\274\232", nullptr));
        comboBox_screen->setItemText(0, QApplication::translate("ScreenRecorderClass", "Desktop", nullptr));
        comboBox_screen->setItemText(1, QApplication::translate("ScreenRecorderClass", "Monitor1", nullptr));
        comboBox_screen->setItemText(2, QApplication::translate("ScreenRecorderClass", "Monitor2", nullptr));

    } // retranslateUi

};

namespace Ui {
    class ScreenRecorderClass: public Ui_ScreenRecorderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENRECORDER_H
