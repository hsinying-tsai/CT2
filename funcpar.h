#ifndef FUNCPAR_H
#define FUNCPAR_H
#include <QListWidget>
#include <QDialog>
#include <QComboBox>
#include <QFile>
#include <QSettings>
#include <QComboBox>
#include <QButtonGroup>
namespace Ui {
class FuncPar;
}

class FuncPar : public QDialog
{
    Q_OBJECT

public:
    Ui::FuncPar *ui;
    explicit FuncPar(QWidget *parent = nullptr);
    void fpupdatecombopattern(QListWidgetItem *item, int i);
    struct BorDpoint{
        int threshlodHigh,threshlodLow,boundaryHigh,boundaryLow;
    };
    struct spinBox{
        int inte;
        double dou;
    };
    struct pattern_name{
      QString name;
      int index;
      bool BP = false,DP = false;
      BorDpoint BPoint;
      BorDpoint DPoint;

    };

    QString receiptFilePath;
    void INI(QStringList patternName,QString receiptFilePath);
    void reviseconfigINI(QString section, QString key ,QString Value);
    QStringList spinBoxNames;
    void onRadioButtonClicked(bool checked);
    QList<int> spinBoxValues;
    void receiveFileinfo(QString moduleName, QString modulePath, bool isNew,QStringList patternName);

    ~FuncPar();

private slots:
    void on_puB_save_clicked();
    void on_comboBox_pattern_activated();


};

#endif // FUNCPAR_H
