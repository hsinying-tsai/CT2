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
#include"defineCoordinate.h"
#include <QListWidget>
#include <QColor>
#include "guicamera.h"
#include "maindialog.h"
#include <QMainWindow>
#include "funcpar.h"
#include "ini.h"
#include <QTableWidgetItem>
#include "structImage.h"
#include "imageprocess.h"
#include "my_qlabel.h"
#include <QQueue>
#include <QTimer>
QT_BEGIN_NAMESPACE
extern char buffIni[40];
extern char iniFile[20];
extern uint8_t CAM1_exposureTime;
extern uint16_t COORDINATE_PTsX,COORDINATE_PTsY;
namespace Ui {
class Widget;
}
QT_END_NAMESPACE
class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    int count_num = 0,num = 1, time = 0,ARM_posX = 0,ARM_posY = 0,count_runModeclickedtime=1,runMode = 0,LightBlink = 0,count = 1;
    double factor_X=1,factor_Y=1;
    bool ReadpuB_isPressed = false, WritepuB_isPressed=false,change_flawPG = false,checkbox_onlyThisTime = false
            , revisePatternList = false,addPattern = false,error = false,everOccurSocketError = false
            , clearCommand = true, firstTryReconnect = true,firstTryReconnectsuccess = false;
    Logger logger;
    defineCoordinate DC;
    INI ini;
    imageprocess process;
    my_qlabel MQ;
    tcp_client *tc= new tcp_client(nullptr);
    QPixmap pix_Ini,pix2;
    QString rev_text, new_send_data, str1;
    const QByteArray send_data;
    QList<QLineEdit*> lineEdits;
    QVector<QString> orgi_text,new_text;
    QRegularExpression regex;
    QRegularExpressionMatch match;
    QStringList parts,run_pattern_name,show_pattern_name;
    QString configFilePath;

    //save the defect point and pattern in order to draw rectangle,[0]->patternName,[...]->defectPoint
    struct defectInfo{
      QString PatternName;
      QVector<QPoint> defectPoint;
    };
    QList<defectInfo> DrawRectangle;

    double calculateMean(const QString &imagepath);
    //存文字
    QVector<QString> matrix_buffer_name = {"DM200", "DM202","DM204","DM206","R200","R201","R202","R203","R204","R205"
                                               ,"R206","R207","R212","R214"};
    //存變數數值
    QVector<QString> orig_buffer;
    QVector<QString> buffer = {"DM200", "DM202","DM204","DM206","R200","R201","R202","R203","R204","R205"
                               ,"R206","R207","R212","R214"};

    //find model name and pattern name
    struct model_name{
      QString modelName;
      QList<QPair<int,QString>> patternsInfo; //{patternIndex, patternName}
    };

    QList<model_name> modelList;
    QString picfoldpath;

    QVector<ImageProcess> Images;
    ImageProcess tmp;

    //GUI
    QVector<QImage> imageVectorBig,imageVectorSmall;
    ~Widget();

    //camera
    void ShowWarning(QString warningText);
private slots:
    //關於defect viewer界面
    void on_puB_pre_clicked();
    void on_puB_next_clicked();
    void MouseCurrentPos();
    void updatelblPic();
    void change_pic2(int index);

    void Qtimer(); //per second clock for DM200
    void QtimerError(); //per 900ms clock for R215


    //關於設置將更改後的內文以紅色標注
    void saveText();
    void comp_text();

    //關於傳送訊息至PLC
    void on_puB_connect_clicked();
    void WR_command(QString WR_message);
    void on_puB_sent_clicked();
    void on_puB_start_clicked();
    void on_puB_read_clicked();
    void on_puB_write_clicked();
    void recv_label_update(QString message);

    void connect_label_update(); //socket通訊狀態改變
    void INI_UI();
    void on_puB_runMode_clicked();
    void on_puB_saveINI_clicked();
    void on_puB_remove_p_clicked();
    void on_puB_add_p_clicked();
    void on_puB_save_p_clicked();
    void updatetextlog(QString type, QString message);
    void prettiertextlog();
    void on_checkBox_onlyThisTime_stateChanged(int state);
    void displayLastLog();
    void updatecombopattern();
    void on_puB_gui_clicked();
    void on_puB_func_clicked();
    void on_puB_remove_m_clicked();
    void on_puB_add_m_clicked();    
    void on_list_model_itemDoubleClicked(QListWidgetItem *item);
    void on_puB_setCur_m_clicked();
    void on_table_defectlist_cellClicked(int row, int column);
    void updateComboBoxModel();
    void imagesprocess();

    //用於表單顯示
    void on_comboBox_model_activated(const QString ModelName);
    void on_comboBox_date_activated(const QString TimeDir);

    //文件儲存
    void CopyRecipe(QString originFilePath,QString CopyFilePath);
    void CreateMap(QString path);
    void CreateNReadRecipe();
    void CreateFolder(QString path,QString FolderName);
    void reviseconfigINI(QString section, QString key,QString Value);

    // Slots for GuiCamera signals
    void OnNewGrabResult(int userHint);
    void on_puB_bigGrab_clicked();
    void on_puB_samllGrab_clicked();
    void takeQImagetoList(const QImage &image, int OisBig);
    void on_puB_cameraINI_clicked();

    void runInit();
    void mySQL();
    void addNewModel(QString ModelName, QList<QPair<int,QString>> patternList);//由mySQL建立新model

private:

    Ui::Widget *ui;
    MainDialog MD;
    FuncPar FP;
    QThread clientThread;

    //camera para
    Pylon::DeviceInfoList_t m_devices;
    static const int MaxCamera = 3;
    CGuiCamera m_camera[MaxCamera];
    void cameraInit();

    //About save pic
    QDateTime RunCurrentDateTime;
    QString RunCurrentModel,RundataTimeString,RunTimefolderpath;
    int RunDefectNumber,RunPatternAmount,RunPatternNumber = 0;
    //RunPatternNumber用於切換下張pattern
    QList<QPair<int,QString>> patternIndexNname;

    //for command
    QQueue<QString> commandQueue;
    QTimer *timer_connect = new QTimer(this);

    //用於顯示表格
    QList<QString> defectTypes = {"BP","DP","HOpen", "VOpen", "HShort", "VShort"};

    //用於將讀取mySQL的資料存下
    struct patterns{  
      int patternIndex;
      QString patternName;
      bool checkBP;
      bool checkDP;
      bool checkBL;
      bool checkDL;
      bool checkMura;
    };
    QList<patterns> CurModel;
    patterns NewPattern;


private slots:
    //Alarm
    void showAlarm(bool isSocketError);
    void onConnectTimeout();
    void onStateChanged();

    void on_puB_SQL_clicked();
};

#endif // WIDGET_H
