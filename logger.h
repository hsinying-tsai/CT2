#ifndef LOGGER_H
#define LOGGER_H
#include<QDateTime>
#include<QFile>
#include<QTextStream>
#include<QDir>
#include<QComboBox>
#include<QTextBrowser>
//Info, Warning, Error, Fatal
extern uint8_t CAM1_parm1;
class Logger : public QObject
{
    Q_OBJECT

public:
    explicit Logger(QObject *parent = nullptr);
    enum LogType{Info, Warning, Error, Fatal};
    void writeLog(LogType type, const QString &message);
    void populateCombowithFileName(QComboBox *combobox, const QString directoryPath);
    void on_comboBox_currentIndexChanged(int index, QTextBrowser *textBrowser,QComboBox *combobox, const QString &directoryPath);
    void create_file();
    void autoUpdateLog(QTextBrowser *textBrowser,QComboBox *combobox, const QString &directoryPath);
    QString filePath,logFileName,timestamp,comboText,formattedMessage;
    QFile logFile;
    QTextStream m_textstream;
    QDir logDir;
    QString dataTimeString;



private:
    QString logTypeToString(LogType type);
signals:
    void updateLog();

};

#endif // LOGGER_H
