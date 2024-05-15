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
#include <QMessageBox>
#include <QDateTime>
#include <QGroupBox>

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    logger.create_file();
    logger.populateCombowithFileName(ui->comboBox_logger, "Log");
    connect(ui->lbl_pic, SIGNAL(set_pic2(int)), this, SLOT(change_pic2(int)));
    connect(ui->lbl_pic, SIGNAL(Mouse_Pos()), this, SLOT(MouseCurrentPos()));
    connect(ui->comboBox_logger, QOverload<int>::of(&QComboBox::activated), [&](int index) {
        if(checkbox_onlyThisTime == false){
            logger.on_comboBox_currentIndexChanged(index, ui->text_log, ui->comboBox_logger, "Log");
            prettiertextlog();
        }
    });

    connect(ui->list_model, &QListWidget::itemEntered, this, &Widget::updateComboBoxModel);
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

    //AUO logo
    QPixmap pic_logo;
    pic_logo.load(QCoreApplication::applicationDirPath()+"/AUO.jpg");
    ui->lbl_logo->setPixmap(pic_logo);

    // clock (per second
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Qtimer()));
    timer->start(1000);

    // clock (per 0.6 second
    QTimer *timer_error = new QTimer(this);
    connect(timer_error,SIGNAL(timeout()),this,SLOT(QtimerError()));
    timer_error->start(600);

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
//    qDebug()<<"COORDINATE_PTsX :"<<COORDINATE_PTsX;
//    qDebug()<<"COORDINATE_PTsY :"<<COORDINATE_PTsY;

    // pixel size = 3840X2160, label size = 768X432

    // calculate the factor between pixel and move distance(MD)
//    factor_X = COORDINATE_PTsX/768;
//    factor_Y = COORDINATE_PTsY/432;
    factor_X = 1;
    factor_Y = 1;

    clientThread.start();
    cameraInit();

}
void Widget::INI_UI()
{
    CreateNReadRecipe();
    connect(&clientThread, &QThread::finished, tc, &QWidget::deleteLater);
    connect(tc, SIGNAL(recv_update(QString)), this, SLOT(recv_label_update(QString)));
    connect(tc, SIGNAL(connect_UIupdate()), this, SLOT(connect_label_update()));
    ui->PT_width_Edit->setText(QString("%1").arg(COORDINATE_PTsX));
    ui->PT_height_Edit->setText(QString("%1").arg(COORDINATE_PTsY));
    connect(ui->puB_read, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_write, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_saveINI, &QPushButton::clicked, this, &Widget::saveText);
    connect(ui->puB_connect, &QPushButton::clicked, this, &Widget::saveText);
    //要動態分配
    picfoldpath = QCoreApplication::applicationDirPath()+"/Pictures/";
    pix_Ini.load(picfoldpath + QString::number(num) + ".bmp");
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
    ui->list_Pattern->setSpacing(5);

    //定義comboBox_model下拉式選單
    updateComboBoxModel();



}

void Widget::cameraInit()
{

    for (int i = 0; i < MaxCamera; i++)
    {
        m_camera[i].SetUserHint( i );

//         Connect signals from CGuiCamera class to this dialog.
                QObject::connect( &(m_camera[i]), &CGuiCamera::NewGrabResult, this, &Widget::OnNewGrabResult );
//                QObject::connect( &(m_camera[i]), &CGuiCamera::StateChanged, this, &MainDialog::OnStateChanged );
//                QObject::connect( &(m_camera[i]), &CGuiCamera::DeviceRemoved, this, &MainDialog::OnDeviceRemoved );
//                QObject::connect( &(m_camra[i]), &CGuiCamera::NodeUpdated, this, &MainDialog::OnNodeUpdated );
    }

    Pylon::CDeviceInfo devInfo[2];
//    devInfo[0].SetSerialNumber("40053677");
//    devInfo[1].SetSerialNumber("40067726");

    devInfo[0].SetSerialNumber("40347555");
    devInfo[1].SetSerialNumber("40273082");
    try
    {
        // Open() may throw exceptions.
        m_camera[0].Open( devInfo[0] );
        m_camera[1].Open( devInfo[1] );
    }
    catch (const Pylon::GenericException& e)
    {
        ShowWarning( QString("Could not open camera!\n") + QString( e.GetDescription() ) );

//        return false;
    }
}
void Widget::OnNewGrabResult(int userHint)
{
    qDebug()<<"174";
    if ((userHint == 0) && m_camera[0].IsOpen())
    {

        QMutexLocker locker( m_camera[0].GetBmpLock() );
        QImage image = m_camera[0].GetImage();
        this->ui->lbl_Img->setPixmap(QPixmap::fromImage(image));
        takeQImagetoList(image,0);
        qDebug()<<"182";

        // Make sure to repaint the image control.
        // The actual drawing is done in paintEvent.
//        ui->image_1->repaint();
    }
    if ((userHint == 1) && m_camera[1].IsOpen())
    {
        QMutexLocker locker( m_camera[1].GetBmpLock() );
        QImage image = m_camera[1].GetImage();
        this->ui->lbl_Img->setPixmap(QPixmap::fromImage(image));
        takeQImagetoList(image,1);
        qDebug()<<"193";
    }

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
    qDebug()<<mean;
    return mean;

}

Widget::~Widget()
{
    tc->client->abort();
    clientThread.quit();
    clientThread.wait();
    logger.writeLog(Logger::null, "-------------------End-------------------.");
    delete ui;
}

void Widget::MouseCurrentPos()
{
    ui->lbl_Pos->setText(QString("X = %1, Y = %2").arg(ui->lbl_pic->x).arg(ui->lbl_pic->y));

}
void Widget::updatelblPic()
{
//    qDebug()<<"num"<<num;
    //revise pic amount
    int MaxPatternAmount = show_pattern_name.size();
    if (num == 0) {
        num = MaxPatternAmount;
    }else if (num == MaxPatternAmount+1) {
        num = 1;
    }
    // if crashed, checked whether the amounts of show_pattern_name are right
    ui->lbl_pattern->setText("Pattern = " + QString(show_pattern_name.at(num - 1)));
    pix_Ini.load(picfoldpath + QString::number(num) + ".bmp");
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
//    factor_X = COORDINATE_PTsX/768;
//    factor_Y = COORDINATE_PTsY/432
    factor_X = 1;
    factor_Y = 1;
    message = message.replace("\r\n","");

    recevZero = false;

    //show somethig in textRecv
    if (message == "SocketError"){
        ui->textRecv->append(message);
        qDebug()<<"received:"<<message;
    }else{
        ui->textRecv->append("received:"+message+"\tfrom: "+str1);
        qDebug()<<"received:"<<message<<"\tfrom"<<str1;
    }

    if (message == "SocketError"){
        logger.writeLog(Logger::Warning, "Socket " + tc->client->errorString()+".");
    }else if(str1 == "RD R215"){
        if(message == "1"){
            qDebug()<<"ERRRRRRRRRRRRRROR";
        }
    }else if(message == "0"){
        //0\r\n
        recevZero = true;
    }else {
        logger.writeLog(Logger::Info, "Received message " + message + ".");
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
        } else if (WritepuB_isPressed == true){
            if (count_num == 8) {
                WritepuB_isPressed = false;
                logger.writeLog(Logger::Info, "Wrote PLC index.");
                count_num = 0;
                //OK\r\n
            } else if (message == "OK"){
                if(count_num == 4 || count_num == 6){
                    //DM204,DM206
                    bool ok;
                    int tmp_buffer= buffer[count_num / 2].toInt(&ok);
                    if(ok){
                        if(tmp_buffer<0){
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
        } else if (message == "OK"){
            recevZero = false;
            // WR事件
            parts = str1.split(" ");
            if(runMode == 1 && parts[0]=="WRS"){
                if(parts[1]== "R200"){
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(RunPatternIndex);
                    WR_command(buffer_combined);
                    qDebug()<<"--------------Step_3.1巨觀change PG_num to"<<RunPatternIndex;
                    ui->lbl_state->setText("｜巨觀｜切換pattern至"+RunPatternName.at(RunPatternIndex-1));
                    RunPatternIndex++;
                }else if(parts[1] == "R202"){
                    if(change_flawPG == true){
                        if(DC.current == DC.first){
                            RunDefectNumber = 1;
                            qDebug()<<"first run need to plus compensate";
                            ARM_posX =(DC.current->x - DC.center_posX)*factor_X;
                            ARM_posY =(DC.current->y - DC.center_posY)*factor_Y;
                        }else{
                            ARM_posX = (DC.current->x - DC.current->prev->x)*factor_X;
                            ARM_posY = (DC.current->y - DC.current->prev->y)*factor_Y;
                        }
                        qDebug() << "--------------Step_6.server已回應OK.\n";
                        qDebug() << "--------------Step_6.Sending x = " << ARM_posX;
                        QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
                        WR_command(buffer_combined);
                    }else{
                        Images.push_back(tmp);

                        qDebug()<<"push";
                        if(RunPatternIndex<(RunPatternAmount+1)){
                            //To the next PG_num
                            qDebug() << "--------------Step_3.server已回應OK.\n";
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(RunPatternIndex);
                            WR_command(buffer_combined);
                            qDebug()<<"--------------Step_3.2巨觀change PG_num to"<<RunPatternIndex;
                            ui->lbl_state->setText("｜巨觀｜切換pattern至"+RunPatternName.at(RunPatternIndex-1));
                            RunPatternIndex++;
                        }else{
                            imagesprocess(imageVectorBig);
                            qDebug() << "--------------Step_3.All pattern was changed.";
                            WR_command("WR R204 1");
                        }
                    }
                }else if(parts[1] == "R204"){
                    //if change_flawPG==true ->to step.5
                    change_flawPG = true;
                    QString buffer_combined = QString("%1 %2").arg("WR DM202").arg(DC.current->index);
                    ui->lbl_state->setText("｜微觀｜切換至 "+RunPatternName.at(DC.current->index-1)+" Pattern");
                    WR_command(buffer_combined);
                }else if(parts[1] == "R206"){
                    if(DC.current->next == NULL){
                        //change all flaw pattern and sent all flaws->go to step7
                        change_flawPG = false;
                        WR_command("WR R214 1");
                        qDebug()<<"--------------Step_7.send end signal to PLC.";
                    }else{
                        DC.current = DC.current->next;
                        qDebug()<<"change pattern";
                        if(DC.current->index == DC.current->prev->index){
                            //don't change pattern
                            ARM_posX = (DC.current->x - DC.current->prev->x)*factor_X;
                            ARM_posY = (DC.current->y - DC.current->prev->y)*factor_Y;
                            qDebug() << "--------------Step_6.Sending x = " << ARM_posX;
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
                            WR_command(buffer_combined);
                        }else{
                            // change pattern
                            RunDefectNumber = 1;
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(DC.current->index);
                            WR_command(buffer_combined);
                        }     
                        ui->lbl_state->setText("|微觀|已將pattern切換至"+RunPatternName.at(DC.current->index-1));
                    }
                }else{
                    //receive OK from "WRS DM201 6 0 0 0 0 0 0
//                    qDebug()<<"--------------Step_7.Finish.";
                    runInit();
                    qDebug()<<"New action";
                    str1.clear();
                    parts_R[1] = "R211";
                    recevNULL = true;
                    WR_command("RD R212");
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
                if(change_flawPG == false){
                    qDebug()<<"拍攝巨觀"<<RunPatternName.at(RunPatternIndex-2);
                    ui->lbl_state->setText("目前在拍攝"+RunPatternName.at(RunPatternIndex-2)+"巨觀照片");
                    on_puB_bigGrab_clicked();
                }
                WR_command("WR R202 1");
            }else if(change_flawPG == true){
                // change flaw pattern
                // step5 step6 step7
                if(parts[1] == "DM204"){
                    qDebug() << "--------------Step_6.Sending y = " << ARM_posY;
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM206").arg(ARM_posY);
                    WR_command(buffer_combined);
                }else if(parts[1] == "DM206"){
                    qDebug()<<"拍攝微觀";
                    qDebug()<<"目前在拍攝"+RunPatternName.at(DC.current->index-1)+" 微觀照片";
                    ui->lbl_state->setText("目前在拍攝"+RunPatternName.at(DC.current->index-1)+" 微觀照片");
                    on_puB_samllGrab_clicked();
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
                qDebug()<< "創Pic資料夾";
                RunCurrentDateTime = QDateTime::currentDateTime();
                RundataTimeString = RunCurrentDateTime.toString("yyyyMMdd_hhmm");
                RunCurrentModel = ui->lbl_CurModel->text();
                RunTimefolderpath = "/Model/"+RunCurrentModel+"/";
                CreateFolder(RunTimefolderpath, RundataTimeString);
                CopyRecipe(RunTimefolderpath,"/"+RundataTimeString+"/");
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
                buffer[8] = "0";
                buffer[9] = "0";
                logger.writeLog(Logger::Info, "Edge reset R204 and R205.");
                qDebug() << "--------------Step_4.server已回應OK，並將R204、R205歸零\n";
                ui->lbl_state->setText("｜微觀｜開始拍攝微觀");
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
                if(runMode == 1){
                    WR_command("WR R200 1");
                }
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
        RunPatternIndex = 1;
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
        ui->puB_runMode->setEnabled(false);
        ui->puB_start->setEnabled(false);
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
    //檢測日期是否有更改
    //日期改變,需創建新的.log(writeLog->改路徑, 需更新comboBox->logger選單)以及model需創建新的資料夾
    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    if(currentDate!=logger.m_lastDateChecked){
        qDebug()<<"<隔天>";
        logger.create_file();
        logger.populateCombowithFileName(ui->comboBox_logger, "Log");
    }
    //每秒傳送WR DM200
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

void Widget::QtimerError()
{
    //read error signal
//    WR_command("RD R215");
}

void Widget::change_pic2(int index)
{
    logger.writeLog(Logger::Info, "User changed pic2.");
    pix2.load(picfoldpath + QString::number(num) + "_"+ QString::number(index) + ".bmp");
    ui->lbl_pic2->setPixmap(pix2);
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
    if(ui->lbl_CurModel->text()=="(null)"){
        ui->lbl_state->setText("請先設置將運行的Model");
    }else{
        if(ui->puB_runMode->text()=="-"){
            ui->lbl_state->setText("請先設置將運行的Mode");
        }else{
            ui->puB_runMode->setEnabled(false);
            if(runMode == 1){
               runInit();
               WR_command("WR R200 1");
            }
        }
    }
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
    QString PT_width = ui->PT_width_Edit->text();
    QString PT_height = ui->PT_height_Edit->text();
    reviseconfigINI("COORDINATE","PT_sizeX",PT_width);
    reviseconfigINI("COORDINATE","PT_sizeY",PT_height);
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

}

void Widget::on_puB_remove_p_clicked()
{  
    revisePatternList = true;
    QListWidgetItem *PTselectedItem = ui->list_Pattern->currentItem();
    if(PTselectedItem){
        QMessageBox::StandardButton removePCheck;
        removePCheck = QMessageBox::question(this, "確認", "確定要刪除pattern:"+PTselectedItem->text()+" ？", QMessageBox::Yes|QMessageBox::No);
        if (removePCheck == QMessageBox::Yes) {
            QString ModelName = ui->lbl_modelPT->text();
            QString ModelPath = QString("%1%2%3").arg(QCoreApplication::applicationDirPath()+"/Model/").arg(ModelName).arg("/recipe.ini");
            FP.removePattern(PTselectedItem->text(),ModelPath);
            delete ui->list_Pattern->takeItem(ui->list_Pattern->row(PTselectedItem));
            qDebug()<<"已刪除pattern:"<<PTselectedItem->text();
            ui->lbl_state->setText("已刪除pattern:"+PTselectedItem->text());
        } else {
            ui->lbl_state->setText("取消刪除pattern:"+PTselectedItem->text());
            qDebug() << "取消刪除pattern:"<<PTselectedItem->text();
        }
    }else{
        qDebug()<<"尚未選擇要刪除的pattern！";
        ui->lbl_state->setText("尚未選擇要刪除的pattern！");
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


void Widget::on_puB_add_p_clicked()
{
    revisePatternList = true;

    if(ui->list_Pattern->count() == 0){
        qDebug()<<"未選擇Model";
        ui->lbl_state->setText("尚未選擇Model！");
    }else{
        bool ok;
        QString newItemText = QInputDialog::getText(this,"新增",
                                                    "請輸入要新增的pattern:",
                                                    QLineEdit::Normal,
                                                    QString(),&ok);
        if(ok && !newItemText.isEmpty()){
            addPattern = true;
            ui->list_Pattern->addItem(newItemText);
        }
    }

}


void Widget::on_puB_save_p_clicked()
{
    revisePatternList = false;
    if(ui->list_Pattern->count()== 0){
        qDebug()<<"pattern list不能為空，，請先選擇Model！";
        ui->lbl_state->setText("pattern list不能為空，，請先選擇Model！");
    }else{
        QString currentModel = ui->lbl_modelPT->text();
        QString tmp;
        for(model_name &model : modelList) {
             if(model.modelName == currentModel){
                model.pattern_names.clear();
                for(int i=0; i<ui->list_Pattern->count();i++){
                    model.pattern_names.append(ui->list_Pattern->item(i)->text());
                    tmp = ui->list_Pattern->item(i)->text();
                }
            }
        }
        QStringList tmpStringList;
        tmpStringList.append(tmp);
        QString tmpModePath = QString("%1%2").arg(QCoreApplication::applicationDirPath()+"/Model/"+currentModel+"/").arg("recipe.ini");
        FP.INI(tmpStringList, tmpModePath, currentModel,false);
        updatecombopattern();
        if(addPattern == true){
            FP.show();
            addPattern = false;
        }
    }
}

void Widget::updatetextlog(QString type, QString message)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dataTimeString = currentDateTime.toString("hh:mm:ss");
    ui->text_log->append(QString("%1 %2 %3").arg(dataTimeString).arg(type).arg(message));
    prettiertextlog();
}

void Widget::prettiertextlog()
{
    QTextCursor cursor(ui->text_log->document());
    cursor.movePosition(QTextCursor::Start);
    QString tmp_string;
    QString htmlText;
    int countCharPos = 0;
    // 将 RGB 值转换为 QColor 对象
    QColor color(115,184,57);
    // 获取十六进制颜色代码
    QString hexColor = color.name();
    while (!cursor.atEnd()) {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        QString character = cursor.selectedText();
        if (character.at(0).isLetter()) {
            tmp_string+=character;
        }else{
            if(tmp_string == "INFO"){
                htmlText += "<span style='background-color: "+hexColor+";'>";
                htmlText += tmp_string;
                htmlText += "</span>";
            }else if(tmp_string == "WARNING"){
                htmlText += "<span style='background-color: yellow;'>";
                htmlText += tmp_string;
                htmlText += "</span>";
            }else if(tmp_string == "ERROR"){
                htmlText += "<span style='background-color: red;'>";
                htmlText += tmp_string;
                htmlText += "</span>";
            }else{
                htmlText += tmp_string;
            }
            tmp_string.clear();
            if(character == "."){
                countCharPos = -1;
            }
            if(countCharPos<9 && countCharPos>-1){
                htmlText += "<span style='color: blue;'>";
                htmlText += character;
                htmlText += "</span>";
            }else{
                htmlText += character;
            }
            countCharPos++;
        }
        cursor.movePosition(QTextCursor::Right);
    }
    ui->text_log->clear();
    ui->text_log->insertHtml(htmlText);
}
// Show a warning dialog.
void Widget::ShowWarning( QString warningText )
{
    QMessageBox::warning( this, "WARNING", warningText, QMessageBox::Ok );
}

void Widget::on_puB_bigGrab_clicked()
{
    try
    {
        m_camera[0].SingleGrab();

    }
    catch (const Pylon::GenericException& e)
    {
        ShowWarning( QString( "Could not start grab!\n" ) + QString( e.GetDescription()));
    }
}

void Widget::on_puB_samllGrab_clicked()
{
    try
    {
        m_camera[1].SingleGrab();
    }
    catch (const Pylon::GenericException& e)
    {
        ShowWarning( QString( "Could not start grab!\n" ) + QString( e.GetDescription() ) );
    }
}


void Widget::on_checkBox_onlyThisTime_stateChanged(int state)
{
    if(state == Qt::Checked){
        checkbox_onlyThisTime = true;
        displayLastLog();
        prettiertextlog();
    }else{
        checkbox_onlyThisTime = false;
    }
}
void Widget::displayLastLog() {
    QFile file(logger.filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open log file for reading.";
        return;
    }
    QTextStream in(&file);
    QString lastLogContent;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains("End")) {
            lastLogContent.clear();
        } else {
            if (!lastLogContent.isEmpty()) {
                lastLogContent += "\n";
            }
            lastLogContent += line;
        }
    }
    file.close();
    ui->text_log->setText(lastLogContent);
}

void Widget::updatecombopattern()
{
    for(int i = 0; i < ui->list_Pattern->count(); ++i) {
        FP.fpupdatecombopattern(ui->list_Pattern->item(i),i);
    }
}


void Widget::on_puB_gui_clicked()
{
    m_camera[0].Close();
    MD.show();
}

void Widget::on_puB_func_clicked()
{
    if(revisePatternList == true){
        ui->lbl_state->setText("請先點選save，在進行參數設置！");
    }else{
        FP.show();
    }
}

void Widget::on_radioButton_pattern_clicked(bool checked)
{
    QList<QWidget *> widgets = ui->groupBox->findChildren<QWidget *>();
    foreach (QWidget *widget, widgets){
        if(checked == true){
            widget->setEnabled(true);
        }else{
            widget->setEnabled(false);
        }
    }
}

void Widget::on_puB_add_m_clicked()
{
    revisePatternList = true;
    bool ok;
    QString newItemText = QInputDialog::getText(this,"Add Model",
                                                "Enter new model:",
                                                QLineEdit::Normal,
                                                QString(),&ok);

    QString inputModel = newItemText;
    QString modelPath= "/Model/";
    QStringList createNewPattern;
    QDir modelFolder(modelPath);
    QMessageBox::StandardButton doublecheck;
    if (ok == true) {
        doublecheck = QMessageBox::question(this, "確認", "創建model:"+inputModel+"？", QMessageBox::Yes|QMessageBox::No);
        if(doublecheck == QMessageBox::Yes){
            // 檢查Model是否存在
           if (!modelFolder.exists()) {
               CreateFolder(modelPath, inputModel);
               CreateNReadRecipe();
               modelPath += inputModel+"/recipe.ini";
               QFile modelFile(modelPath);
               if (modelFile.open(QIODevice::WriteOnly)) {
                   modelFile.close();
                   QStringList tmp_b;
                   FP.INI(tmp_b, modelPath, inputModel,true);
                   qDebug() << "新模型的.ini檔已创建： " << modelPath;
                   QMessageBox::information(this, "提示", "新的model已新增至recipe，請先建立新的pattern list，再進行連線");
                   ui->lbl_state->setText("新的model已新增至recipe，請先建立新的pattern list");
               } else {
                   qDebug() << "無法打開配置文件： " << modelPath;
               }
           }else{
               ShowWarning("Model:"+inputModel+" 已存在");
               qDebug() << "配置文件已存在： " << modelPath;
               FP.INI(createNewPattern,modelPath,inputModel,false);
           }
        }else{
            ui->lbl_state->setText("取消創建新model:"+inputModel);
            qDebug() << "取消創建配置文件。";
        }
    }else{
        ui->lbl_state->setText("取消創建新model:"+inputModel);
        qDebug() << "取消創建配置文件。";
    }
    updateComboBoxModel();
}
void Widget::on_puB_remove_m_clicked()
{
    revisePatternList = true;
    QListWidgetItem *selectedItem = ui->list_model->currentItem();
    if(selectedItem){
        QMessageBox::StandardButton removeCheck;
        removeCheck = QMessageBox::question(this, "確認", "確定要刪除model:"+selectedItem->text()+" ？", QMessageBox::Yes|QMessageBox::No);
        if (removeCheck == QMessageBox::Yes) {
            QString deleDirPath = QCoreApplication::applicationDirPath() + "/Model/" + selectedItem->text();
//            qDebug() << "欲刪除資料夾路徑：" << deleDirPath;
            // 創建 QDir 物件
            QDir dir(deleDirPath);

            // 檢查資料夾是否存在
            if (dir.exists()) {
                // 刪除資料夾及其內容
                if (dir.removeRecursively()) {
//                    qDebug() << "資料夾" << deleDirPath << "已成功刪除";
                    ui->lbl_state->setText("已刪除 model:" + selectedItem->text());
                    if(ui->lbl_CurModel->text()==selectedItem->text()){
                        ui->lbl_CurModel->setText("(null)");
                    }
                    delete ui->list_model->takeItem(ui->list_model->row(selectedItem));
                } else {
//                    qDebug() << "刪除資料夾" << deleDirPath << "失敗";
                    ui->lbl_state->setText("刪除資料夾"+ deleDirPath + "失敗");
                }
            } else {

                qDebug() << "資料夾" << deleDirPath << "不存在";
            }

        } else {
            ui->lbl_state->setText("取消刪除model:"+selectedItem->text());
//            qDebug() << "取消刪除model:"<<selectedItem->text();
        }
    }else{
        ui->lbl_state->setText("尚未選擇要刪除的model！");
    }
    updateComboBoxModel();
}



void Widget::on_list_model_itemDoubleClicked(QListWidgetItem *item)
{
    revisePatternList = true;
    QString currentModel = item->text();
    CreateNReadRecipe();
    foreach (const model_name &model, modelList) {
        if(model.modelName == currentModel){
            ui->list_Pattern->clear();
            foreach (const QString &pattern, model.pattern_names) {
                ui->list_Pattern->addItem(pattern);
            }
        }
    }
    ui->lbl_modelPT->setText(currentModel);
}

void Widget::on_puB_setCur_m_clicked()
{

    QListWidgetItem *selectedItem = ui->list_model->currentItem();
    if (selectedItem) {
        selectedItem->setForeground(Qt::blue);
        ui->lbl_state->setText("已將"+selectedItem->text()+"設為當前運轉model");
        for (int i = 0; i < ui->list_model->count(); ++i) {
            QListWidgetItem *item = ui->list_model->item(i);
            if (item != selectedItem) {
                item->setForeground(Qt::black);
            }
        }
        ui->lbl_CurModel->setText(selectedItem->text());
        QString runModelname = ui->lbl_CurModel->text();
        QString filePath = QCoreApplication::applicationDirPath()+"/Model/"+runModelname+"/recipe.ini";
        QSettings setting(filePath, QSettings::IniFormat);
        QStringList groups = setting.childGroups();
        // 逐一讀取每個組的內容
        foreach (QString group, groups) {
            if(group != "COORDINATE" && group != "CAM1"){
                RunPatternName.append(group);
            }
        }
        runInit();
    }else{
        ui->lbl_state->setText("尚未選擇model！");
    }
}

void Widget::on_table_defectlist_cellClicked(int row, int column)
{
    QString CurrentDateDir = ui->table_defectlist->item(row, 7)->text();
    QString CurrentTimeDir = ui->table_defectlist->item(row, 8)->text();
    QString CurrentModel = ui->table_defectlist->item(row, 1)->text();
    QString patternIndex =ui->table_defectlist->item(row, 2)->text();
    QString Number = ui->table_defectlist->item(row, 0)->text();
    QPixmap pic2;
    QString pic2Path = QCoreApplication::applicationDirPath()+"/Model/"+CurrentModel+"/"+CurrentDateDir+"_"+CurrentTimeDir+"/"+patternIndex+"_"+Number;
    pic2.load(pic2Path);
    ui->lbl_pic2->setPixmap(pic2);
    QString DF_XY = ui->table_defectlist->item(row, 3)->text();
    ui->lbl_DFcoodinate->setText(DF_XY);
}

void Widget::CreateNReadRecipe()
{
    modelList.clear();
    QDir modelDir("Model");
    if(!modelDir.exists()){
        modelDir.mkpath(".");
        qDebug()<<"|Initial|已創建Model資料夾";
        logger.writeLog(Logger::Info, "|SYSYEM| Model folder create.");
    }else{
        qDebug()<<"|Initial|已存在Model資料夾";
        logger.writeLog(Logger::Info, "|SYSYEM| Model folder exist.");
        // 獲取目錄中的所有檔案
        QStringList FilesinModelfolders = modelDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(FilesinModelfolders.isEmpty()){
            qDebug()<<"|Initial|Model資料夾中沒有資料夾（沒有Model）";
            ui->lbl_state->setText("沒有Model Recipe，請先建立Model Recipe！");
        }else{
            qDebug()<<"|Initial|Model資料夾中有資料夾（有Model）";
            qDebug()<<"|Initial|現有的Model Name"<<FilesinModelfolders;
            foreach(QString modelnamefolder,FilesinModelfolders){
//                qDebug()<<modelnamefolder;
                // 检查config.ini配置文件是否存在
                configFilePath = QCoreApplication::applicationDirPath() + "/Model/"+modelnamefolder+"/recipe.ini";
                QFile configFile(configFilePath);
                if (!configFile.exists()) {
                    QStringList tmp;
                    FP.INI(tmp, configFilePath, modelnamefolder,true);
                    qDebug()<< "Create"<<modelnamefolder<<"'s recipe";
                } else {
//                    qDebug() << modelnamefolder<<" exist";
                }

                QDir ModelName("Model/"+modelnamefolder);
                // 設定要讀取的目錄
                ModelName.setFilter(QDir::Files | QDir::NoDotAndDotDot);
                QStringList FilesinModelNamefolders = ModelName.entryList(QDir::Files | QDir::NoDotAndDotDot);
                if(FilesinModelNamefolders.isEmpty()){
                    qDebug()<<modelnamefolder<<"is empty->error";
                }
                // 逐一讀取每個檔案
                foreach (QString file, FilesinModelNamefolders) {
                    if (file.endsWith(".ini")) {
                        QString filePath = QCoreApplication::applicationDirPath() + "/Model/"+modelnamefolder+"/"+file;
                        QSettings settings(filePath, QSettings::IniFormat);
                        QStringList groups = settings.childGroups();
                        model_name newModel;
                        QString modelName = modelnamefolder;
                        newModel.modelName = modelName;
                        // 逐一讀取每個組的內容
                        foreach (QString group, groups) {
                            if(group != "COORDINATE" && group != "CAM1"){
                                QStringList patternName = group.split(" ");
                                newModel.pattern_names.append(patternName);
                            }
                        }
                        modelList.append(newModel);
                    }
                }
            }
        }
    }
    ui->list_model->clear();
    // 輸出Model List結構列表
    foreach (const model_name &model, modelList) {
        ui->list_model->addItem(model.modelName);
    }
}

void Widget::CreateFolder(QString path, QString FolderName)
{
    QString combinePath = QCoreApplication::applicationDirPath()+path+FolderName;
    qDebug()<<combinePath;
    QDir folder(combinePath);
    if (!folder.exists()){
        folder.mkpath(".");
        qDebug()<<"創建"<<FolderName<<"資料夾";
    }else{
        qDebug()<<FolderName<<"資料夾已存在";
    }
}

void Widget::on_puB_cameraINI_clicked()
{
    cameraInit();
}

void Widget::updateComboBoxModel()
{
    //定義comboBox_model下拉式選單
    //獲取all model name
    ui->comboBox_model->clear();
    QString pathModel = QCoreApplication::applicationDirPath()+"/Model/";
    QDir modeldirectory(pathModel);
    QStringList ModelName = modeldirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &modelname, ModelName){
        ui->comboBox_model->addItem(modelname);
    }
}

void Widget::imagesprocess(QVector<QImage> BigGrabImages)
{
    qDebug()<<"全部巨觀照片一起做image process";
    qDebug()<<"Assume received defect info";


    //for test
//    tmp.index = 1;
//    tmp.patternName = "White";
//    tmp.meanGray = 0.1;
//    tmp.defectPoint = {QPoint(10, 10), QPoint(20, 20)};

//    Images.push_back(tmp);

//    tmp.index = 2;
//    tmp.patternName = "Black";
//    tmp.meanGray = 0.2;
//    tmp.defectPoint = {QPoint(50, 50), QPoint(60, 60)};
//    Images.push_back(tmp);
    qDebug()<<"顯示";
    QVector<ImageProcess>::iterator it = Images.begin();
    for(; it != Images.end(); it++) {
        if(it->patternName == "White"){
            QImage imageTest(QCoreApplication::applicationDirPath()+"/EXP45606_WHITE.bmp");
            it->image = imageTest;
            qDebug()<<"White";
        }else{
            QImage imageTest(QCoreApplication::applicationDirPath()+"/denoise.bmp");
            it->image = imageTest;
        }
    }


    process.process(&Images);
    foreach(const ImageProcess &image, Images){
        qDebug()<<image.defectPoint;
    }

    bool isHead = true;
    foreach(const ImageProcess &image, Images){
        DC.defNode(image.defectPoint,image.index,isHead);
        isHead = false;
    }
    DC.current = DC.first;

    QString MapPath = QCoreApplication::applicationDirPath()+RunTimefolderpath+RundataTimeString;
    CreateMap(MapPath+"/map.ini");

}

void Widget::CopyRecipe(QString originFilePath, QString CopyFilePath)
{
    QString combineOriPath = QCoreApplication::applicationDirPath()+originFilePath+"recipe.ini";
    QString combineCopyPath = QCoreApplication::applicationDirPath()+originFilePath+CopyFilePath+"recipe.ini";
    QSettings oriSettings(combineOriPath,QSettings::IniFormat);
    QSettings copySettings(combineCopyPath,QSettings::IniFormat);
    // 讀取原始檔案中的所有組和鍵值對
    QStringList groups = oriSettings.childGroups();
    foreach (const QString &group, groups) {
        oriSettings.beginGroup(group);
        copySettings.beginGroup(group);

        QStringList keys = oriSettings.allKeys();
        foreach (const QString &key, keys) {
            copySettings.setValue(key, oriSettings.value(key));
        }

        oriSettings.endGroup();
        copySettings.endGroup();
    }
    qDebug()<<"Recipe.ini copied successfully.";
}

void Widget::CreateMap(QString path)
{
    qDebug()<<"CreateMap:"<<path;
    QFile mapfile(path);
    if(!mapfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        qDebug()<<"|MAP|Failed to create map file.";
        return;
    }
    QSettings settings(path, QSettings::IniFormat);
    //save all info
    foreach(const ImageProcess &image, Images){
        settings.beginGroup(image.patternName);
        settings.setValue("index",image.index);
        settings.setValue("MeanGray",image.meanGray);
        int DefectAmount = 1;
        foreach(const QPoint &point, image.defectPoint) {
            QString tmp = QString("%1_%2").arg("DefectPoint").arg(QString::number(DefectAmount));
            QString tmpPoint = QString("(%1,%2)").arg(point.x()).arg(point.y());
            settings.setValue(tmp,tmpPoint);
            DefectAmount++;
        }
        settings.endGroup();
    }
}



void Widget::takeQImagetoList(const QImage &image, int OisBig)
{

    QString folderpath = QCoreApplication::applicationDirPath()+RunTimefolderpath+RundataTimeString;
    QString imgName;
    if(OisBig == 0){
        qDebug()<<"got big";
        imageVectorBig.append(image);
        imgName = QString("%1%2").arg(QString::number(RunPatternIndex-1)).arg(".bmp");
    }else{
        qDebug()<<"got small";
        imageVectorSmall.append(image);
        imgName = QString("%1%2%3").arg(QString::number(DC.current->index)+"_").arg(QString::number(RunDefectNumber)).arg(".bmp");
        RunDefectNumber++;
    }
    QString imagePath = folderpath+"/"+imgName;
    if (image.save(imagePath)){
        qDebug() << "Image saved successfully at:" << imagePath;
    } else {
        qWarning() << "Failed to save image at:" << imagePath;
    }
    if(OisBig == 0){
        tmp.index = RunPatternIndex-1;
        tmp.image = image;
        tmp.patternName = RunPatternName.at(RunPatternIndex-2);
        tmp.meanGray = calculateMean(imagePath);
        QString runModelname = ui->lbl_CurModel->text();
        QString filePath = QCoreApplication::applicationDirPath()+"/Model/"+runModelname+"/recipe.ini";
        QSettings setting(filePath, QSettings::IniFormat);
        QStringList groups = setting.childGroups();
        QStringList keys;
        // 逐一讀取每個組的內容
        foreach (QString group, groups) {
            if(group == tmp.patternName){
                keys = setting.allKeys();
                foreach(QString key,keys){
                    if(key==group+"/checkBP"){
                        tmp.BPenable = setting.value(key).toBool();
                    }else if(key==group+"/checkDP"){
                        tmp.DPenable = setting.value(key).toBool();
                    }
                }
            }
        }
        qDebug()<<"-------"<<tmp.index<<tmp.patternName<<tmp.meanGray;
    }
}

void Widget::runInit()
{
    qDebug()<<"|Initial|Initial Run parameter";
    //initial
    RunPatternName.clear();
    change_flawPG = false;
    Images.clear();
    DC.current = DC.first;
    RunPatternIndex = 1;
    QString runModelname = ui->lbl_CurModel->text();
    QString filePath = QCoreApplication::applicationDirPath()+"/Model/"+runModelname+"/recipe.ini";
    QSettings setting(filePath, QSettings::IniFormat);
    QStringList groups = setting.childGroups();
    // 逐一讀取每個組的內容
    foreach (QString group, groups) {
        if(group != "COORDINATE" && group != "CAM1"){
            RunPatternName.append(group);
        }
    }
    RunPatternAmount = RunPatternName.size();
    qDebug()<<"RUN->"<<runModelname<<":"<<RunPatternName<<","<<RunPatternAmount;
}


void Widget::on_comboBox_model_activated(const QString ModelName)
{
    QDir Modelfolder("Model/"+ModelName);
    // 設定要讀取的目錄
    QStringList DateDirsinModel = Modelfolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug()<<DateDirsinModel<<DateDirsinModel.size();
    ui->comboBox_date->clear();
    foreach(QString DateDir,DateDirsinModel){
        ui->comboBox_date->addItem(DateDir);
    }
}



void Widget::on_comboBox_date_activated(const QString TimeDir)
{
    qDebug()<<"--"<<TimeDir;
    QString ModelName = ui->comboBox_model->currentText();
    QString MapPath = "Model/"+ModelName+"/"+TimeDir+"/map.ini";
    QFile MapFile(MapPath);
    QRegularExpression regex1("(\\d{8})_(\\d{4})"); // 正則表達式，匹配8個數字後跟下劃線，再匹配4個數字
    QRegularExpressionMatch match1 = regex1.match(TimeDir);
    QString datePart,timePart;
    if (match1.hasMatch()) {
        datePart = match1.captured(1); // 第一個捕獲組，即日期部分
        timePart = match1.captured(2); // 第二個捕獲組，即時間部分
    } else {
        qDebug() << "No match found.";
    }
    ui->table_defectlist->clearContents(); // 清除內容
    ui->table_defectlist->setRowCount(0); // 清除行數

    int countDefectAmount = 0;
    if(!MapFile.exists()){
        qDebug()<<MapPath<<"->invalid";
        ui->table_defectlist->insertRow(0);
        QTableWidgetItem *item_model = new QTableWidgetItem("NULL");
        ui->table_defectlist->setItem(0, 0, item_model);
    }else{
        QSettings MapSetting(MapPath, QSettings::IniFormat);
        QStringList MapKeys = MapSetting.allKeys();
        QStringList MapGroups = MapSetting.childGroups();
        foreach(const QString mapkey,MapKeys){
            if(mapkey.contains("DefectPoint")){
                countDefectAmount++;
            }
        }
        countDefectAmount = 0;
        for(int i=0 ; i<MapGroups.count(); i++){
            QString mapgroupName = MapGroups[i];
            QString MeanGray,index,stringValue;
            QStringList defectList;
            //MapGroups ("ABlack", "ARed", "AWhite")
            qDebug()<<"["<<mapgroupName<<"]";
            for(int j=0 ;j<MapKeys.count() ; j++){
                QString mapkey = MapKeys[j];
                //MapKeys ("ABlack/DefectPoint_1", "ABlack/DefectPoint_2", "ABlack/MeanGray", "ABlack/index", "ARed/DefectPoint_1", "ARed/DefectPoint_2", "ARed/MeanGray", "ARed/index", "AWhite/DefectPoint_1", "AWhite/DefectPoint_2", "AWhite/MeanGray", "AWhite/index")
                if(mapkey.contains(mapgroupName)){
                    QString key = mapkey.remove(mapgroupName+"/");
                    if(mapkey.contains("DefectPoint")){
                        stringValue = MapSetting.value(mapgroupName+"/"+mapkey).toString();
                        defectList.append(stringValue);
                    }else if(mapkey.contains("MeanGray")){
                        // value is like 0.2(double)
                        MeanGray = MapSetting.value(mapgroupName+"/"+mapkey).toString();
                    }else if(mapkey.contains("index")){
                        // value is like 1(int)
                        index = MapSetting.value(mapgroupName+"/"+mapkey).toString();
                    }else{
                        qDebug()<<"未定義變數";
                    }
                }
            }
            qDebug()<<MeanGray<<index<<defectList;
            int defectNumber = 1;
            for(int t=0 ; t<defectList.size();t++){
                ui->table_defectlist->insertRow(t);
                QTableWidgetItem *item_number = new QTableWidgetItem(QString::number(defectNumber));
                QTableWidgetItem *item_patternName = new QTableWidgetItem(mapgroupName);
                QTableWidgetItem *item_modelName = new QTableWidgetItem(ModelName);
                QTableWidgetItem *item_index = new QTableWidgetItem(index);
                QTableWidgetItem *item_XY = new QTableWidgetItem(defectList[t]);
                QTableWidgetItem *item_meanGray = new QTableWidgetItem(MeanGray);
                QTableWidgetItem *item_Date = new QTableWidgetItem(datePart);
                QTableWidgetItem *item_Time = new QTableWidgetItem(timePart);


                ui->table_defectlist->setItem(t, 0, item_number);
                ui->table_defectlist->setItem(t, 1, item_modelName);
                ui->table_defectlist->setItem(t, 2, item_index);
                ui->table_defectlist->setItem(t, 3, item_patternName);
                ui->table_defectlist->setItem(t, 4, item_XY);

                ui->table_defectlist->setItem(t, 6, item_meanGray);
                ui->table_defectlist->setItem(t, 7, item_Date);
                ui->table_defectlist->setItem(t, 8, item_Time);
                defectNumber++;
            }
            qDebug()<<"-------------";
        }
    }
}
