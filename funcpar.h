#ifndef FUNCPAR_H
#define FUNCPAR_H
#include <QListWidget>
#include <QDialog>
#include <QComboBox>
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
    ~FuncPar();

};

#endif // FUNCPAR_H
