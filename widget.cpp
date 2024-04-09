#include "widget.h"
#include <QCursor>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QValidator>
#include <QWheelEvent>
#include "./ui_widget.h"
#include "my_qlabel.h"
#include "tcp_client.h"
#include <math.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    logger.create_file();
    logger.populateCombowithFileName(ui->comboBox, "Log");
    connect(ui->lbl_pic, SIGNAL(set_pic2(int)), this, SLOT(change_pic2(int)));
    connect(ui->lbl_pic, SIGNAL(Mouse_Pos()), this, SLOT(MouseCurrentPos()));

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated), [&](int index) {
        logger.on_comboBox_currentIndexChanged(index, ui->text_log, ui->comboBox, "Log");
    });
    //    tc = new tcp_client();
    tc->moveToThread(&clientThread);

    connect(&clientThread, &QThread::finished, tc, &QWidget::deleteLater);
    connect(tc, SIGNAL(recv_update(QString)), this, SLOT(recv_label_update(QString)));
    connect(tc, SIGNAL(connect_UIupdate()), this, SLOT(connect_label_update()));
    lineEdits = QWidget::findChildren<QLineEdit *>();
    for (auto lineEdit : lineEdits) {
        new_text.append(lineEdit->text());
        orgi_text.append(lineEdit->text());
    }
    connect(ui->puB_read, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_write, &QPushButton::clicked, this, &Widget::saveText);
    for (auto lineEdit : lineEdits) {
        connect(lineEdit, &QLineEdit::textChanged, this, &Widget::comp_text);
    }
    pix_Ini.load("/home/agx/Desktop/0321_qt/Pictures/" + QString::number(num) + ".bmp");
    ui->lbl_pic->setImage(pix_Ini);
    ui->lbl_pattern->setText("Pattern = " + QString::fromStdString(matrix_pattern_name[num - 1]));
    ui->textRecv->setEnabled(false);
    ui->textSend->setEnabled(false);
    ui->puB_sent->setEnabled(false);

    QTimer *timer = new QTimer(this);
    QTimer *timer_RD = new QTimer(this);
    QTimer *timer_WR = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Qtimer()));

    connect(timer_RD,&QTimer::timeout,this,[this](){
        if(parts_R.size()>1){
            RD(parts_R[1]);
        }else{

        }
    });
    connect(timer_WR,&QTimer::timeout,this,[this](){
        if(recevZero==true){
            WR_command(str1);
        }

    });
    timer->start(1000);
    timer_RD->start(900);
    timer_WR->start(900);
    qDebug() << "CAM1_parm1" << CAM1_parm1;
    qDebug() << "CAM1_parm2" << CAM1_parm2;
    qDebug() << "RUNMODE_autoRun" << RUNMODE_autoRun;
    clientThread.start();
}

Widget::~Widget()
{
    tc->client->abort();
    clientThread.quit();
    clientThread.wait();
    logger.writeLog(Logger::Info, "Ended.");
    delete ui;
}

void Widget::MouseCurrentPos()
{
    ui->lbl_Pos->setText(QString("X = %1, Y = %2").arg(ui->lbl_pic->x).arg(ui->lbl_pic->y));
}
void Widget::updatelblPic()
{
    if (num == 0) {
        num = 5;
    } else if (num == 6) {
        num = 1;
    }
    ui->lbl_pattern->setText("Pattern = " + QString::fromStdString(matrix_pattern_name[num - 1]));
    pix_Ini.load("/home/agx/Desktop/0321_qt/Pictures/" + QString::number(num) + ".bmp");
    ui->lbl_pic->setImage(pix_Ini);
}

void Widget::on_puB_sent_clicked()
{
    logger.writeLog(Logger::Info, "User clicled Button puB_sent.");
    sending_ms = true;
    const QByteArray send_data = ui->textSend->toPlainText().toUtf8();
    WR_command(send_data);
}
void Widget::on_puB_pre_clicked()
{
    //    qDebug()<<CAM1_parm1;
    logger.writeLog(Logger::Info, "User clicled Button puB_pre.");
    //上一張照片
    num--;
    updatelblPic();
    logger.writeLog(Logger::Info, "User changed pattern to " + QString::number(num) + ".");
}

void Widget::on_puB_next_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_next.");
    //下一張照片
    num++;
    updatelblPic();
    logger.writeLog(Logger::Info, "User changed pattern to " + QString::number(num) + ".");
}

void Widget::recv_label_update(QString message)
{
    message = message.replace("\r\n","");
    qDebug()<<"received:"<<message<<"\tfrom"<<str1<<"\n";
    ARM_posX = 10;
    ARM_posY = 10;
    recevZero = false;
    if (message == "SocketError") {
        logger.writeLog(Logger::Warning, "Socket " + tc->client->errorString());
    }else if(message == "0"){
        //0\r\n
        recevZero = true;
    }else {
        ui->textRecv->append(message);
        logger.writeLog(Logger::Info, "Received message " + message);
        //Read
        if (ReadpuB_isPressed == true) {
            ui->DM200_Edit->setText(QString(buffer[0]));
            if (str1 == "RDS R200 7") {
                QStringList parts = message.split(" ");
                ui->R200_Edit->setText(parts[0]);
                ui->R201_Edit->setText(parts[1]);
                ui->R202_Edit->setText(parts[2]);
                ui->R203_Edit->setText(parts[3]);
                ui->R204_Edit->setText(parts[4]);
                ui->R205_Edit->setText(parts[5]);
                ui->R206_Edit->setText(parts[6]);
                buffer[4] = ui->R200_Edit->text();
                buffer[5] = ui->R201_Edit->text();
                buffer[6] = ui->R202_Edit->text();
                buffer[7] = ui->R203_Edit->text();
                buffer[8] = ui->R204_Edit->text();
                buffer[9] = ui->R205_Edit->text();
                buffer[10] = ui->R206_Edit->text();
                WR_command("RDS DM200 7");
            } else if (str1 == "RDS DM200 7") {
                QStringList parts = message.split(" ");
                ui->DM200_Edit->setText(parts[0]);
                ui->DM202_Edit->setText(parts[2]);
                ui->DM204_Edit->setText(parts[4]);
                ui->DM206_Edit->setText(parts[6]);
                buffer[0] = ui->DM200_Edit->text();
                buffer[1] = ui->DM202_Edit->text();
                buffer[2] = ui->DM204_Edit->text();
                buffer[3] = ui->DM206_Edit->text();
                ReadpuB_isPressed = false;
                qDebug()<<"readAll";
//                str1 == "RDS R200 7";
                logger.writeLog(Logger::Info, "Read PLC index.");
            }
        //Write
        } else if (WritepuB_isPressed == true) {
            if (count_num == 8) {
                WritepuB_isPressed = false;
                logger.writeLog(Logger::Info, "Wrote PLC index.");
                count_num = 0;
                //OK\r\n
            } else if (message == "OK") {
                QString buffer_combined = QString("%1 %2%3 %4")
                                              .arg("WR")
                                              .arg("DM")
                                              .arg(200 + count_num)
                                              .arg(buffer[count_num / 2]);
                WR_command(buffer_combined);
                count_num += 2;
            }
        } else if (message == "OK") {
            recevZero = false;
            // WR事件
            parts = str1.split(" ");
            if(parts[1]=="DM200"){
//                qDebug()<<"alive";
            }else if(parts[1] == "R202"){
                parts_R[1] = "R202";
                recevNULL = true;
            }else if(change_flawPG == true){
                // change flaw pattern
                QStringList flawPGs = {"3"};
                if(count_PG<flawPGs.size()){
                    qDebug()<<"--------------Step_5.The falw PG is pattern"<<flawPGs.at(count_PG);
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(flawPGs.at(count_PG));
                    WR_command(buffer_combined);
                    count_PG++;
                }else{
                    qDebug()<<"--------------Step_5.All flaw pattern was changed.";
                    WR_command("WR R202 1");
                }
            }else if(parts[1] == "DM202"){
                WR_command("WR R202 1");
            }else if(parts[1] == "DM204"){
                if(sending_pos == true){
                    qDebug() << "--------------Step_6.Sending y = " << ARM_posY;
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM206").arg(ARM_posY);
                    parts[1] = "DM206";
                    WR_command(buffer_combined);
                }

            }else if(parts[1] == "DM206"){
                if(sending_pos == true){
                    sending_pos = false;
                    WR_command("WR R206 1");
                    isEnding = true;
                }
            }else if(parts[1] == "R206"){
                parts_R[1] = "R204";
                recevNULL = true;
            }else{
                if(parts[0]=="RD"){
                    recevNULL = true;
                }else{
                    //R201,R203,R205
                    recevNULL = true;
                    parts_R = str1.split(" ");
                }
            }
        } else{
            recevNULL = false;
            // RD事件
            QStringList parts = message.split(" ");
            if (parts.size() == 1) {
                QStringList parts = str1.split(" ");
                for (int i = 0; i < 12; i++) {
                    if (matrix_buffer_name[i] == parts[1]) {
                        buffer[i] = message;
                    }
                }
            } else {
                qDebug() <<"Invalid commend";
                ui->textRecv->append("Invalid commend");
            }
        }


        if (buffer[5] == "1") {
            //R200、R201
            buffer[4] = "0";
            buffer[5] = "0";
            logger.writeLog(Logger::Info, "Edge reset R200 and R201.");
            qDebug() << "--------------Step_2.server已回應OK，並已將R200、R201歸零\n";

            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(PG_num);
            WR_command(buffer_combined);
            qDebug()<<"--------------Step_3.change PG_num to"<<PG_num;
            PG_num++;

        }


        if (buffer[7] == "1") {
            //R202、R203 ->change pattern
            buffer[6] = "0";
            buffer[7] = "0";
            logger.writeLog(Logger::Info, "Edge reset R202 and R203.");
            qDebug() << "--------------Step_3.server已回應OK，並將R202、R203歸零\n";
            if(change_flawPG == true){
                sending_pos = true;
                qDebug() << "--------------Step_6.Sending x = " << ARM_posX;
                QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
                WR_command(buffer_combined);
                change_flawPG = false;
            }else{
                if(PG_num<3){
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(PG_num);
                    WR_command(buffer_combined);
                    qDebug()<<"--------------Step_3.change PG_num to"<<PG_num;
                    PG_num++;
                }else{
                    qDebug() << "All pattern was changed.";
                    WR_command("WR R204 1");
                }
            }


        }
        if (buffer[9] == "1"){
            //R205 1
            if(isEnding == true){
                buffer[9] = "0";
                qDebug()<<"--------------Step_7.Finish.";
//                  WR_command("WR R205 0");
                isEnding = false;
            }else{
                //R204、R205 ->open microscopic photography
                StartMicroscopic();
                buffer[8] = "0";
                buffer[9] = "0";
                logger.writeLog(Logger::Info, "Edge reset R204 and R205.");
                qDebug() << "--------------Step_4.server已回應OK，並將R204、R205歸零\n";
                WR_command("WR R205 0");

            }
        }
    }
}
void Widget::RD(QString part)
{
    if(recevZero == false){
        if(recevNULL == true){
            //RD R201、RD R203、RD R205
            QRegularExpression regex("(\\D+)(\\d+)");
            match = regex.match(part);
            if(match.hasMatch()){
                stringPart = match.captured(1);
                numberPart = match.captured(2).toInt();
                QString buffer_combined = QString("%1 %2%3").arg("RD").arg(stringPart).arg(numberPart+1);
                WR_command(buffer_combined);
            }else{
                qDebug()<<"RD()_ERROR:No match found.";
            }
        }
    }

}

void Widget::connect_label_update()
{
    if (tc->client->state() == QAbstractSocket::ConnectedState) {
        setWindowTitle(QString("Client[%1:%2]")
                           .arg(tc->client->localAddress().toString())
                           .arg(tc->client->localPort()));
    } else {
        setWindowTitle("Client");
    }
    //connect state
    if (tc->connnect_state == 1) {
        qDebug() << "--------------Step_1.Connection Successful";
        ui->textRecv->clear();
        ui->textSend->clear();
        ui->textRecv->setText("Socket connect.");
        //連線成功

        ui->lbl_plc->setStyleSheet("border-width: 3px;"
                                   "border-radius: 20px;"
                                   "margin:10px;"
                                   "padding:20px;"
                                   "background-color: green;");
        ui->puB_connect->setText("Disconnect");
        ui->AddressEdit->setEnabled(false);
        ui->PortEdit->setEnabled(false);
        ui->textRecv->setEnabled(true);
        ui->textSend->setEnabled(true);
        ui->puB_sent->setEnabled(true);

    } else if (tc->connnect_state == 0) {
        PG_num = 1;
        //連線失敗
        ui->textRecv->clear();
        ui->textSend->clear();
        ui->textRecv->setText("Socket unconnected.");
        ui->lbl_plc->setStyleSheet("border-width: 3px;"
                                   "border-radius: 20px;"
                                   "margin:10px;"
                                   "padding:20px;"
                                   "background-color: red;");
        ui->puB_connect->setText("Connect");
        ui->AddressEdit->setEnabled(true);
        ui->PortEdit->setEnabled(true);
        ui->textRecv->setEnabled(false);
        ui->textSend->setEnabled(false);
        ui->puB_sent->setEnabled(false);
    }
}


void Widget::WR_command(QString WR_message)
{
    if (tc->connnect_state == 1) {
        qDebug()<<WR_message;
        logger.writeLog(Logger::Info, "User sent Message'" + WR_message + "'.");
        sending_ms = true;
        str1 = WR_message;
        if (str1 == "WR R205 0") {
            change_flawPG = true;
        }
        const QByteArray send_data = str1.toUtf8();
        if (send_data.isEmpty()) {
            return;
        }
        new_send_data = QString("%1\r").arg(QString::fromUtf8(send_data));
        tc->client->write(new_send_data.toUtf8());
        sending_ms = false;
    }
}

void Widget::on_puB_connect_clicked()
{
    logger.writeLog(Logger::Info, "User clicked ButtonpuB_connect.");
    //初始化計時器
    time = 0;
    // 判斷當前是否有連接，連接了就斷開
    if (tc->client->state() == QAbstractSocket::ConnectedState) {
    } else if (tc->client->state() == QAbstractSocket::UnconnectedState) {
        tc->address = QHostAddress(ui->AddressEdit->text());
        tc->port = ui->PortEdit->text().toUShort();
    }
    tc->initClent();
}

void Widget::Qtimer()
{
    buffer[0] = QString::number(time);
    time++;
    if (tc->client->state() == QAbstractSocket::ConnectedState) {
        if (sending_ms == false) {
            if(ReadpuB_isPressed == false){
                QString command = QString("%1 %2").arg("WR DM200").arg(time);
                WR_command(command);
            }
        }
    }
}

void Widget::change_pic2(int index)
{
    logger.writeLog(Logger::Info, "User changed pic2.");
    pix2.load("/home/agx/Desktop/0321_qt/Pictures/" + QString::number(num) + "_"
              + QString::number(index) + ".bmp");
    ui->lbl_pic2->setPixmap(pix2);
}

void Widget::StartMicroscopic()
{
    logger.writeLog(Logger::Info, "Start to Microscopic Photography.");
}

void Widget::saveText()
{
    orgi_text.clear();
    for (auto lineEdit : lineEdits) {
        orgi_text.append(lineEdit->text());
    }
}

void Widget::comp_text()
{
    new_text.clear();
    for (auto lineEdit : lineEdits) {
        new_text.append(lineEdit->text());
    }
    for (int i = 0; i < 14; i++) {
        if (new_text.at(i) != orgi_text.at(i)) {
            lineEdits.at(i)->setStyleSheet("color:red");
        } else {
            lineEdits.at(i)->setStyleSheet("color:black");
        }
    }
}


void Widget::on_puB_start_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_start.");

    WR_command("WR R200 1");
}
void Widget::on_puB_read_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_read.");
    ReadpuB_isPressed = true;

    WR_command("RDS R200 7");
}

void Widget::on_puB_write_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_write.");
    WritepuB_isPressed = true;
    buffer[0] = ui->DM200_Edit->text();
    buffer[1] = ui->DM202_Edit->text();
    buffer[2] = ui->DM204_Edit->text();
    buffer[3] = ui->DM206_Edit->text();
    buffer[4] = ui->R200_Edit->text();
    buffer[5] = ui->R201_Edit->text();
    buffer[6] = ui->R202_Edit->text();
    buffer[7] = ui->R203_Edit->text();
    buffer[8] = ui->R204_Edit->text();
    buffer[9] = ui->R205_Edit->text();
    buffer[10] = ui->R206_Edit->text();
    for(int i = 0 ; i<12 ; i++){
        if(buffer[i].isEmpty()){
            buffer[i]="0";
        }
    }
    QString buffer_combined = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
                                  .arg("WRS")
                                  .arg("R200")
                                  .arg("7")
                                  .arg(buffer[4])
                                  .arg(buffer[5])
                                  .arg(buffer[6])
                                  .arg(buffer[7])
                                  .arg(buffer[8])
                                  .arg(buffer[9])
                                  .arg(buffer[10]);

    WR_command(buffer_combined);
}
