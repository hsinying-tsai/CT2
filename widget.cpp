#include "widget.h"
#include <QCursor>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QThread>
#include <QValidator>
#include <QWheelEvent>
#include "./ui_widget.h"
#include <math.h>
#include <QInputDialog>
#include <QTableWidget>
#include <QSettings>
#include <QImage>
#include <QMessageBox>
#include <QDateTime>
#include <QGroupBox>

//MySQL
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

//JSON
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);
    this->setWindowTitle("CT2");
    //define label size
    MQ.labelWidth = ui->lbl_pic->width();
    MQ.labelHeight = ui->lbl_pic->height();
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
    connect(tc->client, &QTcpSocket::stateChanged, this, &Widget::onStateChanged);

//        tc = new tcp_client();
    tc->moveToThread(&clientThread);

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
    QDir currentDir = QDir::currentPath();
    currentDir.cdUp();
    pic_logo.load(currentDir.filePath("CT2_mySQL/build/AUO.jpg"));//路徑可能要改
    ui->lbl_logo->setPixmap(pic_logo);
    ui->lbl_logo_2->setPixmap(pic_logo);

    // clock (per second
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Qtimer()));
    timer->start(1000);

    // clock (per second
    QTimer *timer_error = new QTimer(this);
    connect(timer_error,SIGNAL(timeout()),this,SLOT(QtimerError()));
    timer_error->start(900);

    // clock to check socket connect alarm
    timer_connect->setInterval(5000);
    timer_connect->setSingleShot(true);
    connect(timer_connect,SIGNAL(timeout()),this,SLOT(onConnectTimeout()));

    // do reschedule and send command
    QTimer *timer_command = new QTimer(this);
    connect(timer_command,&QTimer::timeout,this,[this](){
        //連線狀態下才需check
        if(tc->connnect_state == 1){
            if(!commandQueue.isEmpty()){
                int valueIndex = -1;
                //DM200,R215的指令優先傳送
//                qDebug()<<"ori："<<commandQueue;
                for(int i = 0; i < commandQueue.size(); ++i){
                    if (commandQueue[i].contains("DM200") || commandQueue[i].contains("R215")) {
                        valueIndex = i;
                    }
                    if (valueIndex != -1) {
                        QString InsertCommand = commandQueue.takeAt(valueIndex);
//                        qDebug()<<"Insert"<<InsertCommand<<"to the head";
                        commandQueue.prepend(InsertCommand);
                        valueIndex=-1;
                    }
                }
                //error狀態閃黃燈
                if(error){
                    LightBlink++;
                    if(LightBlink%2 == 1){
                        ui->lbl_plc->setStyleSheet("border-width: 3px;"
                                                   "border-radius: 20px;"
                                                   "margin:10px;"
                                                   "padding:20px;"
                                                   "background-color: gold;");
                    }else{
                        ui->lbl_plc->setStyleSheet("border-width: 3px;"
                                                   "border-radius: 20px;"
                                                   "margin:10px;"
                                                   "padding:20px;"
                                                   "background-color: transparent;");
                    }
                }
//                qDebug()<<"fixed："<<commandQueue;                
                WR_command(commandQueue.head());
                qDebug()<<"check"<<commandQueue;
            }   
        }
    });
    timer_command->start(200);
    clientThread.start();
    cameraInit();
    mySQL();
    INI_UI();
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
    ui->CAM1_exposure_Edit->setText(QString::number(CAM1_exposureTime));
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

void Widget::showAlarm(bool isSocketError)
{
    clearCommand = false;
    QMessageBox AlarmCheck;
    //true  -> socket connection error
    //false -> detect flow error
    if(isSocketError){      
        logger.writeLog(Logger::Warning, "Socket connection error occurred.");
        AlarmCheck.setWindowTitle("通訊異常");
        AlarmCheck.setText("偵測到socket通訊異常,請再嘗試連接或中斷連接.");
        QPushButton *tryButton = AlarmCheck.addButton(tr("Try"), QMessageBox::ActionRole);
        QPushButton *abortButton = AlarmCheck.addButton(QMessageBox::Abort);
        AlarmCheck.setIcon(QMessageBox::Critical);
        //第一次中斷->嘗試自動重新連線
        if(firstTryReconnect){
            timer_connect->start();
            qDebug()<<"Auto reconnect";
            tc->client->abort();
            tc->address = QHostAddress(ui->AddressEdit->text());
            tc->port = ui->PortEdit->text().toUShort();
            tc->initClent();
            ui->lbl_state->setText("監測到socket中斷,自動嘗試連接...");
        }else{
            //其餘讓user決定 try or abort
            AlarmCheck.exec();
            if (AlarmCheck.clickedButton() == tryButton) {
                qDebug()<<"try";
                logger.writeLog(Logger::Info, "User clicked `try` button.");
                tc->client->abort();
                tc->address = QHostAddress(ui->AddressEdit->text());
                tc->port = ui->PortEdit->text().toUShort();
                tc->initClent();
                ui->lbl_state->setText("等待連接...");
                timer_connect->start();
            }else if (AlarmCheck.clickedButton() == abortButton) {
                logger.writeLog(Logger::Warning, "User clicked `abort` button.");
                everOccurSocketError = false;
                qDebug()<<"clicked absort";
                tc->client->abort();
                ui->lbl_state->setText("已手動中斷通訊");
            }
            AlarmCheck.close();
        }
    }else{
        ShowWarning("接收到R215異常訊號,請解除異常再按OK.");
        logger.writeLog(Logger::Warning, "R215 signal detected.");
    }
}

void Widget::onConnectTimeout()
{
    qDebug()<<"onConnectTimeout"<<tc->client->state();
    if (tc->client->state() != QAbstractSocket::ConnectedState) {
        if(firstTryReconnect){
            firstTryReconnect = false;
            showAlarm(true);
        }
        ui->lbl_state->setText("重新連接失敗");
    }
}

void Widget::onStateChanged()
{
    if (tc->client->state() == QAbstractSocket::ConnectedState) { 

        ui->lbl_plc->setStyleSheet("border-width: 3px;"
                                   "border-radius: 20px;"
                                   "margin:10px;"
                                   "padding:20px;"
                                   "background-color: green;");

        //確認是否為斷線後重新連接
        if(everOccurSocketError){
            if(firstTryReconnect){
                qDebug()<<"Auto";
                firstTryReconnectsuccess = true;
                ui->lbl_state->setText("自動重新連接成功");
                logger.writeLog(Logger::Info, "Auto reconnection successful.");
            }else{
                qDebug()<<"manual";
                ui->lbl_state->setText("重新連接成功");     
                logger.writeLog(Logger::Info, "Manal reconnection successful.");
                qDebug()<<"onStateChange:"<<commandQueue;
                //找出檢測指令
                QString CurrentStep;
                for(int i=0;i<commandQueue.count();i++){
                    if(CurrentStep.isEmpty()){
                        if(!commandQueue[i].contains("R215") && !commandQueue[i].contains("DM200")){
                            CurrentStep = commandQueue[i];
                            qDebug()<<CurrentStep;
                        }
                    }
                }
                QString message = QString("目前流程為: %1 ，是否要繼續流程？").arg(CurrentStep);
                QMessageBox::StandardButton checkContinueRun;
                checkContinueRun = QMessageBox::question(this, "確認", message, QMessageBox::Yes | QMessageBox::No);
                if (checkContinueRun == QMessageBox::Yes) {
                    ui->lbl_state->setText("｜繼續運行檢測流程｜");
                }else{
                    ui->lbl_state->setText("請確認是否重新運行");
                    QMessageBox::StandardButton checkClearCommand;
                    QString message = QString("將重新運行檢測流程");
                    checkClearCommand = QMessageBox::question(this, "重新運行" , message, QMessageBox::Yes| QMessageBox::No);
                    if (checkClearCommand == QMessageBox::Yes) {
                        //去刪除comandQueue
                        ui->lbl_state->setText("｜重新運行檢測流程｜");
                        clearCommand = true;
                        connect_label_update();
                    }else{
                        //回到上個問題
                        onStateChanged();
                    }
                }
            }
            everOccurSocketError = false;
            firstTryReconnect = true;
            timer_connect->stop();  // 成功連接，停止計時器
        }
    }
}


void Widget::OnNewGrabResult(int userHint)
{
    if ((userHint == 0) && m_camera[0].IsOpen())
    {
        QMutexLocker locker( m_camera[0].GetBmpLock() );
        QImage image = m_camera[0].GetImage();
        this->ui->lbl_Img->setPixmap(QPixmap::fromImage(image));
        takeQImagetoList(image,0);
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
    for(defectInfo &Pattern : DrawRectangle){
        if(Pattern.PatternName == show_pattern_name.at(num - 1)){
            ui->lbl_pic->setImage(pix_Ini,Pattern.defectPoint);
        }
    }
}

void Widget::on_puB_sent_clicked()
{
    logger.writeLog(Logger::Info, "User clicled Button puB_sent.");
    const QByteArray send_data = ui->textSend->toPlainText().toUtf8();
    commandQueue.enqueue(send_data);
}
void Widget::on_puB_pre_clicked()
{
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
    if(message.contains("SocketError")){
        ui->textRecv->append(message);
        qDebug()<<"received:"<<message;
        everOccurSocketError = true;
        logger.writeLog(Logger::Warning, "Socket " + tc->client->errorString()+".");
        showAlarm(true);
    }else if (str1.isEmpty()){
        qDebug()<<"prevent crushed";
        for(int i=0;i<commandQueue.count();i++){
            if(commandQueue[i].isNull()){
                //解決check ("RD R215", "")的問題
                commandQueue.removeAt(i);
            }
        }
    }else if(str1 == "RD R215"){
        commandQueue.dequeue();
        if(message == "1"){
            error = true;
            ui->lbl_state->setText("！！出現異常事件,檢測流程暫停！！");
            for(int i = 0 ; i<commandQueue.size();i++){
                if(!commandQueue[i].contains("R215") && !commandQueue[i].contains("DM200")){
                    qDebug()<<"clear:"<<commandQueue[i];
                    commandQueue[i].clear();
                }
            }
            showAlarm(false);
            qDebug()<<"ERRRRRRRRRRRRRROR";
        }else{
            error = false;
            ui->lbl_state->setText(" ");
            ui->lbl_plc->setStyleSheet("border-width: 3px;"
                                       "border-radius: 20px;"
                                       "margin:10px;"
                                       "padding:20px;"
                                       "background-color: green;");
        }
    }else {
        ui->textRecv->append("received:"+message+"\tfrom: "+str1);
        qDebug()<<"received:"<<message<<"\tfrom"<<str1;
        logger.writeLog(Logger::Info, "Received message " + message + ".");
        //Read
        if (ReadpuB_isPressed == true){
            commandQueue.dequeue();
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
                commandQueue.enqueue("RDS DM200 7");
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
            qDebug()<<"Write"<<str1;
            for(int i = 0;i<commandQueue.count();i++){
                if(commandQueue[i] == str1){
                    commandQueue.removeAt(i);
                }
            }
            if (count_num == 8) {
                WritepuB_isPressed = false;
                logger.writeLog(Logger::Info, "Wrote PLC index.");
                count_num = 0;
                //OK\r\n
            } else if (message == "OK"){
                qDebug()<<"count_num"<<count_num;
                if(count_num == 4 || count_num == 6){
                    //DM204,DM206
                    qDebug()<<buffer[count_num / 2];
                    QString buffer_combined = QString("%1 %2%3%4 %5")
                                                  .arg("WR")
                                                  .arg("DM")
                                                  .arg(200 + count_num)
                                                  .arg(".S")
                                                  .arg(buffer[count_num / 2]);
                    commandQueue.enqueue(buffer_combined);
                }else{
                    QString buffer_combined = QString("%1 %2%3 %4")
                                                  .arg("WR")
                                                  .arg("DM")
                                                  .arg(200 + count_num)
                                                  .arg(buffer[count_num / 2]);
                    commandQueue.enqueue(buffer_combined);             
                }
                count_num += 2;
            }
        } else if (message == "OK"){
            // WR事件
            parts = str1.split(" ");
            if(runMode == 1 && parts[0]=="WRS"){
                if(parts[1]== "R200"){
                    commandQueue.dequeue();
                    qDebug()<<"dequeue WRS R200 2 0 0";
                    showDetectFlow(ui->lbl_step2);
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(patternIndexNname.at(RunPatternNumber).first);
                    commandQueue.enqueue(buffer_combined);
                    qDebug()<<"--------------Step_3.1巨觀change PG_num to"<<patternIndexNname.at(RunPatternNumber).first;
                    ui->lbl_state->setText("｜巨觀｜切換pattern至"+patternIndexNname.at(RunPatternNumber).second);
                    RunPatternNumber++;
                }else if(parts[1] == "R202"){
                    commandQueue.dequeue();
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
                        ui->lbl_state->setText("｜微觀｜移動至X="+QString::number(ARM_posX));
                        commandQueue.enqueue(buffer_combined);
                    }else{
                        Images.push_back(tmp);
                        qDebug()<<"push";
                        if(RunPatternNumber<RunPatternAmount){
                            //To the next PG_num
                            qDebug() << "--------------Step_3.server已回應OK.\n";
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(patternIndexNname.at(RunPatternNumber).first);
                            commandQueue.enqueue(buffer_combined);
                            qDebug()<<"--------------Step_3.2巨觀change PG_num to"<<patternIndexNname.at(RunPatternNumber).first;
                            ui->lbl_state->setText("｜巨觀｜切換pattern至"+patternIndexNname.at(RunPatternNumber).second);
                            RunPatternNumber++;
                        }else{
                            imagesprocess();
                            qDebug() << "--------------Step_3.All pattern was changed.";
                            commandQueue.enqueue("WR R204 1");
                        }
                    }
                }else if(parts[1] == "R204"){
                    commandQueue.dequeue();
                    //if change_flawPG == true ->to step.5
                    if(!DC.vector_PG_flaw.isEmpty()){
                        change_flawPG = true;
                        qDebug()<<"微觀"<<DC.current->index;
                        QString buffer_combined = QString("%1 %2").arg("WR DM202").arg(DC.current->index);  
                        commandQueue.enqueue(buffer_combined);
                    }else{
                        qDebug()<<"no flaws";
                        ui->lbl_state->setText("未檢測出瑕疵點");
                        commandQueue.enqueue("WR R214 1");
                    }
                }else if(parts[1] == "R206"){
                    commandQueue.dequeue();
                    if(DC.current->next == NULL){
                        //change all flaw pattern and sent all flaws->go to step7
                        change_flawPG = false;
                        commandQueue.enqueue("WR R214 1");
                        qDebug()<<"--------------Step_7.send end signal to PLC.";
                    }else{
                        DC.current = DC.current->next;
                        //如果只有定義一個node,會在這crashed,DC.current->next = 0X18
                        if(DC.current->index == DC.current->prev->index){
                            //don't change pattern
                            ARM_posX = (DC.current->x - DC.current->prev->x)*factor_X;
                            ARM_posY = (DC.current->y - DC.current->prev->y)*factor_Y;
                            qDebug() << "--------------Step_6.Sending x = " << ARM_posX;
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM204").arg(ARM_posX);
                            ui->lbl_state->setText("｜微觀｜移動至X="+QString::number(ARM_posX));
                            commandQueue.enqueue(buffer_combined);
                        }else{
                            // change pattern
                            qDebug()<<"change pattern";
                            RunDefectNumber = 1;
                            QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM202").arg(DC.current->index);
                            commandQueue.enqueue(buffer_combined);
                        }     
                        ui->lbl_state->setText("｜微觀｜切換至 "+patternIndexNname.at(DC.current->index-1).second+" Pattern");
                    }
                }else{
                    //receive OK from "WRS DM201 6 0 0 0 0 0 0
//                    qDebug()<<"--------------Step_7.Finish.";
                    runInit();
                    commandQueue.dequeue();
                    qDebug()<<"New action";
                    showDetectFlow(ui->lbl_step5);
                    ui->lbl_state->setText("檢測流程結束"); 
                    str1.clear();
                    commandQueue.enqueue("RD R212");
                }
            }else if(parts[0] == "WRS"){
                //Manual state -> stop the loop.
            }else if(parts[1]=="R214"){
                commandQueue.dequeue();
                commandQueue.enqueue("WRS DM201 6 0 0 0 0 0 0");
            }else if(parts[1]=="DM200"){
                commandQueue.dequeue();
//              qDebug()<<"alive";
            }else if(parts[1] == "R202"){
                commandQueue.dequeue();
                commandQueue.enqueue("RD R203");
            }else if(parts[1] == "DM202"){
                commandQueue.dequeue();
                if(change_flawPG == false){
                    qDebug()<<"拍攝巨觀"<<patternIndexNname.at(RunPatternNumber-1).second+" Pattern";
                    ui->lbl_text2->setText("拍攝巨觀->"+patternIndexNname.at(RunPatternNumber-1).second);
                    on_puB_bigGrab_clicked();
                }
                commandQueue.enqueue("WR R202 1");
            }else if(change_flawPG == true){
                // change flaw pattern
                // step5 step6 step7
                if(parts[1].contains("DM204")){
                    commandQueue.dequeue();
                    qDebug() << "--------------Step_6.Sending y = " << ARM_posY;
                    QString buffer_combined = QString("%1 %2 %3").arg("WR").arg("DM206").arg(ARM_posY);
                    ui->lbl_state->setText("｜微觀｜移動至Y="+QString::number(ARM_posY));          
                    commandQueue.enqueue(buffer_combined);
                }else if(parts[1].contains("DM206")){
                    commandQueue.dequeue();
                    QString patternName;
                    foreach(QPair pattern,patternIndexNname){
                        if(pattern.first == DC.current->index){
                            patternName = pattern.second;
                        }
                    }
                    ui->lbl_state->setText("｜微觀｜目前在拍攝"+patternName+" 微觀照片");
                    qDebug()<<"目前在拍攝"+patternName+" 微觀照片";
                    ui->lbl_text4->setText("拍攝微觀->"+patternName);
                    on_puB_samllGrab_clicked();
                    commandQueue.enqueue("WR R206 1");
                }else if(parts[1] == "R206"){
                    commandQueue.dequeue();
                    commandQueue.enqueue("RD R207");
                }
            }else{
                if(parts[0]=="RD"){
                   qDebug()<<"keep Read";
                }else{
                    qDebug()<<"R200,R202,R204";
                    if(parts[1]=="R200"){
                        commandQueue.dequeue();
                        commandQueue.enqueue("RD R201");
                    }else if(parts[1]=="R202"){
                        commandQueue.dequeue();
                        commandQueue.enqueue("RD R203");
                    }else if(parts[1]=="R204"){
                        commandQueue.dequeue();
                        commandQueue.enqueue("RD R205");
                    }else{
                        //WRITE DM204 DM206
                        for(int i = 0;i<commandQueue.count();i++){
                            if(commandQueue[i] == str1){
                                qDebug()<<"remove"<<commandQueue[i];
                                commandQueue.removeAt(i);
                            }
                        }
                    }
                }
            }
        }else{
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
                commandQueue.dequeue();
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
                commandQueue.enqueue("WRS R200 2 0 0");
            }
            if (buffer[7] == "1") {
                //R203 1
                //R202、R203 ->change pattern
                commandQueue.dequeue();
                buffer[6] = "0";
                buffer[7] = "0";
                logger.writeLog(Logger::Info, "Edge reset R202 and R203.");
                qDebug() << "--------------Step_2.server已回應OK，並將R202、R203歸零\n";
                commandQueue.enqueue("WRS R202 2 0 0");
            }
            if (buffer[9] == "1"){
                //R205 1
                //R204、R205->open microscopic photography
                commandQueue.dequeue();
                buffer[8] = "0";
                buffer[9] = "0";
                logger.writeLog(Logger::Info, "Edge reset R204 and R205.");
                qDebug() << "--------------Step_4.server已回應OK，並將R204、R205歸零\n";
                ui->lbl_state->setText("開始拍攝微觀");
                commandQueue.enqueue("WRS R204 2 0 0");
                showDetectFlow(ui->lbl_step4);
            }
            if(buffer[11] == "1"){
                //R207 1
                //R206、R207->sending and checking position
                commandQueue.dequeue();
                buffer[10] = "0";
                buffer[11] = "0";
                logger.writeLog(Logger::Info, "Edge reset R206 and R207.");
                qDebug() << "--------------Step_4.server已回應OK，並將R206、R207歸零\n";
                commandQueue.enqueue("WRS R206 2 0 0");
            }
            if(buffer[12] == "1"){
                //R212 1
                commandQueue.dequeue();
                buffer[12] = "0";
                qDebug()<<"--------------Step_2.PLC ready to run.";
                ui->puB_start->setEnabled(true);
                if(runMode == 1){
                    commandQueue.enqueue("WR R200 1");
                }
            }
        }
    }
}

void Widget::connect_label_update()
{

    if (tc->client->state() == QAbstractSocket::ConnectedState) {
        setWindowTitle(QString("CT2[%1:%2]")
                           .arg(tc->client->localAddress().toString())
                           .arg(tc->client->localPort()));
    } else {
        setWindowTitle("Client");
    }
    //connect state
    if (tc->connnect_state == 1) {
        if(firstTryReconnectsuccess){
            firstTryReconnectsuccess = false;
            qDebug()<<"Auto"<<patternIndexNname;
            return ;
        }
        firstTryReconnect = true;
        if(tc->client->errorString() !="The remote host closed the connection"){
            qDebug() << "--------------Step_1.Connection Successful";
            change_flawPG = false;
            if(clearCommand){
                commandQueue.clear();
                str1.clear();
                clearCommand = false;
            }
            if(!commandQueue.contains("RD R212")){
                commandQueue.enqueue("RD R212");
            }
            DC.current = DC.first;
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
        }
    }else if (tc->connnect_state == 0) {
        RunPatternNumber = 1;
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
        //將負值加上.S
        QStringList tmp_parts;
        tmp_parts = WR_message.split(" ");
        if(ARM_posX < 0 || ARM_posY < 0){
            if(tmp_parts[1]=="DM204" || tmp_parts[1]=="DM206"){
                commandQueue.dequeue();
                QString buffer_combined = QString("%1 %2 %3").arg(tmp_parts[0]).arg(tmp_parts[1]+".S").arg(tmp_parts[2]);
                WR_message = buffer_combined;
                commandQueue.enqueue(WR_message);
            }
        }
        str1 = WR_message;
        logger.writeLog(Logger::Info, "User sent Message'" + WR_message + "'.");
        const QByteArray send_data = WR_message.toUtf8();
        if (send_data.isEmpty()){
            return;
        }
        new_send_data = QString("%1\r").arg(QString::fromUtf8(send_data));
        tc->client->write(new_send_data.toUtf8());
    }
}

void Widget::on_puB_connect_clicked()
{
    logger.writeLog(Logger::Info, "User clicked ButtonpuB_connect.");
    //初始化計時器
    time = 0;
    commandQueue.clear();
    // 判斷當前是否有連接，連接了就斷開
    if (tc->client->state() == QAbstractSocket::ConnectedState) {
    } else if (tc->client->state() == QAbstractSocket::UnconnectedState) {
        tc->address = QHostAddress(ui->AddressEdit->text());
        tc->port = ui->PortEdit->text().toUShort();
        showDetectFlow(ui->lbl_step1);
    }
    tc->initClent();
}

void Widget::Qtimer()
{
    //判斷檢測日期是否有更改
    //日期改變->需創建新的.log(writeLog->改路徑, 需更新comboBox->logger選單)以及model需創建新的資料夾
    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    if(currentDate!=logger.m_lastDateChecked){
        qDebug()<<"<隔天>";
        logger.create_file();
        logger.populateCombowithFileName(ui->comboBox_logger, "Log");
    }
    if(tc->connnect_state == 1){
        if(ReadpuB_isPressed == false){
            //每秒傳送WR DM200
            buffer[0] = QString::number(time);
            time++;
            QString command = QString("%1 %2").arg("WR DM200").arg(time);
            commandQueue.enqueue(command);
        }
    }
    if(timer_connect->isActive()){
        qDebug()<<"----------";
        tc->initClent();
    }
}

void Widget::QtimerError()
{
    //read error signal
    if(tc->connnect_state == 1){
        commandQueue.enqueue("RD R215");
    }
}

void Widget::change_pic2(int index)
{
    logger.writeLog(Logger::Info, "User changed pic2.");
    pix2.load(picfoldpath + QString::number(num) + "_"+ QString::number(index) + ".bmp");
    qDebug()<<"pic2:"<<picfoldpath + QString::number(num) + "_"+ QString::number(index) + ".bmp";
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
               commandQueue.enqueue("WR R200 1");
            }
        }
    }
}
void Widget::on_puB_read_clicked()
{
    logger.writeLog(Logger::Info, "User clicked Button puB_read.");
    ReadpuB_isPressed = true;
    commandQueue.enqueue("RDS R200 10");
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
    QString buffer_combined = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11").arg("WRS R200 10")
         .arg(buffer[4]).arg(buffer[5]).arg(buffer[6]).arg(buffer[7]).arg(buffer[8])
         .arg(buffer[9]).arg(buffer[10]).arg(buffer[11]).arg(buffer[12]).arg(buffer[13]);
    commandQueue.enqueue(buffer_combined);
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
    // 已將config.ini合併至recipe.ini
    QString PT_width = ui->PT_width_Edit->text();
    QString PT_height = ui->PT_height_Edit->text();
    QString CAM1_ExposureTime = ui->CAM1_exposure_Edit->text();
    reviseconfigINI("COORDINATE","PT_sizeX",PT_width);
    reviseconfigINI("COORDINATE","PT_sizeY",PT_height);
    reviseconfigINI("CAM1","exposureTime",CAM1_ExposureTime);

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
    qDebug()<<"CAM1_ExposureTime"<<CAM1_ExposureTime;
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
            qDebug()<<"已刪除pattern:"<<PTselectedItem->text();
            ui->lbl_state->setText("已刪除pattern:"+PTselectedItem->text());
            delete ui->list_Pattern->takeItem(ui->list_Pattern->row(PTselectedItem));
            //如果正在修改設為運行的model,需重新setCurrent
            if(ModelName == ui->lbl_CurModel->text()){
                ui->lbl_CurModel->setText("(null)");
            }
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
    if(!ui->lbl_modelPT->text().isEmpty()){
        bool ok;
        QString newItemText = QInputDialog::getText(this,"新增",
                                                    "請輸入要新增的pattern:",
                                                    QLineEdit::Normal,
                                                    QString(),&ok);
        if(ok && !newItemText.isEmpty()){
            //check pattern exist
           addPattern = true;
           for(model_name &model : modelList) {
               if(model.modelName == ui->lbl_modelPT->text()){
                   for(QPair pattern : model.patternsInfo){
                       if(newItemText == pattern.second){
                           qDebug()<<"pattern.second"<<pattern.second;
                           ShowWarning("Pattern:"+newItemText+" 已存在");
                           addPattern = false;
                           ui->lbl_state->setText("輸入的"+newItemText+" 已存在");
                       }
                   }
               }
           }

           if(addPattern == true){
                ui->list_Pattern->addItem(newItemText);
                //如果正在修改設為運行的model,需重新setCurrent
                if(ui->lbl_modelPT->text() == ui->lbl_CurModel->text()){
                    ui->lbl_CurModel->setText("(null)");
                }
           }
        }

    }else{
        qDebug()<<"未選擇Model";
        ui->lbl_state->setText("尚未選擇Model！");
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
        QList<QPair<int,QString>> tmpList;
        for(model_name &model : modelList) {
             if(model.modelName == currentModel){
                model.patternsInfo.clear();
                int defalutIndex = 1;
                for(int i=0; i<ui->list_Pattern->count();i++){
                    qDebug()<<defalutIndex<<":"<<ui->list_Pattern->item(i)->text();
                    model.patternsInfo.append({defalutIndex,ui->list_Pattern->item(i)->text()});
                    tmpList.append({{defalutIndex,ui->list_Pattern->item(i)->text()}});
                    defalutIndex++;
                }
            }
        }
        qDebug()<<"tmpList"<<tmpList;
        QString tmpModePath = QString("%1%2").arg(QCoreApplication::applicationDirPath()+"/Model/"+currentModel+"/").arg("recipe.ini");
        FP.INI(tmpList, tmpModePath, currentModel,false);
        updatecombopattern();
        if(addPattern){
            FP.show();
            addPattern = false;
        }
        //如果正在修改設為運行的model,需重新setCurrent
        if(ui->lbl_modelPT->text() == ui->lbl_CurModel->text()){
            ui->lbl_CurModel->setText("(null)");
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
        ui->lbl_state->setText("請先點選save，再進行參數設置！");
    }else{
        FP.show();
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
    QString modelPath = "/Model/";
    QDir modelFolder("Model/"+inputModel);
    QMessageBox::StandardButton doublecheck;
    if (ok) {
        doublecheck = QMessageBox::question(this, "確認", "創建model:"+inputModel+"？", QMessageBox::Yes|QMessageBox::No);
        if(doublecheck == QMessageBox::Yes){
            // 檢查Model是否存在
           if (!modelFolder.exists()) {
               CreateFolder(modelPath, inputModel);
               CreateNReadRecipe();
               qDebug()<<"1234";
           }else{
               ShowWarning("Model:"+inputModel+" 已存在");
               qDebug() << "配置文件已存在： " << modelPath;
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
            qDebug() << "欲刪除資料夾路徑：" << deleDirPath;
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
    QString currentModel = item->text();
    CreateNReadRecipe();
    foreach (const model_name &model, modelList) {
        if(model.modelName == currentModel){
            ui->list_Pattern->clear();
            foreach (const QPair pattern, model.patternsInfo) {
                ui->list_Pattern->addItem(pattern.second);
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
                int patternIndex = setting.value(group+"/Index").toInt();
                QString patternName = group;
                patternIndexNname.append({patternIndex,patternName});
            }
        }
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
    QString pic2Path = QCoreApplication::applicationDirPath()+"/Model/"+CurrentModel+"/"+CurrentDateDir+"_"+CurrentTimeDir+"/"+patternIndex+"_"+Number+".bmp";
    pic2.load(pic2Path);
    qDebug()<<"pic2Path"<<pic2Path;
    ui->lbl_pic2->setPixmap(pic2);
    QString DF_XY = ui->table_defectlist->item(row, 4)->text();
    QString DF_type = ui->table_defectlist->item(row, 5)->text();
    ui->lbl_DFcoodinate->setText(DF_XY);
    ui->lbl_DFtype->setText(DF_type);
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
            qDebug()<<"|Initial|Model資料夾中有資料夾（有Model）："<<FilesinModelfolders;
            foreach(QString modelnamefolder,FilesinModelfolders){
//                qDebug()<<modelnamefolder;
                // recipe.ini配置文件是否存在
                configFilePath = QCoreApplication::applicationDirPath() + "/Model/"+modelnamefolder+"/recipe.ini";
                QFile configFile(configFilePath);
                if (!configFile.exists()) {
                    QList<QPair<int,QString>> tmp;
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
                                int patternIndex = settings.value(group+"/Index").toInt();
                                QString patternName = group;
//                                qDebug()<<"CreateNread:"<<patternIndex<<patternName;
                                newModel.patternsInfo.append({patternIndex,patternName});
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

void Widget::imagesprocess()
{
    qDebug()<<"全部巨觀照片一起做image process";
    showDetectFlow(ui->lbl_step3);
    //for test
    Images.clear();

    tmp.index = 1;
    tmp.patternName = "White";
    tmp.meanGray = 0.1;
    tmp.BPenable = true;
    tmp.DPenable = false;
    tmp.BLenable = true;
    tmp.DLenable = false;
    tmp.Muraenable = false;
    tmp.defectPoint = {{ImageProcess::BP,{QPoint(10, 10)}},{ImageProcess::VOpen,{QPoint(30, 30),QPoint(60, 60)}}};
    Images.push_back(tmp);

    tmp.index = 2;
    tmp.patternName = "Black";
    tmp.meanGray = 0.2;
    tmp.BPenable = false;
    tmp.DPenable = true;
    tmp.BLenable = false;
    tmp.DLenable = true;
    tmp.Muraenable = false;
    tmp.defectPoint = {{ImageProcess::HOpen,{QPoint(100, 100)}}};
    Images.push_back(tmp);

    tmp.index = 3;
    tmp.patternName = "Red";
    tmp.meanGray = 0.3;
    tmp.BPenable = true;
    tmp.DPenable = true;
    tmp.BLenable = true;
    tmp.DLenable = true;
    tmp.Muraenable = false;
    tmp.defectPoint = {{ImageProcess::BP,{QPoint(150, 150)}}};
    Images.push_back(tmp);

    tmp.index = 4;
    tmp.patternName = "Green";
    tmp.meanGray = 0.4;
    tmp.BPenable = false;
    tmp.DPenable = false;
    tmp.BLenable = false;
    tmp.DLenable = false;
    tmp.Muraenable = true;
    tmp.defectPoint = {{ImageProcess::BP,{QPoint(150, 150)}}};
    Images.push_back(tmp);

    //for test
//    QVector<ImageProcess>::iterator it = Images.begin();
//    for(; it != Images.end(); it++) {
//        if(it->patternName == "White"){
//            QImage imageTest(QCoreApplication::applicationDirPath()+"/EXP45606_WHITE.bmp");
//            it->image = imageTest;
//        }else{
//            QImage imageTest(QCoreApplication::applicationDirPath()+"/denoise.bmp");
//            it->image = imageTest;
//        }
//    }


    process.process(&Images);

    bool isHead = true;
    foreach(const ImageProcess &image, Images){
        foreach(QPair pairList , image.defectPoint){
            foreach(QPoint point , pairList.second){
                qDebug()<<point;
                DC.defNode(point,image.index,isHead);
                isHead = false;
            }
        }
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
    QString curDefectType;
    foreach(const ImageProcess &image, Images){
        settings.beginGroup(image.patternName);
        settings.setValue("index",image.index);
        settings.setValue("MeanGray",image.meanGray);
        qDebug()<<image.patternName;
        int DefectAmount = 1;
        foreach(QPair pairList , image.defectPoint){
            curDefectType = defectTypes.at(pairList.first);
            foreach(QPoint point , pairList.second){
                qDebug()<<curDefectType<<point.x()<<point.y();
                QString tmp = QString("%1_%2_%3").arg(curDefectType).arg("Defect").arg(QString::number(DefectAmount));
                QString tmpPoint = QString("(%1,%2)").arg(point.x()).arg(point.y());
                settings.setValue(tmp,tmpPoint);
                DefectAmount++;
            }
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
        imgName = QString("%1%2").arg(QString::number(patternIndexNname.at(RunPatternNumber-1).first)).arg(".bmp");
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
        tmp.index = patternIndexNname.at(RunPatternNumber-1).first;
        tmp.image = image;
        tmp.patternName = patternIndexNname.at(RunPatternNumber-1).second;
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
                    }else if(key==group+"/checkBL"){
                        tmp.BLenable = setting.value(key).toBool();
                    }else if(key==group+"/checkDL"){
                        tmp.DLenable = setting.value(key).toBool();
                    }else if(key==group+"/checkMura"){
                        tmp.Muraenable = setting.value(key).toBool();
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
    clearCommand = true;
    change_flawPG = false;
    Images.clear();
    DC.current = DC.first;
    RunPatternNumber = 0;
    QString runModelname = ui->lbl_CurModel->text();
    for(model_name &model : modelList) {
        if(model.modelName == runModelname){
            RunPatternAmount = model.patternsInfo.size();
            patternIndexNname.clear();
            for(QPair pattern : model.patternsInfo){
                patternIndexNname.append(pattern);
            }
         }
    }

    qDebug()<<"NOW:"<<RunPatternAmount<<patternIndexNname;
    QString showPatterns;
    foreach(QPair pattern, patternIndexNname){
        showPatterns.append(QString("%1->%2, ").arg(pattern.first).arg(pattern.second));
    }
    qDebug()<<"檢測："<<showPatterns;
    ui->lbl_state->setText("檢測："+showPatterns);

//    qDebug()<<"RUN->"<<runModelname<<":"<<RunPatternName<<","<<RunPatternAmount;
    //將流程label初始化
    for (int i = 1; i <= 5; ++i) {
        QLabel *lbl = findChild<QLabel *>(QString("lbl_step%1").arg(i));
        if (lbl) {
            lbl->setStyleSheet("margin:10px;"
                               "border-width: 3px;"
                               "background-color: gray;");

        }
    }
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
    if(!MapFile.exists()){
        qDebug()<<MapPath<<"->invalid";
        ui->table_defectlist->insertRow(0);
        QTableWidgetItem *item_model = new QTableWidgetItem("NULL");
        ui->table_defectlist->setItem(0, 0, item_model);
    }else{
        qDebug()<<"on_comboBox_date_activated";
        QSettings MapSetting(MapPath, QSettings::IniFormat);
        QStringList MapKeys = MapSetting.allKeys();
        QStringList MapGroups = MapSetting.childGroups();
        show_pattern_name = MapSetting.childGroups();
        //show Big pic
        picfoldpath = QCoreApplication::applicationDirPath()+"/Model/"+ModelName+"/"+TimeDir+"/";
        pix_Ini.load(picfoldpath + QString::number(num) + ".bmp");
        ui->lbl_pattern->setText("Pattern = " + QString(show_pattern_name.at(num - 1)));
        //MapGroups ("Black", "Red", "White")
        for(int i=0 ; i<MapGroups.count(); i++){
            QString mapgroupName = MapGroups[i];
            QString MeanGray,index,stringValue;
            QStringList defectList,defectType;
            qDebug()<<"["<<mapgroupName<<"]";
            defectInfo newPattern;
            newPattern.PatternName = mapgroupName;
            QStringList defectName;
            for(int j=0 ;j<MapKeys.count() ; j++){
                QString mapkey = MapKeys[j];
                //MapKeys ("White/BP_Defect1", "White/BP_Defect2", "Gray/HOpen_Defect1")
                if(mapkey.contains(mapgroupName)){
                    mapkey.remove(mapgroupName+"/");
                    //mapkey:HOpen_Defect_1
                    qDebug()<<mapkey;
                    if(mapkey.contains("_Defect")){
                        //to figure out defect type
                        defectName = mapkey.split('_');
                        defectType.append(defectName[0]);
                        //to figure out value
                        stringValue = MapSetting.value(mapgroupName+"/"+mapkey).toString();
                        defectList.append(stringValue);
                        QString trimmedStr = stringValue.mid(1, stringValue.length() - 2);
                        QStringList parts = trimmedStr.split(',');
                        int x = parts[0].toInt();
                        int y = parts[1].toInt();
                        QPoint coordinate =QPoint(x, y);
                        newPattern.defectPoint.append(coordinate);
                    }else if(mapkey == "MeanGray"){
                        // value is like 0.2(double)
                        qDebug()<<"1:"<<mapkey;
                        MeanGray = MapSetting.value(mapgroupName+"/"+mapkey).toString();
                    }else if(mapkey == "index"){
                        // value is like 1(int)
                        index = MapSetting.value(mapgroupName+"/"+mapkey).toString();
                    }else{
                        qDebug()<<"未定義變數";
                    }
                }
            }
            DrawRectangle.append(newPattern);
            qDebug()<<MeanGray<<index<<defectList<<num;

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
                QTableWidgetItem *item_defectType = new QTableWidgetItem(defectType[t]);

                ui->table_defectlist->setItem(t, 0, item_number);
                ui->table_defectlist->setItem(t, 1, item_modelName);
                ui->table_defectlist->setItem(t, 2, item_index);
                ui->table_defectlist->setItem(t, 3, item_patternName);
                ui->table_defectlist->setItem(t, 4, item_XY);
                ui->table_defectlist->setItem(t, 5, item_defectType);
                ui->table_defectlist->setItem(t, 6, item_meanGray);
                ui->table_defectlist->setItem(t, 7, item_Date);
                ui->table_defectlist->setItem(t, 8, item_Time);
                defectNumber++;
            }
        }
        for(defectInfo &Pattern : DrawRectangle){
            if(Pattern.PatternName == show_pattern_name.at(num - 1)){
                if(!pix_Ini.isNull()){
                    ui->lbl_pic->setImage(pix_Ini,Pattern.defectPoint);
                }else{
                    ui->table_defectlist->clearContents(); // 清除內容
                    ui->lbl_pic->clear();
                    ui->lbl_pic->setText("資料夾中無照片.");
                }
            }
        }
//        for(defectInfo &Pattern : DrawRectangle){
//            qDebug()<<Pattern.PatternName<<Pattern.defectPoint;
//        }
    }
}

void Widget::mySQL()
{
    //mySQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("172.20.10.3");
    db.setPort(3306);
    db.setDatabaseName("recipe");
    db.setUserName("agxCT2");
    db.setPassword("agx");
    qDebug()<<db.database();

    CurModel.clear();
    if(!db.open()){
        QString ms = QString("%1%2").arg("Database Error:\n").arg(db.lastError().text());
        ShowWarning(ms);
        ui->lbl_state->setText("未讀到mySQL資料");
        logger.writeLog(Logger::Warning, "MySQL Databse connection failed.");
    }else{
        ui->lbl_state->setText("已讀取mySQL資料");
        logger.writeLog(Logger::Info, "MySQL Databse connection successful.");
        QSqlQuery query;
        QString modelName;
        QList<QPair<int,QString>> singlePattern; //將patternName,patternIndex存下,用於創recipe.ini
        query.exec("SELECT * from `Cur_model_data`;");
        patternIndexNname.clear();
        while(query.next()){
            int id = query.value(0).toInt();
            int site = query.value(1).toInt();
            modelName = query.value(2).toString();
            QString data = query.value(3).toString();
            qDebug()<<id<<site<<modelName<<data;
            QJsonParseError jsonError;
            QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &jsonError);
            if(jsonError.error != QJsonParseError::NoError &&!doc.isNull()){
                qDebug()<<"JSON格式錯誤"<<jsonError.error;
            }
            QJsonObject rootObj = doc.object();
            QJsonValue pixels = rootObj.value("pixels");
            QJsonValue dimensions = rootObj.value("dimensions");

            qDebug()<<"pixels:"<<pixels.toObject();
            qDebug()<<"dimensions:"<<dimensions.toObject();

            NewPattern.patternName = rootObj.value("patternName").toString();
            NewPattern.patternIndex = rootObj.value("patternIndex").toInt();
            NewPattern.checkBP = rootObj.value("BP").toBool();
            NewPattern.checkDP = rootObj.value("DP").toBool();
            NewPattern.checkBL = rootObj.value("BL").toBool();
            NewPattern.checkDL = rootObj.value("DL").toBool();
            NewPattern.checkMura = rootObj.value("Mura").toBool();
            CurModel.append(NewPattern);
            qDebug()<<NewPattern.patternName<<NewPattern.checkBP<<NewPattern.checkDP<<NewPattern.checkBL<<NewPattern.checkDL;

            //RUN
//            RunPatternName.append(rootObj.value("patternName").toString());
            patternIndexNname.append({NewPattern.patternIndex,NewPattern.patternName});
            singlePattern.append({NewPattern.patternIndex,NewPattern.patternName});
            qDebug()<<"--------------------";
        }
        ui->lbl_CurModel->setText(modelName);
        addNewModel(modelName,singlePattern);
    }
}

void Widget::addNewModel(QString ModelName, QList<QPair<int, QString> > patternList)
{
   //判斷recipe是否存在,如果存在->刪除,創建最新內容
   QString modelPath = QCoreApplication::applicationDirPath()+"/Model/"+ModelName+"/recipe.ini";
   QFile modelRecipe(modelPath);

   if (modelRecipe.exists()) {
       modelRecipe.remove();
       qDebug()<<"刪除recipe";
   }else{
       CreateFolder("/Model/", ModelName);
   }
   //create recipe
   FP.INI(patternList,modelPath,ModelName,false);
   updateComboBoxModel();
   foreach(patterns p,CurModel){
       qDebug()<<p.patternName<<p.checkBP<<p.checkDP<<p.checkBL<<p.checkDL<<p.checkMura;
       QString PcheckBP = p.checkBP ? "true" : "false";
       QString PcheckDP = p.checkDP ? "true" : "false";
       QString PcheckBL = p.checkBL ? "true" : "false";
       QString PcheckDL = p.checkDL ? "true" : "false";
       QString PcheckMura = p.checkMura ? "true" : "false";
       FP.reviseModelINI(p.patternName, "checkBP" ,PcheckBP);
       FP.reviseModelINI(p.patternName, "checkDP" ,PcheckDP);
       FP.reviseModelINI(p.patternName, "checkBL" ,PcheckBL);
       FP.reviseModelINI(p.patternName, "checkDL" ,PcheckDL);
       FP.reviseModelINI(p.patternName, "checkMura" ,PcheckMura);
   }
}



void Widget::on_puB_SQL_clicked()
{
    mySQL();
}

void Widget::showDetectFlow(QLabel *label)
{
    label->setStyleSheet("margin:10px;"
                         "border-width: 3px;"
                         "background-color: darkblue;");
    QRegExp getNumber("(\\d+)");
    QString number;
    if (getNumber.indexIn(label->objectName()) != -1) {
        number = getNumber.cap(1);
    }
    for (int i = 1; i <= 5; ++i) {
        if (i != number.toInt()) {
            QLabel *lbl = findChild<QLabel *>(QString("lbl_step%1").arg(i));
            if (lbl) {
                lbl->setStyleSheet("margin:10px;"
                                   "border-width: 3px;"
                                   "background-color: gray;");
            }
            if(i == 3){
                ui->lbl_text2->setText("拍攝巨觀->(色)");
            }
            if(i == 5){
                ui->lbl_text4->setText("拍攝微觀->(色)");
            }
        }
    }
}
