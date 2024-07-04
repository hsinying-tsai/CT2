#include "widget.h"
#include <QApplication>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ini.h"
#include "myFuncts.h"
#include <QFile>
#include <QTextStream>
#include "logger.h"
#include <QMessageLogContext>
#include <pylon/PylonIncludes.h>
char buffIni[40];
char iniFile[20];

uint8_t CAM1_exposureTime;
uint16_t COORDINATE_PTsX,COORDINATE_PTsY;
Logger logger;
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    switch (type) {

    case QtCriticalMsg:
        logger.writeLog(Logger::Error, msg);
        break;
    case QtFatalMsg:
        logger.writeLog(Logger::Error, "Fatal error: " + msg);
        // 調用標準的崩潰處理程序
        abort();
    }
}
int main(int argc, char *argv[])
{

    //192.168.1.168
    //8501
    if(argc>1)
    {
        strcpy(iniFile,argv[1]);
    }
    else
    {
        strcpy(iniFile,"config.ini");
    }
    if(!initFuncts(iniFile))
    {
        puts("initFuncts error");
        return EXIT_FAILURE;
    }else{
        printf("Setting CAM1_exposureTime %i\n",CAM1_exposureTime);
        printf("Setting COORDINATE_PTsX %i\n", COORDINATE_PTsX);
        printf("Setting COORDINATE_PTsY %i\n", COORDINATE_PTsY);

//        return EXIT_SUCCESS;
    }

    QApplication a(argc, argv);
    Pylon::PylonAutoInitTerm pylonInit;
    //如果取消註解,會將應用程式輸出的所有訊息寫入log,包含qDebug()<<...
//    qInstallMessageHandler(customMessageHandler);
    Widget w;
    w.show();

     return a.exec();
}
