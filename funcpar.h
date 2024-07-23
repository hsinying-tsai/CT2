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
    QString recipeFilePath;
    QStringList spinBoxNames;
    QList<int> spinBoxValues;
    ~FuncPar();

public slots:
    void fpupdatecombopattern(QListWidgetItem *item, int i);
    void INI(QList<QPair<int,QString>>patternList, QString recipetFilePath, QString ModelName, bool isNew);
    void reviseRecipeINI(QString section, QString key ,QString Value);
    void removePattern(QString patternName,QString ModelPath);

private slots:    
    void on_puB_save_clicked();
    void on_comboBox_pattern_activated(const QString &patternName);
    void on_horizontalSlider_exposureTime_valueChanged(int value);
    void onRadioButtonClicked(bool checked);
};

#endif // FUNCPAR_H
