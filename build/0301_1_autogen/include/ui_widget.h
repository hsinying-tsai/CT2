/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <my_qlabel.h>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_3;
    QTextBrowser *textSend;
    QPushButton *puB_sent;
    QGroupBox *groupBox_2;
    QTextBrowser *textRecv;
    QTabWidget *tabWidget_2;
    QWidget *tab_6;
    QPushButton *puB_add;
    QListWidget *list_Pattern;
    QPushButton *puB_remove;
    QPushButton *puB_save;
    QWidget *tab_7;
    QLabel *lbl_DM204;
    QLabel *lbl_DM206;
    QLabel *lbl_R200;
    QLineEdit *R206_Edit;
    QLineEdit *DM202_Edit;
    QLineEdit *DM200_Edit;
    QLabel *lbl_R206;
    QLineEdit *R202_Edit;
    QLabel *lbl_R203;
    QPushButton *puB_read;
    QLabel *lbl_R204;
    QLineEdit *R203_Edit;
    QLineEdit *R207_Edit;
    QLineEdit *R201_Edit;
    QLabel *lbl_DM200;
    QLineEdit *R200_Edit;
    QLineEdit *R204_Edit;
    QLabel *lbl_R205;
    QPushButton *puB_write;
    QLabel *lbl_DM202;
    QLineEdit *DM206_Edit;
    QLabel *lbl_R202;
    QLabel *lbl_R201;
    QLineEdit *DM204_Edit;
    QLineEdit *R205_Edit;
    QLabel *lbl_R207;
    QLabel *lbl_R212;
    QLineEdit *R212_Edit;
    QLabel *lbl_R214;
    QLineEdit *R214_Edit;
    QWidget *tab_3;
    QComboBox *comboBox;
    QTextBrowser *text_log;
    QWidget *tab_4;
    QGroupBox *groupBox_10;
    QLineEdit *CAM1_exposure_Edit_3;
    QLabel *lbl_exposure_7;
    QGroupBox *groupBox_11;
    QLineEdit *CAM2_exposure_Edit_3;
    QLabel *lbl_exposure_8;
    QGroupBox *groupBox_12;
    QLineEdit *CAM3_exposure_Edit_3;
    QLabel *lbl_exposure_9;
    QLineEdit *PT_width_Edit;
    QLabel *lbl_PT_width;
    QLineEdit *PT_height_Edit;
    QLabel *lbl_PT_height;
    QLineEdit *path_Edit;
    QLabel *lbl_PT_height_2;
    QPushButton *puB_saveINI;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QPushButton *puB_runMode;
    QLineEdit *PortEdit;
    QPushButton *puB_connect;
    QLabel *lbl_plc;
    QPushButton *puB_start;
    QLineEdit *AddressEdit;
    QWidget *tab_2;
    QPushButton *puB_pre;
    my_qlabel *lbl_pic;
    QPushButton *puB_next;
    QLabel *lbl_Pos;
    QGroupBox *groupBox_7;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lbl_pattern_4;
    QLabel *lbl_pattern_3;
    QLabel *lbl_pic2;
    QTableWidget *table_defectlist;
    QSplitter *splitter;
    QLabel *lbl_pattern_2;
    QLabel *lbl_pattern;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1268, 771);
        verticalLayout_3 = new QVBoxLayout(Widget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setFocusPolicy(Qt::TabFocus);
        tabWidget->setAcceptDrops(false);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setDocumentMode(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(410, 240, 361, 461));
        QFont font;
        font.setPointSize(14);
        groupBox_3->setFont(font);
        textSend = new QTextBrowser(groupBox_3);
        textSend->setObjectName(QString::fromUtf8("textSend"));
        textSend->setGeometry(QRect(16, 40, 331, 371));
        QFont font1;
        font1.setPointSize(12);
        textSend->setFont(font1);
        textSend->setReadOnly(false);
        puB_sent = new QPushButton(groupBox_3);
        puB_sent->setObjectName(QString::fromUtf8("puB_sent"));
        puB_sent->setGeometry(QRect(250, 420, 101, 31));
        puB_sent->setFont(font);
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 240, 361, 461));
        groupBox_2->setFont(font);
        textRecv = new QTextBrowser(groupBox_2);
        textRecv->setObjectName(QString::fromUtf8("textRecv"));
        textRecv->setGeometry(QRect(16, 40, 331, 371));
        textRecv->setFont(font1);
        textRecv->setFrameShadow(QFrame::Plain);
        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(790, 30, 431, 671));
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        puB_add = new QPushButton(tab_6);
        puB_add->setObjectName(QString::fromUtf8("puB_add"));
        puB_add->setGeometry(QRect(230, 540, 101, 31));
        puB_add->setFont(font);
        list_Pattern = new QListWidget(tab_6);
        QFont font2;
        font2.setStrikeOut(false);
        font2.setKerning(true);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(list_Pattern);
        __qlistwidgetitem->setFont(font2);
        __qlistwidgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        QFont font3;
        font3.setUnderline(false);
        font3.setStyleStrategy(QFont::PreferAntialias);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(list_Pattern);
        __qlistwidgetitem1->setFont(font3);
        __qlistwidgetitem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        new QListWidgetItem(list_Pattern);
        new QListWidgetItem(list_Pattern);
        new QListWidgetItem(list_Pattern);
        list_Pattern->setObjectName(QString::fromUtf8("list_Pattern"));
        list_Pattern->setGeometry(QRect(60, 30, 311, 501));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(list_Pattern->sizePolicy().hasHeightForWidth());
        list_Pattern->setSizePolicy(sizePolicy);
        QFont font4;
        font4.setPointSize(13);
        list_Pattern->setFont(font4);
        list_Pattern->setDragEnabled(true);
        list_Pattern->setDragDropOverwriteMode(true);
        list_Pattern->setDragDropMode(QAbstractItemView::InternalMove);
        puB_remove = new QPushButton(tab_6);
        puB_remove->setObjectName(QString::fromUtf8("puB_remove"));
        puB_remove->setGeometry(QRect(120, 540, 91, 31));
        puB_remove->setFont(font);
        puB_save = new QPushButton(tab_6);
        puB_save->setObjectName(QString::fromUtf8("puB_save"));
        puB_save->setGeometry(QRect(180, 590, 81, 31));
        puB_save->setFont(font);
        tabWidget_2->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        lbl_DM204 = new QLabel(tab_7);
        lbl_DM204->setObjectName(QString::fromUtf8("lbl_DM204"));
        lbl_DM204->setGeometry(QRect(20, 170, 121, 41));
        lbl_DM204->setFont(font);
        lbl_DM206 = new QLabel(tab_7);
        lbl_DM206->setObjectName(QString::fromUtf8("lbl_DM206"));
        lbl_DM206->setGeometry(QRect(20, 240, 121, 41));
        lbl_DM206->setFont(font);
        lbl_R200 = new QLabel(tab_7);
        lbl_R200->setObjectName(QString::fromUtf8("lbl_R200"));
        lbl_R200->setGeometry(QRect(37, 310, 121, 41));
        lbl_R200->setFont(font);
        R206_Edit = new QLineEdit(tab_7);
        R206_Edit->setObjectName(QString::fromUtf8("R206_Edit"));
        R206_Edit->setGeometry(QRect(293, 240, 80, 41));
        R206_Edit->setFont(font);
        DM202_Edit = new QLineEdit(tab_7);
        DM202_Edit->setObjectName(QString::fromUtf8("DM202_Edit"));
        DM202_Edit->setGeometry(QRect(100, 100, 80, 41));
        DM202_Edit->setFont(font);
        DM200_Edit = new QLineEdit(tab_7);
        DM200_Edit->setObjectName(QString::fromUtf8("DM200_Edit"));
        DM200_Edit->setGeometry(QRect(100, 30, 80, 41));
        DM200_Edit->setFont(font);
        lbl_R206 = new QLabel(tab_7);
        lbl_R206->setObjectName(QString::fromUtf8("lbl_R206"));
        lbl_R206->setGeometry(QRect(230, 240, 121, 41));
        lbl_R206->setFont(font);
        R202_Edit = new QLineEdit(tab_7);
        R202_Edit->setObjectName(QString::fromUtf8("R202_Edit"));
        R202_Edit->setGeometry(QRect(100, 450, 80, 41));
        R202_Edit->setFont(font);
        lbl_R203 = new QLabel(tab_7);
        lbl_R203->setObjectName(QString::fromUtf8("lbl_R203"));
        lbl_R203->setGeometry(QRect(230, 30, 121, 41));
        lbl_R203->setFont(font);
        puB_read = new QPushButton(tab_7);
        puB_read->setObjectName(QString::fromUtf8("puB_read"));
        puB_read->setGeometry(QRect(70, 550, 121, 41));
        puB_read->setFont(font);
        lbl_R204 = new QLabel(tab_7);
        lbl_R204->setObjectName(QString::fromUtf8("lbl_R204"));
        lbl_R204->setGeometry(QRect(230, 100, 121, 41));
        lbl_R204->setFont(font);
        R203_Edit = new QLineEdit(tab_7);
        R203_Edit->setObjectName(QString::fromUtf8("R203_Edit"));
        R203_Edit->setGeometry(QRect(293, 30, 80, 41));
        R203_Edit->setFont(font);
        R207_Edit = new QLineEdit(tab_7);
        R207_Edit->setObjectName(QString::fromUtf8("R207_Edit"));
        R207_Edit->setGeometry(QRect(293, 310, 80, 41));
        R207_Edit->setFont(font);
        R201_Edit = new QLineEdit(tab_7);
        R201_Edit->setObjectName(QString::fromUtf8("R201_Edit"));
        R201_Edit->setGeometry(QRect(100, 380, 80, 41));
        R201_Edit->setFont(font);
        lbl_DM200 = new QLabel(tab_7);
        lbl_DM200->setObjectName(QString::fromUtf8("lbl_DM200"));
        lbl_DM200->setGeometry(QRect(20, 30, 121, 41));
        QFont font5;
        font5.setPointSize(14);
        font5.setStyleStrategy(QFont::PreferDefault);
        lbl_DM200->setFont(font5);
        R200_Edit = new QLineEdit(tab_7);
        R200_Edit->setObjectName(QString::fromUtf8("R200_Edit"));
        R200_Edit->setGeometry(QRect(100, 310, 80, 41));
        R200_Edit->setFont(font);
        R204_Edit = new QLineEdit(tab_7);
        R204_Edit->setObjectName(QString::fromUtf8("R204_Edit"));
        R204_Edit->setGeometry(QRect(293, 100, 80, 41));
        R204_Edit->setFont(font);
        lbl_R205 = new QLabel(tab_7);
        lbl_R205->setObjectName(QString::fromUtf8("lbl_R205"));
        lbl_R205->setGeometry(QRect(230, 170, 121, 41));
        lbl_R205->setFont(font);
        puB_write = new QPushButton(tab_7);
        puB_write->setObjectName(QString::fromUtf8("puB_write"));
        puB_write->setEnabled(true);
        puB_write->setGeometry(QRect(230, 550, 121, 41));
        puB_write->setFont(font);
        lbl_DM202 = new QLabel(tab_7);
        lbl_DM202->setObjectName(QString::fromUtf8("lbl_DM202"));
        lbl_DM202->setGeometry(QRect(20, 100, 121, 41));
        lbl_DM202->setFont(font);
        DM206_Edit = new QLineEdit(tab_7);
        DM206_Edit->setObjectName(QString::fromUtf8("DM206_Edit"));
        DM206_Edit->setGeometry(QRect(100, 240, 80, 41));
        DM206_Edit->setFont(font);
        lbl_R202 = new QLabel(tab_7);
        lbl_R202->setObjectName(QString::fromUtf8("lbl_R202"));
        lbl_R202->setGeometry(QRect(37, 450, 121, 41));
        lbl_R202->setFont(font);
        lbl_R201 = new QLabel(tab_7);
        lbl_R201->setObjectName(QString::fromUtf8("lbl_R201"));
        lbl_R201->setGeometry(QRect(37, 380, 121, 41));
        lbl_R201->setFont(font);
        DM204_Edit = new QLineEdit(tab_7);
        DM204_Edit->setObjectName(QString::fromUtf8("DM204_Edit"));
        DM204_Edit->setGeometry(QRect(100, 170, 80, 41));
        DM204_Edit->setFont(font);
        R205_Edit = new QLineEdit(tab_7);
        R205_Edit->setObjectName(QString::fromUtf8("R205_Edit"));
        R205_Edit->setGeometry(QRect(293, 170, 80, 41));
        R205_Edit->setFont(font);
        lbl_R207 = new QLabel(tab_7);
        lbl_R207->setObjectName(QString::fromUtf8("lbl_R207"));
        lbl_R207->setGeometry(QRect(230, 310, 121, 41));
        lbl_R207->setFont(font);
        lbl_R212 = new QLabel(tab_7);
        lbl_R212->setObjectName(QString::fromUtf8("lbl_R212"));
        lbl_R212->setGeometry(QRect(230, 380, 121, 41));
        lbl_R212->setFont(font);
        R212_Edit = new QLineEdit(tab_7);
        R212_Edit->setObjectName(QString::fromUtf8("R212_Edit"));
        R212_Edit->setGeometry(QRect(293, 380, 80, 41));
        R212_Edit->setFont(font);
        lbl_R214 = new QLabel(tab_7);
        lbl_R214->setObjectName(QString::fromUtf8("lbl_R214"));
        lbl_R214->setGeometry(QRect(230, 450, 121, 41));
        lbl_R214->setFont(font);
        R214_Edit = new QLineEdit(tab_7);
        R214_Edit->setObjectName(QString::fromUtf8("R214_Edit"));
        R214_Edit->setGeometry(QRect(293, 450, 80, 41));
        R214_Edit->setFont(font);
        tabWidget_2->addTab(tab_7, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        comboBox = new QComboBox(tab_3);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 10, 181, 31));
        comboBox->setFont(font4);
        text_log = new QTextBrowser(tab_3);
        text_log->setObjectName(QString::fromUtf8("text_log"));
        text_log->setGeometry(QRect(10, 50, 411, 581));
        text_log->setFont(font1);
        text_log->setFrameShadow(QFrame::Plain);
        text_log->setReadOnly(true);
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        groupBox_10 = new QGroupBox(tab_4);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 10, 381, 92));
        CAM1_exposure_Edit_3 = new QLineEdit(groupBox_10);
        CAM1_exposure_Edit_3->setObjectName(QString::fromUtf8("CAM1_exposure_Edit_3"));
        CAM1_exposure_Edit_3->setGeometry(QRect(160, 37, 131, 35));
        CAM1_exposure_Edit_3->setFont(font1);
        lbl_exposure_7 = new QLabel(groupBox_10);
        lbl_exposure_7->setObjectName(QString::fromUtf8("lbl_exposure_7"));
        lbl_exposure_7->setGeometry(QRect(16, 32, 161, 41));
        lbl_exposure_7->setFont(font5);
        groupBox_11 = new QGroupBox(tab_4);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(10, 110, 381, 142));
        CAM2_exposure_Edit_3 = new QLineEdit(groupBox_11);
        CAM2_exposure_Edit_3->setObjectName(QString::fromUtf8("CAM2_exposure_Edit_3"));
        CAM2_exposure_Edit_3->setGeometry(QRect(160, 37, 131, 35));
        CAM2_exposure_Edit_3->setFont(font1);
        lbl_exposure_8 = new QLabel(groupBox_11);
        lbl_exposure_8->setObjectName(QString::fromUtf8("lbl_exposure_8"));
        lbl_exposure_8->setGeometry(QRect(16, 32, 161, 41));
        lbl_exposure_8->setFont(font5);
        groupBox_12 = new QGroupBox(tab_4);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        groupBox_12->setGeometry(QRect(20, 260, 391, 191));
        CAM3_exposure_Edit_3 = new QLineEdit(groupBox_12);
        CAM3_exposure_Edit_3->setObjectName(QString::fromUtf8("CAM3_exposure_Edit_3"));
        CAM3_exposure_Edit_3->setGeometry(QRect(160, 37, 131, 35));
        CAM3_exposure_Edit_3->setFont(font1);
        lbl_exposure_9 = new QLabel(groupBox_12);
        lbl_exposure_9->setObjectName(QString::fromUtf8("lbl_exposure_9"));
        lbl_exposure_9->setGeometry(QRect(16, 32, 161, 41));
        lbl_exposure_9->setFont(font5);
        PT_width_Edit = new QLineEdit(tab_4);
        PT_width_Edit->setObjectName(QString::fromUtf8("PT_width_Edit"));
        PT_width_Edit->setGeometry(QRect(242, 460, 171, 35));
        PT_width_Edit->setFont(font1);
        lbl_PT_width = new QLabel(tab_4);
        lbl_PT_width->setObjectName(QString::fromUtf8("lbl_PT_width"));
        lbl_PT_width->setGeometry(QRect(52, 456, 191, 41));
        lbl_PT_width->setFont(font5);
        PT_height_Edit = new QLineEdit(tab_4);
        PT_height_Edit->setObjectName(QString::fromUtf8("PT_height_Edit"));
        PT_height_Edit->setGeometry(QRect(242, 503, 171, 35));
        PT_height_Edit->setFont(font1);
        lbl_PT_height = new QLabel(tab_4);
        lbl_PT_height->setObjectName(QString::fromUtf8("lbl_PT_height"));
        lbl_PT_height->setGeometry(QRect(42, 500, 201, 41));
        lbl_PT_height->setFont(font5);
        path_Edit = new QLineEdit(tab_4);
        path_Edit->setObjectName(QString::fromUtf8("path_Edit"));
        path_Edit->setGeometry(QRect(242, 545, 171, 35));
        path_Edit->setFont(font1);
        lbl_PT_height_2 = new QLabel(tab_4);
        lbl_PT_height_2->setObjectName(QString::fromUtf8("lbl_PT_height_2"));
        lbl_PT_height_2->setGeometry(QRect(6, 541, 241, 41));
        lbl_PT_height_2->setFont(font5);
        puB_saveINI = new QPushButton(tab_4);
        puB_saveINI->setObjectName(QString::fromUtf8("puB_saveINI"));
        puB_saveINI->setGeometry(QRect(180, 590, 101, 41));
        QFont font6;
        font6.setPointSize(15);
        puB_saveINI->setFont(font6);
        tabWidget_2->addTab(tab_4, QString());
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 80, 121, 20));
        label->setFont(font);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 130, 71, 20));
        label_2->setFont(font);
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(460, 100, 121, 41));
        label_4->setFont(font);
        puB_runMode = new QPushButton(tab);
        puB_runMode->setObjectName(QString::fromUtf8("puB_runMode"));
        puB_runMode->setGeometry(QRect(570, 100, 121, 41));
        puB_runMode->setFont(font);
        PortEdit = new QLineEdit(tab);
        PortEdit->setObjectName(QString::fromUtf8("PortEdit"));
        PortEdit->setGeometry(QRect(240, 130, 151, 35));
        PortEdit->setFont(font);
        puB_connect = new QPushButton(tab);
        puB_connect->setObjectName(QString::fromUtf8("puB_connect"));
        puB_connect->setGeometry(QRect(210, 190, 111, 41));
        puB_connect->setFont(font);
        lbl_plc = new QLabel(tab);
        lbl_plc->setObjectName(QString::fromUtf8("lbl_plc"));
        lbl_plc->setGeometry(QRect(30, 20, 61, 61));
        lbl_plc->setAutoFillBackground(false);
        lbl_plc->setStyleSheet(QString::fromUtf8("border-width: 3px;        \n"
"border-radius: 20px;\n"
"margin:10px;\n"
"padding:20px;\n"
"background-color: red;"));
        lbl_plc->setInputMethodHints(Qt::ImhNone);
        puB_start = new QPushButton(tab);
        puB_start->setObjectName(QString::fromUtf8("puB_start"));
        puB_start->setGeometry(QRect(540, 190, 111, 41));
        puB_start->setFont(font);
        AddressEdit = new QLineEdit(tab);
        AddressEdit->setObjectName(QString::fromUtf8("AddressEdit"));
        AddressEdit->setGeometry(QRect(240, 70, 151, 35));
        AddressEdit->setFont(font);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        puB_pre = new QPushButton(tab_2);
        puB_pre->setObjectName(QString::fromUtf8("puB_pre"));
        puB_pre->setGeometry(QRect(204, 430, 111, 41));
        puB_pre->setFont(font4);
        lbl_pic = new my_qlabel(tab_2);
        lbl_pic->setObjectName(QString::fromUtf8("lbl_pic"));
        lbl_pic->setGeometry(QRect(50, 100, 576, 324));
        lbl_pic->setCursor(QCursor(Qt::ArrowCursor));
        lbl_pic->setMouseTracking(true);
        lbl_pic->setFrameShape(QFrame::Box);
        lbl_pic->setFrameShadow(QFrame::Plain);
        lbl_pic->setScaledContents(true);
        puB_next = new QPushButton(tab_2);
        puB_next->setObjectName(QString::fromUtf8("puB_next"));
        puB_next->setGeometry(QRect(350, 430, 111, 41));
        puB_next->setFont(font4);
        lbl_Pos = new QLabel(tab_2);
        lbl_Pos->setObjectName(QString::fromUtf8("lbl_Pos"));
        lbl_Pos->setGeometry(QRect(50, 430, 181, 51));
        lbl_Pos->setFont(font1);
        groupBox_7 = new QGroupBox(tab_2);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(650, 50, 561, 391));
        groupBox_7->setFont(font);
        layoutWidget = new QWidget(groupBox_7);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 270, 301, 91));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lbl_pattern_4 = new QLabel(layoutWidget);
        lbl_pattern_4->setObjectName(QString::fromUtf8("lbl_pattern_4"));
        lbl_pattern_4->setFont(font);

        verticalLayout->addWidget(lbl_pattern_4);

        lbl_pattern_3 = new QLabel(layoutWidget);
        lbl_pattern_3->setObjectName(QString::fromUtf8("lbl_pattern_3"));
        lbl_pattern_3->setFont(font);

        verticalLayout->addWidget(lbl_pattern_3);

        lbl_pic2 = new QLabel(groupBox_7);
        lbl_pic2->setObjectName(QString::fromUtf8("lbl_pic2"));
        lbl_pic2->setGeometry(QRect(30, 50, 259, 194));
        QFont font7;
        font7.setPointSize(17);
        lbl_pic2->setFont(font7);
        lbl_pic2->setCursor(QCursor(Qt::CrossCursor));
        lbl_pic2->setMouseTracking(false);
        lbl_pic2->setFrameShape(QFrame::Box);
        lbl_pic2->setFrameShadow(QFrame::Plain);
        lbl_pic2->setScaledContents(true);
        table_defectlist = new QTableWidget(tab_2);
        if (table_defectlist->columnCount() < 4)
            table_defectlist->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_defectlist->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table_defectlist->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table_defectlist->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table_defectlist->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (table_defectlist->rowCount() < 8)
            table_defectlist->setRowCount(8);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(5, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(6, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table_defectlist->setVerticalHeaderItem(7, __qtablewidgetitem11);
        table_defectlist->setObjectName(QString::fromUtf8("table_defectlist"));
        table_defectlist->setGeometry(QRect(40, 500, 1181, 201));
        splitter = new QSplitter(tab_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(50, 50, 581, 41));
        splitter->setOrientation(Qt::Horizontal);
        lbl_pattern_2 = new QLabel(splitter);
        lbl_pattern_2->setObjectName(QString::fromUtf8("lbl_pattern_2"));
        lbl_pattern_2->setFont(font);
        splitter->addWidget(lbl_pattern_2);
        lbl_pattern = new QLabel(splitter);
        lbl_pattern->setObjectName(QString::fromUtf8("lbl_pattern"));
        lbl_pattern->setFont(font);
        splitter->addWidget(lbl_pattern);
        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        groupBox_3->setTitle(QApplication::translate("Widget", "Send\357\274\232", nullptr));
        puB_sent->setText(QApplication::translate("Widget", "Send", nullptr));
        groupBox_2->setTitle(QApplication::translate("Widget", "Receive", nullptr));
        puB_add->setText(QApplication::translate("Widget", "add", nullptr));

        const bool __sortingEnabled = list_Pattern->isSortingEnabled();
        list_Pattern->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = list_Pattern->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("Widget", "Black", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = list_Pattern->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("Widget", "White", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = list_Pattern->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("Widget", "Gray", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = list_Pattern->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("Widget", "Red", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = list_Pattern->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("Widget", "Green", nullptr));
        list_Pattern->setSortingEnabled(__sortingEnabled);

        puB_remove->setText(QApplication::translate("Widget", "remove", nullptr));
        puB_save->setText(QApplication::translate("Widget", "save", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QApplication::translate("Widget", "Pattern List", nullptr));
        lbl_DM204->setText(QApplication::translate("Widget", "DM204\357\274\232", nullptr));
        lbl_DM206->setText(QApplication::translate("Widget", "DM206\357\274\232", nullptr));
        lbl_R200->setText(QApplication::translate("Widget", "R200\357\274\232", nullptr));
        R206_Edit->setText(QString());
        DM202_Edit->setText(QString());
        DM200_Edit->setText(QString());
        lbl_R206->setText(QApplication::translate("Widget", "R206\357\274\232", nullptr));
        R202_Edit->setText(QString());
        lbl_R203->setText(QApplication::translate("Widget", "R203\357\274\232", nullptr));
        puB_read->setText(QApplication::translate("Widget", "Read", nullptr));
        lbl_R204->setText(QApplication::translate("Widget", "R204\357\274\232", nullptr));
        R203_Edit->setText(QString());
        R207_Edit->setText(QString());
        R201_Edit->setText(QString());
        lbl_DM200->setText(QApplication::translate("Widget", "DM200\357\274\232", nullptr));
        R200_Edit->setText(QString());
        R204_Edit->setText(QString());
        lbl_R205->setText(QApplication::translate("Widget", "R205\357\274\232", nullptr));
        puB_write->setText(QApplication::translate("Widget", "Write", nullptr));
        lbl_DM202->setText(QApplication::translate("Widget", "DM202\357\274\232", nullptr));
        DM206_Edit->setText(QString());
        lbl_R202->setText(QApplication::translate("Widget", "R202\357\274\232", nullptr));
        lbl_R201->setText(QApplication::translate("Widget", "R201\357\274\232", nullptr));
        DM204_Edit->setText(QString());
        R205_Edit->setText(QString());
        lbl_R207->setText(QApplication::translate("Widget", "R207\357\274\232", nullptr));
        lbl_R212->setText(QApplication::translate("Widget", "R212\357\274\232", nullptr));
        R212_Edit->setText(QString());
        lbl_R214->setText(QApplication::translate("Widget", "R214\357\274\232", nullptr));
        R214_Edit->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_7), QApplication::translate("Widget", "Commend Demo", nullptr));
        comboBox->setItemText(0, QApplication::translate("Widget", "select logger", nullptr));
        comboBox->setItemText(1, QApplication::translate("Widget", "1", nullptr));

        comboBox->setCurrentText(QApplication::translate("Widget", "select logger", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("Widget", "Log", nullptr));
        groupBox_10->setTitle(QApplication::translate("Widget", "CAM1", nullptr));
        CAM1_exposure_Edit_3->setText(QString());
        lbl_exposure_7->setText(QApplication::translate("Widget", "exposure time :", nullptr));
        groupBox_11->setTitle(QApplication::translate("Widget", "CAM2", nullptr));
        CAM2_exposure_Edit_3->setText(QString());
        lbl_exposure_8->setText(QApplication::translate("Widget", "exposure time :", nullptr));
        groupBox_12->setTitle(QApplication::translate("Widget", "CAM3", nullptr));
        CAM3_exposure_Edit_3->setText(QString());
        lbl_exposure_9->setText(QApplication::translate("Widget", "exposure time :", nullptr));
        PT_width_Edit->setText(QString());
        lbl_PT_width->setText(QApplication::translate("Widget", "The width of pattern:", nullptr));
        PT_height_Edit->setText(QString());
        lbl_PT_height->setText(QApplication::translate("Widget", "The height of pattern :", nullptr));
        path_Edit->setText(QString());
        lbl_PT_height_2->setText(QApplication::translate("Widget", "The path of picture folder :", nullptr));
        puB_saveINI->setText(QApplication::translate("Widget", "save", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("Widget", "INI", nullptr));
        label->setText(QApplication::translate("Widget", "Address\357\274\232", nullptr));
        label_2->setText(QApplication::translate("Widget", "Port\357\274\232", nullptr));
        label_4->setText(QApplication::translate("Widget", "Run Mode\357\274\232", nullptr));
        puB_runMode->setText(QApplication::translate("Widget", "-", nullptr));
        PortEdit->setText(QApplication::translate("Widget", "8501", nullptr));
#ifndef QT_NO_WHATSTHIS
        puB_connect->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        puB_connect->setText(QApplication::translate("Widget", "Connect", nullptr));
        lbl_plc->setText(QString());
        puB_start->setText(QApplication::translate("Widget", "Start", nullptr));
        AddressEdit->setText(QApplication::translate("Widget", "192.168.1.168", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Widget", "Home", nullptr));
        puB_pre->setText(QApplication::translate("Widget", "Previous", nullptr));
        lbl_pic->setText(QString());
        puB_next->setText(QApplication::translate("Widget", "Next", nullptr));
        lbl_Pos->setText(QApplication::translate("Widget", "X = 0 , Y = 0 ", nullptr));
        groupBox_7->setTitle(QApplication::translate("Widget", "Defect point:", nullptr));
        lbl_pattern_4->setText(QApplication::translate("Widget", "Coordinate:", nullptr));
        lbl_pattern_3->setText(QApplication::translate("Widget", "Type :", nullptr));
        lbl_pic2->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = table_defectlist->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Widget", "number", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table_defectlist->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Widget", "Pattern", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table_defectlist->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Widget", "Coordinate", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table_defectlist->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("Widget", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table_defectlist->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("Widget", "defect1", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table_defectlist->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("Widget", "defect2", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_defectlist->verticalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("Widget", "defect3", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table_defectlist->verticalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("Widget", "defect4", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table_defectlist->verticalHeaderItem(4);
        ___qtablewidgetitem8->setText(QApplication::translate("Widget", "defect5", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table_defectlist->verticalHeaderItem(5);
        ___qtablewidgetitem9->setText(QApplication::translate("Widget", "defect6", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table_defectlist->verticalHeaderItem(6);
        ___qtablewidgetitem10->setText(QApplication::translate("Widget", "defect7", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table_defectlist->verticalHeaderItem(7);
        ___qtablewidgetitem11->setText(QApplication::translate("Widget", "defect8", nullptr));
        lbl_pattern_2->setText(QApplication::translate("Widget", "\347\224\242\345\223\201\345\220\215\347\250\261 \357\274\232", nullptr));
        lbl_pattern->setText(QApplication::translate("Widget", "Pattern :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Widget", "Image viewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
