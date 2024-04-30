/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QGridLayout *gridLayout;
    QWidget *image_2;
    QLineEdit *editUserID_2;
    QLabel *label;
    QComboBox *cameraList;
    QComboBox *triggerSource_1;
    QLineEdit *editSN_1;
    QLabel *pixelFormatLabel_1;
    QSpacerItem *verticalSpacer;
    QPushButton *stop_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QSlider *gain_1;
    QLabel *gainLabel_1;
    QLineEdit *editUserID_1;
    QPushButton *close_2;
    QLineEdit *editSN_2;
    QWidget *image_1;
    QPushButton *openSelected_2;
    QLabel *triggerModeLabel_1;
    QPushButton *singleShot_1;
    QComboBox *triggerMode_1;
    QPushButton *openBySN_2;
    QLabel *softwareTriggerLabel_1;
    QPushButton *openByUserID_2;
    QLineEdit *statusbar_2;
    QPushButton *openSelected_1;
    QPushButton *continuous_1;
    QPushButton *stop_1;
    QComboBox *pixelFormat_1;
    QLabel *label_3;
    QPushButton *scanButton;
    QLineEdit *statusbar_1;
    QHBoxLayout *horizontalLayout_3;
    QSlider *exposure_2;
    QLabel *exposureLabel_2;
    QCheckBox *invertPixel_1;
    QPushButton *singleShot_2;
    QPushButton *softwareTrigger_1;
    QPushButton *openBySN_1;
    QHBoxLayout *horizontalLayout;
    QSlider *exposure_1;
    QLabel *exposureLabel_1;
    QPushButton *openByUserID_1;
    QPushButton *close_1;
    QLabel *invertPixelLabel_1;
    QLabel *triggerSourceLabel_1;
    QPushButton *continuous_2;
    QHBoxLayout *horizontalLayout_4;
    QSlider *gain_2;
    QLabel *gainLabel_2;
    QLabel *pixelFormatLabel_2;
    QLabel *triggerModeLabel_2;
    QComboBox *pixelFormat_2;
    QComboBox *triggerMode_2;
    QComboBox *triggerSource_2;
    QLabel *triggerSourceLabel_2;
    QPushButton *softwareTrigger_2;
    QLabel *softwareTriggerLabel_2;
    QLabel *invertPixelLabel_2;
    QCheckBox *invertPixel_2;
    QFrame *line;
    QLabel *label_4;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QString::fromUtf8("MainDialog"));
        MainDialog->resize(799, 614);
        gridLayout = new QGridLayout(MainDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        image_2 = new QWidget(MainDialog);
        image_2->setObjectName(QString::fromUtf8("image_2"));

        gridLayout->addWidget(image_2, 4, 4, 1, 3);

        editUserID_2 = new QLineEdit(MainDialog);
        editUserID_2->setObjectName(QString::fromUtf8("editUserID_2"));

        gridLayout->addWidget(editUserID_2, 3, 6, 1, 1);

        label = new QLabel(MainDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 7, 0, 1, 1);

        cameraList = new QComboBox(MainDialog);
        cameraList->setObjectName(QString::fromUtf8("cameraList"));

        gridLayout->addWidget(cameraList, 0, 1, 1, 6);

        triggerSource_1 = new QComboBox(MainDialog);
        triggerSource_1->setObjectName(QString::fromUtf8("triggerSource_1"));

        gridLayout->addWidget(triggerSource_1, 11, 1, 1, 2);

        editSN_1 = new QLineEdit(MainDialog);
        editSN_1->setObjectName(QString::fromUtf8("editSN_1"));

        gridLayout->addWidget(editSN_1, 3, 1, 1, 1);

        pixelFormatLabel_1 = new QLabel(MainDialog);
        pixelFormatLabel_1->setObjectName(QString::fromUtf8("pixelFormatLabel_1"));

        gridLayout->addWidget(pixelFormatLabel_1, 9, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        stop_2 = new QPushButton(MainDialog);
        stop_2->setObjectName(QString::fromUtf8("stop_2"));

        gridLayout->addWidget(stop_2, 6, 6, 1, 1);

        label_2 = new QLabel(MainDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 8, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gain_1 = new QSlider(MainDialog);
        gain_1->setObjectName(QString::fromUtf8("gain_1"));
        gain_1->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(gain_1);

        gainLabel_1 = new QLabel(MainDialog);
        gainLabel_1->setObjectName(QString::fromUtf8("gainLabel_1"));

        horizontalLayout_2->addWidget(gainLabel_1);


        gridLayout->addLayout(horizontalLayout_2, 8, 1, 1, 2);

        editUserID_1 = new QLineEdit(MainDialog);
        editUserID_1->setObjectName(QString::fromUtf8("editUserID_1"));

        gridLayout->addWidget(editUserID_1, 3, 2, 1, 1);

        close_2 = new QPushButton(MainDialog);
        close_2->setObjectName(QString::fromUtf8("close_2"));

        gridLayout->addWidget(close_2, 3, 4, 1, 1);

        editSN_2 = new QLineEdit(MainDialog);
        editSN_2->setObjectName(QString::fromUtf8("editSN_2"));

        gridLayout->addWidget(editSN_2, 3, 5, 1, 1);

        image_1 = new QWidget(MainDialog);
        image_1->setObjectName(QString::fromUtf8("image_1"));

        gridLayout->addWidget(image_1, 4, 0, 1, 3);

        openSelected_2 = new QPushButton(MainDialog);
        openSelected_2->setObjectName(QString::fromUtf8("openSelected_2"));

        gridLayout->addWidget(openSelected_2, 2, 4, 1, 1);

        triggerModeLabel_1 = new QLabel(MainDialog);
        triggerModeLabel_1->setObjectName(QString::fromUtf8("triggerModeLabel_1"));

        gridLayout->addWidget(triggerModeLabel_1, 10, 0, 1, 1);

        singleShot_1 = new QPushButton(MainDialog);
        singleShot_1->setObjectName(QString::fromUtf8("singleShot_1"));

        gridLayout->addWidget(singleShot_1, 6, 0, 1, 1);

        triggerMode_1 = new QComboBox(MainDialog);
        triggerMode_1->setObjectName(QString::fromUtf8("triggerMode_1"));

        gridLayout->addWidget(triggerMode_1, 10, 1, 1, 2);

        openBySN_2 = new QPushButton(MainDialog);
        openBySN_2->setObjectName(QString::fromUtf8("openBySN_2"));

        gridLayout->addWidget(openBySN_2, 2, 5, 1, 1);

        softwareTriggerLabel_1 = new QLabel(MainDialog);
        softwareTriggerLabel_1->setObjectName(QString::fromUtf8("softwareTriggerLabel_1"));

        gridLayout->addWidget(softwareTriggerLabel_1, 12, 0, 1, 1);

        openByUserID_2 = new QPushButton(MainDialog);
        openByUserID_2->setObjectName(QString::fromUtf8("openByUserID_2"));

        gridLayout->addWidget(openByUserID_2, 2, 6, 1, 1);

        statusbar_2 = new QLineEdit(MainDialog);
        statusbar_2->setObjectName(QString::fromUtf8("statusbar_2"));
        statusbar_2->setEnabled(false);
        statusbar_2->setReadOnly(true);

        gridLayout->addWidget(statusbar_2, 5, 4, 1, 3);

        openSelected_1 = new QPushButton(MainDialog);
        openSelected_1->setObjectName(QString::fromUtf8("openSelected_1"));

        gridLayout->addWidget(openSelected_1, 2, 0, 1, 1);

        continuous_1 = new QPushButton(MainDialog);
        continuous_1->setObjectName(QString::fromUtf8("continuous_1"));

        gridLayout->addWidget(continuous_1, 6, 1, 1, 1);

        stop_1 = new QPushButton(MainDialog);
        stop_1->setObjectName(QString::fromUtf8("stop_1"));

        gridLayout->addWidget(stop_1, 6, 2, 1, 1);

        pixelFormat_1 = new QComboBox(MainDialog);
        pixelFormat_1->setObjectName(QString::fromUtf8("pixelFormat_1"));

        gridLayout->addWidget(pixelFormat_1, 9, 1, 1, 2);

        label_3 = new QLabel(MainDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 7, 4, 1, 1);

        scanButton = new QPushButton(MainDialog);
        scanButton->setObjectName(QString::fromUtf8("scanButton"));

        gridLayout->addWidget(scanButton, 0, 0, 1, 1);

        statusbar_1 = new QLineEdit(MainDialog);
        statusbar_1->setObjectName(QString::fromUtf8("statusbar_1"));
        statusbar_1->setEnabled(false);
        statusbar_1->setReadOnly(true);

        gridLayout->addWidget(statusbar_1, 5, 0, 1, 3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        exposure_2 = new QSlider(MainDialog);
        exposure_2->setObjectName(QString::fromUtf8("exposure_2"));
        exposure_2->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(exposure_2);

        exposureLabel_2 = new QLabel(MainDialog);
        exposureLabel_2->setObjectName(QString::fromUtf8("exposureLabel_2"));

        horizontalLayout_3->addWidget(exposureLabel_2);


        gridLayout->addLayout(horizontalLayout_3, 7, 5, 1, 2);

        invertPixel_1 = new QCheckBox(MainDialog);
        invertPixel_1->setObjectName(QString::fromUtf8("invertPixel_1"));

        gridLayout->addWidget(invertPixel_1, 13, 1, 1, 2);

        singleShot_2 = new QPushButton(MainDialog);
        singleShot_2->setObjectName(QString::fromUtf8("singleShot_2"));

        gridLayout->addWidget(singleShot_2, 6, 4, 1, 1);

        softwareTrigger_1 = new QPushButton(MainDialog);
        softwareTrigger_1->setObjectName(QString::fromUtf8("softwareTrigger_1"));

        gridLayout->addWidget(softwareTrigger_1, 12, 1, 1, 2);

        openBySN_1 = new QPushButton(MainDialog);
        openBySN_1->setObjectName(QString::fromUtf8("openBySN_1"));

        gridLayout->addWidget(openBySN_1, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        exposure_1 = new QSlider(MainDialog);
        exposure_1->setObjectName(QString::fromUtf8("exposure_1"));
        exposure_1->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(exposure_1);

        exposureLabel_1 = new QLabel(MainDialog);
        exposureLabel_1->setObjectName(QString::fromUtf8("exposureLabel_1"));

        horizontalLayout->addWidget(exposureLabel_1);


        gridLayout->addLayout(horizontalLayout, 7, 1, 1, 2);

        openByUserID_1 = new QPushButton(MainDialog);
        openByUserID_1->setObjectName(QString::fromUtf8("openByUserID_1"));

        gridLayout->addWidget(openByUserID_1, 2, 2, 1, 1);

        close_1 = new QPushButton(MainDialog);
        close_1->setObjectName(QString::fromUtf8("close_1"));

        gridLayout->addWidget(close_1, 3, 0, 1, 1);

        invertPixelLabel_1 = new QLabel(MainDialog);
        invertPixelLabel_1->setObjectName(QString::fromUtf8("invertPixelLabel_1"));

        gridLayout->addWidget(invertPixelLabel_1, 13, 0, 1, 1);

        triggerSourceLabel_1 = new QLabel(MainDialog);
        triggerSourceLabel_1->setObjectName(QString::fromUtf8("triggerSourceLabel_1"));

        gridLayout->addWidget(triggerSourceLabel_1, 11, 0, 1, 1);

        continuous_2 = new QPushButton(MainDialog);
        continuous_2->setObjectName(QString::fromUtf8("continuous_2"));

        gridLayout->addWidget(continuous_2, 6, 5, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        gain_2 = new QSlider(MainDialog);
        gain_2->setObjectName(QString::fromUtf8("gain_2"));
        gain_2->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(gain_2);

        gainLabel_2 = new QLabel(MainDialog);
        gainLabel_2->setObjectName(QString::fromUtf8("gainLabel_2"));

        horizontalLayout_4->addWidget(gainLabel_2);


        gridLayout->addLayout(horizontalLayout_4, 8, 5, 1, 2);

        pixelFormatLabel_2 = new QLabel(MainDialog);
        pixelFormatLabel_2->setObjectName(QString::fromUtf8("pixelFormatLabel_2"));

        gridLayout->addWidget(pixelFormatLabel_2, 9, 4, 1, 1);

        triggerModeLabel_2 = new QLabel(MainDialog);
        triggerModeLabel_2->setObjectName(QString::fromUtf8("triggerModeLabel_2"));

        gridLayout->addWidget(triggerModeLabel_2, 10, 4, 1, 1);

        pixelFormat_2 = new QComboBox(MainDialog);
        pixelFormat_2->setObjectName(QString::fromUtf8("pixelFormat_2"));

        gridLayout->addWidget(pixelFormat_2, 9, 5, 1, 2);

        triggerMode_2 = new QComboBox(MainDialog);
        triggerMode_2->setObjectName(QString::fromUtf8("triggerMode_2"));

        gridLayout->addWidget(triggerMode_2, 10, 5, 1, 2);

        triggerSource_2 = new QComboBox(MainDialog);
        triggerSource_2->setObjectName(QString::fromUtf8("triggerSource_2"));

        gridLayout->addWidget(triggerSource_2, 11, 5, 1, 2);

        triggerSourceLabel_2 = new QLabel(MainDialog);
        triggerSourceLabel_2->setObjectName(QString::fromUtf8("triggerSourceLabel_2"));

        gridLayout->addWidget(triggerSourceLabel_2, 11, 4, 1, 1);

        softwareTrigger_2 = new QPushButton(MainDialog);
        softwareTrigger_2->setObjectName(QString::fromUtf8("softwareTrigger_2"));

        gridLayout->addWidget(softwareTrigger_2, 12, 5, 1, 1);

        softwareTriggerLabel_2 = new QLabel(MainDialog);
        softwareTriggerLabel_2->setObjectName(QString::fromUtf8("softwareTriggerLabel_2"));

        gridLayout->addWidget(softwareTriggerLabel_2, 12, 4, 1, 1);

        invertPixelLabel_2 = new QLabel(MainDialog);
        invertPixelLabel_2->setObjectName(QString::fromUtf8("invertPixelLabel_2"));

        gridLayout->addWidget(invertPixelLabel_2, 13, 4, 1, 1);

        invertPixel_2 = new QCheckBox(MainDialog);
        invertPixel_2->setObjectName(QString::fromUtf8("invertPixel_2"));

        gridLayout->addWidget(invertPixel_2, 13, 5, 1, 1);

        line = new QFrame(MainDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 2, 3, 12, 1);

        label_4 = new QLabel(MainDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 8, 4, 1, 1);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setRowStretch(1, 1);
        gridLayout->setRowStretch(2, 1);
        gridLayout->setRowStretch(3, 1);
        gridLayout->setRowStretch(4, 10);
        gridLayout->setRowStretch(5, 1);
        gridLayout->setRowStretch(6, 1);
        gridLayout->setRowStretch(7, 1);
        gridLayout->setRowStretch(8, 1);
        gridLayout->setRowStretch(9, 1);
        gridLayout->setRowStretch(10, 1);
        gridLayout->setRowStretch(11, 1);
        gridLayout->setRowStretch(12, 1);
        gridLayout->setRowStretch(13, 1);
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(4, 1);
        gridLayout->setColumnStretch(5, 1);
        gridLayout->setColumnStretch(6, 1);

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "Pylon GUI MultiCam Sample", nullptr));
#ifndef QT_NO_TOOLTIP
        editUserID_2->setToolTip(QApplication::translate("MainDialog", "Check for a device user ID in the camera list and enter it here. You may need to specify a device user ID in the pylon Viewer first.", nullptr));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainDialog", "Exposure Time", nullptr));
#ifndef QT_NO_TOOLTIP
        editSN_1->setToolTip(QApplication::translate("MainDialog", "Check for a camera serial number in the camera list and enter it here.", nullptr));
#endif // QT_NO_TOOLTIP
        pixelFormatLabel_1->setText(QApplication::translate("MainDialog", "Pixel Format", nullptr));
        stop_2->setText(QApplication::translate("MainDialog", "Stop", nullptr));
        label_2->setText(QApplication::translate("MainDialog", "Gain", nullptr));
        gainLabel_1->setText(QApplication::translate("MainDialog", "---", nullptr));
#ifndef QT_NO_TOOLTIP
        editUserID_1->setToolTip(QApplication::translate("MainDialog", "Check for a device user ID in the camera list and enter it here. You may need to specify a device user ID in the pylon Viewer first.", nullptr));
#endif // QT_NO_TOOLTIP
        close_2->setText(QApplication::translate("MainDialog", "Close", nullptr));
#ifndef QT_NO_TOOLTIP
        editSN_2->setToolTip(QApplication::translate("MainDialog", "Check for a camera serial number in the camera list and enter it here.", nullptr));
#endif // QT_NO_TOOLTIP
        openSelected_2->setText(QApplication::translate("MainDialog", "Open Selected", nullptr));
        triggerModeLabel_1->setText(QApplication::translate("MainDialog", "Frame Start Trigger", nullptr));
        singleShot_1->setText(QApplication::translate("MainDialog", "Single Shot", nullptr));
        openBySN_2->setText(QApplication::translate("MainDialog", "Open by SN", nullptr));
        softwareTriggerLabel_1->setText(QApplication::translate("MainDialog", "Software Trigger", nullptr));
        openByUserID_2->setText(QApplication::translate("MainDialog", "Open by User ID", nullptr));
        openSelected_1->setText(QApplication::translate("MainDialog", "Open Selected", nullptr));
        continuous_1->setText(QApplication::translate("MainDialog", "Continuous Shot", nullptr));
        stop_1->setText(QApplication::translate("MainDialog", "Stop", nullptr));
        label_3->setText(QApplication::translate("MainDialog", "Exposure Time", nullptr));
        scanButton->setText(QApplication::translate("MainDialog", "Discover Cameras", nullptr));
        exposureLabel_2->setText(QApplication::translate("MainDialog", "---", nullptr));
        invertPixel_1->setText(QApplication::translate("MainDialog", "Enable", nullptr));
        singleShot_2->setText(QApplication::translate("MainDialog", "Single Shot", nullptr));
        softwareTrigger_1->setText(QApplication::translate("MainDialog", "Execute", nullptr));
        openBySN_1->setText(QApplication::translate("MainDialog", "Open by SN", nullptr));
        exposureLabel_1->setText(QApplication::translate("MainDialog", "---", nullptr));
        openByUserID_1->setText(QApplication::translate("MainDialog", "Open by User ID", nullptr));
        close_1->setText(QApplication::translate("MainDialog", "Close", nullptr));
        invertPixelLabel_1->setText(QApplication::translate("MainDialog", "Invert Pixels", nullptr));
        triggerSourceLabel_1->setText(QApplication::translate("MainDialog", "Trigger Source", nullptr));
        continuous_2->setText(QApplication::translate("MainDialog", "Continuous Shot", nullptr));
        gainLabel_2->setText(QApplication::translate("MainDialog", "---", nullptr));
        pixelFormatLabel_2->setText(QApplication::translate("MainDialog", "Pixel Format", nullptr));
        triggerModeLabel_2->setText(QApplication::translate("MainDialog", "Frame Start Trigger", nullptr));
        triggerSourceLabel_2->setText(QApplication::translate("MainDialog", "Trigger Source", nullptr));
        softwareTrigger_2->setText(QApplication::translate("MainDialog", "Execute", nullptr));
        softwareTriggerLabel_2->setText(QApplication::translate("MainDialog", "Software Trigger", nullptr));
        invertPixelLabel_2->setText(QApplication::translate("MainDialog", "Invert Pixels", nullptr));
        invertPixel_2->setText(QApplication::translate("MainDialog", "Enable", nullptr));
        label_4->setText(QApplication::translate("MainDialog", "Gain", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
