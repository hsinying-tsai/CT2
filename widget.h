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
#include<QTextStream>
#include<QTextBrowser>
#include<ini.h>
#include<QQueue>

QT_BEGIN_NAMESPACE
extern char buffIni[40];
extern char iniFile[20];
extern uint8_t CAM1_parm1,CAM1_parm2;
extern bool RUNMODE_autoRun;
namespace Ui {
class Widget;
}
QT_END_NAMESPACE
class Widget : public QWidget
{
    Q_OBJECT

public:
    Ui::Widget *ui;
    Widget(QWidget *parent = nullptr);
    int i = 0,j = 0,count_num = 0,num = 1, PG_num = 1, time = 0,ARM_posX,ARM_posY,numberPart
    ,count_runModeclickedtime=1,runMode = 0, count_runPG=0,count_runPGflaw=0,delta_ARMposX,delta_ARMposY;
    bool ReadpuB_isPressed = false, WritepuB_isPressed=false, sending_ms = false,sending_pos = false
            ,change_flawPG = false, recevNULL = false,recevZero = false,sendingTime = false;
    Logger logger;
    tcp_client *tc= new tcp_client(nullptr);
    QPixmap pix_Ini,pix2;
    std::vector<std::string> matrix_pattern_name = {"Black1", "Black2","Gray1", "Gray2","White"};
    QString rev_text, str1, str2,stringPart;
    QString DM200,DM202,DM204,DM206,R200,R201,R202,R203,R204,R205,R206,R207;
    QString new_send_data;
    const QByteArray send_data;
    QList<QLineEdit*> lineEdits;
    QVector<QString> orgi_text,new_text;
    QRegularExpression regex;
    QRegularExpressionMatch match;
    QStringList parts,parts_R,parts_DM200;
    QQueue<QString> commandQ;
    QVector<QVector<int>> vector_PG_flaw = {{10,10,20,20,50,50},{30,30},{70,70,90,90}};
    QStringList flawPGs = {"3","5","7"};

    //存文字
    std::vector<QString> matrix_buffer_name = {"DM200", "DM202","DM204", "DM206","R200","R201","R202",
                                                                    "R203","R204","R205","R206","R207"};
    //存變數數值
    std::vector<QString> orig_buffer;
    std::vector<QString> buffer = {DM200,DM202,DM204,DM206,R200,R201,R202,R203,R204,R205,R206,R207};

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


    void on_puB_runMode_clicked();

private:
    QLabel *label;
    QThread clientThread;
    QMutex mu;


};
#endif // WIDGET_H
