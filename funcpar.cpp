
#include "funcpar.h"
#include "ui_funcpar.h"
#include <QDebug>
#include <QString>

FuncPar::FuncPar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuncPar)
{
    ui->setupUi(this);

    //未選擇pattern時,不能更改參數
    foreach(QWidget *widget, ui->tabWidget->findChildren<QWidget *>()) {
        widget->setEnabled(false);
    }

    // 將所有 radioButton 存儲到 radioButtons
    QList<QRadioButton*> radioButtons = findChildren<QRadioButton*>();
    foreach(QRadioButton *radioButton, radioButtons) {
        connect(radioButton, &QRadioButton::clicked, this, &FuncPar::onRadioButtonClicked);
    }
}

void FuncPar::fpupdatecombopattern(QListWidgetItem *item,int i)
{
    if(i==0){
        ui->comboBox_pattern->clear();
    }
    ui->comboBox_pattern->addItem(item->text());
}

void FuncPar::INI(QStringList patternName, QString recipetFilePath)
{
    recipeFilePath = recipetFilePath;
    QSettings settings(recipetFilePath, QSettings::IniFormat);
    foreach(const QString &name, patternName) {
//        qDebug()<<name;
        ui->comboBox_pattern->addItem(name);
        settings.beginGroup(name);
        settings.setValue("checkDP", false);
        settings.setValue("checkBP", false);
        settings.setValue("checkDPBP", false);
        settings.setValue("BypassUpH", 0);
        settings.setValue("BypassDownH", 0);
        settings.setValue("BypassLeftY", 0);
        settings.setValue("BypassRightY", 0);
        settings.setValue("LineCut", 0);
        settings.setValue("LineShortCut", 0);
        settings.setValue("LineOverNum", 0);
        settings.setValue("BlockOverNum", 0);
        settings.setValue("range", 0);
        settings.setValue("BL_threshlodHigh", 0);
        settings.setValue("BL_threshlodLow", 0);
        settings.setValue("DL_threshlodHigh", 0);
        settings.setValue("DL_threshlodLow", 0);
        settings.setValue("maxgrayfullnessMin", 0);
        settings.setValue("GrayMean_Min", 0);
        settings.setValue("fullnessMin", 0);
        settings.setValue("elongationMax", 0);
        settings.setValue("compactnessMin", 0);
        settings.setValue("MaxGray_Min", 0);
        settings.setValue("fullnessMax", 0);
        settings.setValue("MaxGray_Max", 0);
        settings.setValue("compactnessMax", 0);
        settings.setValue("elongationMin", 0);
        settings.setValue("elongationMin_3", 0);
        settings.setValue("elongationMax_3", 0);
        settings.setValue("fullnessMin_3", 0);
        settings.setValue("fullnessMax_3", 0);
        settings.setValue("maxgrayfullnessMin_3", 0);
        settings.setValue("GrayMean_Max", 0);
        settings.setValue("MinGray_Max", 0);
        settings.setValue("MinGray_Min", 0);
        settings.setValue("compactnessMax_3", 0);
        settings.setValue("compactnessMin_3", 0);
        settings.setValue("threshlodHigh_2", 0);
        settings.setValue("boundaryHigh_2", 0);
        settings.setValue("threshlodLow_2", 0);
        settings.setValue("boundaryLow_2", 0);
        settings.setValue("threshlodHigh", 0);
        settings.setValue("threshlodLow", 0);
        settings.setValue("boundaryHigh", 0);
        settings.setValue("boundaryLow", 0);
        settings.setValue("bpAreaMax", 0);
        settings.setValue("bpAreaMin", 0);
        settings.setValue("dpAreaMin_2", 0);
        settings.setValue("dpAreaMin", 0);
        settings.setValue("byPassX", 0);
        settings.setValue("byPassX_2", 0);
        settings.setValue("overnum", 0);
        settings.setValue("ifArea", 0);
        settings.setValue("BPBlob", 0);
        settings.setValue("splitNum", 0);
        settings.setValue("edgewidth", 0);
        settings.setValue("edgeoffset", 0);
        settings.setValue("maxvalue", 0);
        settings.setValue("minvalue", 0);
        settings.endGroup();
    }
}
void FuncPar::reviseModelINI(QString section, QString key ,QString Value)
{
    qDebug()<<"recipeFilePath"<<recipeFilePath;
    QSettings settings(recipeFilePath, QSettings::IniFormat);
    int currentValue = settings.value(section + "/" + key).toInt();
    qDebug()<<"Current:"<< currentValue;
    settings.setValue(section+"/"+key,Value);
    settings.sync();
    qDebug()<<"//"<<section<<"//"<<key<<"//"<<Value;
}

void FuncPar::onRadioButtonClicked(bool checked)
{
    qDebug()<<checked;
    // 获取发出信号的 radioButton
    QRadioButton *radioButton = qobject_cast<QRadioButton*>(sender());

    // 如果没有发出信号的 radioButton，退出
    if (!radioButton) return;

    // 获取与 radioButton 对应的 spinBox 的对象名
    QString spinBoxObjectName = radioButton->objectName().replace("radioB_", "spinBox_");

    // 查找与 radioButton 对应的 spinBox
    QSpinBox *spinBox = findChild<QSpinBox*>(spinBoxObjectName);
    if (!spinBox) return; // 如果找不到对应的 spinBox，退出

    // 根据 radioButton 的状态设置 spinBox 的 enabled 属性
    spinBox->setEnabled(checked);

}

void FuncPar::receiveFileinfo(QString modelName, QString modelPath ,bool isNew,QStringList patternName)
{
    qDebug()<<"patternName"<<patternName;
    qDebug()<<"modelName"<<modelName;
    qDebug()<<"modelPath"<<modelPath;
    if(isNew == true){
        patternName = defalutPattern;
        INI(patternName,modelPath);
    }
}

void FuncPar::removePattern(QString patternName, QString ModelPath)
{
    QFile Modelfile(ModelPath);
    if (!Modelfile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading and writing.";
        return;
    }
    QTextStream in(&Modelfile);
    QStringList lines;
    bool inSection = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("[" + patternName + "]")) {
            inSection = true;
            continue;
        } else if (inSection && line.startsWith("[")) {
            inSection = false;
        }

        if (!inSection) {
            lines << line;
        }
    }
    Modelfile.resize(0); // 清空文件內容
    QTextStream out(&Modelfile);
    for (const QString &line : lines) {
        out << line << "\n";
    }
    Modelfile.close();
}
FuncPar::~FuncPar()
{
    delete ui;
}

void FuncPar::on_puB_save_clicked()
{
    QString selectedOption = ui->comboBox_pattern->currentText();
    qDebug()<<selectedOption;
    spinBoxNames.clear();
    for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
        // 获取当前页
        QWidget *currentTab = ui->tabWidget->widget(tabIndex);
        // 遍历当前页中的所有子控件
        foreach(QWidget *widget, currentTab->findChildren<QWidget *>()) {
            // 判断子控件是否是QSpinBox、QRadioButton或者QCheckBox
            if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(widget)) {
                QString name = spinBox->objectName();
                // 去除名稱中的"spinBox_"並只保留後半部分
                name.remove("spinBox_");
                spinBoxNames.append(name);
                if(spinBox->text()==""){
                    qDebug()<<"empty";
                    spinBox->text() = "0";
                }
                qDebug()<<"--"<<spinBox->text();
                qDebug()<<"---"<<selectedOption<<name<<spinBox->text();
                reviseModelINI(selectedOption, name , spinBox->text());
            } else if (QRadioButton *radioButton = qobject_cast<QRadioButton*>(widget)) {
//                qDebug() << "RadioButton Name: " << radioButton->objectName();
            } else if (QCheckBox *checkBox = qobject_cast<QCheckBox*>(widget)) {
//                qDebug() << "CheckBox Name: " << checkBox->objectName();
            }
        }
    }


    pattern_name newPattern;
    newPattern.name = selectedOption;
    newPattern.BPoint.threshlodHigh = ui->spinBox_threshlodHigh->text().toInt();
    newPattern.BP = ui->radioB_BP;
    newPattern.DP = ui->radioB_DP;
    QButtonGroup buttonGroup(this);
    QList<QRadioButton *> radioButtons = ui->groupBox->findChildren<QRadioButton *>();
    foreach (QRadioButton *radioButton, radioButtons) {
        buttonGroup.addButton(radioButton);
    }
    // 建立一個 QMap 來存儲單選按鈕的狀態（true 或 false）
     QMap<QString, bool> radioButtonStates;

     // 遍歷每個單選按鈕，檢查它們的狀態並存儲到 QMap 中
     foreach(QAbstractButton *button, buttonGroup.buttons()) {
         radioButtonStates.insert(button->objectName(), button->isChecked());
     }

     // 輸出 QMap 中存儲的單選按鈕狀態
     QMapIterator<QString, bool> iter(radioButtonStates);
     while (iter.hasNext()) {
         iter.next();
//         qDebug() << iter.key() << ": " << iter.value();
     }

}

void FuncPar::on_comboBox_pattern_activated()
{
    foreach(QWidget *widget, ui->tabWidget->findChildren<QWidget *>()) {
        widget->setEnabled(true);
    }
    foreach(QSpinBox *spinbox, ui->tabWidget->findChildren<QSpinBox *>()) {
        spinbox->setEnabled(false);
    }
}


void FuncPar::on_horizontalSlider_exposureTime_valueChanged(int value)
{
    ui->spinBox_exposureTime->setValue(value);
}
