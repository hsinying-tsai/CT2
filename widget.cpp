#include "widget.h"
#include "./ui_widget.h"
#include"my_qlabel.h"
#include<QMouseEvent>
#include<QString>
#include<QDebug>
#include<QWheelEvent>
#include<QLabel>
#include<math.h>
#include<QCursor>
#include"tcp_client.h"
#include<QTimer>
#include<QValidator>
#include<QLineEdit>
#include<QThread>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    logger.create_file();
    logger.populateCombowithFileName(ui->comboBox,"Log");
    connect(ui->lbl_pic,SIGNAL(set_pic2(int)), this, SLOT(change_pic2(int)));
    connect(ui->lbl_pic, SIGNAL(Mouse_Pos()), this, SLOT(MouseCurrentPos()));

    connect(ui->comboBox,QOverload<int>::of(&QComboBox::activated),
            [&](int index){logger.on_comboBox_currentIndexChanged(index,ui->text_log,ui->comboBox,"Log");});
    tc = new tcp_client();
    tc->moveToThread(&clientThread);

    connect(&clientThread, &QThread::finished, tc,&QObject::deleteLater);
    connect(tc,SIGNAL(recv_update(QString)), this, SLOT(recv_label_update(QString)));
    connect(tc,SIGNAL(connect_UIupdate()), this, SLOT(connect_label_update()));
    QList<QLineEdit*> lineEdits = QWidget::findChildren<QLineEdit*>();
//    QVector<QString> orig_lineEdits[30];
//    for(auto lineEdit : lineEdits){
//        orig_lineEdits[i] = lineEdit->text();
//        i++;
//    }

    //    for(auto lineEdit : lineEdits){
//        orig_buffer[i] = lineEdit->text();
//        i++;
//    }
//    connect(ui->puB_save,&QPushButton::clicked,[&](){
//        orig_buffer[j] = lineEdit->text();
//    });

//    for(auto lineEdit : lineEdits){
//        connect(lineEdit,&QLineEdit::textChanged, [=](){
//            QString newText = lineEdit->text();
//            if(newText != orig_buffer[j]){
//                lineEdit->setStyleSheet("color:red");
//            }else{
//                lineEdit->setStyleSheet("color:black");
//            }
//            j++;
//        });
//    }



    pix_Ini.load("/home/agx/Desktop/0321_qt/Pictures/"+QString::number(num)+".bmp");
    ui->lbl_pic->setImage(pix_Ini);
    ui->lbl_pattern->setText("Pattern = "+QString::fromStdString(matrix_pattern_name[num-1]));
    ui->textRecv->setEnabled(false);
    ui->textSend->setEnabled(false);
    ui->puB_sent->setEnabled(false);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Qtimer()));
    timer->start(1000);

    qDebug()<<"CAM1_parm1"<<CAM1_parm1;
    qDebug()<<"CAM1_parm2"<<CAM1_parm2;
    clientThread.start();

}

Widget::~Widget()
{
    tc->client->abort();
    clientThread.quit();;
    clientThread.wait();
    logger.writeLog(Logger::Info,"Ended.");
    delete ui;
}

void Widget::MouseCurrentPos()
{
    ui->lbl_Pos->setText(QString("X = %1, Y = %2").arg(ui->lbl_pic->x).arg(ui->lbl_pic->y));
}
void Widget::updatelblPic()
{
    // WR_command("WR R202 1");
    // num = buffer[1];
    if(num == 0){
        num = 5;
    }else if(num == 6){
        num = 1;
    }
    ui->lbl_pattern->setText("Pattern = "+QString::fromStdString(matrix_pattern_name[num-1]));
    pix_Ini.load("/home/agx/Desktop/0321_qt/Pictures/"+QString::number(num)+".bmp");
    ui->lbl_pic->setImage(pix_Ini);
}
void Widget::on_puB_pre_clicked()
{
//    qDebug()<<CAM1_parm1;
    logger.writeLog(Logger::Info,"User clicled Button puB_pre.");
    //上一張照片
    num--;
    updatelblPic();
    logger.writeLog(Logger::Info,"User changed pattern to " + QString::number(num) + ".");
}

void Widget::on_puB_next_clicked()
{
    logger.writeLog(Logger::Info,"User clicked Button puB_next.");
    //下一張照片
    num++;
    updatelblPic();
    logger.writeLog(Logger::Info,"User changed pattern to " + QString::number(num) + ".");
}

void Widget::recv_label_update(const QString message)
{
    if(message == "SocketError"){
        logger.writeLog(Logger::Warning,"Socket " + tc->client->errorString());
    }else{
        ui->textRecv->append(message);
        logger.writeLog(Logger::Info,"Received message "+message);
        qDebug()<<"12";
        if(ReadpuB_isPressed== true)
        {
            ui->DM200_Edit->setText(QString(buffer[0]));
            if(str1=="RDS R200 8")
            {
                qDebug()<<"RDS R200"<<message;
                QStringList parts = message.split(" ");
                ui->R200_Edit->setText(parts[0]);
                ui->R201_Edit->setText(parts[1]);
                ui->R202_Edit->setText(parts[2]);
                ui->R203_Edit->setText(parts[3]);
                ui->R204_Edit->setText(parts[4]);
                ui->R205_Edit->setText(parts[5]);
                ui->R206_Edit->setText(parts[6]);
                ui->R207_Edit->setText(parts[7]);
                buffer[4] = ui->R200_Edit->text();
                buffer[5] = ui->R201_Edit->text();
                buffer[6] = ui->R202_Edit->text();
                buffer[7] = ui->R203_Edit->text();
                buffer[8] = ui->R204_Edit->text();
                buffer[9] = ui->R205_Edit->text();
                buffer[10] = ui->R206_Edit->text();
                buffer[11] = ui->R207_Edit->text();
                WR_command("RDS DM200 7");
            }else if(str1=="RDS DM200 7"){
                // qDebug()<<"RDS WD200"<<message;
                QStringList parts = message.split(" ");
                ui->DM200_Edit->setText(parts[0]);
                ui->DM202_Edit->setText(parts[2]);
                ui->DM204_Edit->setText(parts[4]);
                ui->DM206_Edit->setText(parts[6]);
                buffer[0] = ui->DM200_Edit->text();
                buffer[1] = ui->DM202_Edit->text();
                buffer[2] = ui->DM204_Edit->text();
                buffer[3] = ui->DM206_Edit->text();
                str1 == "RDS R200 8";
                ReadpuB_isPressed = false;
                logger.writeLog(Logger::Info,"Read PLC index.");
             }
        }else if(WritepuB_isPressed == true){
            if(count_num == 8){
                WritepuB_isPressed = false;
                logger.writeLog(Logger::Info,"Wrote PLC index.");
                count_num = 0;
                //OK\r\n
            }else if(message == "OK"){
                QString buffer_combined = QString("%1 %2%3 %4").arg("WR").arg("DM").arg(200+count_num).arg(buffer[count_num/2]);
                WR_command(buffer_combined);
                qDebug()<<"DM20"<<count_num;
                count_num += 2;
            }
        }else if(sending_pos == true){
            if(message == "OK"){
                qDebug()<<"Sending y ="<<ARM_posY;
                QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM206").arg(ARM_posY);
                WR_command(buffer_combined);
                buffer[10] = "0";
                buffer[11] = "0";
                logger.writeLog(Logger::Info,"Edge reset R206 and R207.");
                qDebug()<<"將R206、R207歸零\n"<<"R206:"<<QString(buffer[10])<<"R207"<<QString(buffer[11]);
                sending_pos = false;
            }
        }else{
            //receive single command
            QStringList parts = message.split(" ");
            qDebug()<<parts[0];
            qDebug()<<parts[1];
            for(int i = 0;i<12;i++){
                if(matrix_buffer_name[i] == parts[0]){
                    buffer[i] = parts[1];
                }
            }
        }
        if(buffer[5] == "1"){
            //R200、R201
            buffer[4] = "0";
            buffer[5] = "0";
            logger.writeLog(Logger::Info,"Edge reset R200 and R201.");
            qDebug()<<"server已回應OK，並已將R200、R201歸零\n"<<"R200:"<<QString(buffer[4])<<"R201:"<<QString(buffer[5]);
            // WR_commandg("WRS R200 2 0 0");
        }
        if(buffer[7]== "1"){
            //R202、R203 ->change pattern
            buffer[1] = QString::number(PG_num);
            qDebug()<<"切換至 pattern = " + buffer[1] + " 的照片";
            buffer[6] = "0";
            buffer[7] = "0";
            logger.writeLog(Logger::Info,"Edge reset R202 and R203.");
            qDebug()<<"server已回應OK，並將R202、R203歸零\n"<<"R202:"<<QString(buffer[6])<<"R203:"<<QString(buffer[7]);
        }
        if(buffer[9]== "1"){
            //R204、R205 ->open microscopic photography
            StartMicroscopic();
            buffer[8] = "0";
            buffer[9] = "0";
            logger.writeLog(Logger::Info,"Edge reset R204 and R205.");
            qDebug()<<"server已回應OK，並將R204、R205歸零\n"<<"R204:"<<QString(buffer[8])<<"R205"<<QString(buffer[9]);
        }
        if(buffer[11] == "1"){
            //R207 ON ->record X,Y in DM204,DM206
            ARM_posX = 10;
            ARM_posY = 10;
            qDebug()<<"Sendind x = "<< ARM_posX;
            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
            WR_command(buffer_combined);
            sending_pos = true;
            qDebug()<<sending_pos;
        }
    }
}

void Widget::connect_label_update()
{
    qDebug()<<"13";

    if(tc->client->state()==QAbstractSocket::ConnectedState){
        setWindowTitle(QString("Client[%1:%2]")
                           .arg(tc->client->localAddress().toString())
                           .arg(tc->client->localPort()));
    }else{
        setWindowTitle("Client");
    }
    //connect state
    if(tc->connnect_state == 1){
        qDebug()<<"14";
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



    }else if(tc->connnect_state == 0){
        qDebug()<<"15";
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

void Widget::on_puB_sent_clicked()
{
    logger.writeLog(Logger::Info,"User clicled Button puB_sent.");
    sending_ms= true;
    const QByteArray send_data = ui->textSend->toPlainText().toUtf8();
    WR_command(send_data);
}
void Widget::WR_command(QString WR_message)
{
    logger.writeLog(Logger::Info,"User sent Message'"+WR_message+"'.");
    sending_ms = true;
    str1 = WR_message;
    const QByteArray send_data = str1.toUtf8();
    if(send_data.isEmpty()){
        return;
    }
    new_send_data = QString("%1\r").arg(QString::fromUtf8(send_data));
    // qDebug()<<QString("%1\r").arg(send_data);
    tc->client->write(new_send_data.toUtf8());
    sending_ms = false;
    if(str1=="WR R202 1"){
        //R202 ON
        buffer[1] = QString::number(PG_num);
        QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(buffer[1]);
        WR_command(buffer_combined);
        PG_num++;
    }

}
void Widget::on_puB_connect_clicked()
{
    logger.writeLog(Logger::Info,"User clicked ButtonpuB_connect.");
    qDebug()<<"17";
    //初始化計時器
    time = 0;
    // 判斷當前是否有連接，連接了就斷開
    if(tc->client->state()==QAbstractSocket::ConnectedState){
        qDebug()<<"4";
    }else if(tc->client->state()==QAbstractSocket::UnconnectedState){
        qDebug()<<"2";
        tc->address = QHostAddress(ui->AddressEdit->text());
        tc->port= ui->PortEdit->text().toUShort();
    }
    tc->initClent();
}


void Widget::Qtimer()
{
     buffer[0] = QString::number(time);
     if(tc->client->state()==QAbstractSocket::ConnectedState)
     {
         if(sending_ms==false){
             QString command = QString("%1 %2").arg("WR DM200").arg(time);
             WR_command(command);
             time++;
             buffer[0] = QString::number(time);
         }else{
             time++;
             buffer[0] = QString::number(time);
         }
     }
}

void Widget::change_pic2(int index)
{
    logger.writeLog(Logger::Info,"User changed pic2.");
    pix2.load("/home/agx/Desktop/0321_qt/Pictures/"+QString::number(num)+"_"+QString::number(index)+".bmp");
    ui->lbl_pic2->setPixmap(pix2);

}

void Widget::on_puB_changePG_clicked()
{
    logger.writeLog(Logger::Info,"User clicked Button puB_changePG.");
    WR_command("WR R202 1");
}

void Widget::StartMicroscopic()
{
    logger.writeLog(Logger::Info,"Start to Microscopic Photography.");
}

void Widget::on_puB_start_clicked()
{
    logger.writeLog(Logger::Info,"User clicked Button puB_start.");
    WR_command("WR R200 1");
}

void Widget::on_puB_read_clicked()
{
    logger.writeLog(Logger::Info,"User clicked Button puB_read.");
    ReadpuB_isPressed = true;
    WR_command("RDS R200 8");
}

void Widget::on_puB_write_clicked()
{
    logger.writeLog(Logger::Info,"User clicked Button puB_write.");
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
    buffer[11] = ui->R207_Edit->text();

    QString buffer_combined = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").arg("WRS").arg("R200").arg("8").arg(buffer[4])
                                  .arg(buffer[5]).arg(buffer[6]).arg(buffer[7]).arg(buffer[8])
                                  .arg(buffer[9]).arg(buffer[10]).arg(buffer[11]);
    WR_command(buffer_combined);
}
