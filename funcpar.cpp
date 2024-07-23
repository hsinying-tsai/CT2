#include "funcpar.h"
#include "ui_funcpar.h"
#include <QDebug>
#include <QString>
FuncPar::FuncPar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuncPar)
{
    ui->setupUi(this);
    this->setWindowTitle("Func");
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

void FuncPar::INI(QList<QPair<int,QString>>patternList, QString recipetFilePath, QString ModelName,bool isNew)
{
    if(isNew){
        //預設檢測patterns
        patternList.append({1,"White"});
        patternList.append({2,"Black"});
    }
    recipeFilePath = recipetFilePath;
    QSettings settings(recipetFilePath, QSettings::IniFormat);
    ui->lbl_ModelName->setText(ModelName);
    settings.beginGroup("CAM1");
    settings.setValue("exposureTime", 1);
    settings.endGroup();

    settings.beginGroup("COORDINATE");
    settings.setValue("PT_sizeX", 1152);
    settings.setValue("PT_sizeY", 648);
    settings.endGroup();
    foreach(const QPair p, patternList) {
        int index = p.first;
        QString name = p.second;
        ui->comboBox_pattern->addItem(name);
        settings.beginGroup(name);
        settings.setValue("Index",index);
        settings.setValue("checkDP", false);
        settings.setValue("checkBP", false);
        settings.setValue("checkBL", false);
        settings.setValue("checkDL", false);
        settings.setValue("checkMura", false);

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
        settings.setValue("BPmaxgrayfullnessMin", 0);
        settings.setValue("BPGrayMean_Min", 0);
        settings.setValue("BPfullnessMin", 0);
        settings.setValue("BPelongationMax", 0);
        settings.setValue("BPcompactnessMin", 0);
        settings.setValue("BPMaxGray_Min", 0);
        settings.setValue("BPfullnessMax", 0);
        settings.setValue("BPMaxGray_Max", 0);
        settings.setValue("BPcompactnessMax", 0);
        settings.setValue("BPelongationMin", 0);
        settings.setValue("DPelongationMin", 0);
        settings.setValue("DPelongationMax", 0);
        settings.setValue("DPfullnessMin", 0);
        settings.setValue("DPfullnessMax", 0);
        settings.setValue("DPmaxgrayfullnessMin", 0);
        settings.setValue("DPGrayMean_Max", 0);
        settings.setValue("DPMinGray_Max", 0);
        settings.setValue("DPMinGray_Min", 0);
        settings.setValue("DPcompactnessMax", 0);
        settings.setValue("DPcompactnessMin", 0);
        settings.setValue("DPthreshlodHigh", 0);
        settings.setValue("DPboundaryHigh", 0);
        settings.setValue("DPthreshlodLow", 0);
        settings.setValue("DPboundaryLow", 0);
        settings.setValue("BPthreshlodHigh", 0);
        settings.setValue("BPthreshlodLow", 0);
        settings.setValue("BPboundaryHigh", 0);
        settings.setValue("BPboundaryLow", 0);
        settings.setValue("BPAreaMax", 0);
        settings.setValue("BPAreaMin", 0);
        settings.setValue("DPAreaMin", 0);
        settings.setValue("DPAreaMin", 0);
        settings.setValue("byPassX", 0);
        settings.setValue("byPassY", 0);
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
void FuncPar::reviseRecipeINI(QString section, QString key ,QString Value)
{
    QSettings settings(recipeFilePath, QSettings::IniFormat);
    int currentValue = settings.value(section + "/" + key).toInt();
    settings.setValue(section+"/"+key,Value);
    settings.sync();
}



void FuncPar::removePattern(QString patternName, QString ModelPath)
{
    QFile Modelfile(ModelPath);
    if (!Modelfile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading and writing.";
        return;
    }else{
        qDebug()<<"Success open file to remove!";
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
    qDebug()<<"clear the pattern info";
}
FuncPar::~FuncPar()
{
    delete ui;
}

void FuncPar::on_puB_save_clicked()
{
    QString selectedOption = ui->comboBox_pattern->currentText();
    if(!selectedOption.contains("select")){
        qDebug()<<selectedOption;
        spinBoxNames.clear();
        for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
            QWidget *currentTab = ui->tabWidget->widget(tabIndex);
            QString tmp_text;
            foreach(QWidget *widget, currentTab->findChildren<QWidget *>()) {
                // 判斷子控件是否是QSpinBox、QRadioButton或者QCheckBox
                if (QSpinBox *spinBox = qobject_cast<QSpinBox*>(widget)) {
                    QString name = spinBox->objectName();
                    // 去除名稱中的"spinBox_"並只保留後半部分
                    name.remove("spinBox_");
                    spinBoxNames.append(name);
                    if(spinBox->text()==""){
                        tmp_text = "0";
                    }else{
                        tmp_text = spinBox->text();
                    }
    //                qDebug()<<selectedOption<<name<<tmp_text;
                    reviseRecipeINI(selectedOption, name , tmp_text);
                } else if (QRadioButton *radioButton = qobject_cast<QRadioButton*>(widget)) {
                    if(radioButton->objectName().contains("check")){
                        bool isChecked = radioButton->isChecked();
                        QString name = radioButton->objectName();
                        name.remove("radioB_");
                        if(isChecked == true){
                            reviseRecipeINI(selectedOption,  name, "true");
                        }else{
                            reviseRecipeINI(selectedOption,  name, "false");
                        }
    //                    qDebug() << "RadioButton Name: " << name <<isChecked;
                    }

                } else if (QCheckBox *checkBox = qobject_cast<QCheckBox*>(widget)) {
    //                 qDebug() << "CheckBox Name: " << checkBox->objectName();
                }
            }
        }
    }

}

void FuncPar::on_comboBox_pattern_activated(const QString &patternName)
{

    foreach(QWidget *widget, ui->tabWidget->findChildren<QWidget *>()) {
        widget->setEnabled(true);
    }
    foreach(QSpinBox *spinbox, ui->tabWidget->findChildren<QSpinBox *>()) {
        spinbox->setEnabled(false);
    }
    QSettings openrecipe(recipeFilePath, QSettings::IniFormat);
    QStringList groups = openrecipe.childGroups();
    for(const QString &group :groups){
        if(group.toLower() == patternName.toLower()){
            openrecipe.beginGroup(group);
            QStringList keys = openrecipe.allKeys();
            for (QString &key : keys) {
                QString value = openrecipe.value(key).toString();
                 if(value == "true" || value == "false"){
                    QString controlName = QString("%1%2").arg("radioB_").arg(key);
                    QRadioButton *radio = findChild<QRadioButton*>(controlName);
                    if (radio) {
                        if(value == "true"){
                            radio->setChecked(true);;
                        }else{
                            radio->setChecked(false);
                        }
                    }
                }else{
                    QString controlName = QString("%1%2").arg("spinBox_").arg(key);
                    // Assuming QSpinBox for demonstration, you can replace with appropriate UI control
                    QSpinBox *spinBox = findChild<QSpinBox*>(controlName);
                    if (spinBox) {
                        spinBox->setValue(value.toInt());
                    }
                }
            }
        }
    }
}
void FuncPar::onRadioButtonClicked(bool checked)
{
    qDebug()<<"onRadioButtonClicked";
    // 获取发出信号的 radioButton
    QRadioButton *radioButton = qobject_cast<QRadioButton*>(sender());

    // 获取与 radioButton 对应的 spinBox 的对象名
    QString spinBoxObjectName = radioButton->objectName().replace("radioB_", "spinBox_");
   // 查找与 radioButton 对应的 spinBox
    QSpinBox *spinBox = findChild<QSpinBox*>(spinBoxObjectName);
    if(spinBox){
        spinBox->setEnabled(checked);
    }
}


void FuncPar::on_horizontalSlider_exposureTime_valueChanged(int value)
{
    ui->spinBox_exposureTime->setValue(value);
}
