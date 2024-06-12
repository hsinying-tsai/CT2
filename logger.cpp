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
        return "ERROR";
    case null:
        return "";
    default:
        return "UNKNOWN";
    }
}

void Logger::writeLog(LogType type, const QString &message)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    dataTimeString = currentDateTime.toString("hh:mm:ss");

    //如果沒有下三行->QFSFileEngine::open: No file name specified
    timestamp = QDateTime::currentDateTime().toString("yyyyMMdd");
    logFileName = QString("log_%1.log").arg(timestamp);
    QDir logDir("Log");
    filePath = logDir.filePath(logFileName);
    QFile logFile(filePath);

//    qDebug()<<filePath;
    emit updateUILog(logTypeToString(type),message);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "|logger|Failed to open log file.";
        return;
    }
    QTextStream m_textstream(&logFile);
    m_textstream << dataTimeString << "\t" << logTypeToString(type)<<" : " << message << "\n";
    m_textstream.flush();
}


void Logger::populateCombowithFileName(QComboBox *combobox, const QString directoryPath)
{

    QDir directory(directoryPath);
    QStringList fileNames = directory.entryList(QStringList()<<"*.log", QDir::Files);
    combobox->clear();
    foreach (const QString &fileName, fileNames){
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
}

void Logger::create_file()
{
    //check "LOG" exist
    QDir logDir("Log");
    if(!logDir.exists()){
        logDir.mkpath(".");
        writeLog(Logger::Info, "|SYSTEM| New log folder create.");

    }else{
        writeLog(Logger::Info, "|SYSTEM| Log folder exist.");
    }

    timestamp = QDateTime::currentDateTime().toString("yyyyMMdd");
    logFileName = QString("log_%1.log").arg(timestamp);
    filePath = logDir.filePath(logFileName);

    QFile logFile(filePath);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        qDebug()<<"|logger|Failed to create log file.";
        return;
    }
    if(!logFile.exists()){
        qDebug()<<".log file not exist";
        writeLog(Logger::Info, "|SYSTEM| New .log file create.");
    }

    m_lastDateChecked = timestamp;
}

