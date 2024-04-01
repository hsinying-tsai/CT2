#include "logger.h"
#include<QDebug>
Logger::Logger(QObject *parent) : QObject(parent)
{

}
QString Logger::logTypeToString(LogType type)
{
    switch(type){
    case Info:
        return "INFO";
    case Warning:
            return "WARNING";
    case Error:
        return "ERROR\t";
    default:
            return "UNKNOWN";

    }
}


void Logger::writeLog(LogType type, const QString &message)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dataTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QFile logFile(filePath);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Failed to open log file.";
        return;
    }
    QTextStream m_textstream(&logFile);
    m_textstream<<dataTimeString<<"\t"<<logTypeToString(type)<<" : "<<message<<"\n";;
    m_textstream.flush();
}

void Logger::populateCombowithFileName(QComboBox *combobox, const QString directoryPath)
{
    QDir directory(directoryPath);
    QStringList fileName = directory.entryList(QStringList()<<"*.log", QDir::Files);
    combobox->clear();
    foreach (const QString &fileName, fileName) {
        QString name = fileName.split('.').first();
        combobox->addItem(name);
    }
}

void Logger::on_comboBox_currentIndexChanged(int index, QTextBrowser *textBrowser,QComboBox *combobox, const QString &directoryPath)
{
    if(index == -1){
        return;
    }
    QString fileName = QString("%1/%2.log").arg(directoryPath).arg(combobox->currentText());
    QFile logFile(fileName);
    if(!logFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        textBrowser->setPlainText("Failed to open file");
        return;
    }
    QTextStream in(&logFile);
    textBrowser->setPlainText(in.readAll());
    logFile.close();
}

void Logger::create_file()
{
    //check "LOG" exist
    QDir logDir("Log");
    if(!logDir.exists()){
        logDir.mkpath(".");
    }

    timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    logFileName = QString("log_%1.log").arg(timestamp);
    filePath = logDir.filePath(logFileName);


    QFile logFile(filePath);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Failed to create log file.";
        return;
    }
    writeLog(Logger::Info, "New log file create.");
//    logFile.close();
}