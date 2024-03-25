#ifndef LOGGER_H
#define LOGGER_H
#include<QDateTime>
#include<QFile>
#include<QTextStream>
#include<QDir>
#include<QComboBox>
#include<QTextBrowser>

class Logger : public QObject
{
    Q_OBJECT
public:

    explicit Logger(QObject *parent = nullptr);
    enum LogType{Info, Warning, Error, Fatal};
    void writeLog(LogType type, const QString &message);
    void populateCombowithFileName(QComboBox *combobox, const QString directoryPath);
    void on_comboBox_currentIndexChanged(int index, QTextBrowser *textBrowser, const QString &directoryPath);
    void createLogFile();
    QString logFileName;
private:
    QString filename;
    QFile logFile;
    QTextStream m_textstream;
    QString logTypeToString(LogType type);



signals:

};

#endif // LOGGER_H
