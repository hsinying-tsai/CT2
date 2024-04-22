#ifndef WIDGET_H
#define WIDGET_H

#include<QWidget>
#include<QLabel>
#include<QObject>
#include<QPixmap>
#include<vector>
#include<QString>
#include<QHostAddress>
#include"tcp_client.h"
#include<QPaintEvent>
#include<QLineEdit>
#include<QVector>
#include<QThread>
#include<QMutex>
#include"logger.h"
#include<QComboBox>
#include<QDir>
#include<QStringList>
#include<QTextBrowser>
#include<QQueue>
#include<iostream>
#include"defineCoordinate.h"
#include <QListWidget>
#include <QColor>
QT_BEGIN_NAMESPACE
extern char buffIni[40];
extern char iniFile[20];
extern uint8_t CAM1_exposureTime,CAM2_exposureTime,CAM3_exposureTime;
extern uint16_t COORDINATE_PTsX,COORDINATE_PTsY;
extern char picfoldpath[100];
namespace Ui {
class Widget;
class testaddUI;
}
QT_END_NAMESPACE
class Widget : public QWidget
{
    Q_OBJECT

public:
    Ui::Widget *ui;
    Ui::testaddUI *uit;
    Widget(QWidget *parent = nullptr);
    int i = 0,j = 0,count_num = 0,num = 1, PG_num = 1, time = 0,ARM_posX = 0,ARM_posY = 0,numberPart,count_runModeclickedtime=1
            ,runMode = 0;
    double factor_X,factor_Y;
    bool ReadpuB_isPressed = false, WritepuB_isPressed=false, sending_ms = false,sending_pos = false
            ,change_flawPG = false, recevNULL = false,recevZero = false,sendingTime = false;
    Logger logger;
    defineCoordinate DC;
    typedef struct Node node;
    tcp_client *tc= new tcp_client(nullptr);
    QPixmap pix_Ini,pix2;
//    std::vector<std::string> matrix_pattern_name = {"Black1", "Black2","Gray1", "Gray2","White"};
    QString rev_text, str1, str2,stringPart;
    QString DM200,DM202,DM204,DM206,R200,R201,R202,R203,R204,R205,R206,R207,R212,R214;
    QString new_send_data;
    const QByteArray send_data;
    QList<QLineEdit*> lineEdits;
    QVector<QString> orgi_text,new_text;
    QRegularExpression regex;
    QRegularExpressionMatch match;
    //"Black", "White", "Gray1", "Gray2", "Green"
    QStringList show_pattern_name={ "Black", "White", "Gray1", "Gray2", "Green"};
    QStringList parts,parts_R,run_pattern_name;
    QQueue<QString> commandQ;
    QString configFilePath;

    double calculateMean(const QString &imagepath);
    //存文字
    std::vector<QString> matrix_buffer_name = {"DM200", "DM202","DM204","DM206","R200","R201","R202","R203","R204","R205"
                                               ,"R206","R207","R212","R214"};
    //存變數數值
    std::vector<QString> orig_buffer;
    std::vector<QString> buffer = {DM200,DM202,DM204,DM206,R200,R201,R202,R203,R204,R205,R206,R207,R212,R214};
    ~Widget();

private slots:
    void on_puB_pre_clicked();
    void on_puB_next_clicked();
    void on_puB_connect_clicked();
    void MouseCurrentPos();
    void updatelblPic();
    void WR_command(QString WR_message);
    void Qtimer();
    void change_pic2(int index);
    void StartMicroscopic();
    void saveText();
    void comp_text();
    //TCP/IP
    void on_puB_sent_clicked();
    void on_puB_start_clicked();
    void on_puB_read_clicked();
    void on_puB_write_clicked();
    void recv_label_update(QString message);
    void connect_label_update();
    void RD(QString part);
    void INI_UI();

    void on_puB_runMode_clicked();
    void on_puB_saveINI_clicked();
    void on_puB_remove_clicked();
    void reviseconfigINI(QString section, QString key,QString Value);
    void on_puB_add_clicked();
    void on_puB_save_clicked();

private:
    QLabel *label;
    QThread clientThread;
    QMutex mu;
};
#endif // WIDGET_H
