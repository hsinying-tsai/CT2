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
    struct pattern_name{
      QString name;
      int index;
      bool BP = false,DP = false;
      BorDpoint BPoint;
      BorDpoint DPoint;
      int exposureT;
    };
    QStringList defalutPattern = {"Red","Green"};
    QString recipeFilePath;
    void INI(QStringList patternName, QString recipetFilePath, QString ModelName, bool isNew);
    void reviseModelINI(QString section, QString key ,QString Value);
    QStringList spinBoxNames;
    QList<int> spinBoxValues;
    void removePattern(QString patternName,QString ModelPath);
    ~FuncPar();

private slots:
    void on_puB_save_clicked();
    void on_comboBox_pattern_activated(const QString &patternName);
    void on_horizontalSlider_exposureTime_valueChanged(int value);
    void onRadioButtonClicked(bool checked);
    void on_puB_load_clicked();
};

#endif // FUNCPAR_H
