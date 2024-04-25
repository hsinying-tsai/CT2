#include "funcpar.h"
#include "ui_funcpar.h"
#include <QDebug>
#include <QComboBox>
FuncPar::FuncPar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuncPar)
{
    ui->setupUi(this);
}

void FuncPar::fpupdatecombopattern(QListWidgetItem *item,int i)
{
    if(i==0){
        ui->comboBox_pattern->clear();
    }
    ui->comboBox_pattern->addItem(item->text());
}

FuncPar::~FuncPar()
{
    delete ui;
}
