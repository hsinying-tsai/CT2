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
QT_BEGIN_NAMESPACE

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
    int num = 1, time = 0;
    bool ReadpuB_isPressed = false, WritepuB_isPressed=false, sending_ms = false;
    Logger logger;
    tcp_client *tc;
    QPixmap pix_Ini,pix2;
    std::vector<std::string> matrix_pattern_name = {"Black1", "Black2","Gray1", "Gray2","White"};
    QString rev_text, str1;
    QString DM200,DM202,DM204,DM206,R200,R201,R202,R203,R204,R205,R206,R207;
    QString new_send_data;
    const QByteArray send_data;

    //存文字
    std::vector<QString> matrix_buffer_name = {"DM200", "DM202","DM204", "DM206","R200","R201","R202",
                                                                    "R203","R204","R205","R206","R207"};
    //存變數數值
    std::vector<QString> buffer = {DM200,DM202,DM204,DM206,R200,R201,R202,R203,R204,R205,R206,R207};

    ~Widget();

private slots:
    void on_puB_pre_clicked();
    void on_puB_next_clicked();
    void on_puB_connect_clicked();
    void MouseCurrentPos();
    void updatelblPic();
    void WR_command(QString WR_messenge);
    void Qtimer();
    void change_pic2(int index);

    //TCP/IP
    void on_puB_sent_clicked();
    void on_puB_start_clicked();
    void on_puB_read_clicked();
    void on_puB_write_clicked();
    void recv_label_update(const QString message);
    void connect_label_update();


private:
    QLabel *label;
    QThread clientThread;
    QMutex mu;


};
#endif // WIDGET_H
