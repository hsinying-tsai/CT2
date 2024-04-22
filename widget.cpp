#include "widget.h"
#include <QCursor>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QThread>
#include <QTimer>
#include <QValidator>
#include <QWheelEvent>
#include "./ui_widget.h"
#include <math.h>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QTableWidget>
#include <QSettings>
#include <QImage>
#include<QDateTime>
#include<QTextStream>
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

    INI_UI();

    lineEdits = QWidget::findChildren<QLineEdit *>();
    for (auto lineEdit : lineEdits) {
        new_text.append(lineEdit->text());
        orgi_text.append(lineEdit->text());
    }
    for (auto lineEdit : lineEdits) {
        connect(lineEdit, &QLineEdit::textChanged, this, &Widget::comp_text);
    }

    // clock (per second
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Qtimer()));
    timer->start(1000);

    // persist send RD until receive 1
    QTimer *timer_RD = new QTimer(this);
    connect(timer_RD,&QTimer::timeout,this,[this](){
        if(parts_R.size()>1){
            RD(parts_R[1]);
        }
    });
    timer_RD->start(900);

    // persist send WR until receive OK
    QTimer *timer_WR = new QTimer(this);
    connect(timer_WR,&QTimer::timeout,this,[this](){
        if(recevZero==true){
            WR_command(str1);
        }
    });
    timer_WR->start(900);

//    qDebug()<<"CAM1_parm1 :"<< CAM1_parm1;
//    qDebug()<<"CAM1_parm2 :"<< CAM1_parm2;
//    qDebug()<<"COORDINATE_PTsX :"<<COORDINATE_PTsX;
//    qDebug()<<"COORDINATE_PTsY :"<<COORDINATE_PTsY;

    // pixel size = 3840X2160, label size = 575X324
    // calculate the factor between pixel and move distance(MD)
    factor_X = COORDINATE_PTsX/576;
    factor_Y = COORDINATE_PTsY/324;
    DC.defNode();
    clientThread.start();
}

double Widget::calculateMean(const QString &imagepath)
{
    QImage image(imagepath);
    if(image.isNull()){
        qWarning()<<"Failed to load image!";
        return -1;
    }
    int width = image.width();
    int height = image.height();
    int totalPixels = width*height;
    int totalGrayValue = 0;
    for(int y=0; y<height ; y++){
        for(int x=0; x<width; x++){
            QColor color = image.pixelColor(x,y);
            int grayValue = qGray(color.red(), color.green(),color.blue());
            totalGrayValue += grayValue;
        }
    }
    double mean = static_cast<double>(totalGrayValue) /totalPixels;
    return mean;

}
void Widget::INI_UI()
{
    connect(&clientThread, &QThread::finished, tc, &QWidget::deleteLater);
    connect(tc, SIGNAL(recv_update(QString)), this, SLOT(recv_label_update(QString)));
    connect(tc, SIGNAL(connect_UIupdate()), this, SLOT(connect_label_update()));
    ui->PT_width_Edit->setText(QString("%1").arg(COORDINATE_PTsX));
    ui->PT_height_Edit->setText(QString("%1").arg(COORDINATE_PTsY));
    ui->path_Edit->setText(QString("%1").arg(picfoldpath));
    connect(ui->puB_read, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_write, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_saveINI, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_connect, &QPushButton::clicked, this, &Widget::saveText);
    configFilePath = QCoreApplication::applicationDirPath() + "/config.ini";
    // 检查配置文件是否存在
    QFile configFile(configFilePath);
    if (!configFile.exists()) {
        QSettings settings(configFilePath, QSettings::IniFormat);
        settings.beginGroup("CAM1");
        settings.setValue("exposureTime", 1);
        settings.setValue("parmeter2", 1);
        settings.setValue("parmeter3", 0);
        settings.setValue("parmeter4", 0);
        settings.setValue("parmeter5", 0);
        settings.endGroup();

        settings.beginGroup("CAM2");
        settings.setValue("exposureTime", 2);
        settings.setValue("parmeter2", 0);
        settings.setValue("parmeter3", 0);
        settings.setValue("parmeter4", 0);
        settings.setValue("parmeter5", 0);
        settings.endGroup();

        settings.beginGroup("CAM3");
        settings.setValue("exposureTime", 3);
        settings.setValue("parmeter2", 0);
        settings.setValue("parmeter3", 0);
        settings.setValue("parmeter4", 0);
        settings.setValue("parmeter5", 0);
        settings.endGroup();

        settings.beginGroup("COORDINATE");
        settings.setValue("PT_sizeX", 1152);
        settings.setValue("PT_sizeY", 648);
        settings.endGroup();

        settings.beginGroup("PICTURE");
        settings.setValue("pic_fold_path", "Pictures/");
        settings.endGroup();
        qDebug() << "配置文件已创建： " << configFilePath;
    } else {
        qDebug() << "配置文件已存在： " << configFilePath;
    }

    pix_Ini.load(picfoldpath + QString::number(num) + ".bmp");
//    pix_Ini.load("Pictures/" + QString::number(num) + ".bmp");
    ui->lbl_pic->setImage(pix_Ini);
    ui->lbl_pattern->setText("Pattern = " + QString(show_pattern_name.at(num - 1)));

    ui->table_defectlist->verticalHeader()->setDefaultSectionSize(30);
    ui->table_defectlist->horizontalHeader()->setDefaultSectionSize(100);
    ui->textRecv->setEnabled(false);
    ui->textSend->setEnabled(false);
    ui->puB_sent->setEnabled(false);
    ui->puB_runMode->setEnabled(false);
    ui->puB_start->setEnabled(false);
    ui->DM202_Edit->setPlaceholderText(" Pattern number");
    ui->DM204_Edit->setPlaceholderText(" X");
    ui->DM206_Edit->setPlaceholderText(" Y");
    ui->CAM1_exposure_Edit_3->setText(QString::number(CAM1_exposureTime));
    ui->CAM2_exposure_Edit_3->setText(QString::number(CAM2_exposureTime));
    ui->CAM3_exposure_Edit_3->setText(QString::number(CAM3_exposureTime));
    ui->list_Pattern->setSpacing(5);

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
    //revise pic amount
    if (num == 0) {
        num = 3;
    } else if (num == 4) {
        num = 1;
    }
    ui->lbl_pattern->setText("Pattern = " + QString(show_pattern_name.at(num - 1)));
    pix_Ini.load(picfoldpath + QString::number(num) + ".bmp");
//    pix_Ini.load("/home/agx/Desktop/0321_qt/Pictures/" + QString::number(num) + ".bmp");
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
    factor_X = COORDINATE_PTsX/576;
    factor_Y = COORDINATE_PTsY/324;
    message = message.replace("\r\n","");
//    ui->textRecv->append("received:"+message+"\tfrom: "+str1);
    qDebug()<<"received:"<<message<<"\tfrom"<<str1;
    recevZero = false;
    if (message == "SocketError"){
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
            if (str1 == "RDS R200 10") {
                parts = message.split(" ");
                ui->R200_Edit->setText(parts[0]);
                ui->R201_Edit->setText(parts[1]);
                ui->R202_Edit->setText(parts[2]);
                ui->R203_Edit->setText(parts[3]);
                ui->R204_Edit->setText(parts[4]);
                ui->R205_Edit->setText(parts[5]);
                ui->R206_Edit->setText(parts[6]);
                ui->R207_Edit->setText(parts[7]);
                ui->R212_Edit->setText(parts[8]);
                ui->R214_Edit->setText(parts[9]);
                buffer[4] = ui->R200_Edit->text();
                buffer[5] = ui->R201_Edit->text();
                buffer[6] = ui->R202_Edit->text();
                buffer[7] = ui->R203_Edit->text();
                buffer[8] = ui->R204_Edit->text();
                buffer[9] = ui->R205_Edit->text();
                buffer[10] = ui->R206_Edit->text();
                buffer[11] = ui->R207_Edit->text();
                buffer[12] = ui->R212_Edit->text();
                buffer[13] = ui->R214_Edit->text();
                WR_command("RDS DM200 7");
            } else if (str1 == "RDS DM200 7") {
                parts = message.split(" ");
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
                if(count_num == 4 || count_num == 6){
                    //DM204,DM206
                    bool ok;
                    int tmp_buffer= buffer[count_num / 2].toInt(&ok);
                    if(ok){
                        if(tmp_buffer<0){
                            qDebug()<<"got it";
                            QString buffer_combined = QString("%1 %2%3%4 %5")
                                                          .arg("WR")
                                                          .arg("DM")
                                                          .arg(200 + count_num)
                                                          .arg(".S")
                                                          .arg(buffer[count_num / 2]);
                            WR_command(buffer_combined);
                            count_num += 2;
                        }
                    }
                }else{
                    QString buffer_combined = QString("%1 %2%3 %4")
                                                  .arg("WR")
                                                  .arg("DM")
                                                  .arg(200 + count_num)
                                                  .arg(buffer[count_num / 2]);
                    WR_command(buffer_combined);
                    count_num += 2;
                }

            }
        } else if (message == "OK") {
            recevZero = false;
            // WR事件
            parts = str1.split(" ");
            if(runMode == 1 && parts[0]=="WRS"){
                if(parts[1]== "R200"){
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(PG_num);
                    WR_command(buffer_combined);
                    qDebug()<<"--------------Step_3.change PG_num to"<<PG_num;
                    PG_num++;
                }else if(parts[1] == "R202"){
                    if(change_flawPG == true){
                        if(DC.current == DC.first){
                            qDebug()<<"first run need to plus compensate";
                            ARM_posX =(DC.current->x - DC.center_posX)*factor_X;
                            ARM_posY =(DC.current->y - DC.center_posY)*factor_Y;
                        }else{
                            ARM_posX = (DC.current->x - DC.current->prev->x)*factor_X;
                            ARM_posY = (DC.current->y - DC.current->prev->y)*factor_Y;
//                            ARM_posX = (DC.current->x)*factor_X;
//                            ARM_posY = (DC.current->y)*factor_Y;
                        }
                        qDebug() << "--------------Step_6.server已回應OK.\n";
                        qDebug() << "--------------Step_6.Sending x = " << ARM_posX;
                        QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
                        WR_command(buffer_combined);
                    }else{
                        //assume 2 pieces of pattern
                        if(PG_num<3){
                            //To the next PG_num
                            qDebug() << "--------------Step_3.server已回應OK.\n";
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(PG_num);
                            WR_command(buffer_combined);
                            qDebug()<<"--------------Step_3.change PG_num to"<<PG_num;
                            PG_num++;
                        }else{
                            qDebug() << "--------------Step_3.All pattern was changed.";
                            WR_command("WR R204 1");
                        }
                    }
                }else if(parts[1] == "R204"){
                    //if change_flawPG==true ->to step.5
                    change_flawPG = true;
                    QString buffer_combined = QString("%1 %2").arg("WR DM202").arg(DC.current->index);
                    WR_command(buffer_combined);
                }else if(parts[1] == "R206"){
                    if(DC.current->next == NULL){
                        //change all flaw pattern and sent all flaws->go to step7
                        change_flawPG = false;
                        WR_command("WR R214 1");
                        qDebug()<<"--------------Step_7.send end signal to PLC.";
                    }else{
                        DC.current = DC.current->next;
                        if(DC.current->index == DC.current->prev->index){
                            //don't change pattern
                            ARM_posX = (DC.current->x - DC.current->prev->x)*factor_X;
                            ARM_posY = (DC.current->y - DC.current->prev->y)*factor_Y;
                            qDebug() << "--------------Step_6.Sending x = " << ARM_posX;
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
                            WR_command(buffer_combined);
                        }else{
                            // change pattern
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(DC.current->index);
                            WR_command(buffer_combined);
                        }
                    }
                }else{
                    //receive OK from "WRS DM201 6 0 0 0 0 0 0
//                    qDebug()<<"--------------Step_7.Finish.";
                    qDebug()<<"New action";
                    change_flawPG = false;
                    str1.clear();
                    parts_R.clear();
                    DC.current = DC.first;
                    PG_num = 1;
                    WR_command("WR R200 1");
                }
            }else if(parts[0] == "WRS"){
                //Manual state -> stop the loop.
            }else if(parts[1]=="R214"){
                WR_command("WRS DM201 6 0 0 0 0 0 0");
            }else if(parts[1]=="DM200"){
//                qDebug()<<"alive";
            }else if(parts[1] == "R202"){
                parts_R[1] = "R202";
                recevNULL = true;
            }else if(parts[1] == "DM202"){
                WR_command("WR R202 1");
            }else if(change_flawPG == true){
                // change flaw pattern
                // step5 step6 step7
                if(parts[1] == "DM204"){
                    qDebug() << "--------------Step_6.Sending y = " << ARM_posY;
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM206").arg(ARM_posY);
                    WR_command(buffer_combined);

                }else if(parts[1] == "DM206"){
                    WR_command("WR R206 1");

                }else if(parts[1] == "R206"){
                    parts_R[1] = "R206";
                    recevNULL = true;
                }
            }else{
                if(parts[0]=="RD"){
                    recevNULL = true;
                }else{
                    //R200,R202,R204
                    recevNULL = true;
                    parts_R = str1.split(" ");
                }
            }
        }else {
            recevNULL = false;
            // RD事件
            parts = message.split(" ");
            if (message == "1" || message == "0") {
                QStringList parts = str1.split(" ");
                for (int i = 0; i < matrix_buffer_name.size(); i++) {
                    if (matrix_buffer_name[i] == parts[1]) {
                        buffer[i] = message;
                    }
                }
            }else {
                qDebug() <<"Invalid commend";
                ui->textRecv->append("Invalid commend");
            }
        }
        if(ReadpuB_isPressed == false){
            if (buffer[5] == "1") {
                //R201 1
                //R200、R201 ->successful connection
                buffer[4] = "0";
                buffer[5] = "0";
                logger.writeLog(Logger::Info, "Edge reset R200 and R201.");
                qDebug() << "--------------Step_2.server已回應OK，並將R200、R201歸零\n";
                WR_command("WRS R200 2 0 0");
            }
            if (buffer[7] == "1") {
                //R203 1
                //R202、R203 ->change pattern
                buffer[6] = "0";
                buffer[7] = "0";
                logger.writeLog(Logger::Info, "Edge reset R202 and R203.");
                qDebug() << "--------------Step_2.server已回應OK，並將R202、R203歸零\n";
                WR_command("WRS R202 2 0 0");
            }
            if (buffer[9] == "1"){
                //R205 1
                //R204、R205->open microscopic photography
                StartMicroscopic();
                buffer[8] = "0";
                buffer[9] = "0";
                logger.writeLog(Logger::Info, "Edge reset R204 and R205.");
                qDebug() << "--------------Step_4.server已回應OK，並將R204、R205歸零\n";
                WR_command("WRS R204 2 0 0");       
            }
            if(buffer[11] == "1"){
                //R207 1
                //R206、R207->sending and checking position
                buffer[10] = "0";
                buffer[11] = "0";
                logger.writeLog(Logger::Info, "Edge reset R206 and R207.");
                qDebug() << "--------------Step_4.server已回應OK，並將R206、R207歸零\n";
                WR_command("WRS R206 2 0 0");
            }
            if(buffer[12] == "1"){
                //R212 1
                buffer[12] = "0";
                qDebug()<<"--------------Step_2.PLC ready to run.";
                ui->puB_start->setEnabled(true);
            }
        }
    }
}
void Widget::RD(QString part)
{
    if(recevZero == false){
        if(recevNULL == true){
            //RD R201、R203、R205、R207、R212
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
        change_flawPG = false;
        str1.clear();
        parts_R.clear();
        DC.current = DC.first;
        //consist read R212 until receive 1
        WR_command("RD R212");
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
        ui->puB_runMode->setEnabled(true);
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
        QStringList tmp_parts;
        tmp_parts = WR_message.split(" ");
        if(ARM_posX < 0 || ARM_posY < 0){
            if(tmp_parts[1]=="DM204" || tmp_parts[1]=="DM206"){
                QString buffer_combined = QString("%1 %2 %3").arg(tmp_parts[0]).arg(tmp_parts[1]+".S").arg(tmp_parts[2]);
                qDebug()<<ARM_posX<<buffer_combined;
                WR_message = buffer_combined;
                str1 = QString("%1 %2 %3").arg(tmp_parts[0]).arg(tmp_parts[1]).arg(tmp_parts[2]);
            }else{
                str1 = WR_message;
            }
        }else{
            str1 = WR_message;
        }
//        qDebug()<<WR_message;
        logger.writeLog(Logger::Info, "User sent Message'" + WR_message + "'.");
        sending_ms = true;
        const QByteArray send_data = WR_message.toUtf8();
        if (send_data.isEmpty()){
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
//                WR_command(command);
            }
        }
    }
}

void Widget::change_pic2(int index)
{
    logger.writeLog(Logger::Info, "User changed pic2.");
    pix2.load(picfoldpath + QString::number(num) + "_"+ QString::number(index) + ".bmp");
//    pix2.load("/home/agx/Desktop/0321_qt/Pictures/" + QString::number(num) + "_"+ QString::number(index) + ".bmp");
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
        lineEdit->setStyleSheet("color:black");
    }
}

void Widget::comp_text()
{
    new_text.clear();
    for (auto lineEdit : lineEdits) {
        new_text.append(lineEdit->text());
    }
    for (int i = 0; i < lineEdits.size(); i++){
        if (new_text.at(i) != orgi_text.at(i)){
            lineEdits.at(i)->setStyleSheet("color:red");
        } else {
            lineEdits.at(i)->setStyleSheet("color:black");
        }
    }
}


void Widget::on_puB_start_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_start.");
    ui->puB_runMode->setEnabled(false);
    if(runMode == 1){
       WR_command("WR R200 1");
    }
//        ui->puB_start->setText("Stop");
}
void Widget::on_puB_read_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_read.");
    ReadpuB_isPressed = true;

    WR_command("RDS R200 10");
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
    buffer[11] = ui->R207_Edit->text();
    buffer[12] = ui->R212_Edit->text();
    buffer[13] = ui->R214_Edit->text();
    for(int i=0; i<buffer.size(); i++){
        if(buffer[i].isEmpty()){
            buffer[i]="0";
        }
    }
    QString buffer_combined = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13")
                                  .arg("WRS")
                                  .arg("R200")
                                  .arg("10")
                                  .arg(buffer[4])
                                  .arg(buffer[5])
                                  .arg(buffer[6])
                                  .arg(buffer[7])
                                  .arg(buffer[8])
                                  .arg(buffer[9])
                                  .arg(buffer[10])
                                  .arg(buffer[11])
                                  .arg(buffer[12])
                                  .arg(buffer[13]);

    WR_command(buffer_combined);
}

void Widget::on_puB_runMode_clicked()
{
    if(count_runModeclickedtime%2 == 1){
        ui->puB_runMode->setText("Auto");
        runMode = 1;
    }else{
        ui->puB_runMode->setText("Manual");
        runMode = 0;
    }
    count_runModeclickedtime++;
}


void Widget::on_puB_saveINI_clicked()
{
    QString path = ui->path_Edit->text();
    QString PT_width = ui->PT_width_Edit->text();
    QString PT_height = ui->PT_height_Edit->text();
    reviseconfigINI("COORDINATE","PT_sizeX",PT_width);
    reviseconfigINI("COORDINATE","PT_sizeY",PT_height);
    reviseconfigINI("PICTURE","pic_fold_path",path);
    bool conversionSuccess = false;
    unsigned int tempValue = PT_width.toInt(&conversionSuccess);
    if(conversionSuccess && tempValue <= UINT16_MAX){
        COORDINATE_PTsX = static_cast<uint16_t>(tempValue);
        qDebug()<<"COORDINATE_PTsX"<<COORDINATE_PTsX;
    }else{
        qDebug()<<"Failed_X";
    }

    conversionSuccess = false;
    tempValue = PT_height.toInt(&conversionSuccess);
    if(conversionSuccess && tempValue <= UINT16_MAX){
        COORDINATE_PTsY = static_cast<uint16_t>(tempValue);
        qDebug()<<"COORDINATE_PTsY"<<COORDINATE_PTsY;
    }else{
        qDebug()<<"Failed_Y";
    }

    strncpy(picfoldpath, path.toLocal8Bit().data(), sizeof(picfoldpath) - 1);
    picfoldpath[sizeof(picfoldpath) - 1] = '\0';
//    qDebug()<<"picfoldpath"<<picfoldpath;
}

void Widget::on_puB_remove_clicked()
{
    QListWidgetItem *selectedItem = ui->list_Pattern->currentItem();
    if(selectedItem){
        delete ui->list_Pattern->takeItem(ui->list_Pattern->row(selectedItem));
    }
}

void Widget::reviseconfigINI(QString section, QString key ,QString Value)
{
    qDebug()<<configFilePath;
    QSettings settings(configFilePath, QSettings::IniFormat);
    int currentValue = settings.value(section + "/" + key).toInt();
    qDebug()<<"Current:"<< currentValue;
    settings.setValue(section+"/"+key,Value);
    settings.sync();

}


void Widget::on_puB_add_clicked()
{

    bool ok;
    QString newItemText = QInputDialog::getText(this,"Add Item",
                                                "Enter new item:",
                                                QLineEdit::Normal,
                                                QString(),&ok);
    if(ok && !newItemText.isEmpty()){
        ui->list_Pattern->addItem(newItemText);
    }
}


void Widget::on_puB_save_clicked()
{
    run_pattern_name.clear();
    for(int i=0; i<ui->list_Pattern->count();i++){
        run_pattern_name.append(ui->list_Pattern->item(i)->text());
    }
    qDebug()<<"run_pattern_name:"<<run_pattern_name;
}




