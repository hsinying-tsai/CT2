#include "logger.h"
#include<QDebug>
Logger::Logger(QObject *parent) : QObject(parent)
{
    logFile.setFileName(filename);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Failed to open file";
        return;
    }

    m_textstream.setDevice(&logFile);

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
    default:
            return "UNKNOWN";

    }
}

void Logger::createLogFile()
{
    QDir logDir("Log");
    if(!logDir.exists()){
        logDir.mkpath(".");
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QString logFileName = QString("log_%1.log").arg(timestamp);
    QString filePath = logDir.filePath(logFileName);


    QFile logFile(filePath);
    if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Failed to create log file.";
        return;
    }
    out(&logFile);
    out<<"New log file create.\n";
//    logFile.close();


}



void Logger::writeLog(LogType type, const QString &message)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dataTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    out<<dataTimeString<<" |"<<logTypeToString(type)<<"| "<<message<<"\n";
    out.flush();
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

void Logger::on_comboBox_currentIndexChanged(int index, QTextBrowser *textBrowser, const QString &directoryPath)
{
    if(index == -1){
        return;
    }

    QString fileName = QString("%1/%2.log").arg(directoryPath, textBrowser->toPlainText());
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        textBrowser->setPlainText("Failed to open file");
        return;
    }
    QTextStream in(&file);
    textBrowser->setPlainText(in.readAll());
    file.close();
}
