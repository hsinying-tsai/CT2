#ifndef LOGGER_H
#define LOGGER_H
#include<QDateTime>
#include<QFile>
#include<QTextStream>
#include<QDir>
#include<QComboBox>
#include<QTextBrowser>

//Info, Warning, Error, Fatal
class Logger : public QObject
{
    Q_OBJECT

public:
    explicit Logger(QObject *parent = nullptr);
    enum LogType{Info, Warning, Error, null};
    void writeLog(LogType type, const QString &message);
    void populateCombowithFileName(QComboBox *combobox, const QString directoryPath);
    void on_comboBox_currentIndexChanged(int index, QTextBrowser *textBrowser,QComboBox *combobox, const QString &directoryPath);
    void create_file();
    void autoUpdateLog(QTextBrowser *textBrowser,QComboBox *combobox, const QString &directoryPath);
    QString filePath,logFileName,timestamp,comboText,formattedMessage,currentDate;
    QTextStream m_textstream;
    QDir logDir;
    QString dataTimeString;
    QString m_lastDateChecked;



private:
    QString logTypeToString(LogType type);
signals:
    void updateUILog(QString type, QString message);

};

#endif // LOGGER_H
